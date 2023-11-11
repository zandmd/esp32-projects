#include <array>
#include <assert.h>
#include <chrono>
#include <stddef.h>
#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <zandmd/bsp/charges.hpp>
#include <zandmd/drivers/power_dump.hpp>
#include <zandmd/peripheral-alloc/generic_adc.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

#define DEBOUNCE_SAMPLES 10
#define HYSTERESIS_LOW 1000
#define HYSTERESIS_HIGH 2000
#define TAG "charges"

using namespace std;
using namespace std::chrono;
using namespace zandmd::bsp;
using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

charges::charges(UBaseType_t priority, const array<generic_adc, 4> &senses, const array<generic_gpio, 4> &fires, milliseconds duration) noexcept
        : senses(senses[0]),
          senses1(senses[1]),
          senses2(senses[2]),
          senses3(senses[3]),
          fires(fires[0], duration),
          fires1(fires[1], duration),
          fires2(fires[2], duration),
          fires3(fires[3], duration) {
    sem = xSemaphoreCreateMutexStatic(&sem_mem);
    assert(sem != nullptr);
    task = xTaskCreateStatic(&charges::task_func, "charges", sizeof(stack), this, priority, stack, &task_mem);
    assert(task != nullptr);
}

charges::~charges() noexcept {
    vTaskDelete(task);
    vSemaphoreDelete(sem);
}

charges::mask charges::check() const noexcept {
    assert(xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE);
    charges::mask continuity = this->continuity;
    assert(xSemaphoreGive(sem) == pdTRUE);
    return continuity;
}

bool charges::arm(mask charges) noexcept {
    bool success = true;
    for (size_t i = 0; i < 4; ++i) {
        if (charges[i]) {
            success = (&fires)[i].arm() && success;
        }
    }
    return success;
}

bool charges::disarm(mask charges) noexcept {
    bool success = true;
    for (size_t i = 0; i < 4; ++i) {
        if (charges[i]) {
            success = (&fires)[i].disarm() && success;
        }
    }
    return success;
}

void charges::disarm_all() noexcept {
    for (size_t i = 0; i < 4; ++i) {
        (&fires)[i].disarm();
    }
}

charges::mask charges::are_armed() const noexcept {
    mask res;
    for (size_t i = 0; i < 4; ++i) {
        res[i] = (&fires)[i].is_armed();
    }
    return res;
}

bool charges::fire(mask charges) noexcept {
    power_dump *dumps[4];
    power_dump **end = dumps;
    for (size_t i = 0; i < 4; ++i) {
        if (charges[i]) {
            *end++ = &fires + i;
        }
    }
    return power_dump::dump_all(dumps, end);
}

void charges::task_func(void *context) noexcept {
    charges &crg = *static_cast<charges *>(context);
    int samples_in_disagreement[4] = { 0, 0, 0, 0 };
    int raw[4];
    TickType_t last_print_sec = 0;
    while (true) {
        mask new_state = crg.continuity;
        for (size_t i = 0; i < 4; ++i) {
            raw[i] = (&crg.senses)[i].poll();
            if (new_state[i]) {
                if (raw[i] < HYSTERESIS_LOW) {
                    ++samples_in_disagreement[i];
                } else {
                    samples_in_disagreement[i] = 0;
                    continue;
                }
            } else {
                if (raw[i] > HYSTERESIS_HIGH) {
                    ++samples_in_disagreement[i];
                } else {
                    samples_in_disagreement[i] = 0;
                    continue;
                }
            }
            if (samples_in_disagreement[i] >= DEBOUNCE_SAMPLES) {
                new_state.flip(i);
            }
        }
        if (crg.continuity != new_state) {
            mask old_state = crg.continuity;
            assert(xSemaphoreTake(crg.sem, portMAX_DELAY) == pdTRUE);
            crg.continuity = new_state;
            assert(xSemaphoreGive(crg.sem) == pdTRUE);
            if (crg.charge_event) {
                crg.charge_event(new_state, old_state);
            }
        }
        TickType_t sec = xTaskGetTickCount() / pdMS_TO_TICKS(1000);
        if (last_print_sec != sec) {
            last_print_sec = sec;
            // ESP_LOGI(TAG, "raw ADC counts: % 4d, % 4d, % 4d, % 4d", raw[0], raw[1], raw[2], raw[3]);
        }
        vTaskDelay(1);
    }
}
