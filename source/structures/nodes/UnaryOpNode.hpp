#pragma once

#include <string>
#include <memory>

#include "Node.hpp"
#include "../Token.hpp"

struct UnaryOpNode : public virtual Node
{
  Token op_tok;
  std::shared_ptr<Node> operand;

  UnaryOpNode(const Token& _op_tok, std::shared_ptr<Node> _node);

  std::string to_string(int depth = 0) override;
};