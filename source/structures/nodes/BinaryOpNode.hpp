#pragma once

#include <string>
#include <memory>

#include "../Token.hpp"
#include "Node.hpp"

struct BinaryOpNode : public virtual Node
{
  std::shared_ptr<Node> left;
  Token op_tok;
  std::shared_ptr<Node> right;

  BinaryOpNode(std::shared_ptr<Node> _left, const Token& _op_tok, std::shared_ptr<Node> _right);

  std::string to_string(int depth = 0) override;
};