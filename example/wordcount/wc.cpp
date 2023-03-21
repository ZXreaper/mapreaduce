//
// Created by 张旭 on 2023/3/7.
//
#include "wc.h"
#include <iostream>
#include <string>

KeyValues MapReduce::Map(std::string filename,
                                     std::string contents) {
  KeyValues kvs;
  // 按照空格进行分割单词
  int len = contents.size();
  int i = 0, j = 0;
  while (j < len) {
    while (j < len && contents[j] != ' ') {
      j++;
    }
    std::string word = contents.substr(i, j - i);
    kvs.push_back({word, "1"});
    j++;
    i = j;
  }
  return kvs;
}

std::string MapReduce::Reduce(std::string key,
                              std::vector<std::string> values) {
  return std::to_string(values.size());
}