idf_component_register(
    SRCS
        src/wifi.cpp
    INCLUDE_DIRS
        include
)

component_compile_options(-std=gnu++17)

add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_SOURCE_DIR}/src/wifi.cpp"
    COMMAND "${PYTHON}" "${CMAKE_CURRENT_SOURCE_DIR}/src/generate.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
    USES_TERMINAL
)
