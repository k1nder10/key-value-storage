#pragma once
#include <optional>

#include "request_facade.hpp"
#include "storage/btree/btree.hpp"
#include "types.hpp"

namespace network {

class RequestDispatcher {
public:
  std::pair<StatusCode, std::optional<Value>> dispatch(const RequestFacade& req);

private:
  std::unique_ptr<Btree> btree_{new Btree()};
};

}  // namespace network