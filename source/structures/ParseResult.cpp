#include "ParseResult.hpp"

void ParseResult::register_advancement()
{
  last_registered_advance_count = 1;
  advance_count++;
}

std::shared_ptr<Node> ParseResult::register_result(ParseResult result)
{
  last_registered_advance_count = result.advance_count;
  advance_count += result.advance_count;

  if (result.error != nullptr)
  {
    error = result.error;
  }

  return result.node;
}

std::shared_ptr<Node> ParseResult::try_register(ParseResult result)
{
  if (result.error != nullptr)
  {
    to_reverse_count = result.advance_count;
    return nullptr;
  }

  return register_result(result);
}

ParseResult ParseResult::success(std::shared_ptr<Node> return_node)
{
  node = return_node;
  return *this;
}

ParseResult ParseResult::failure(std::shared_ptr<Error> return_error)
{
  if (error == nullptr || advance_count == 0)
  {
    error = return_error;
  }

  return *this;
}