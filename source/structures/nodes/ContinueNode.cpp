#include "ContinueNode.hpp"

ContinueNode::ContinueNode(Position _start, Position _end)
: Node(NodeType::CONTINUE, _start, _end) {}

std::string ContinueNode::to_string(int depth)
{
  return "ContinueNode()";
}