#include "IfNode.hpp"

IfNode::IfNode(std::shared_ptr<Node> _if_cond, std::shared_ptr<Node> _if_body, std::shared_ptr<Node> _else_body)
: Node(NodeType::IF, _if_cond->start, _if_body->end), if_cond(_if_cond), if_body(_if_body), else_body(_else_body)
{
  if (_else_body != nullptr)
  {
    end = _else_body->end;
  }
}

std::string IfNode::to_string(int depth)
{
  depth++;
  std::string res = "";

  res += "IfNode(";
  res += std::string(2, ' ') + if_cond->to_string() + ",\n";
  res += std::string(2, ' ') + if_body->to_string() + ",\n";

  if (else_body != nullptr)
  {
    res += std::string(2, ' ') + else_body->to_string() + ",\n";
  }

  return res;
}