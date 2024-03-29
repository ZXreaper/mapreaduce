//
// Created by 张旭 on 2023/3/12.
//

#ifndef MAPREDUCE_RPC_H
#define MAPREDUCE_RPC_H
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "mrrpcfunction.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class Master; // 解决和master.h互相引用的问题

class RpcServiceImpl final : public mrrpc::RpcAssignTask::Service {
public:
  // 将rpc server绑定到对应的master上
  void SetMaster(Master *m);

private:
  // worker给master发送请求，获取任务
  Status AssignTask(ServerContext *context,
                    const ::mrrpc::AssignTaskRequest *request,
                    ::mrrpc::RPCTask *response);
  // worker向master报告任务已经做完
  Status TaskCompleted(ServerContext *context, const ::mrrpc::RPCTask *request,
                       ::mrrpc::TaskCompletedReply *response);

private:
  Master *m_;
};

#endif // MAPREDUCE_RPC_H
