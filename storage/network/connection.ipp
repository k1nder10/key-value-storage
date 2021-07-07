#pragma once
#include <spdlog/spdlog.h>

#include <asio/buffer.hpp>
#include <string>

#include "connection.hpp"
#include "request.pb.h"
#include "request_facade.hpp"
#include "response.pb.h"
#include "response_facade.hpp"
#include "storage/result.hpp"

namespace network {

template <typename RequestDispatcher>
Connection<RequestDispatcher>::Connection(asio::io_context& io_context,
                                          RequestDispatcher& dispatcher)
    : socket_(io_context), request_dispatcher_(dispatcher) {}

template <typename RequestDispatcher>
typename Connection<RequestDispatcher>::Ptr Connection<RequestDispatcher>::Create(
    asio::io_context& io_context, RequestDispatcher& dispatcher) {
  return Ptr(new Connection(io_context, dispatcher));
}

template <typename RequestDispatcher>
asio::ip::tcp::socket& Connection<RequestDispatcher>::Socket() {
  return socket_;
}

template <typename RequestDispatcher>
void Connection<RequestDispatcher>::Start() {
  is_started_ = true;
  spdlog::info("New connection... {}:{}", socket_.remote_endpoint().address().to_string(),
               socket_.remote_endpoint().port());

  ReadFromSocket();
}

template <typename RequestDispatcher>
void Connection<RequestDispatcher>::WriteToSocket(Response::StatusCode code,
                                                  std::optional<Value> value) {
  ResponseFacade resp(std::make_shared<Response>());
  write_buffer_ = resp.Serialize(code, value);

  asio::async_write(socket_, asio::buffer(write_buffer_),
                    [self = this->shared_from_this()](const asio::error_code& ec,
                                                      size_t bytes_transferred) {
                      self->HandleWrite(ec, bytes_transferred);
                    });
}

template <typename RequestDispatcher>
void Connection<RequestDispatcher>::ReadFromSocket() {
  asio::async_read_until(socket_, read_buffer_, null_terminator,
                         [self = this->shared_from_this()](const asio::error_code& ec,
                                                           size_t bytes_accepted) {
                           self->HandleRead(ec, bytes_accepted);
                         });
}

template <typename RequestDispatcher>
void Connection<RequestDispatcher>::HandleWrite(
    const asio::error_code& ec, [[maybe_unused]] size_t bytes_transferred) {
  if (ec) {
    spdlog::warn("Write error: {}, code: {}", ec.message(), ec.value());
    Stop();
    return;
  }

  ReadFromSocket();
}

template <typename RequestDispatcher>
void Connection<RequestDispatcher>::HandleRead(const asio::error_code& ec,
                                               size_t bytes_accepted) {
  if (ec) {
    spdlog::warn("Read error: {}, code: {}", ec.message(), ec.value());
    Stop();
    return;
  }

  RequestFacade req(std::make_shared<Request>());
  if (!req.Deserialize(read_buffer_.data().data(), bytes_accepted)) {
    WriteToSocket(Response_StatusCode_Error);
    return;
  }
  read_buffer_.consume(bytes_accepted);  // clear read buffer

  req.Print();

  Result result = request_dispatcher_.dispatch(req);
  WriteToSocket(result.code, result.value);
}

template <typename RequestDispatcher>
void Connection<RequestDispatcher>::Stop() {
  if (is_started_) {
    spdlog::info("Closing connetion...");
    is_started_ = false;
    socket_.close();
  }
}

}  // namespace network