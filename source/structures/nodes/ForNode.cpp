#include "ForNode.hpp"

ForNode::ForNode(Token _name_token, std::shared_ptr<Node> _iterable, std::shared_ptr<Node> _body, bool _should_return_null)
: Node(NodeType::FOR, _iterable->start, _body->end), name_token(_name_token), iterable(_iterable), body(_body), should_return_null(_should_return_null) {}

std::string ForNode::to_string(int depth)
{
  depth++;
  std::string res = "";

  res += "ForNode(\n";
  res += std::string(2 * depth, ' ') + name_token.to_string() + ",\n";
  res += std::string(2 * depth, ' ') + iterable->to_string(depth) + ",\n";
  res += std::string(2 * depth, ' ') + body->to_string(depth) + "\n";
  res += std::string(2 * (depth - 1), ' ') + ")";
  
  return res;
}