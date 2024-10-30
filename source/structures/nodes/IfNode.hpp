#pragma once

#include <string>
#include <memory>

#include "Node.hpp"

struct IfNode : public virtual Node
{
  std::shared_ptr<Node> if_cond;
  std::shared_ptr<Node> if_body;
  std::shared_ptr<Node> else_body;

  IfNode(std::shared_ptr<Node> _if_cond, std::shared_ptr<Node> _if_body, std::shared_ptr<Node> _else_body);

  std::string to_string(int depth = 0) override;
};