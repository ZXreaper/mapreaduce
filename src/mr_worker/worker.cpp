//
// Created by 张旭 on 2023/3/7.
//
#include "worker.h"
#include "master.h"
#include "wc.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <unistd.h>

// 从文件中加载map function 和 reduce function
void Worker::LoadPlugin() {
  mapf_ = MapReduce::Map;
  reducef_ = MapReduce::Reduce;
}

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
Task Worker::GetTask() {
  mrrpc::AssignTaskRequest args;
  mrrpc::RPCTask reply;
  args.set_assign_arg(1); // no use
  ClientContext context;
  Status status = stub_->AssignTask(&context, args, &reply);

  if (status.ok()) {
    std::string inputs = *(reply.mutable_inputs());
    int task_no = reply.task_no();
    STATE state;
    switch (reply.task_state()) {
    case 0:
      state = MAP;
      break;
    case 1:
      state = REDUCE;
      break;
    case 2:
      state = EXIT;
      break;
    case 3:
      state = WAIT;
      break;
    }
    int nreduce = reply.nreducer();
    Task task = Task(inputs, nreduce, task_no, state);
    return task;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return Task{};
  }
}

std::string Worker::ReadFile(std::string filename) {
  char line[256];
  std::ifstream infile;
  infile.open(filename);
  if (!infile) {
    std::cout << "load file failed!" << std::endl;
    return "";
  }
  std::string contents = "";
  while (infile.getline(line, 256, '\n')) {
    std::string cur(line);
    int len = cur.size();
    int i = 0, j = 0;
    while (j < len) {
      // 先找到第一个不是空格的位置
      while (i < len && cur[i] == ' ') {
        i++;
      }
      // 这行是空的
      if (i == len)
        break;
      j = i;
      while (j < len && cur[j] != ' ') {
        j++;
      }
      std::string word = cur.substr(i, j - i);
      contents += word + " ";
      i = j;
    }
  }
  infile.close();
  return contents;
}

// 将字符串映射成int
int ihash(std::string key) {
  std::hash<std::string> str_hash;
  int ans = static_cast<int>(str_hash(key) & 0x7fffffff);
  return ans;
}

// worker获得MapTask，交给mapper处理
void Worker::Mapper(Task &task) {
  std::string filename = task.Input_;
  std::string content = Worker::ReadFile(filename);
  if (content == "") {
    return;
  }
  // 将content交给mapf，缓存结果
  KeyValues intermediates = mapf_(filename, content);

  // 缓存后的结果会写到本地磁盘，并切成R份
  // 切分方式是根据key做hash
  std::vector<KeyValues> buffer(task.NReducer_);
  for (auto intermediate : intermediates) {
    int slot = ihash(intermediate.Key) % task.NReducer_;
    buffer[slot].push_back(intermediate);
  }
  std::vector<std::string> mapOutput; // 存储map阶段结果所存储在的文件地址
  for (int i = 0; i < task.NReducer_; i++) {
    std::string outfilepath = WriteToLocalFile(task.TaskNumber_, i, buffer[i]);
    mapOutput.push_back(outfilepath);
  }
  task.Intermediates_ = mapOutput;
  TaskCompleted(task);
}

// worker获得ReduceTask，交给reducer处理
void Worker::Reducer(Task &task) {}

// 将中间结果写到本地文件
std::string Worker::WriteToLocalFile(int x, int y, KeyValues &kvs) {
  char file_path[32];
  getcwd(file_path, 32);
  // 创建文件夹
  std::string dir = std::string(file_path) + "/mr-tmp";
  // 创建文件夹
  if (0 != std::filesystem::create_directory(dir)) {
    // 返回 0 表示创建成功，-1 表示失败
    std::cout << "create directories failed!" << std::endl;
  }
  // TODO: json序列化key value
  std::string msg = "hello world!";
  std::ofstream out(file_path, std::ios::trunc);
  int len = msg.size();
  for (int i = 0; i < len; i++) {
    out << msg[i];
  }
  out.close();
}

// 从本地读取中间文件获得map阶段的key values
KeyValues &Worker::ReadFromLocalFile(std::vector<std::string> files) {}

// worker任务完成后通知master。rpc方法
void Worker::TaskCompleted(Task &task) {
  mrrpc::RPCTask args;
  mrrpc::TaskCompletedReply reply;
  args.set_inputs(task.Input_);
  args.set_outputs(task.Output_);
  int intermediates_size = args.intermediates_size();
  for (int i = 0; i < intermediates_size; i++) {
    args.mutable_intermediates(i)->AppendToString(&task.Intermediates_[i]);
  }
  ClientContext context;
  Status status = stub_->TaskCompleted(&context, args, &reply);
}