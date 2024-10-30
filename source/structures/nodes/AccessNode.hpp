#pragma once

#include <string>
#include <memory>

#include "Node.hpp"
#include "../Token.hpp"

struct AccessNode : public virtual Node
{
  std::shared_ptr<Node> var_node;
  std::shared_ptr<Node> index_node;

  AccessNode(std::shared_ptr<Node> _var_node, std::shared_ptr<Node> _index_node);

  std::string to_string(int depth = 0) override;
};