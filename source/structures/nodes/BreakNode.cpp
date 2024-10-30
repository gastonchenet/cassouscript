#include "BreakNode.hpp"

BreakNode::BreakNode(Position _start, Position _end)
: Node(NodeType::BREAK, _start, _end) {}

std::string BreakNode::to_string(int depth)
{
  return "BreakNode()";
}