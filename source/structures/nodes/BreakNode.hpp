#pragma once

#include <string>
#include <memory>

#include "Node.hpp"

struct BreakNode : public virtual Node
{
  BreakNode(Position _start, Position _end);

  std::string to_string(int depth) override;
};