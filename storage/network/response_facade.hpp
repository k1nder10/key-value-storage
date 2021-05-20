#pragma once
#include <memory>

#include "response.pb.h"

namespace network {

class ResponseFacade {
public:
  ResponseFacade(std::shared_ptr<Response> resp) : resp_(resp) {}

  bool Deserialize(const void* buf, size_t size) {
    size -= 1;  // we do not encode a last character (e.g. '\0') to the message
    return resp_->ParseFromArray(buf, size);
  }

private:
  std::shared_ptr<Response> resp_{nullptr};
};

}  // namespace network