idf_component_register(
    SRCS
        src/leds.cpp
        src/peripherals.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        color
        drivers
        graphics
        wifi
        peripheral-alloc
)

component_compile_options(-std=gnu++17)
