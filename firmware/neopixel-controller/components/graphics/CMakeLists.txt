idf_component_register(
    SRCS
        src/multi_span.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        color
        drivers
        math
)

component_compile_options(-std=gnu++17)
