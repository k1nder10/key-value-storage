#pragma once
#include <memory>

#include "storage/network/types.hpp"
#include "storage/utils/noncopyable.hpp"

class BtreeImpl;

class Btree : utils::NonCopyable {
public:
  using OperationResult = bool;

  Btree();
  ~Btree();

  OperationResult Set(const network::Key& key, const network::Value& value);
  OperationResult Get(const network::Key& key);
  OperationResult Exists(const network::Key& key);
  OperationResult Delete(const network::Key& key);

private:
  std::unique_ptr<BtreeImpl> pimpl_;
};