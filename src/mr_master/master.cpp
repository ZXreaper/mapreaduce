#include "master.h"
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
bool Master::AssignTask() {}

// Master启动：开启rpc监听
bool Master::StartServer() {}

// Map过程
bool Master::createMapTask() {
  // 根据传入的filename，每个文件对应一个map task
  int len = InputFiles_.size();
  // map任务加入队列阶段应该不需要互斥锁
  for (int idx = 0; idx < len; idx++) {
    Task_que_.push(
        std::make_shared<Task>(Task(InputFiles_[idx], idx, MAP, IDLE)));
  }
}

// Reduce过程
bool Master::createReduceTask() {}

// 判断任务是否完全结束
bool Master::Done() {}

// 超时任务判断
bool Master::CatchTimeOut() {
  std::thread catchtimeoutTask([&]() {

  });

  // 设置分离线程，守护线程
  catchtimeoutTask.detach();
}