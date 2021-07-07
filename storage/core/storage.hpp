#pragma once
#include <memory>
#include <string>

#include "storage/result.hpp"
#include "storage/utils/noncopyable.hpp"

namespace core {

template <typename Strategy>
class Storage : utils::NonCopyable {
public:
  Storage(Strategy& strategy) : strategy_(strategy) {}
  ~Storage() = default;

  Storage(Storage&&) = default;
  Storage& operator=(Storage&&) = default;

  Result Set(const Key& key, const Value& value) {
    return strategy_.Set(key, value);
  }

  Result Get(const Key& key) {
    return strategy_.Get(key);
  }

  Result Exists(const Key& key) {
    return strategy_.Exists(key);
  }

  Result Delete(const Key& key) {
    return strategy_.Delete(key);
  }

private:
  Strategy& strategy_;
};

}  // namespace core