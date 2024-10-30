#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../Token.hpp"
#include "Node.hpp"

struct FuncDefNode : public virtual Node
{
  Token name_tok;
  std::vector<Token> arg_name_toks;
  std::shared_ptr<Node> body_node;
  bool should_auto_return;

  FuncDefNode(const Token& _name_tok, std::vector<Token> _arg_name_toks, std::shared_ptr<Node> _body_node, bool _should_auto_return);

  std::string to_string(int depth = 0) override;
};