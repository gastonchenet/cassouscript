#pragma once

#include <iostream>
#include <string>

#include "Node.hpp"
#include "../Token.hpp"

struct NullNode : public virtual Node
{
  NullNode(const Token& _value_tok);
  std::string to_string(int depth = 0) override;
};