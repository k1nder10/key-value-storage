#include "connection.hpp"

#include "storage/core/btree_storage.hpp"
#include "storage/core/storage.hpp"
#include "storage/network/request_dispatcher.hpp"

using namespace network;
using namespace core;

template class network::Connection<RequestDispatcher<Storage<BtreeStorage>>>;