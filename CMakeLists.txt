cmake_minimum_required(VERSION 3.5.1)
project(mapreduce)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 17)

include(./cmake/common.cmake)

# Proto file
get_filename_component(mr_proto "./protos/mrrpcfunction.proto" ABSOLUTE)
get_filename_component(mr_proto_path "${mr_proto}" PATH)

# # Generated sources
# set(mr_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/mrrpcfunction.pb.cc")
# set(mr_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/mrrpcfunction.pb.h")
# set(mr_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/mrrpcfunction.grpc.pb.cc")
# set(mr_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/mrrpcfunction.grpc.pb.h")
# add_custom_command(
#         OUTPUT "${mr_proto_srcs}" "${mr_proto_hdrs}" "${mr_grpc_srcs}" "${mr_grpc_hdrs}"
#         COMMAND ${_PROTOBUF_PROTOC}
#         ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
#         --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
#         -I "${mr_proto_path}"
#         --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
#         "${mr_proto}"
#         DEPENDS "${mr_proto}")

set(mr_proto_srcs "${PROJECT_SOURCE_DIR}/src/rpc/mrrpcfunction.pb.cc")
set(mr_proto_hdrs "${PROJECT_SOURCE_DIR}/src/rpc/mrrpcfunction.pb.h")
set(mr_grpc_srcs "${PROJECT_SOURCE_DIR}/src/rpc/mrrpcfunction.grpc.pb.cc")
set(mr_grpc_hdrs "${PROJECT_SOURCE_DIR}/src/rpc/mrrpcfunction.grpc.pb.h")

# # Include generated *.pb.h files
# include_directories("${CMAKE_CURRENT_BINARY_DIR}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/src/rpc)
include_directories(${PROJECT_SOURCE_DIR}/example/wordcount)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# mr_grpc_proto
add_library(mr_grpc_proto
        ${mr_grpc_srcs}
        ${mr_grpc_hdrs}
        ${mr_proto_srcs}
        ${mr_proto_hdrs})
target_link_libraries(mr_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

add_subdirectory(src)
add_subdirectory(example/wordcount)