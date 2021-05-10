#pragma once
#include <asio.hpp>
#include "connection.hpp"

namespace network {

class Server {
public:
  Server(asio::io_context& io_context, unsigned port)
      : io_context_(io_context),
        acceptor_(io_context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
        port_(port) {}

  // blocking call
  void Start();

private:
  void HandleAccept(network::Connection::Ptr client_conn, const std::error_code& ec);

  asio::io_context& io_context_;
  asio::ip::tcp::acceptor acceptor_;
  const unsigned port_{27901};
};

}  // namespace network