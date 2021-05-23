#include "request_facade.hpp"

using namespace network;

RequestFacade::RequestFacade(std::shared_ptr<Request> req) : req_(req) {}

std::string RequestFacade::Serialize(RequestMethod method, Key key,
                                     std::optional<Value> value) {
  req_->set_method(method);
  req_->set_key(std::move(key));
  if (value)
    req_->set_value(value.value());
  return req_->SerializeAsString() + null_terminator;
}

bool RequestFacade::Deserialize(const void *buf, size_t size) {
  size -= 1;  // we do not encode the last character (e.g. '\0')
  return req_->ParseFromArray(buf, size);
}

bool RequestFacade::HasValue() const {
  return req_->has_value();
}

Key RequestFacade::GetKey() const {
  return req_->key();
}

Value RequestFacade::GetValue() const {
  return req_->value();
}

RequestMethod RequestFacade::GetRequestMethod() const {
  return req_->method();
}

void RequestFacade::Print() const {
  const auto req_method = RequestMethodToString(GetRequestMethod());
  spdlog::info("Request: {} <{}, {}>", req_method, GetKey(),
               HasValue() ? GetValue() : "null");
}

std::string RequestFacade::RequestMethodToString(RequestMethod method) {
  if (method == Request_Method_Set)
    return "Set";
  else if (method == Request_Method_Get)
    return "Get";
  else if (method == Request_Method_Delete)
    return "Delete";
  else if (method == Request_Method_Exists)
    return "Exists";
  else
    return "Unknown";
}