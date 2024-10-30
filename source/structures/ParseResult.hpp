#pragma once

#include <memory>

#include "nodes/Node.hpp"
#include "Error.hpp"

struct ParseResult
{
  std::shared_ptr<Node> node;
  std::shared_ptr<Error> error;
  int last_registered_advance_count = 0;
  int advance_count = 0;
  int to_reverse_count = 0;

  void register_advancement();
  std::shared_ptr<Node> register_result(ParseResult result);
  std::shared_ptr<Node> try_register(ParseResult result);
  ParseResult success(std::shared_ptr<Node> return_node);
  ParseResult failure(std::shared_ptr<Error> return_error);
};