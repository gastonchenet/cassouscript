#include "VarAccessNode.hpp"

VarAccessNode::VarAccessNode(const Token& _var_name)
: Node(NodeType::VAR_ACCESS, _var_name.start, _var_name.end), var_name(_var_name) {}

std::string VarAccessNode::to_string(int depth)
{
  depth++;
  std::string res = "";
  
  res += "VarAccessNode(\n";
  res += std::string(2 * depth, ' ') + var_name.to_string() + "\n";
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}