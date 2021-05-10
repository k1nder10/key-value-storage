#pragma once
#include <array>
#include <memory>
#include <system_error>
#include <asio.hpp>
#include <asio/buffer.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>

namespace network {

class Connection : public std::enable_shared_from_this<Connection> {
public:
  using Ptr = std::shared_ptr<Connection>;

  static Ptr Create(asio::io_context& io_context) {
    return Ptr(new Connection(io_context));
  }

  asio::ip::tcp::socket& Socket() { return socket_; }

  void Start();
  bool IsStarted() const { return is_started_; }

  void Stop();
  
private:
  Connection(asio::io_context& io_context) : socket_(io_context) {}

  void WriteToSocket(const std::string& message);
  void ReadFromSocket();

  void HandleWrite(const std::error_code& ec, size_t bytes_transffered);
  void HandleRead(const std::error_code& ec, size_t bytes_accepted);

  asio::ip::tcp::socket socket_;
  bool is_started_{false};
  enum { buffer_size_ = 2048 };
  std::array<char, buffer_size_> read_buffer_{};
  std::array<char, buffer_size_> write_buffer_{};
};

}  // namespace network