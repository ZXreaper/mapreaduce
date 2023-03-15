cmake_minimum_required(VERSION 3.23)
project(mapreduce)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 17)

include(./cmake/common.cmake)
#include(FetchContent)
#FetchContent_Declare(
#        gRPC
#        GIT_REPOSITORY https://github.com/grpc/grpc
#        GIT_TAG RELEASE_TAG_HERE  # e.g v1.28.0
#)
#set(FETCHCONTENT_QUIET OFF)
#FetchContent_MakeAvailable(gRPC)

# Proto file
get_filename_component(mr_proto "./protos/mrrpcfunction.proto" ABSOLUTE)
get_filename_component(mr_proto_path "${mr_proto}" PATH)

# Generated sources
set(mr_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/src/mrrpcfunction.pb.cc")
set(mr_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/src/include/mrrpcfunction.pb.h")
set(mr_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/src/mrrpcfunction.grpc.pb.cc")
set(mr_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/src/include/mrrpcfunction.grpc.pb.h")
add_custom_command(
        OUTPUT "${mr_proto_srcs}" "${mr_proto_hdrs}" "${mr_grpc_srcs}" "${mr_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${mr_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${mr_proto}"
        DEPENDS "${mr_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# hw_grpc_proto
add_library(mr_grpc_proto
        ${mr_grpc_srcs}
        ${mr_grpc_hdrs}
        ${mr_proto_srcs}
        ${mr_proto_hdrs})
target_link_libraries(mr_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

## Targets greeter_[async_](client|server)
#foreach (_target
#        mrrpc_client mrrpc_server)
#    add_executable(${_target} "${_target}.cc")
#    target_link_libraries(${_target}
#            mr_grpc_proto
#            ${_REFLECTION}
#            ${_GRPC_GRPCPP}
#            ${_PROTOBUF_LIBPROTOBUF})
#endforeach ()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example/wordcount)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)
add_subdirectory(example/wordcount)