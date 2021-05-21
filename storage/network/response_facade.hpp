#pragma once
#include <spdlog/spdlog.h>

#include <memory>
#include <optional>

#include "response.pb.h"
#include "types.hpp"

namespace network {

class ResponseFacade {
  using StatusCode = Response::StatusCode;
  using Value = std::string;

public:
  ResponseFacade(std::shared_ptr<Response> resp) : resp_(resp) {}

  std::string Serialize(StatusCode code, std::optional<Value> value = {}) {
    resp_->set_code(code);
    if (value)
      resp_->set_value(std::move(value.value()));
    return resp_->SerializeAsString() + null_terminator;
  }

  bool Deserialize(const void* buf, size_t size) {
    size -= 1;  // we do not encode the last character (e.g. '\0')
    return resp_->ParseFromArray(buf, size);
  }

  bool HasValue() const { return resp_->has_value(); }
  std::string GetValue() const { return resp_->value(); }
  StatusCode GetStatusCode() const { return resp_->code(); }

  void Print() const {
    const auto code = StatusCodeToString(GetStatusCode());
    spdlog::info("Response: {} {}", code, HasValue() ? GetValue() : "null");
  }

  static std::string StatusCodeToString(StatusCode code) {
    if (code == StatusCode::Response_StatusCode_Ok)
      return "Ok";
    else if (code == StatusCode::Response_StatusCode_Error)
      return "Error";
    else
      return "Unknown";
  }

private:
  std::shared_ptr<Response> resp_{nullptr};
};

}  // namespace network