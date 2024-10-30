#pragma once

#include <string>
#include <memory>

#include "Node.hpp"
#include "../Token.hpp"

struct VarAccessNode : public virtual Node
{
  Token var_name;

  VarAccessNode(const Token& _var_name);

  std::string to_string(int depth = 0) override;
};