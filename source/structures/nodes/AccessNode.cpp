#include "AccessNode.hpp"

AccessNode::AccessNode(std::shared_ptr<Node> _var_node, std::shared_ptr<Node> _index_node)
: Node(NodeType::ACCESS, _var_node->start, _index_node->end), var_node(_var_node), index_node(_index_node) {}

std::string AccessNode::to_string(int depth)
{
  depth++;
  std::string res = "";

  res += "AccessNode(\n";
  res += std::string(2 * depth, ' ') + var_node->to_string(depth) + ",\n";
  res += std::string(2 * depth, ' ') + index_node->to_string(depth) + "\n";
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}