cmake_minimum_required(VERSION 3.8)

find_package(Protobuf REQUIRED)

set(PROTO_FILES
        history_types.proto
        )

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_FILES})

# adding PROTO_FILES to library so IDEs recognize them
add_library(history_types_lib ${PROTO_HEADER} ${PROTO_SRC} ${PROTO_FILES})
if(MSVC)
  target_compile_options(history_types_lib PRIVATE /wd4018 /wd4146 /wd4244 /wd4251 /wd4267 /wd4305 /wd4355 /wd4800 /wd4996)
endif()
target_link_libraries(history_types_lib PUBLIC ${PROTOBUF_LIBRARIES})
target_include_directories(history_types_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${PROTOBUF_INCLUDE_DIRS})
