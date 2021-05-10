#pragma once
#include <memory>

#include "command.pb.h"

namespace network {

class CommandFacade {
  using Key = std::string;
  using Value = std::string;
  using Type = Command::Type;

public:
  CommandFacade(Command* cmd) : cmd_(cmd) {}

  bool Deserialize(const std::string& str) { return cmd_->ParseFromString(str); }
  std::pair<Key, Value> GetKeyValuePair() const { return {cmd_->key(), cmd_->value()}; }
  Type GetType() const { return cmd_->op_type(); }

private:
  std::shared_ptr<Command> cmd_{nullptr};
};

}  // namespace network