#pragma once

#include <string>
#include <memory>

#include "Node.hpp"

struct ContinueNode : public virtual Node
{
  ContinueNode(Position _start, Position _end);

  std::string to_string(int depth) override;
};