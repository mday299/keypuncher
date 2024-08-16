cmake_minimum_required(VERSION 3.16)
project (test_proto)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

set(PROTO_SRCS test.pb.cc)
set(PROTO_HDRS test.pb.h)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS test.proto)

add_executable(test_proto_wrapper test_proto.cpp ${PROTO_SRCS} ${PROTO_HDRS} )
target_link_libraries(test_proto_wrapper ${Protobuf_LIBRARIES})