idf_component_register(
    SRCS
        src/main.cpp
    REQUIRES
        bsp
        drivers
        graphics
)

component_compile_options(-std=gnu++17)
