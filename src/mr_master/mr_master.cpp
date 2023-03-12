//
// Created by 张旭 on 2023/3/7.
//
#include "master.h"
#include "rpc.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: wc inputfiles..." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string server_address("127.0.0.1:50051");
  RpcServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // 读数据
  std::vector<std::string> files;
  for (int i = 1; argv[i] != NULL; i++) {
    std::string filename(argv[i]);
    files.emplace_back(filename);
  }

  // 创建master
  std::shared_ptr<Master> m = Master::MakeMaster(files, 10);

  // 结束后通知
  while (m->Done() == false) {
    sleep(1);
  }

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  // Block until the server shuts down.
  server->Wait();

  return 0;
}