#include <esp_err.h>
#include <esp_event.h>
#include <zandmd/drivers/event_loop.hpp>

using namespace zandmd::drivers;

event_loop::event_loop() noexcept {
    ESP_ERROR_CHECK(esp_event_loop_create_default());
}

event_loop::~event_loop() noexcept {
    ESP_ERROR_CHECK(esp_event_loop_delete_default());
}
