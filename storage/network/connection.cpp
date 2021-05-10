#include "connection.hpp"

#include <spdlog/spdlog.h>

#include <asio/buffer.hpp>
#include <string>

#include "command.pb.h"
#include "command_facade.hpp"

using namespace network;

void Connection::Start() {
  spdlog::info("New connection. Remote address: {}",
               socket_.remote_endpoint().address().to_string());

  std::string init_message =
      "You can find how to use server on https://github.com/k1nder10/key-value-storage";

  asio::async_write(socket_, asio::buffer(init_message),
                    std::bind(&Connection::HandleWrite, shared_from_this(),
                              std::placeholders::_1, std::placeholders::_2));
}

void Connection::HandleWrite(const std::error_code& ec,
                             [[maybe_unused]] size_t bytes_transferred) {
  if (ec) {
    spdlog::warn("HandleWrite error: {}", ec.message());
  }

  ReadFromSocket();
}

void Connection::HandleRead(const std::error_code& ec, size_t bytes_accepted) {
  if (bytes_accepted == 0)
    return;

  const bool eof = ec.value() == 2;
  if (ec && !eof)
    spdlog::info("HandleRead error: {}", ec.message());

  std::string message(read_buffer_.begin(), bytes_accepted);
  CommandFacade cmd(new Command);
  if (!cmd.Deserialize(message)) {
    spdlog::warn("Client's message is corrupted. Couldn't deserialize it.");
    WriteToSocket("I don't undestand you.");
  }

  const auto cmd_type = cmd.GetType();
  const auto [key, value] = cmd.GetKeyValuePair();
  spdlog::info("Client's command: {} [{}, {}]", cmd_type, key, value);
  WriteToSocket("Got you.");
}

void Connection::ReadFromSocket() {
  asio::async_read(socket_, asio::buffer(read_buffer_),
                   std::bind(&Connection::HandleRead, shared_from_this(),
                             std::placeholders::_1, std::placeholders::_2));
}

void Connection::WriteToSocket(const std::string& message) {
  asio::async_write(socket_, asio::buffer(message),
                    std::bind(&Connection::HandleWrite, shared_from_this(),
                              std::placeholders::_1, std::placeholders::_2));
}

void Connection::Stop() {
  if (is_started_) {
    is_started_ = false;
    socket_.close();
  }
}