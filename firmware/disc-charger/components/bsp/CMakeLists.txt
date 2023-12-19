idf_component_register(
    SRCS
        src/analog.cpp
        src/bus.cpp
        src/leds.cpp
        src/peripherals.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        bus
        drivers
)

component_compile_options(-std=gnu++17)
