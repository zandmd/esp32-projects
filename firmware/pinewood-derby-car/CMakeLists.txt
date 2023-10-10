cmake_minimum_required(VERSION 3.0)
project(pinewood-derby-car LANGUAGES NONE)
include(../../cmake/setup.cmake)

find_package(EspIdf REQUIRED)

esp_idf_project(NAME hwtest)
