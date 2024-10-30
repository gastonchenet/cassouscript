#include "NumListNode.hpp"

NumListNode::NumListNode(std::shared_ptr<Node> _start_node, std::shared_ptr<Node> _end_node, std::shared_ptr<Node> _step_node)
: Node(NodeType::NUM_LIST, _start_node->start, _end_node->end), start_node(_start_node), end_node(_end_node), step_node(_step_node)
{

  if (step_node != nullptr)
{

    end = step_node->end;
  }
}

std::string NumListNode::to_string(int depth)
{
  depth++;
  std::string res = "";

  res += "NumListNode(\n";
  res += std::string(2 * depth, ' ') + start_node->to_string(depth) + ",\n";
  res += std::string(2 * depth, ' ') + end_node->to_string(depth) + ",\n";

  if (step_node != nullptr)
{

    res += std::string(2 * depth, ' ') + step_node->to_string(depth) + ",\n";
  }
  
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}