#ifndef ZANDMD_ROCKET_LAUNCHER_TIMINGS_HPP
#define ZANDMD_ROCKET_LAUNCHER_TIMINGS_HPP

#include <freertos/FreeRTOS.h>

namespace zandmd {
    namespace rocket_launcher {
        struct timings {
            static constexpr TickType_t TX_PERIOD = pdMS_TO_TICKS(150);
            static constexpr TickType_t MAX_LATENCY = pdMS_TO_TICKS(80);
            static constexpr TickType_t MAX_DELAY = pdMS_TO_TICKS(10);
            static constexpr TickType_t COMM_TIMEOUT = pdMS_TO_TICKS(1500);
            static constexpr int TX_PER_DEBUG = 100;
        };
    }
}

#endif
