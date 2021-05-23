#pragma once
#include <string>

#include "request.pb.h"
#include "response.pb.h"

namespace network {

using Value = std::string;
using Key = std::string;
using StatusCode = Response::StatusCode;
using RequestMethod = Request::Method;

inline constexpr char null_terminator = '\0';

}  // namespace network