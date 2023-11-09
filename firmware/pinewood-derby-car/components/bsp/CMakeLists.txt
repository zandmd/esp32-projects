idf_component_register(
    SRCS
        src/bus.cpp
        src/peripherals.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        bus
        drivers
        peripheral-alloc
)

component_compile_options(-std=gnu++17)
