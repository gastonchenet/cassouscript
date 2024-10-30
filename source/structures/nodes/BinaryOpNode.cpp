#include "BinaryOpNode.hpp"

BinaryOpNode::BinaryOpNode(std::shared_ptr<Node> _left, const Token& _op_tok, std::shared_ptr<Node> _right)
: Node(NodeType::BINARY_OPERATION, _left->start, _right->end), left(_left), op_tok(_op_tok), right(_right) {}

std::string BinaryOpNode::to_string(int depth)
{
  depth++;
  std::string res = "";
  
  res += "BinaryOpNode(\n";
  res += std::string(2 * depth, ' ') + left->to_string(depth) + ",\n";
  res += std::string(2 * depth, ' ') + op_tok.to_string() + ",\n";
  res += std::string(2 * depth, ' ') + right->to_string(depth) + ",\n";
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}