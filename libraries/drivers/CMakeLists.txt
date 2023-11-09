idf_component_register(
    SRCS
        src/button.cpp
        src/gpio_isr_service.cpp
        src/lora.cpp
        src/lsm6dso.cpp
        src/power_dump.cpp
        src/ws2811.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        bus
        color
        driver
        peripheral-alloc
)

component_compile_options(-std=gnu++17)
