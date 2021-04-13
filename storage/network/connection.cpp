#include "connection.hpp"

#include <spdlog/spdlog.h>

#include <functional>
#include <iostream>
#include <string>

using namespace network;

void Connection::Start() {
  spdlog::info("New connection. Remote address: {}",
               socket_.remote_endpoint().address().to_string());
  asio::async_read(socket_, asio::buffer(read_buffer_),
                   std::bind(&Connection::HandleRead, shared_from_this(),
                             std::placeholders::_1, std::placeholders::_2));
}

void Connection::HandleWrite(const std::error_code& ec, size_t bytes_transferred) {
  if (ec) {
    spdlog::warn("HandleWrite error");
    return;
  }

  asio::async_read(socket_, asio::buffer(read_buffer_),
                   std::bind(&Connection::HandleRead, shared_from_this(),
                             std::placeholders::_1, std::placeholders::_2));
}

void Connection::HandleRead(const std::error_code& ec, size_t bytes_accepted) {
  if (ec) {
    spdlog::warn("HandleRead error: {}", ec.message());
    return;
  }

  std::string message(read_buffer_.begin(), bytes_accepted);
  spdlog::info("Message from client: {}", message);

  // some logic & async_write
  asio::async_write(socket_, asio::buffer("Got you!"),
                    std::bind(&Connection::HandleWrite, shared_from_this(),
                              std::placeholders::_1, std::placeholders::_2));
}

void Connection::Stop() {
  if (is_started_) {
    is_started_ = false;
    socket_.close();
  }
}