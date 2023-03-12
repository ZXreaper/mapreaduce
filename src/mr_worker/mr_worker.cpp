//
// Created by 张旭 on 2023/3/11.
//
#include "worker.h"
#include <iostream>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: mr_worker xxx.so" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string target_str = "127.0.0.1:50051";

  Worker worker(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  // 获取map和reduce方法
  worker.LoadPlugin(std::string(argv[1]));

  // worker启动
  worker.StartWorker();

  return 0;
}