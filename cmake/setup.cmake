cmake_minimum_required(VERSION 3.0)

# Ignore the build folder from Git
file(WRITE "${CMAKE_BINARY_DIR}/.gitignore" "*\n")

# Add /cmake/ to CMAKE_MODULE_PATH
list(FIND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}" res)
if (res LESS 0)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
endif()
