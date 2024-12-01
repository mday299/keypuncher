project(coord_conversion_intro)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/cmake/geographiclib")

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

find_package (GeographicLib REQUIRED)

include_directories (${GeographicLib_INCLUDE_DIRS})
    
add_executable (coordConvertIntro ${CMAKE_SOURCE_DIR}/coordConversionIntro.cpp)

target_link_libraries (coordConvertIntro ${GeographicLib_LIBRARIES}) 
