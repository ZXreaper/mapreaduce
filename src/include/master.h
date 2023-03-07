//
// Created by 张旭 on 2023/3/7.
//

#ifndef MAPREDUCE_MASTER_H
#define MAPREDUCE_MASTER_H

#include <mutex>
#include <queue>
#include <string>
#include <vector>

enum STATE { MAP, REDUCE, EXIT, WAIT };
enum TASK_STATUS { IDLE, IN_PROGRESS, Completed };

// 任务类型
struct Task {
  std::string Input;                      // 任务所需要的输入
  std::string Output;                     // 任务的输出
  std::vector<std::string> Intermediates; // 中间结果
  int NReducer;                           // 使用的reducer的编号
  STATE TaskState;                        // 任务当前所处的阶段
  TASK_STATUS TaskStatus; // 任务当前的状态：是空闲、在进行、完成
  time_t StartTime;       // 任务启动的时间
};

class Master {
public:
  Master(STATE MasterPhrase, int nreduce, std::vector<std::string> inputfiles);

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

private:
  std::queue<Task> Task_que_;           // 任务队列
  STATE MasterPhrase_;                  // Master所属阶段
  int NReduce_;                         // 总共使用多少个reducer
  std::vector<std::string> InputFiles_; // 输入的文件。总的任务
  std::vector<std::vector<std::string>>
      Intermediates_; // Map任务产生的R个中间文件的信息
  std::mutex mtx_;    // 互斥锁
};

#endif // MAPREDUCE_MASTER_H
