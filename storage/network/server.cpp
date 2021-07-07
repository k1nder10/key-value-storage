#include "server.hpp"

#include <spdlog/spdlog.h>

#include "request_dispatcher.hpp"

namespace network {

template <typename RequestDispatcher>
Server<RequestDispatcher>::Server(unsigned port, RequestDispatcher& dispatcher)
    : port_(port), request_dispatcher_(dispatcher) {}

template <typename RequestDispatcher>
void Server<RequestDispatcher>::Start() {
  spdlog::info("Server started working on port: {}", port_);

  ConnectionPtr client_conn =
      Connection<RequestDispatcher>::Create(io_context_, request_dispatcher_);

  acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address());
  acceptor_.async_accept(
      client_conn->Socket(),
      std::bind(&Server::HandleAccept, this, client_conn, std::placeholders::_1));

  io_context_.run();
}

template <typename RequestDispatcher>
void Server<RequestDispatcher>::HandleAccept(ConnectionPtr client_conn,
                                             const asio::error_code& ec) {
  if (!ec)
    client_conn->Start();
  else {
    spdlog::info("Accept error: {}", ec.message());
  }

  ConnectionPtr new_client_conn =
      Connection<RequestDispatcher>::Create(io_context_, request_dispatcher_);

  acceptor_.async_accept(
      new_client_conn->Socket(),
      std::bind(&Server::HandleAccept, this, new_client_conn, std::placeholders::_1));
}

}  // namespace network