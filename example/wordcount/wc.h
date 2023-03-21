//
// Created by 张旭 on 2023/3/15.
//

#ifndef MAPREDUCE_WC_H
#define MAPREDUCE_WC_H

#include "kv.hpp"
#include <string>
#include <vector>

class MapReduce {
public:
  static KeyValues Map(std::string filename, std::string contents);
  static std::string Reduce(std::string key, std::vector<std::string>);
};

#endif // MAPREDUCE_WC_H
