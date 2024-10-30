#include "WhileNode.hpp"

WhileNode::WhileNode(std::shared_ptr<Node> _condition, std::shared_ptr<Node> _body, bool _should_return_null)
: Node(NodeType::WHILE, _condition->start, _body->end), condition(_condition), body(_body), should_return_null(_should_return_null) {}

std::string WhileNode::to_string(int depth)
{
  depth++;
  std::string res = "";

  res += "WhileNode(\n";
  res += std::string(2 * depth, ' ') + condition->to_string(depth) + ",\n";
  res += std::string(2 * depth, ' ') + body->to_string(depth) + "\n";
  res += std::string(2 * (depth - 1), ' ') + ")";
  
  return res;
}