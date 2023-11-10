idf_component_register(
    SRCS
        src/battery.cpp
        src/bus.cpp
        src/button.cpp
        src/charges.cpp
        src/leds.cpp
        src/peripherals.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        drivers
        esp_adc
        peripheral-alloc
)

component_compile_options(-std=gnu++17)
