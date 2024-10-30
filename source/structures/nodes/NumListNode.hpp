#pragma once

#include <string>
#include <memory>

#include "Node.hpp"

struct NumListNode : public virtual Node
{
  std::shared_ptr<Node> start_node;
  std::shared_ptr<Node> end_node;
  std::shared_ptr<Node> step_node;

  NumListNode(std::shared_ptr<Node> _start_node, std::shared_ptr<Node> _end_node, std::shared_ptr<Node> _step_node);

  std::string to_string(int depth = 0) override;
};