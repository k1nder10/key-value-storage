#pragma once
#include <string>

#include "request.pb.h"
#include "response.pb.h"

using Value = std::string;
using Key = std::string;
using StatusCode = network::Response::StatusCode;
using RequestMethod = network::Request::Method;

inline constexpr char null_terminator = '\0';