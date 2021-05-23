#include "btree_impl.hpp"

using OperationResult = BtreeImpl::OperationResult;

OperationResult BtreeImpl::Set(const network::Key& key, const network::Value& value) {
  return true;
}

OperationResult BtreeImpl::Get(const network::Key& key) {
  return true;
}

OperationResult BtreeImpl::Exists(const network::Key& key) {
  return true;
}

OperationResult BtreeImpl::Delete(const network::Key& key) {
  return true;
}