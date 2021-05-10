#include <exception>
#include <system_error>

#include <spdlog/spdlog.h>
#include <asio/io_context.hpp>

#include "storage/network/server.hpp"

int main() {
  try {
    asio::io_context io_context;
    constexpr unsigned port = 27901;
    network::Server server(io_context, port);
    server.Start();
  }
  catch (const std::system_error& e) {
    spdlog::warn("Error caught: {}.", e.what());
  }
  catch (...) {
    spdlog::warn("Unknown error caught.");
  }

  return 0;
}