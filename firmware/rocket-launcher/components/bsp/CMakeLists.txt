idf_component_register(
    SRCS
        src/button.cpp
        src/leds.cpp
        src/peripherals.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        drivers
        peripheral-alloc
)

component_compile_options(-std=gnu++17)
