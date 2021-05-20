#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <asio.hpp>
#include <chrono>
#include <thread>

#include "storage/network/connection.cpp"
#include "storage/network/request_facade.hpp"

using namespace std::chrono_literals;
using tcp = asio::ip::tcp;

static constexpr unsigned port = 27901;
static const auto address = asio::ip::address::from_string("127.0.0.1");

TEST(SendMessage, SetCommand) {
  asio::io_context io_context;
  tcp::endpoint ep(address, port);
  asio::error_code ec;
  tcp::socket sock(io_context);

  sock.connect(ep, ec);
  ASSERT_EQ(ec.value(), 0);

  // 1. read greeting
  {
    asio::streambuf greeting;
    size_t bytes_accepted = asio::read_until(sock, greeting, null_terminator);
    EXPECT_EQ(ec.value(), 0);
    EXPECT_NE(bytes_accepted, 0);
    EXPECT_STREQ(static_cast<const char*>(greeting.data().data()), "Hello from server!");
  }

  // 2. send request
  {
    Request req;
    req.set_method(::network::Request_Method::Request_Method_Set);
    req.set_key("Hello");
    req.set_value("World");
    std::string message = req.SerializeAsString() + null_terminator;
    size_t bytes_transferred = asio::write(sock, asio::buffer(message), ec);
    EXPECT_NE(bytes_transferred, 0);
    EXPECT_EQ(ec.value(), 0);
  }

  // 3. read response
  {
    asio::streambuf response;
    size_t bytes_accepted = asio::read_until(sock, response, null_terminator);
    EXPECT_EQ(ec.value(), 0);
    EXPECT_NE(bytes_accepted, 0);
    EXPECT_STREQ(static_cast<const char*>(response.data().data()), "ok");
  }

  sock.close();
}