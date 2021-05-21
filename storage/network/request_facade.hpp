#pragma once
#include <spdlog/spdlog.h>

#include <memory>

#include "request.pb.h"
#include "types.hpp"

namespace network {

class RequestFacade {
  using Key = std::string;
  using Value = std::string;
  using RequestMethod = Request::Method;

public:
  RequestFacade(std::shared_ptr<Request> req) : req_(req) {}

  std::string Serialize(RequestMethod method, Key key, Value value) {
    req_->set_method(method);
    req_->set_key(std::move(key));
    req_->set_value(std::move(value));
    return req_->SerializeAsString() + null_terminator;
  }

  bool Deserialize(const void* buf, size_t size) {
    size -= 1;  // we do not encode the last character (e.g. '\0')
    return req_->ParseFromArray(buf, size);
  }

  std::pair<Key, Value> GetKeyValuePair() const { return {req_->key(), req_->value()}; }
  RequestMethod GetRequestMethod() const { return req_->method(); }

  void Print() const {
    const auto& [key, value] = GetKeyValuePair();
    const auto req_method = RequestMethodToString(GetRequestMethod());
    spdlog::info("Request: {} <{}, {}>", req_method, key, value);
  }

  static std::string RequestMethodToString(RequestMethod method) {
    if (method == RequestMethod::Request_Method_Set)
      return "Set";
    else if (method == RequestMethod::Request_Method_Get)
      return "Get";
    else if (method == RequestMethod::Request_Method_Delete)
      return "Delete";
    else if (method == RequestMethod::Request_Method_Exists)
      return "Exists";
    else
      return "Unknown";
  }

private:
  std::shared_ptr<Request> req_{nullptr};
};

}  // namespace network