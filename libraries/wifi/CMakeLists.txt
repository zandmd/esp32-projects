idf_component_register(
    SRCS
        src/station.cpp
    INCLUDE_DIRS
        include
    REQUIRES
        drivers
        esp_netif
        esp_wifi
        secrets
)

component_compile_options(-std=gnu++17)
