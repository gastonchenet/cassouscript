#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Node.hpp"
#include "../Position.hpp"

struct ListNode : public virtual Node
{
  std::vector<std::shared_ptr<Node>> nodes = {};

  ListNode(std::vector<std::shared_ptr<Node>> nodes, Position _start, Position _end);

  std::string to_string(int depth = 0) override;
};