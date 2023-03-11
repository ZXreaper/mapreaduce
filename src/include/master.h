//
// Created by 张旭 on 2023/3/7.
//

#ifndef MAPREDUCE_MASTER_H
#define MAPREDUCE_MASTER_H

#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

enum STATE { MAP, REDUCE, EXIT, WAIT };
enum TASK_STATUS { IDLE, IN_PROGRESS, Completed };

// 任务类型
class Task {
public:
  Task(std::string input, int nreducer, STATE taskstate, TASK_STATUS taskstatus)
      : Input_(input), NReducer_(nreducer), TaskState_(taskstate),
        TaskStatus_(taskstatus) {}
  std::string Input_;                      // 任务所需要的输入
  std::string Output_;                     // 任务的输出
  std::vector<std::string> Intermediates_; // 中间结果
  int NReducer_;                           // 使用的reducer的编号
  STATE TaskState_;                        // 任务当前所处的阶段
  TASK_STATUS TaskStatus_; // 任务当前的状态：是空闲、在进行、完成
  time_t StartTime_;       // 任务启动的时间
};

class Master {
public:
  Master(STATE MasterPhrase, int nreduce, std::vector<std::string> inputfiles);

  static std::shared_ptr<Master> MakeMaster(std::vector<std::string> files,
                                            int NReducer);

  // rpc方法，Master给worker分配任务
  bool AssignTask();

  // Master启动
  bool StartServer();

  // Map过程
  bool createMapTask();

  // Reduce过程
  bool createReduceTask();

  // 判断任务是否完全结束
  bool Done();

  // 超时任务判断
  bool CatchTimeOut();

private:
  std::queue<std::shared_ptr<Task>> Task_que_; // 任务队列
  STATE MasterPhrase_;                         // Master所属阶段
  int NReduce_;                                // 总共使用多少个reducer
  std::vector<std::string> InputFiles_;        // 输入的文件。总的任务
  std::vector<std::vector<std::string>>
      Intermediates_; // Map任务产生的R个中间文件的信息
  std::mutex mtx_;    // 控制任务队列多线程的互斥锁
};

#endif // MAPREDUCE_MASTER_H
