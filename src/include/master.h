//
// Created by 张旭 on 2023/3/7.
//

#ifndef MAPREDUCE_MASTER_H
#define MAPREDUCE_MASTER_H

#include "rpc.h"
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

enum STATE { MAP, REDUCE, EXIT, WAIT };
enum TASK_STATUS { IDLE, IN_PROGRESS, Completed };

// 任务类型
class Task {
public:
  Task() {}
  Task(std::string input, int nreducer, int taskno, STATE taskstate)
      : Input_(input), NReducer_(nreducer), TaskNumber_(taskno),
        TaskState_(taskstate) {}
  std::string Input_;                      // 任务所需要的输入
  std::string Output_;                     // 任务的输出
  std::vector<std::string> Intermediates_; // 中间结果
  int NReducer_;                           // 使用的reducer的编号
  STATE TaskState_;                        // 任务当前所处的阶段
  int TaskNumber_; // 任务标号，使用任务标号在哈希表中找到对应的任务信息
};

struct MasterTask {
  MasterTask() {}
  MasterTask(TASK_STATUS task_status, time_t time, std::shared_ptr<Task> taskref) 
  : TaskStatus_(task_status)
  , StartTime_(time)
  , TaskRef_(taskref)
  {}
  TASK_STATUS TaskStatus_; // 任务当前的状态：是空闲、在进行、完成
  time_t StartTime_;       // 任务启动的时间
  std::shared_ptr<Task> TaskRef_; // 任务的引用
};

class Master {
public:
  Master(STATE MasterPhrase, int nreduce, std::vector<std::string> inputfiles);

  static std::shared_ptr<Master> MakeMaster(std::vector<std::string> files,
                                            int NReducer);

  // rpc方法，Master给worker分配任务
  bool AssignTask(::mrrpc::RPCTask *response);

  // Master启动
  void StartServer();

  // Map过程
  void createMapTask();

  // Reduce过程
  void createReduceTask();

  // 判断任务是否完全结束
  bool Done();

  // worker任务完成后通知master。rpc方法
  bool TaskCompleted(const ::mrrpc::RPCTask *request);

  // 任务结束后调用
  void ProcessTaskResult(const ::mrrpc::RPCTask *request);

  // 判断任务是否都已完成
  bool AllTaskDone();

  // 超时任务判断
  void CatchTimeOut();

private:
  std::queue<std::shared_ptr<Task>> Task_que_; // 任务队列
  std::unordered_map<int, std::shared_ptr<MasterTask>>
      TaskMeta_;                        // 当前所有task的信息
  STATE MasterPhrase_;                  // Master所属阶段
  int NReduce_;                         // 总共使用多少个reducer
  std::vector<std::string> InputFiles_; // 输入的文件。总的任务
  std::vector<std::vector<std::string>>
      Intermediates_; // Map任务产生的R个中间文件的信息
  std::mutex mtx_;    // 控制任务队列多线程的互斥锁
};

#endif // MAPREDUCE_MASTER_H
