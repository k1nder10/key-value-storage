#pragma once
#include <spdlog/spdlog.h>

#include <memory>

#include "request.pb.h"

namespace network {

class RequestFacade {
  using Key = std::string;
  using Value = std::string;
  using RequestMethod = Request::Method;

public:
  RequestFacade(std::shared_ptr<Request> req) : req_(req) {}

  bool Deserialize(const void* buf, size_t size) {
    size -= 1;  // we do not encode a last character (e.g. '\0') to the message
    return req_->ParseFromArray(buf, size);
  }

  std::pair<Key, Value> GetKeyValuePair() const { return {req_->key(), req_->value()}; }
  RequestMethod GetRequestMethod() const { return req_->method(); }

  void print() const {
    const auto& [key, value] = GetKeyValuePair();
    const auto req_method = RequestMethodToString(GetRequestMethod());

    spdlog::info("{} <{}, {}>", req_method, key, value);
  }

  static std::string RequestMethodToString(RequestMethod rm) {
    if (rm == RequestMethod::Request_Method_Set)
      return "Set";
    else if (rm == RequestMethod::Request_Method_Get)
      return "Get";
    else if (rm == RequestMethod::Request_Method_Delete)
      return "Delete";
    else if (rm == RequestMethod::Request_Method_Exists)
      return "Exists";
    else
      return "Unknown";
  }

private:
  std::shared_ptr<Request> req_{nullptr};
};

}  // namespace network