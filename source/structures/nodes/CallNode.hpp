#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Node.hpp"
#include "../Token.hpp"

struct CallNode : public virtual Node
{
  std::shared_ptr<Node> func_name;
  std::vector<std::shared_ptr<Node>> args;

  CallNode(std::shared_ptr<Node> _func_name, std::vector<std::shared_ptr<Node>> _args);

  std::string to_string(int depth = 0) override;
};