#pragma once
#include <string>

#include "types.hpp"

struct Result {
  StatusCode code;
  std::optional<Value> value;
};