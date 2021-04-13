#pragma once
#include <array>
#include <asio.hpp>
#include <asio/buffer.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <memory>
#include <system_error>

namespace network {

class Connection : public std::enable_shared_from_this<Connection> {
public:
  using Ptr = std::shared_ptr<Connection>;

  static Ptr Create(asio::io_context& io_context) {
    Connection::Ptr new_connection(new Connection(io_context));
    return new_connection;
  }

  asio::ip::tcp::socket& Socket() { return socket_; }

  void Start();
  bool IsStarted() const { return is_started_; }

  void Stop();
  
private:
  Connection(asio::io_context& io_context) : socket_(io_context) {}

  void HandleWrite(const std::error_code& ec, size_t bytes_transffered);
  void HandleRead(const std::error_code& ec, size_t bytes_accepted);

  asio::ip::tcp::socket socket_;
  bool is_started_{false};
  enum { buffer_size_ = 1024 };
  std::array<char, buffer_size_> read_buffer_{};
  std::array<char, buffer_size_> write_buffer_{};
};

}  // namespace network