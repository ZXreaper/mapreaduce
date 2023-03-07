set(SRC_LIST wc.cpp)

add_executable(wc ${SRC_LIST})

target_link_libraries(wc grpc)