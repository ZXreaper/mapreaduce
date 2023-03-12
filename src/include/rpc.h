//
// Created by 张旭 on 2023/3/12.
//

#ifndef MAPREDUCE_RPC_H
#define MAPREDUCE_RPC_H

#include "mrrpcfunction.grpc.pb.h"
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class RpcServiceImpl final : public mrrpc::RpcAssignTask::Service {
  // worker给master发送请求，获取任务
  Status AssignTask(ServerContext *context,
                    const ::mrrpc::AssignTaskRequest *request,
                    ::mrrpc::RPCTask *response);
  // worker向master报告任务已经做完
  Status TaskCompleted(ServerContext *context, const ::mrrpc::RPCTask *request,
                       ::mrrpc::TaskCompletedReply *response);
};

#endif // MAPREDUCE_RPC_H
