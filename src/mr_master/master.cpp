#include "master.h"
#include "rpc.h"
#include <ctime>
#include <mutex>
#include <thread>

Master::Master(STATE MasterPhrase, int nreduce,
               std::vector<std::string> inputfiles)
    : MasterPhrase_(MAP), NReduce_(nreduce) {
  InputFiles_ = inputfiles;
}

// 创建Master。整个流程的开始
std::shared_ptr<Master> Master::MakeMaster(std::vector<std::string> files,
                                           int NReducer) {
  // 初始化Master
  std::shared_ptr<Master> m(new Master(MAP, NReducer, files));

  // 开始Map阶段
  m->createMapTask();

  // 启动master服务
  m->StartServer();

  // 开始Reduce阶段
  m->createReduceTask();

  // 启动一个线程检查超时任务
  m->CatchTimeOut();

  return m;
}

// rpc方法，Master给worker分配任务
bool Master::AssignTask(::mrrpc::RPCTask *response) {
  mtx_.lock();
  if (!Task_que_.empty()) {
    std::shared_ptr<Task> cur_task = Task_que_.front();
    Task_que_.pop();
    response->set_inputs(cur_task->Input_);
    response->set_nreducer(cur_task->NReducer_);
    response->set_task_state(cur_task->TaskState_);
    response->set_task_no(cur_task->TaskNumber_);
    TaskMeta_[cur_task->TaskNumber_]->TaskStatus_ = IN_PROGRESS;
    TaskMeta_[cur_task->TaskNumber_]->StartTime_ = time(0);
  } else if (MasterPhrase_ == EXIT) {
    response->set_task_state(EXIT);
  } else {
    response->set_task_state(WAIT);
  }
  mtx_.unlock();
  return true;
}

// Master启动：开启rpc监听，server->Wait()会阻塞住，因此我们要开启一个线程来做监听
bool Master::StartServer() {
  std::thread rpc_server([&]() {
    std::string server_address("127.0.0.1:50051");
    RpcServiceImpl service;
    service.SetMaster(this);

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

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    // Block until the server shuts down.
    server->Wait();
  });

  rpc_server.detach();
}

// Map过程
bool Master::createMapTask() {
  // 根据传入的filename，每个文件对应一个map task
  int len = InputFiles_.size();
  // map任务加入队列阶段应该不需要互斥锁
  for (int idx = 0; idx < len; idx++) {
    std::shared_ptr<Task> m(new Task(InputFiles_[idx], NReduce_, idx, MAP));
    Task_que_.push(m);
    std::shared_ptr<MasterTask> mt(new MasterTask());
    mt->TaskStatus_ = IDLE;
    mt->TaskRef_ = m;
    TaskMeta_[idx] = mt;
  }
}

// Reduce过程
bool Master::createReduceTask() {}

// 判断任务是否完全结束
bool Master::Done() {}

// worker任务完成后通知master。rpc方法
void Master::TaskCompleted(::mrrpc::RPCTask *request) {}

// 超时任务判断
bool Master::CatchTimeOut() {
  std::thread catchtimeoutTask([&]() {

  });

  // 设置分离线程，守护线程
  catchtimeoutTask.detach();
}