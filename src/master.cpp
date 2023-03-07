#include "master.h"

Master::Master(STATE MasterPhrase, int nreduce,
               std::vector<std::string> inputfiles)
    : MasterPhrase_(MAP), NReduce_(nreduce) {
  InputFiles_ = inputfiles;
}

// rpc方法，Master给worker分配任务
bool Master::AssignTask() {}

// Master启动
bool Master::StartServer() {}

// Map过程
bool Master::createMapTask() {}

// Reduce过程
bool Master::createReduceTask() {}

// 判断任务是否完全结束
bool Master::Done() {}