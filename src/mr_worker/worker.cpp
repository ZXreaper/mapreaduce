//
// Created by 张旭 on 2023/3/7.
//
#include "worker.h"
#include "master.h"
#include <unistd.h>

// 从文件中加载map function 和 reduce function
void Worker::LoadPlugin(std::string filename) {}

// 启动worker端
void Worker::StartWorker() {
  while (1) {
    // 从master获取任务
    Task task = GetTask();

    // 拿到task之后，根据task的state来判断
    // state = map：交给mapper
    // state = reduce：交给reducer
    // 额外加两个state，让worker等待(Wait) 或者 直接退出(Exit)
    switch (task.TaskState_) {
    case MAP:
      Mapper(task);
      break;
    case REDUCE:
      Reducer(task);
      break;
    case WAIT:
      sleep(5);
      break;
    case EXIT:
      return;
    }
  }
}

// 获得任务。rpc方法
Task &Worker::GetTask() {}

// worker获得MapTask，交给mapper处理
void Worker::Mapper(Task &task) {}

// worker获得ReduceTask，交给reducer处理
void Worker::Reducer(Task &task) {}

// 将中间结果写到本地文件
std::string Worker::WriteToLocalFile(int x, int y, KeyValues &kvs) {}

// 从本地读取中间文件获得map阶段的key values
KeyValues &Worker::ReadFromLocalFile(std::vector<std::string> files) {}

// worker任务完成后通知master。rpc方法
void Worker::TaskCompleted(Task &task) {}