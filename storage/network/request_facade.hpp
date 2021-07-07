#pragma once
#include <spdlog/spdlog.h>

#include <memory>
#include <optional>

#include "request.pb.h"
#include "storage/types.hpp"

namespace network {

class RequestFacade {
public:
  RequestFacade(std::shared_ptr<Request> req);

  std::string Serialize(RequestMethod method, Key key, std::optional<Value> value = {});
  bool Deserialize(const void* buf, size_t size);

  Key GetKey() const;
  Value GetValue() const;
  RequestMethod GetRequestMethod() const;
  bool HasValue() const;

  void Print() const;
  static std::string RequestMethodToString(RequestMethod method);

private:
  std::shared_ptr<Request> req_{nullptr};
};

}  // namespace network