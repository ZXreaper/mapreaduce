//
// Created by 张旭 on 2023/3/7.
//
#include "wc.h"
#include <iostream>
#include <string>

void filter(std::string& tar) {
    int len = tar.size();
    int i = 0, j = 0;
    while(j<len) {
        if(!((tar[j] >= 'a' && tar[j] <= 'z') || (tar[j] >= 'A' && tar[j] <= 'Z'))) {
            j++;
        } else {
            std::swap(tar[i], tar[j]);
            i++, j++;
        }
    }
    tar = tar.substr(0, i);
}

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
    filter(word);
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