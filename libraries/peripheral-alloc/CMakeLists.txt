idf_component_register(
    SRCS
        src/gpio.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        bsp
)

component_compile_options(-std=gnu++17)
