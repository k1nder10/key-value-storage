#include "request_dispatcher.hpp"

#include "request_facade.hpp"
#include "storage/btree/btree.hpp"
#include "types.hpp"

using namespace network;

std::pair<StatusCode, std::optional<Value>> RequestDispatcher::dispatch(
    const RequestFacade& req) {
  RequestMethod method = req.GetRequestMethod();
  std::pair<StatusCode, std::optional<Value>> res{Response_StatusCode_Error, {}};
  Btree::OperationResult is_ok{};

  if (method == Request_Method_Set) {
    is_ok = btree_->Set(req.GetKey(), req.GetValue());
  }
  else if (method == Request_Method_Get) {
    is_ok = btree_->Get(req.GetKey());
  }
  else if (method == Request_Method_Exists) {
    is_ok = btree_->Exists(req.GetKey());
  }
  else if (method == Request_Method_Delete) {
    is_ok = btree_->Delete(req.GetKey());
  }

  if (is_ok)
    res.first = Response_StatusCode_Ok;

  return res;
}