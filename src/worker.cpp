//
// Created by 张旭 on 2023/3/7.
//
#include "worker.h"

// 加载map function 和 reduce function
void Worker::LoadPlugin(std::string filename) {}

// 启动worker端
void Worker::StartWorker() {}

// 获得任务。rpc方法
Task &Worker::GetTask() {}

// worker获得MapTask，交给mapper处理
void Worker::Mapper(Task &task,
                    std::function<KeyValues(std::string, std::string)> mapf) {}

// worker获得ReduceTask，交给reducer处理
void Worker::Reducer(
    Task &task,
    std::function<std::string(std::string, std::vector<std::string>)> reducef) {
}

// 将中间结果写到本地文件
std::string Worker::WriteToLocalFile(int x, int y, KeyValues &kvs) {}

// 从本地读取中间文件获得map阶段的key values
KeyValues &Worker::ReadFromLocalFile(std::vector<std::string> files) {}

// worker任务完成后通知master。rpc方法
void Worker::TaskCompleted(Task &task) {}