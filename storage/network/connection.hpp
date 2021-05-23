#pragma once
#include <array>
#include <asio.hpp>
#include <asio/buffer.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <memory>
#include <optional>
#include <system_error>

#include "types.hpp"

namespace network {

class Connection : public std::enable_shared_from_this<Connection> {
public:
  using Ptr = std::shared_ptr<Connection>;

  static Ptr Create(asio::io_context& io_context) {
    return Ptr(new Connection(io_context));
  }

  asio::ip::tcp::socket& Socket() { return socket_; }

  void Start();
  void Stop();

private:
  Connection(asio::io_context& io_context) : socket_(io_context) {}

  void WriteToSocket(StatusCode code, std::optional<Value> value);
  void ReadFromSocket();

  void HandleWrite(const std::error_code& ec, size_t bytes_transffered);
  void HandleRead(const std::error_code& ec, size_t bytes_accepted);

  asio::ip::tcp::socket socket_;
  bool is_started_{false};
  asio::streambuf read_buffer_;
  std::string write_buffer_;
};

}  // namespace network