#include "VarReAssignNode.hpp"

VarReAssignNode::VarReAssignNode(const Token& _var_name, std::vector<std::shared_ptr<Node>> _path, std::shared_ptr<Node> _value)
: Node(NodeType::VAR_REASSIGN, _var_name.start, _value->end), var_name(_var_name), path(_path), value(_value) {}

std::string VarReAssignNode::to_string(int depth)
{
  depth++;
  std::string res = "";
  
  res += "VarReAssignNode(\n";
  res += std::string(2 * depth, ' ') + var_name.to_string() + ",\n";
  res += std::string(2 * depth, ' ') + value->to_string(depth) + "\n";
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}