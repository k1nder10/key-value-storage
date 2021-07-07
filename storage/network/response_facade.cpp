#include "response_facade.hpp"

#include <spdlog/spdlog.h>

#include <optional>

#include "response.pb.h"
#include "storage/types.hpp"

namespace network {

ResponseFacade::ResponseFacade(std::shared_ptr<Response> resp) : resp_(resp) {}

std::string ResponseFacade::Serialize(StatusCode code, std::optional<Value> value) {
  resp_->set_code(code);
  if (value)
    resp_->set_value(std::move(value.value()));
  return resp_->SerializeAsString() + null_terminator;
}

bool ResponseFacade::Deserialize(const void* buf, size_t size) {
  size -= 1;  // we do not encode the last character (e.g. '\0')
  return resp_->ParseFromArray(buf, size);
}

bool ResponseFacade::HasValue() const {
  return resp_->has_value();
}

Value ResponseFacade::GetValue() const {
  return resp_->value();
}

StatusCode ResponseFacade::GetStatusCode() const {
  return resp_->code();
}

void ResponseFacade::Print() const {
  const auto code = StatusCodeToString(GetStatusCode());
  spdlog::info("Response: {} {}", code, HasValue() ? GetValue() : "null");
}

std::string ResponseFacade::StatusCodeToString(StatusCode code) {
  if (code == StatusCode::Response_StatusCode_Ok)
    return "Ok";
  else if (code == StatusCode::Response_StatusCode_Error)
    return "Error";
  else
    return "Unknown";
}

}  // namespace network