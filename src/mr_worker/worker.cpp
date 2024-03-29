//
// Created by 张旭 on 2023/3/7.
//
#include "worker.h"
#include "json.hpp"
#include "master.h"
#include "wc.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>
#include <unistd.h>
#include <unordered_map>

using json = nlohmann::json;

// // 从文件中加载map function 和 reduce function
// void Worker::LoadPlugin() {
//   mapf_ = MapReduce::Map;
//   reducef_ = MapReduce::Reduce;
// }

// 启动worker端
void Worker::StartWorker() {
  while (1) {
    // 从master获取任务
    Task task = GetTask();
    std::cout << "Worker::GetTask !" << std::endl;

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
  // args.set_assign_arg(1); // no use
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
    int interm_size = reply.intermediates_size();
    std::cout << "Worker::GetTask reply intermediates size : " << interm_size << std::endl;
    for(int i = 0; i<interm_size; i++) {
      task.Intermediates_.push_back(reply.intermediates(i).key_value_pair());
    }
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
  // KeyValues intermediates = mapf_(filename, content);
  KeyValues intermediates = MapReduce::Map(filename, content);

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
  std::cout << "Worker::Mapper Intermediates size : " << task.Intermediates_.size() << std::endl;
  std::cout << "Worker::Mapper mapoutput : " << task.Intermediates_[0] << std::endl;
  TaskCompleted(task);
}

// worker获得ReduceTask，交给reducer处理
void Worker::Reducer(Task &task) {
  std::cout << "Worker::Reducer !" << std::endl;
  std::cout << "Worker::Reducer task state : " << task.TaskState_ << std::endl;
  std::cout << "Worker::Reducer task Intermediates : " << task.Intermediates_.size() << std::endl;
  KeyValues intermediate = ReadFromLocalFile(task.Intermediates_);
  std::sort(intermediate.begin(), intermediate.end(),
            [&](KeyValue a, KeyValue b) -> bool { return a.Key < b.Key; });

  // 写到对应的output文件中
  char cwd_path[32];
  getcwd(cwd_path, 32);
  std::string dir = std::string(cwd_path) + "/mr-tmp";
  char outputfilename[32];
  sprintf(outputfilename, "/mr-out-%d", task.TaskNumber_);
  std::string file_path = dir + std::string(outputfilename);
  std::ofstream out(file_path, std::ios::app);

  // 将相同的key放在一起并分组合并
  int i = 0, j = i + 1;
  std::cout << "Worker::Reducer intermediate size : " << intermediate.size() << std::endl;
  while (i < intermediate.size()) {
    while (j < intermediate.size() &&
           (intermediate[i].Key == intermediate[j].Key))
      j++;
    std::vector<std::string> values;
    for (int k = i; k < j; k++) {
      values.push_back(intermediate[k].Value);
    }
    // 交给reducef，拿到结果
    // std::string output = reducef_(intermediate[i].Key, values);
    std::string output = MapReduce::Reduce(intermediate[i].Key, values);
    // std::cout << output << std::endl;
    out << (intermediate[i].Key + " " + output + "\n");
    i = j;
  }
  out.close();
  task.Output_ = file_path;
  TaskCompleted(task);
}

// 将中间结果写到本地文件
std::string Worker::WriteToLocalFile(int x, int y, KeyValues &kvs) {
  char file_path[32];
  getcwd(file_path, 32);
  // 创建文件夹
  std::string dir = std::string(file_path) + "/mr-tmp";
  // 创建文件夹
  if(!std::filesystem::exists(dir)) {
    if (!std::filesystem::create_directory(dir)) {
      std::cout << "create directories failed!" << std::endl;
      return "";
    }
  }
  // json序列化
  json js;
  std::unordered_map<std::string, std::string> hashtable;
  for (auto &kv : kvs) {
    hashtable.insert({kv.Key, kv.Value});
  }
  js["kvs"] = hashtable;
  std::string jsonstr = js.dump();
  char tempfilename[32];
  sprintf(tempfilename, "/mr-%d-%d", x, y);
  std::string json_file_path = dir + std::string(tempfilename);
  std::ofstream out(json_file_path, std::ios::trunc);
  int len = jsonstr.size();
  for (int i = 0; i < len; i++) {
    out << jsonstr[i];
  }
  out.close();
  return json_file_path;
}

// 从本地读取中间文件获得map阶段的key values
KeyValues Worker::ReadFromLocalFile(std::vector<std::string> files) {
  KeyValues kvs;
  for (auto filename : files) {
    // 读文件内容，并反序列化
    std::ifstream inFile;
    inFile.open(filename); // open the input file
    std::stringstream strStream;
    strStream << inFile.rdbuf();       // read the file
    std::string str = strStream.str(); // str holds the content of the file

    // 反序列化
    json res = json::parse(str);
    std::unordered_map<std::string, std::string> hashtable = res["kvs"];
    for (auto &[k, v] : hashtable) {
      kvs.push_back({k, v});
    }
  }
  return kvs;
}

// worker任务完成后通知master。rpc方法
void Worker::TaskCompleted(Task &task) {
  std::cout << task.TaskNumber_ << " completed!" << std::endl;
  mrrpc::RPCTask args;
  mrrpc::TaskCompletedReply reply;
  args.set_inputs(task.Input_);
  args.set_outputs(task.Output_);
  args.set_task_state(task.TaskState_);
  int intermediates_size = task.Intermediates_.size();
  for (int i = 0; i < intermediates_size; i++) {
    ::mrrpc::keyvalue* kv = args.add_intermediates();
    std::cout << "Worker::TaskCompleted task Intermediates_ : " << task.Intermediates_[i] << std::endl;
    kv->set_key_value_pair(task.Intermediates_[i]);
  }
  ClientContext context;
  std::cout << "Worker::TaskCompleted intermidiates size : " << args.intermediates_size() << std::endl;
  std::cout << "Worker::TaskCompleted intermidiates : " << args.intermediates(0).key_value_pair() << std::endl;
  Status status = stub_->TaskCompleted(&context, args, &reply);
  std::cout << "recived master reply!" << std::endl;
}