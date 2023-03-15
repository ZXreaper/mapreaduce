//
// Created by 张旭 on 2023/3/15.
//

#ifndef MAPREDUCE_WC_H
#define MAPREDUCE_WC_H

#include <string>
#include <vector>

struct KeyValue {
  std::string Key;
  std::string Value;
};

class MapReduce {
public:
  static std::vector<KeyValue> Map(std::string filename, std::string contents);
  static std::string Reduce(std::string key, std::vector<std::string>);
};

#endif // MAPREDUCE_WC_H
