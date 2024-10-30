#include "FuncDefNode.hpp"

FuncDefNode::FuncDefNode(const Token& _name_tok, std::vector<Token> _arg_name_toks, std::shared_ptr<Node> _body_node, bool _should_auto_return)
: Node(NodeType::FUNC_DEF, _name_tok.start, _body_node->end), name_tok(_name_tok), arg_name_toks(_arg_name_toks), body_node(_body_node), should_auto_return(_should_auto_return) {}

std::string FuncDefNode::to_string(int depth)
{
  depth++;
  std::string res = "";

  res += "FuncDefNode(\n";
  res += std::string(2 * depth, ' ') + name_tok.to_string() + ",\n";

  for (auto& arg : arg_name_toks)
  {
    res += std::string(2 * depth, ' ') + arg.to_string() + ",\n";
  }
  
  res += std::string(2 * depth, ' ') + body_node->to_string(depth) + "\n";
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}