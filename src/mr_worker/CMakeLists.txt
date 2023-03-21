aux_source_directory(. SRC_LIST_1)
add_executable(mr_worker ${SRC_LIST_1})
target_link_libraries(mr_worker
        wc
        mrrpc
        grpc++
        pthread
        mr_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
