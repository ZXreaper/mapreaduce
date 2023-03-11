//
// Created by 张旭 on 2023/3/7.
//
#include "master.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: wc inputfiles..." << std::endl;
    exit(EXIT_FAILURE);
  }

  // 读数据
  std::vector<std::string> files;
  for (int i = 1; argv[i] != NULL; i++) {
    std::string filename(argv[i]);
    files.emplace_back(filename);
  }

  // 创建master
  Master::MakeMaster(files, 10);

  // 结束后通知

  return 0;
}