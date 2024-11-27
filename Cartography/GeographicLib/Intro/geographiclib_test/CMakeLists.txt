project(geographiclib_test)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/cmake/geographiclib")

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

find_package (GeographicLib REQUIRED)

include_directories (${GeographicLib_INCLUDE_DIRS})
    
add_executable (geographiclib_test ${CMAKE_SOURCE_DIR}/geographiclib_test.cpp)

target_link_libraries (geographiclib_test ${GeographicLib_LIBRARIES}) 
