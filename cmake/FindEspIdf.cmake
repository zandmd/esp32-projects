cmake_minimum_required(VERSION 3.2)

get_filename_component(esp_idf_path "${CMAKE_CURRENT_LIST_DIR}/../esp-idf" ABSOLUTE)
if (WIN32)
    string(REPLACE "/" "\\" esp_idf_path "${esp_idf_path}")
endif()
if (NOT EXISTS "${esp_idf_path}/README.md")
    message(FATAL_ERROR
        "Unable to locate ESP IDF.  "
        "Please run the following before running CMake:\n"
        "  git submodule update --init --recursive\n"
    )
endif()

if (WIN32)
    set(esp_idf_shell_interpreter "powershell.exe")
    set(esp_idf_shell_script_extension ".ps1")
    set(esp_idf_shell_exe_extension ".exe")
else()
    set(esp_idf_shell_interpreter "bash")
    set(esp_idf_shell_script_extension ".sh")
    set(esp_idf_shell_exe_extension "")
endif()

get_filename_component(
    esp_idf_tools_path "${esp_idf_path}/../toolchains" ABSOLUTE
)
set(
    esp_idf_python
    "${esp_idf_tools_path}/python_env/idf5.2_py3.9_env/Scripts/python${esp_idf_shell_exe_extension}"
)

function(esp_idf_project)
    set(options)
    set(single TARGET NAME)
    set(multi)
    cmake_parse_arguments(idf "${options}" "${single}" "${multi}" ${ARGN})
    if (NOT idf_TARGET)
        set(idf_TARGET esp32)
    endif()

    set(tool_dir "${esp_idf_tools_path}/tools/xtensa-${idf_TARGET}-elf")

    if (NOT EXISTS "${tool_dir}")
        set(ENV{IDF_PATH} "${esp_idf_path}")
        set(ENV{IDF_TARGET} "${idf_TARGET}")
        set(ENV{IDF_TOOLS_PATH} "${esp_idf_tools_path}")
        execute_process(
            COMMAND "${esp_idf_shell_interpreter}" "${esp_idf_path}/install${esp_idf_shell_script_extension}" "${idf_TARGET}"
            RESULT_VARIABLE error
        )
        if (NOT error EQUAL 0)
            message(FATAL_ERROR
                "Failed to install ESP-IDF.  "
                "Install script reported error: ${error}"
            )
        endif()
        if (NOT EXISTS "${tool_dir}")
            message(FATAL_ERROR
                "Installer did not create tools directory:\n"
                "  ${tool_dir}"
            )
        endif()
    endif()

    add_custom_target(
        "${idf_NAME}" ALL
        COMMAND "${CMAKE_COMMAND}" -E env
            "IDF_PATH=${esp_idf_path}"
            "IDF_TARGET=${idf_TARGET}"
            "IDF_TOOLS_PATH=${esp_idf_tools_path}"
            "${esp_idf_python}"
            "${esp_idf_path}/../cmake/FindEspIdf.py"
            -B "${CMAKE_BINARY_DIR}/${idf_NAME}"
            build
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${idf_NAME}"
        USES_TERMINAL
    )
    add_custom_target(
        "${idf_NAME}-clean"
        COMMAND "${CMAKE_COMMAND}" -E env
            "IDF_PATH=${esp_idf_path}"
            "IDF_TARGET=${idf_TARGET}"
            "IDF_TOOLS_PATH=${esp_idf_tools_path}"
            "${esp_idf_python}"
            "${esp_idf_path}/../cmake/FindEspIdf.py"
            -B "${CMAKE_BINARY_DIR}/${idf_NAME}"
            clean
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${idf_NAME}"
        USES_TERMINAL
    )
    add_custom_target(
        "${idf_NAME}-flash"
        COMMAND "${CMAKE_COMMAND}" -E env
            "IDF_PATH=${esp_idf_path}"
            "IDF_TARGET=${idf_TARGET}"
            "IDF_TOOLS_PATH=${esp_idf_tools_path}"
            "${esp_idf_python}"
            "${esp_idf_path}/../cmake/FindEspIdf.py"
            -B "${CMAKE_BINARY_DIR}/${idf_NAME}"
            build erase-flash flash monitor
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${idf_NAME}"
        USES_TERMINAL
    )
    add_custom_target(
        "${idf_NAME}-monitor"
        COMMAND "${CMAKE_COMMAND}" -E env
            "IDF_PATH=${esp_idf_path}"
            "IDF_TARGET=${idf_TARGET}"
            "IDF_TOOLS_PATH=${esp_idf_tools_path}"
            "${esp_idf_python}"
            "${esp_idf_path}/../cmake/FindEspIdf.py"
            -B "${CMAKE_BINARY_DIR}/${idf_NAME}"
            monitor
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${idf_NAME}"
        USES_TERMINAL
    )
endfunction()

include(UpdateVsCode)
