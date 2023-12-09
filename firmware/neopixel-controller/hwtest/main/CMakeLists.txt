idf_component_register(
    SRCS
        src/main.cpp
    REQUIRES
        bsp
)

component_compile_options(-std=gnu++17)
