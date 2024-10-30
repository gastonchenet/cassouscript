#include "CallNode.hpp"

CallNode::CallNode(std::shared_ptr<Node> _func_name, std::vector<std::shared_ptr<Node>> _args)
: Node(NodeType::CALL, _func_name->start, _func_name->end), func_name(_func_name), args(_args) {
  if (args.size() > 0)
  {
    end = _args.back()->end;
  }
}

std::string CallNode::to_string(int depth)
{
  depth++;
  std::string res = "";
  
  res += "CallNode(\n";
  res += std::string(2 * depth, ' ') + func_name->to_string(depth) + ",\n";
  for (auto& arg : args)
  {
    res += std::string(2 * depth, ' ') + arg->to_string(depth) + ",\n";
  }
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}