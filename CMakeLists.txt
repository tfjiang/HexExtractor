cmake_minimum_required(VERSION 2.8)
project(HexExtractor)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{HOME}/usr/share/cmake/Modules/")
include($ENV{HOME}/usr/share/cmake/Modules/geo_sim_sdk.cmake)
include_geo_sim_sdk()
link_geo_sim_sdk()

add_subdirectory(common)
add_subdirectory(extractor)
add_subdirectory(test)

