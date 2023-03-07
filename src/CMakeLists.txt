aux_source_directory(. SRC_LIST)
add_library(mapreduce ${SRC_LIST})
target_link_libraries(mapreduce)