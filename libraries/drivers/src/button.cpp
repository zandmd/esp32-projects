#include <assert.h>
#include <stdint.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <zandmd/drivers/button.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

#define DEBOUNCE_ON_PRESS_MS 20
#define DEBOUNCE_ON_RELEASE_MS 20
#define MIN_HOLD_TIME_MS 1000
#define MIN_IDLE_TIME_MS 300

using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

button::button(generic_gpio gpio) noexcept : gpio(gpio), last_press(0), last_release(0), clicks(0) {
    gpio_config_t cfg = {
        .pin_bit_mask = 1u << gpio,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };
    ESP_ERROR_CHECK(gpio_config(&cfg));
    timer = xTimerCreateStatic("button gesture timer", 1, false, this, &button::timer_event, &timer_memory);
    assert(timer != nullptr);
}

button::~button() noexcept {
    assert(xTimerDelete(timer, portMAX_DELAY) == pdPASS);
    ESP_ERROR_CHECK(gpio_reset_pin(gpio));
}

void button::enable() noexcept {
    add(gpio);
    ESP_ERROR_CHECK(gpio_intr_enable(gpio));
}

void button::disable() noexcept {
    ESP_ERROR_CHECK(gpio_intr_disable(gpio));
    remove(gpio);
}

void button::isr() noexcept {
    // 1. Capture state
    TickType_t now = xTaskGetTickCountFromISR();
    bool is_button_up = gpio_get_level(gpio);

    // 2. Debouncing
    event ev;
    if (is_button_up) {
        last_release = now;
        if (now - last_press >= pdMS_TO_TICKS(DEBOUNCE_ON_RELEASE_MS)) {
            ev = release;
        } else {
            return;
        }
    } else {
        last_press = now;
        if (now - last_release >= pdMS_TO_TICKS(DEBOUNCE_ON_PRESS_MS)) {
            ev = press;
        } else {
            return;
        }
    }
    send_event(ev, now);

    // 3. Gesture recognition
    if (ev == press) {
        if (clicks >= 0) {
            assert(xTimerStopFromISR(timer, nullptr) == pdPASS);
            assert(xTimerChangePeriodFromISR(timer, pdMS_TO_TICKS(MIN_HOLD_TIME_MS), nullptr) == pdPASS);
            ++clicks;
            assert(xTimerStartFromISR(timer, nullptr) == pdPASS);
        }
    } else {
        assert(xTimerStopFromISR(timer, nullptr) == pdPASS);
        assert(xTimerChangePeriodFromISR(timer, pdMS_TO_TICKS(MIN_IDLE_TIME_MS), nullptr) == pdPASS);
        assert(xTimerStartFromISR(timer, nullptr) == pdPASS);
    }
}

void button::timer_event(TimerHandle_t timer) noexcept {
    button *btn = static_cast<button *>(pvTimerGetTimerID(timer));
    if (btn->last_press > btn->last_release) {
        switch (btn->clicks) {
            case 1:
                btn->send_event(hold, btn->last_press);
                break;
        }
        btn->clicks = -1;
    } else {
        switch (btn->clicks) {
            case 1:
                btn->send_event(click, btn->last_press);
                break;
            case 2:
                btn->send_event(double_click, btn->last_press);
                break;
            case 3:
                btn->send_event(triple_click, btn->last_press);
                break;
        }
        btn->clicks = 0;
    }
}

void button::send_event(event ev, TickType_t timestamp) noexcept {
    TickType_t mask = (1 << (32 - event_bits)) - 1;
    uint32_t data = (static_cast<uint32_t>(ev) << (32 - event_bits)) | (timestamp & mask);
    assert(xTimerPendFunctionCallFromISR(&button::send_event, this, data, nullptr) == pdPASS);
}

void button::send_event(void *context, uint32_t data) noexcept {
    button *btn = static_cast<button *>(context);
    if (btn->callback) {
        event ev = static_cast<event>(data >> (32 - event_bits));
        TickType_t now = xTaskGetTickCount();
        TickType_t mask = (1 << (32 - event_bits)) - 1;
        TickType_t timestamp = (now & ~mask) | (data & mask);
        if (timestamp > now) {
            timestamp -= mask + 1;
        }
        btn->callback(ev, timestamp);
    }
}
