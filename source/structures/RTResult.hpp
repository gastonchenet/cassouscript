#pragma once

#include <string>

struct Value;
struct Error;

#include "interpreter/Value.hpp"
#include "Error.hpp"

struct RTResult
{
private:
  void reset();

public:
  std::shared_ptr<Value> value = nullptr;
  std::shared_ptr<Error> error = nullptr;
  std::shared_ptr<Value> return_value = nullptr;
  bool loop_should_continue = false;
  bool loop_should_break = false;

  RTResult success(std::shared_ptr<Value> value);
  RTResult failure(std::shared_ptr<Error> error);
  RTResult success_return(std::shared_ptr<Value> value);
  RTResult success_continue();
  RTResult success_break();
  std::shared_ptr<Value> register_result(RTResult result);
  bool should_return();
};