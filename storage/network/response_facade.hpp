#pragma once

#include <memory>
#include <optional>
#include <string>

#include "response.pb.h"
#include "types.hpp"

namespace network {

class ResponseFacade {
public:
  ResponseFacade(std::shared_ptr<Response> resp);
  
  std::string Serialize(StatusCode code, std::optional<Value> value = {});
  bool Deserialize(const void* buf, size_t size);

  bool HasValue() const;
  Value GetValue() const;
  StatusCode GetStatusCode() const;

  void Print() const;
  static std::string StatusCodeToString(StatusCode code);

private:
  std::shared_ptr<Response> resp_{nullptr};
};

}  // namespace network