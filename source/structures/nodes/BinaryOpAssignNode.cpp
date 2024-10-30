#include "BinaryOpAssignNode.hpp"

BinaryOpAssignNode::BinaryOpAssignNode(const Token& _left, std::vector<std::shared_ptr<Node>> _path, const Token& _op_tok, std::shared_ptr<Node> _right)
: Node(NodeType::BINARY_OP_ASSIGN, _left.start, _right->end), op_tok(_op_tok), left(_left), right(_right), path(_path) {}

std::string BinaryOpAssignNode::to_string(int depth)
{
  std::string res = "";

  res += "BinaryOpAssignNode(\n";
  res += std::string(2 * depth, ' ') + left.to_string() + ",\n";
  res += std::string(2 * depth, ' ') + op_tok.to_string() + ",\n";
  res += std::string(2 * depth, ' ') + right->to_string(depth + 1) + "\n";
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}