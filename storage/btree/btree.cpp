#include "btree.hpp"

#include "btree_impl.hpp"

Btree::Btree() : pimpl_(new BtreeImpl()) {}
Btree::~Btree() = default;

Btree::OperationResult Btree::Set(const network::Key& key, const network::Value& value) {
  return pimpl_->Set(key, value);
}
Btree::OperationResult Btree::Get(const network::Key& key) {
  return pimpl_->Get(key);
}

Btree::OperationResult Btree::Exists(const network::Key& key) {
  return pimpl_->Exists(key);
}

Btree::OperationResult Btree::Delete(const network::Key& key) {
  return pimpl_->Delete(key);
}