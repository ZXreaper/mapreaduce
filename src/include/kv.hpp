#pragma once

#include <string>
#include <vector>

struct KeyValue {
  std::string Key;
  std::string Value;
};

typedef std::vector<KeyValue> KeyValues;