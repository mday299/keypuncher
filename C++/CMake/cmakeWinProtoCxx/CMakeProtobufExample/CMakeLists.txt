cmake_minimum_required(VERSION 3.15)
project(ProtobufExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define build types
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Use CONFIG mode for vcpkg
find_package(protobuf CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)

# .proto file(s)
file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")

# Create a library for proto files
add_library(proto_library ${PROTO_FILES})
target_link_libraries(proto_library PUBLIC 
    protobuf::libprotobuf 
    utf8_range::utf8_range
)
target_include_directories(proto_library PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Generate protobuf code
protobuf_generate(
    TARGET proto_library
    LANGUAGE cpp
)

# Main executable
add_executable(ProtobufExample main.cpp)
target_link_libraries(ProtobufExample PRIVATE proto_library)

# Debug-specific configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ProtobufExample PRIVATE 
        LIBRARY_DEBUG_BUILD=1
        LIBRARY_ENABLE_LOGGING=1
    )
    message(STATUS "Debug build: Enabling debug features")
else()
    target_compile_definitions(ProtobufExample PRIVATE 
        LIBRARY_DEBUG_BUILD=0
        LIBRARY_ENABLE_LOGGING=0
        NDEBUG
    )
    message(STATUS "Release build: Disabling debug features")
endif()

# Add build type as compile definition
target_compile_definitions(ProtobufExample PRIVATE
    BUILD_TYPE="${CMAKE_BUILD_TYPE}"
)

# Optimization flags
if(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(ProtobufExample PRIVATE /Od /Zi /RTC1)
    else()
        target_compile_options(ProtobufExample PRIVATE /O2 /GL)
        target_link_options(ProtobufExample PRIVATE /LTCG)
    endif()
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(ProtobufExample PRIVATE -O0 -g)
    else()
        target_compile_options(ProtobufExample PRIVATE -O3 -DNDEBUG)
    endif()
endif()
