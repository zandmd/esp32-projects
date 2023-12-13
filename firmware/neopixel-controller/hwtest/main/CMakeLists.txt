idf_component_register(
    SRCS
        src/main.cpp
    REQUIRES
        bsp
        graphics
)

component_compile_options(-std=gnu++17)
