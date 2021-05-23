#pragma once

#include "storage/network/types.hpp"

class BtreeImpl {
public:
  using OperationResult = bool;

  OperationResult Set(const network::Key& key, const network::Value& value);
  OperationResult Get(const network::Key& key);
  OperationResult Exists(const network::Key& key);
  OperationResult Delete(const network::Key& key);
};