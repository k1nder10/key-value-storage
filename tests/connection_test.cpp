#include <gtest/gtest.h>
#include <array>
#include <asio.hpp>
#include <asio/ip/address.hpp>
#include <asio/ip/tcp.hpp>
#include <exception>
#include <iostream>
#include <system_error>
#include <vector>
#include <chrono>

#include "storage/network/connection.cpp"

TEST(ConnectionTest, StartConnection) {
  asio::io_context io_context;
  constexpr unsigned port = 27901;
}