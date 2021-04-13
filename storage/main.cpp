#include <exception>
#include <system_error>

#include <spdlog/spdlog.h>
#include <asio/io_context.hpp>

#include "storage/network/connection.hpp"
#include "storage/network/server.hpp"

int main() {
  try {
    asio::io_context io_context;
    constexpr unsigned port = 27901;
    network::Server server(io_context, port);

    server.Start();
    io_context.run();
  }
  catch (const std::system_error& e) {
    spdlog::warn("main(). Error caught!");
  }

  return 0;
}