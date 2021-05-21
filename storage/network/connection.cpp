#include "connection.hpp"

#include <spdlog/spdlog.h>

#include <asio/buffer.hpp>
#include <string>

#include "request.pb.h"
#include "request_facade.hpp"
#include "response.pb.h"
#include "response_facade.hpp"
#include "types.hpp"

using namespace network;

void Connection::Start() {
  is_started_ = true;
  spdlog::info("New connection... {}:{}", socket_.remote_endpoint().address().to_string(),
               socket_.remote_endpoint().port());

  ReadFromSocket();
}

void Connection::WriteToSocket(Response::StatusCode code,
                               std::optional<std::string> message = {}) {
  ResponseFacade resp(std::make_shared<Response>());
  write_buffer_ = resp.Serialize(code, message);

  asio::async_write(
      socket_, asio::buffer(write_buffer_),
      [self = shared_from_this()](const asio::error_code& ec, size_t bytes_transferred) {
        self->HandleWrite(ec, bytes_transferred);
      });
}

void Connection::ReadFromSocket() {
  asio::async_read_until(
      socket_, read_buffer_, null_terminator,
      [self = shared_from_this()](const asio::error_code& ec, size_t bytes_accepted) {
        self->HandleRead(ec, bytes_accepted);
      });
}

void Connection::HandleWrite(const asio::error_code& ec,
                             [[maybe_unused]] size_t bytes_transferred) {
  if (ec) {
    spdlog::warn("Write error: {}, code: {}", ec.message(), ec.value());
    Stop();
    return;
  }

  ReadFromSocket();
}

void Connection::HandleRead(const asio::error_code& ec, size_t bytes_accepted) {
  if (ec) {
    spdlog::warn("Read error: {}, code: {}", ec.message(), ec.value());
    Stop();
    return;
  }

  RequestFacade req(std::make_shared<Request>());
  if (!req.Deserialize(read_buffer_.data().data(), bytes_accepted)) {
    WriteToSocket(Response::StatusCode::Response_StatusCode_Error);
    return;
  }

  req.Print();
  read_buffer_.consume(bytes_accepted);  // clear read buffer
  WriteToSocket(Response::StatusCode::Response_StatusCode_Ok);
}

void Connection::Stop() {
  if (is_started_) {  // TODO: Do i actually need this variable ?
    spdlog::info("Closing connetion...");
    is_started_ = false;
    socket_.close();
  }
}