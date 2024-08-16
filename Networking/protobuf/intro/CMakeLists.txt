cmake_minimum_required(VERSION 3.16)
project (test_proto)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(PROTO_FILE test.proto)
set(PROTO_SRCS ${CMAKE_CURRENT_BINARY_DIR}/test.pb.cc)
set(PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR}/test.pb.h)

add_custom_command(
    OUTPUT ${PROTO_SRCS} ${PROTO_HDRS}
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --cpp_out ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Running protoc on ${PROTO_FILE}"
    VERBATIM
)

add_custom_target(generate_proto_files DEPENDS ${PROTO_SRCS} ${PROTO_HDRS})

add_executable(test_proto_wrapper test_proto.cpp ${PROTO_SRCS} ${PROTO_HDRS} )
add_dependencies(test_proto_wrapper generate_proto_files)
target_link_libraries(test_proto_wrapper ${Protobuf_LIBRARIES})