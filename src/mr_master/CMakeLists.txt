aux_source_directory(. SRC_LIST_1)
add_library(mapreduce_master ${SRC_LIST_1})
add_executable(mr_master ${SRC_LIST_1})
target_link_libraries(mapreduce_master
        mrrpc
        grpc++
        pthread
        mr_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
