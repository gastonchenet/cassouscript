#include "Node.hpp"

Node::Node(NodeType _type, const Position& _start, const Position& _end)
: type(_type), start(_start), end(_end) {}