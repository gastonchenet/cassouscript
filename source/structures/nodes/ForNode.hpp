#pragma once

#include <string>
#include <memory>

#include "Node.hpp"
#include "../Token.hpp"

struct ForNode : public virtual Node
{
  Token name_token;
  std::shared_ptr<Node> iterable;
  std::shared_ptr<Node> body;
  bool should_return_null = false;

  ForNode(Token _var_name_token, std::shared_ptr<Node> _iterable, std::shared_ptr<Node> _body, bool _should_return_null = true);

  std::string to_string(int depth = 0) override;
};