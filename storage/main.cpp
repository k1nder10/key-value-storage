#include <spdlog/spdlog.h>
#include <asio/io_context.hpp>
#include <exception>
#include <system_error>

#include "storage/network/server.hpp"

int main() {
  try {
    constexpr unsigned port = 27901;
    network::Server server(port);
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