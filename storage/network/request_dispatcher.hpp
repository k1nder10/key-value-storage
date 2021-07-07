#pragma once
#include "request_facade.hpp"
#include "storage/result.hpp"

namespace network {

template <typename Storage>
class RequestDispatcher {
public:
  RequestDispatcher(Storage& storage) : storage_(storage) {}

  Result dispatch(const RequestFacade& req);

private:
  Storage& storage_;
};

template <typename Storage>
Result RequestDispatcher<Storage>::dispatch(const RequestFacade& req) {
  RequestMethod method = req.GetRequestMethod();

  if (method == Request_Method_Set) {
    return storage_->Set(req.GetKey(), req.GetValue());
  }
  else if (method == Request_Method_Get) {
    return storage_->Get(req.GetKey());
  }
  else if (method == Request_Method_Exists) {
    return storage_->Exists(req.GetKey());
  }
  else if (method == Request_Method_Delete) {
    return storage_->Delete(req.GetKey());
  }
}

}  // namespace network