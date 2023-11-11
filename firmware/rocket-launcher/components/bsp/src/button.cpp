#include <assert.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/tasks.hpp>

#include <zandmd/bsp/button.hpp>

using namespace zandmd::bsp;

button::button() noexcept : watchdog(false) {
    sem = xSemaphoreCreateMutexStatic(&sem_mem);
    assert(sem != nullptr);
    task = xTaskCreateStatic(&button::poll_buttons, "poll button", sizeof(stack), this, tasks::buttontask, stack, &task_mem);
    assert(task != nullptr);
}

button::~button() noexcept {
    if (watchdog) {
        ESP_ERROR_CHECK(esp_task_wdt_delete(task));
    }
    vTaskDelete(task);
    vSemaphoreDelete(sem);
}

void button::enable_watchdog() noexcept {
    ESP_ERROR_CHECK(esp_task_wdt_add(task));
    watchdog = true;
}

bool button::get_button_state(int buttonnum) noexcept {
    assert(xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE);
    bool val = lastbuttonval[buttonnum];
    assert(xSemaphoreGive(sem) == pdTRUE);
    return val;
}

void button::poll_buttons(void * context) noexcept {
    gpio_config_t cfgcol = {
        .pin_bit_mask = (1uLL << gpio::btn_c0) | (1uLL << gpio::btn_c1),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

        gpio_config_t cfgrow = {
        .pin_bit_mask = (1uLL << gpio::btn_r0) | (1uLL << gpio::btn_r1) | (1uLL << gpio::btn_r2),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_set_level(gpio::btn_r0,false);
    gpio_set_level(gpio::btn_r1,false);
    gpio_set_level(gpio::btn_r2,false);
    gpio_config(&cfgcol);
    gpio_config(&cfgrow);

    button* btn = (button*)context;

    btn->lastbuttonval = 0;

    while (1) {
        if (btn->watchdog) {
            ESP_ERROR_CHECK(esp_task_wdt_reset());
        }

        gpio_set_level(gpio::btn_r0,true);
        gpio_set_level(gpio::btn_r1,false);
        gpio_set_level(gpio::btn_r2,false);
        vTaskDelay(1);
        btn->buttonval[0] = gpio_get_level(gpio::btn_c0);
        btn->buttonval[1] = gpio_get_level(gpio::btn_c1);


        gpio_set_level(gpio::btn_r0,false);
        gpio_set_level(gpio::btn_r1,true);
        gpio_set_level(gpio::btn_r2,false);
        vTaskDelay(1);
        btn->buttonval[2] = gpio_get_level(gpio::btn_c0);
        btn->buttonval[3] = gpio_get_level(gpio::btn_c1);

        gpio_set_level(gpio::btn_r0,false);
        gpio_set_level(gpio::btn_r1,false);
        gpio_set_level(gpio::btn_r2,true);
        vTaskDelay(1);
        btn->buttonval[4] = gpio_get_level(gpio::btn_c0);
        btn->buttonval[5] = gpio_get_level(gpio::btn_c1);

        if (btn->buttonchange) {
            for(int i = 0; i<6; i++) {
                if (btn->lastbuttonval[i] != btn->buttonval[i]) {
                    btn->buttonchange(btn->buttonval[i],i);
                }
            }
        }

        assert(xSemaphoreTake(btn->sem, portMAX_DELAY) == pdTRUE);
        btn->lastbuttonval = btn->buttonval;
        assert(xSemaphoreGive(btn->sem) == pdTRUE);

    }


}
