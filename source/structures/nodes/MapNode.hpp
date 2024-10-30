#pragma once

#include <string>
#include <memory>
#include <map>

#include "Node.hpp"

struct MapNode : public virtual Node
{
  std::map<std::shared_ptr<Node>, std::shared_ptr<Node>> pairs;

  MapNode(std::map<std::shared_ptr<Node>, std::shared_ptr<Node>> _pairs, Position _start, Position _end);

  std::string to_string(int depth = 0) override;
};