#pragma once
#include <asio.hpp>
#include <memory>

#include "connection.hpp"
#include "storage/utils/noncopyable.hpp"

namespace network {

template <typename RequestDispatcher>
class Server : utils::NonCopyable {
public:
  Server(unsigned port, RequestDispatcher& dispatcher);

  // blocking call
  void Start();

private:
  using ConnectionPtr = typename Connection<RequestDispatcher>::Ptr;

  void HandleAccept(ConnectionPtr client_conn, const asio::error_code& ec);

  const unsigned port_;
  asio::io_context io_context_;
  asio::ip::tcp::acceptor acceptor_{io_context_,
                                    asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_)};
  RequestDispatcher& request_dispatcher_;
};

}  // namespace network