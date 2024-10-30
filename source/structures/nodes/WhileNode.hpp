#pragma once

#include <string>
#include <memory>

#include "Node.hpp"

struct WhileNode : public virtual Node
{
  std::shared_ptr<Node> condition;
  std::shared_ptr<Node> body;
  bool should_return_null;

  WhileNode(std::shared_ptr<Node> _condition, std::shared_ptr<Node> _body, bool _should_return_null = true);

  std::string to_string(int depth = 0) override;
};