cmake_minimum_required(VERSION 3.15)
project(ProtobufExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use CONFIG mode - this is crucial for vcpkg
find_package(protobuf CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)

# .proto file(s)
file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")

# Create a library for proto files
add_library(proto_library ${PROTO_FILES})
target_link_libraries(proto_library PUBLIC protobuf::libprotobuf utf8_range::utf8_range)
target_include_directories(proto_library PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Use protobuf_generate which handles everything
protobuf_generate(
    TARGET proto_library
    LANGUAGE cpp
)

# Main executable
add_executable(ProtobufExample main.cpp)
target_link_libraries(ProtobufExample PRIVATE proto_library)
