#include "server.hpp"

#include <spdlog/spdlog.h>

using namespace network;

void Server::Start() {
  spdlog::info("Server started working on port {}", port_);

  network::Connection::Ptr client_conn = network::Connection::Create(io_context_);
  acceptor_.async_accept(
      client_conn->Socket(),
      std::bind(&Server::HandleAccept, this, client_conn, std::placeholders::_1));
}

void Server::HandleAccept(network::Connection::Ptr client_conn,
                          const std::error_code& ec) {
  if (ec) {
    spdlog::warn("HandleAccept error: {}", ec.message());
  };

  client_conn->Start();

  network::Connection::Ptr new_client_conn = network::Connection::Create(io_context_);
  acceptor_.async_accept(
      new_client_conn->Socket(),
      std::bind(&Server::HandleAccept, this, new_client_conn, std::placeholders::_1));
}