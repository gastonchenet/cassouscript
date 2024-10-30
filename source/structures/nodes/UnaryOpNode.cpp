#include "UnaryOpNode.hpp"

UnaryOpNode::UnaryOpNode(const Token& _op_tok, std::shared_ptr<Node> _node)
: Node(NodeType::UNARY_OPERATION, _op_tok.start, _node->end), op_tok(_op_tok), operand(_node) {}

std::string UnaryOpNode::to_string(int depth)
{
  depth++;
  std::string res = "";
  
  res += "UnaryOpNode(\n";
  res += std::string(2 * depth, ' ') + op_tok.to_string() + ",\n";
  res += std::string(2 * depth, ' ') + operand->to_string(depth) + "\n";
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}