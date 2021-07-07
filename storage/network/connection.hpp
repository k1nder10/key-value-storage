#ifndef STORAGE_NETWORK_CONNECTION_HPP
#define STORAGE_NETWORK_CONNECTION_HPP

#include <array>
#include <asio.hpp>
#include <memory>
#include <optional>
#include <system_error>

#include "storage/types.hpp"

namespace network {

template <typename RequestDispatcher>
class Connection : public std::enable_shared_from_this<Connection<RequestDispatcher>> {
public:
  using Ptr = std::shared_ptr<Connection>;

  static Ptr Create(asio::io_context& io_context, RequestDispatcher& dispatcher);

  asio::ip::tcp::socket& Socket();

  void Start();
  void Stop();

private:
  Connection(asio::io_context& io_context, RequestDispatcher& dispatcher);

  void WriteToSocket(StatusCode code, std::optional<Value> value = {});
  void ReadFromSocket();

  void HandleWrite(const std::error_code& ec, size_t bytes_transffered);
  void HandleRead(const std::error_code& ec, size_t bytes_accepted);

  asio::ip::tcp::socket socket_;
  bool is_started_{false};
  asio::streambuf read_buffer_;
  std::string write_buffer_;
  RequestDispatcher& request_dispatcher_;
};

}  // namespace network

#include "connection.ipp"

#endif