#pragma once

#include <string>
#include <memory>

#include "../Token.hpp"
#include "Node.hpp"

struct VarAssignNode : public virtual Node
{
  Token var_name;
  std::shared_ptr<Node> value;

  VarAssignNode(const Token& _var_name, std::shared_ptr<Node> _value);

  std::string to_string(int depth = 0) override;
};