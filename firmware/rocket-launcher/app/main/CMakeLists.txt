idf_component_register(
    SRCS
        src/lco_main.cpp
        src/lco_to_pad.cpp
        src/main.cpp
        src/pad_main.cpp
        src/pad_to_lco.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        bsp
        color
        drivers
)

component_compile_options(-std=gnu++17)
