#pragma once

#include <string>
#include <memory>

#include "Node.hpp"
#include "../Position.hpp"

struct ReturnNode : public virtual Node
{
  std::shared_ptr<Node> node;

  ReturnNode(Position _start, std::shared_ptr<Node> _node);

  std::string to_string(int depth = 0) override;
};