#pragma once
#include <asio.hpp>

#include "connection.hpp"

namespace network {

class Server {
public:
  Server(unsigned port) : port_(port) {}

  // blocking call
  void Start();

private:
  void HandleAccept(network::Connection::Ptr client_conn, const asio::error_code& ec);

  const unsigned port_;
  asio::io_context io_context_;
  asio::ip::tcp::acceptor acceptor_{io_context_,
                                    asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_)};
};

}  // namespace network