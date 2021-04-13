#pragma once

namespace utils {

class NonCopyable {
protected:
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;

  NonCopyable(NonCopyable&&) = default;
  NonCopyable& operator=(NonCopyable&&) = default;
  
  NonCopyable() = default;
  ~NonCopyable() = default;
};

} // namespace utils