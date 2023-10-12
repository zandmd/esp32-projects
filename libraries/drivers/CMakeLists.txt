idf_component_register(
    SRCS
        src/ws2811.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        color
        driver
        peripheral-alloc
)

component_compile_options(-std=gnu++17)
