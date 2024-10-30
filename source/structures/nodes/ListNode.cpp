#include "ListNode.hpp"

ListNode::ListNode(std::vector<std::shared_ptr<Node>> _nodes, Position _start, Position _end)
: Node(NodeType::LIST, _start, _end), nodes(_nodes) {}

std::string ListNode::to_string(int depth)
{
  depth++;
  std::string res = "";

  res += "ListNode(\n";
  
  for (auto& node : nodes)
  {
    res += std::string(2 * depth, ' ') + node->to_string(depth) + ",\n";
  }

  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}