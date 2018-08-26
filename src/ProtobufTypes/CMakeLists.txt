cmake_minimum_required(VERSION 3.8)

find_package(Protobuf REQUIRED)

message(${PROTOBUF_LIBRARIES})
message(${PROTOBUF_INCLUDE_DIRS})

set(PROTO_FILES
        test.proto
        )


protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_FILES})

# adding PROTO_FILES to library so IDEs recognize them
add_library(proto ${PROTO_HEADER} ${PROTO_SRC} ${PROTO_FILES})
target_link_libraries(proto PRIVATE ${PROTOBUF_LIBRARIES})
target_include_directories(proto PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${PROTOBUF_INCLUDE_DIRS})
