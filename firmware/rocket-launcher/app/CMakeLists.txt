cmake_minimum_required(VERSION 3.0)
set(EXTRA_COMPONENT_DIRS
    "${CMAKE_SOURCE_DIR}/../components"
    "${CMAKE_SOURCE_DIR}/../../../libraries"
)
set(COMPONENTS main)
include("$ENV{IDF_PATH}/tools/cmake/project.cmake")
project(app)
