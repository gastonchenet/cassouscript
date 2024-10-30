#include "StringNode.hpp"

StringNode::StringNode(const Token& _value_tok)
: Node(NodeType::STRING, _value_tok.start, _value_tok.end), value_tok(_value_tok)
{

  if (!value_tok.match(TokenType::TT_STRING))
{

    throw std::runtime_error("Invalid token type");
  }
}

std::string StringNode::to_string(int depth)
{
  return "StringNode(" + value_tok.to_string() + ")";
}