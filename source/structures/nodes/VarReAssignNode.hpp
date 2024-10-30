#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Node.hpp"
#include "../Token.hpp"

struct VarReAssignNode : public virtual Node
{
  Token var_name;
  std::vector<std::shared_ptr<Node>> path;
  std::shared_ptr<Node> value;

  VarReAssignNode(const Token& _var_name, std::vector<std::shared_ptr<Node>> _path, std::shared_ptr<Node> _value);

  std::string to_string(int depth = 0) override;
};