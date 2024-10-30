#include "BooleanNode.hpp"

BooleanNode::BooleanNode(const Token& _value_tok)
: Node(NodeType::BOOLEAN, _value_tok.start, _value_tok.end), value_tok(_value_tok)
{

  if (!value_tok.match(TokenType::TT_KEYWORD, {"true","false"}))
{

    throw std::runtime_error("Invalid token type/value");
  }
}

std::string BooleanNode::to_string(int depth)
{
  return "BooleanNode(" + value_tok.to_string() + ")";
}