idf_component_register(
    SRCS
        src/spi.cpp
        src/spidev.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        peripheral-alloc
)

component_compile_options(-std=gnu++17)
