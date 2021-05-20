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

TEST(SendMessage, ReadGreeting) {
  asio::io_context io_context;
  tcp::endpoint ep(address, port);
  asio::error_code ec;
  tcp::socket sock(io_context);

  sock.connect(ep, ec);
  ASSERT_EQ(ec.value(), 0);

  asio::streambuf greeting;
  size_t bytes_accepted = asio::read_until(sock, greeting, null_terminator);
  ASSERT_EQ(ec.value(), 0);
  ASSERT_NE(bytes_accepted, 0);
  ASSERT_STREQ(static_cast<const char*>(greeting.data().data()), "Hello from server!");
  sock.close();
}