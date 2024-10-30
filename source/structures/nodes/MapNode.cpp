#include "MapNode.hpp"

MapNode::MapNode(std::map<std::shared_ptr<Node>, std::shared_ptr<Node>> _pairs, Position _start, Position _end)
: Node(NodeType::MAP, _start, _end), pairs(_pairs) {}

std::string MapNode::to_string(int depth)
{
  depth++;
  std::string res = "";
  
  res += "MapNode(\n";

  for (auto& pair : pairs)
  {
    res += std::string(2 * depth, ' ') + "Pair(\n";
    res += std::string(2 * (depth + 1), ' ') + pair.first->to_string(depth + 1) + ",\n";
    res += std::string(2 * (depth + 1), ' ') + pair.second->to_string(depth + 1) + ",\n";
    res += std::string(2 * depth, ' ') + "),\n";
  }

  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}