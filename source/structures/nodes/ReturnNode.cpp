#include "ReturnNode.hpp"

ReturnNode::ReturnNode(Position _start, std::shared_ptr<Node> _node)
: Node(NodeType::RETURN, _start, _node->end), node(_node) {}

std::string ReturnNode::to_string(int depth)
{
  return "ReturnNode(" + node->to_string(depth) + ")";
}