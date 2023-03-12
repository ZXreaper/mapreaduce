aux_source_directory(. SRC_LIST)
add_library(mapreduce_worker ${SRC_LIST})
add_executable(mr_worker ${SRC_LIST})
target_link_libraries(mapreduce_worker
        grpc++
        pthread
        mr_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
