#include "master.h"
#include "rpc.h"
#include <ctime>
#include <mutex>
#include <thread>
#include <vector>

Master::Master(STATE MasterPhrase, int nreduce,
               std::vector<std::string> inputfiles)
    : MasterPhrase_(MAP), NReduce_(nreduce) {
  InputFiles_ = inputfiles;
  int files_cnt = inputfiles.size();
  for (int i = 0; i < files_cnt; i++) {
    Intermediates_.push_back(std::vector<std::string>());
  }
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

  // 启动一个线程检查超时任务
  m->CatchTimeOut();

  return m;
}

// rpc方法，Master给worker分配任务
bool Master::AssignTask(::mrrpc::RPCTask *response) {
  std::lock_guard<std::mutex> g(mtx_);
  if (!Task_que_.empty()) {
    std::shared_ptr<Task> cur_task = Task_que_.front();
    Task_que_.pop();
    std::cout << "cur task state : " << cur_task->TaskState_ << std::endl;
    response->set_inputs(cur_task->Input_);
    response->set_nreducer(cur_task->NReducer_);
    response->set_task_state(cur_task->TaskState_);
    response->set_task_no(cur_task->TaskNumber_);
    int interm_size = cur_task->Intermediates_.size();
    std::cout << "Master::AssignTask intermediates(org) size : " << response->intermediates_size() << std::endl;
    for(int i = 0; i<interm_size; i++) {
      ::mrrpc::keyvalue* kv = response->add_intermediates();
      // kv->AppendToString(&cur_task->Intermediates_[i]);
      kv->set_key_value_pair(cur_task->Intermediates_[i]);
    }
    std::cout << "Master::AssignTask intermediates size : " << response->intermediates_size() << std::endl;
    TaskMeta_[cur_task->TaskNumber_]->TaskStatus_ = IN_PROGRESS;
    TaskMeta_[cur_task->TaskNumber_]->StartTime_ = time(0);
  } else if (MasterPhrase_ == EXIT) {
    response->set_task_state(EXIT);
  } else {
    response->set_task_state(WAIT);
  }
  // std::cout << "AssignTask unlock" << std::endl;
  return true;
}

// Master启动：开启rpc监听，server->Wait()会阻塞住，因此我们要开启一个线程来做监听
void Master::StartServer() {
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
    // std::cout << "!!!" << std::endl;
  });

  rpc_server.detach();
  // std::cout << "?????" << std::endl;
}

// Map过程
void Master::createMapTask() {
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
void Master::createReduceTask() {
  // 进入reduce阶段后，TaskMeta_需要清空，重新为当前的任务设置状态
  std::cout << "Master::creatReduceTask !" << std::endl;
  TaskMeta_.clear();
  int file_cnt = Intermediates_.size();
  std::cout << "file_cnt : " << file_cnt << std::endl;
  for(int i = 0; i<file_cnt; i++) {
    std::shared_ptr<Task> task(new Task());
    task->TaskState_ = REDUCE;
    task->NReducer_ = NReduce_;
    task->TaskNumber_ = i;
    task->Intermediates_ = Intermediates_[i];
    std::cout << "Master::createReduceTask task intermediates_(org) size : " << Intermediates_[i].size() << std::endl;
    std::cout << "Master::createReduceTask task intermediates_ size : " << task->Intermediates_.size() << std::endl;
    Task_que_.push(task);
    TaskMeta_[i] = std::make_shared<MasterTask> (IDLE, 0, task);
  }
}

// 判断任务是否完全结束
bool Master::Done() {
  // TODO: 搞清楚函数结束前释放锁和函数结束后释放锁的区别。
  // TODO: 这个位置为什么要加锁
  // std::cout << "Master::Done lock" << std::endl;
  std::lock_guard<std::mutex> g(mtx_);
  bool ans = (MasterPhrase_ == EXIT);
  // std::cout << "Master::Done unlock" << std::endl;
  return ans;
}

// worker任务完成后通知master。rpc方法
// master收到完成后的Task，如果所有的MapTask都已经完成，
// 创建ReduceTask,转入ReduceTask，转入Reduce阶段
// 如果所有的ReduceTask都已经完成，转入Exit阶段
bool Master::TaskCompleted(const ::mrrpc::RPCTask *request) {
  // std::cout << "TaskCompleted lock" << std::endl;
  mtx_.lock();
  // std::cout << "Master recived task result" << std::endl;
  int task_no = request->task_no();
  int task_state = request->task_state();
  std::cout << "TaskCompleted : " << std::endl;
  std::cout << "----- task state : " << task_state << std::endl;
  std::cout << "----- Master Phrase : " << MasterPhrase_ << std::endl;
  if (task_state != MasterPhrase_ || TaskMeta_[task_no]->TaskStatus_ == Completed) {
    // 因为worker写在同一个文件磁盘上，对于重复的结果要丢弃
    mtx_.unlock();   // 记得释放锁
    return true;
  }
  TaskMeta_[task_no]->TaskStatus_ = Completed;
  mtx_.unlock();
  // std::cout << "TaskCompleted unlock" << std::endl;
  return true;
}

// 任务结束后调用
void Master::ProcessTaskResult(const ::mrrpc::RPCTask *request) {
  std::cout << "process task result!" << std::endl;
  std::cout << "ProcessTaskResult lock" << std::endl;
  std::lock_guard<std::mutex> g(mtx_);
  int task_state = request->task_state();
  switch (task_state) {
  case MAP: {
    // 收集intermediate信息
    int intermediates_size = request->intermediates_size();
    std::cout << "Master::ProcessTaskResult : " << std::endl;
    for (int i = 0; i < intermediates_size; i++) {
      Intermediates_[i].push_back(request->intermediates(i).key_value_pair());
      std::cout << "-----intermidiates : " << request->intermediates(i).key_value_pair() << std::endl;
    }
    std::cout << "Master::ProcessTaskResult Intermediates_ size : " << Intermediates_.size() << std::endl;
    if (AllTaskDone()) {
      // 获得所有map task后，进入reduce阶段
      std::cout << "MAP tasks have already done!" << std::endl;
      createReduceTask();
      MasterPhrase_ = REDUCE;
    }
    break;
  }
  case REDUCE: {
    if (AllTaskDone()) {
      // 获得所有reduce task后，进入exit阶段
      std::cout << "REDUCE tasks have already done!" << std::endl;
      MasterPhrase_ = EXIT;
    }
    break;
  }
  }
  std::cout << "ProcessTaskResult unlock" << std::endl;
}

// 判断任务是否都已完成
bool Master::AllTaskDone() {
  for (auto [k, v] : TaskMeta_) {
    if (v->TaskStatus_ != Completed) {
      return false;
    }
  }
  return true;
}

// 超时任务判断
void Master::CatchTimeOut() {
  std::thread catchtimeoutTask([&]() {

  });

  catchtimeoutTask.detach();
}