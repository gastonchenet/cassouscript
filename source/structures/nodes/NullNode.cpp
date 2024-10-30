#include "NullNode.hpp"

NullNode::NullNode(const Token& _value_tok)
: Node(NodeType::NULL_NODE, _value_tok.start, _value_tok.end) {}

std::string NullNode::to_string(int depth)
{
  return "NullNode()";
}