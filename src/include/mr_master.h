//
// Created by 张旭 on 2023/3/7.
//

#ifndef MAPREDUCE_MR_MASTER_H
#define MAPREDUCE_MR_MASTER_H

#include "master.h"

class MasterServer {
public:
  // 创建Master。整个流程的开始
  Master *MakeMaster(std::vector<std::string> files, int NReducer);
};

#endif // MAPREDUCE_MR_MASTER_H
