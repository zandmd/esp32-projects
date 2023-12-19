idf_component_register(
    SRCS
        src/main.cpp
    REQUIRES
        bsp
        color
)

component_compile_options(-std=gnu++17)
