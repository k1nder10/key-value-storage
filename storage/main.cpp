#include <spdlog/spdlog.h>

#include <asio/io_context.hpp>
#include <exception>
#include <memory>
#include <system_error>

#include "storage/core/btree_storage.hpp"
#include "storage/core/storage.hpp"
#include "storage/network/request_dispatcher.hpp"
#include "storage/network/server.hpp"

int main() {
  try {
    core::BtreeStorage btree_storage;
    core::Storage storage(btree_storage);

    constexpr unsigned port = 27901;
    network::RequestDispatcher dispatcher(storage);
    network::Server server(port, dispatcher);
    server.Start();
  }
  catch (const asio::system_error& e) {
    spdlog::warn("asio::system_error caught: {}.", e.what());
  }
  catch (const std::exception& e) {
    spdlog::warn("std::exception caught: {}.", e.what());
  }
  catch (...) {
    spdlog::warn("Unknown error caught.");
  }

  return 0;
}