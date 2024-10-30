#pragma once

#include <iostream>
#include <string>

#include "Node.hpp"
#include "../Token.hpp"

struct BooleanNode : public virtual Node
{
  Token value_tok;

  BooleanNode(const Token& _value_tok);

  std::string to_string(int depth = 0) override;
};