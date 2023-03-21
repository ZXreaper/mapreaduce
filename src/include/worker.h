//
// Created by 张旭 on 2023/3/7.
//

#ifndef MAPREDUCE_WORKER_H
#define MAPREDUCE_WORKER_H

#include "kv.hpp"
#include "master.h"
#include <functional>
#include <grpcpp/grpcpp.h>
#include <string>
#include <vector>

using namespace mrrpc;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class Worker {
public:
  Worker(std::shared_ptr<Channel> channel)
      : stub_(mrrpc::RpcAssignTask::NewStub(channel)) {}

  static std::string ReadFile(std::string filename);

  // 加载map function 和 reduce function。
  // 在example中编写wc文件，在其中编写相应的map function和 reduce function
  // 之后将wc编译成静态库文件，然后直接使用静态库文件
  // g++ -c wc.cpp         ->   生成wc.o
  // ar -crv libwc.a wc.o  ->   生成libwc.a
  // 编译文件时，g++ xxx.cpp -L./ -lwc -o xxx
  // void LoadPlugin();

  // 启动worker端
  void StartWorker();

  // 获得任务。rpc方法
  Task GetTask();

  // worker获得MapTask，交给mapper处理
  void Mapper(Task &task);

  // worker获得ReduceTask，交给reducer处理
  void Reducer(Task &task);

  // 将中间结果写到本地文件
  std::string WriteToLocalFile(int x, int y, KeyValues &kvs);

  // 从本地读取中间文件获得map阶段的key values
  KeyValues ReadFromLocalFile(std::vector<std::string> files);

  // worker任务完成后通知master。rpc方法
  void TaskCompleted(Task &task);

private:
  // std::function<KeyValues(std::string, std::string)> mapf_; // map function
  // std::function<std::string(std::string, std::vector<std::string>)>
  //     reducef_; // reduce function
  std::unique_ptr<mrrpc::RpcAssignTask::Stub> stub_;
};

#endif // MAPREDUCE_WORKER_H
