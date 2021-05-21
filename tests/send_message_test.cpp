#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <asio.hpp>

#include "storage/network/connection.cpp"
#include "storage/network/request_facade.hpp"
#include "storage/network/response_facade.hpp"
#include "storage/network/types.hpp"

using tcp = asio::ip::tcp;

static constexpr unsigned port = 27901;
static const auto address = asio::ip::address::from_string("127.0.0.1");

TEST(SendMessage, SetCommand) {
  asio::io_context io_context;
  tcp::endpoint ep(address, port);
  asio::error_code ec;
  tcp::socket sock(io_context);

  sock.connect(ep, ec);
  ASSERT_EQ(ec.value(), 0);

  // 1. send request
  {
    RequestFacade req(std::make_shared<Request>());
    std::string message =
        req.Serialize(Request::Method::Request_Method_Set, "Hello", "World");

    size_t bytes_transferred = asio::write(sock, asio::buffer(message), ec);

    ASSERT_NE(bytes_transferred, 0);
    ASSERT_EQ(ec.value(), 0);
  }

  // 2. read response
  {
    asio::streambuf response_buf;
    size_t bytes_accepted = asio::read_until(sock, response_buf, null_terminator);

    ResponseFacade resp(std::make_shared<Response>());
    const bool is_deserialized =
        resp.Deserialize(response_buf.data().data(), bytes_accepted);

    ASSERT_EQ(is_deserialized, true);
    ASSERT_EQ(ec.value(), 0);
    ASSERT_NE(bytes_accepted, 0);
    ASSERT_EQ(resp.GetStatusCode(), 0);
    resp.Print();
  }

  sock.close();
}