#include "UnaryOpAssignNode.hpp"

UnaryOpAssignNode::UnaryOpAssignNode(const Token& _op_tok, std::vector<std::shared_ptr<Node>> _path, const Token& _var_name_tok)
: Node(NodeType::UNARY_OP_ASSIGN, _var_name_tok.start, _op_tok.end), op_tok(_op_tok), path(_path), var_name_tok(_var_name_tok) {}

std::string UnaryOpAssignNode::to_string(int depth)
{
  std::string res = "";

  res += "UnaryOpAssignNode(\n";
  res += std::string(2 * depth, ' ') + op_tok.to_string() + ",\n";
  res += std::string(2 * depth, ' ') + var_name_tok.to_string() + ",\n";
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}