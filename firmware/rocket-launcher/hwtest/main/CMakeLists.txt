idf_component_register(
    SRCS
        src/main.cpp
    REQUIRES
        bsp
        color
        drivers
)

component_compile_options(-std=gnu++17)
