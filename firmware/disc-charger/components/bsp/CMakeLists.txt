idf_component_register(
    SRCS
        src/peripherals.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        drivers
)

component_compile_options(-std=gnu++17)
