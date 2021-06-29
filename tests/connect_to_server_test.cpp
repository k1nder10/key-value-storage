#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <asio.hpp>

using tcp = asio::ip::tcp;

static constexpr unsigned port = 27901;
static const auto address = asio::ip::address::from_string("127.0.0.1");

TEST(ConnectionTest, StartSingleSyncConnection) {
  asio::io_context io_context;
  tcp::endpoint ep(address, port);
  asio::error_code ec;
  tcp::socket sock(io_context);

  sock.connect(ep, ec);
  ASSERT_EQ(ec.value(), 0);
  sock.close();
}

TEST(ConnectionTest, StartMultipleSyncConnections) {
  asio::io_context io_context;
  tcp::endpoint ep(address, port);
  asio::error_code ec;
  std::vector<std::unique_ptr<tcp::socket>> sockets;

  const size_t number_of_connections = 5;
  for (size_t i = 0; i < number_of_connections; ++i) {
    std::unique_ptr<tcp::socket> sock_ptr(new tcp::socket(io_context));
    sockets.push_back(std::move(sock_ptr));
  }

  for (auto& sock : sockets) {
    sock->connect(ep, ec);
    ASSERT_EQ(ec.value(), 0);
    sock->close();
  }
}

struct AsyncSocket {
  std::unique_ptr<tcp::socket> sock;
  asio::error_code ec;
};

TEST(ConnectionTest, StartMultipleAsyncConnections) {
  std::vector<std::string> messages = {
      "Hello", "World", "from", "multiple", "async", "connections", "test",
      "Hello", "World", "from", "multiple", "async", "connections", "test"};

  asio::io_context io_context;
  tcp::endpoint ep(address, port);
  std::vector<AsyncSocket> sockets;

  const size_t number_of_connections = 5;
  for (size_t i = 0; i < number_of_connections; ++i) {
    std::unique_ptr<tcp::socket> sock_ptr(new tcp::socket(io_context));
    sockets.push_back({std::move(sock_ptr), {}});
  }

  for (auto& sock : sockets) {
    sock.sock->connect(ep, sock.ec); // TODO: change to async connect
  }
  io_context.run();

  for (auto& sock : sockets) {
    ASSERT_EQ(sock.ec.value(), 0);
    sock.sock->close();
  }
}