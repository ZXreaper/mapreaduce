//
// Created by 张旭 on 2023/3/12.
//
#include "rpc.h"
#include "master.h"

// worker给master发送请求，获取任务
Status RpcServiceImpl::AssignTask(ServerContext *context,
                                  const ::mrrpc::AssignTaskRequest *request,
                                  ::mrrpc::RPCTask *response) {}

// worker向master报告任务已经做完
Status RpcServiceImpl::TaskCompleted(ServerContext *context,
                                     const ::mrrpc::RPCTask *request,
                                     ::mrrpc::TaskCompletedReply *response) {}