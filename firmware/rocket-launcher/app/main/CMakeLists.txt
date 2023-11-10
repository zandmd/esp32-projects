idf_component_register(
    SRCS
        src/lco_main.cpp
        src/main.cpp
        src/pad_main.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        bsp
        color
        drivers
)

component_compile_options(-std=gnu++17)
