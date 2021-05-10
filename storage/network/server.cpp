#include <spdlog/spdlog.h>
#include "server.hpp"

using namespace network;

void Server::Start() {
  spdlog::info("Server started working on port {}", port_);

  Connection::Ptr client_conn = Connection::Create(io_context_);
  acceptor_.async_accept(
      client_conn->Socket(),
      std::bind(&Server::HandleAccept, this, client_conn, std::placeholders::_1));

  io_context_.run();
}

void Server::HandleAccept(Connection::Ptr client_conn, const std::error_code& ec) {
  if (!ec)
    client_conn->Start();

  Connection::Ptr new_client_conn = Connection::Create(io_context_);
  acceptor_.async_accept(
      new_client_conn->Socket(),
      std::bind(&Server::HandleAccept, this, new_client_conn, std::placeholders::_1));
}