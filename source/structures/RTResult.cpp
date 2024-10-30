#include "RTResult.hpp"

void RTResult::reset()
{
  value = nullptr;
  error = nullptr;
  return_value = nullptr;
  loop_should_continue = false;
  loop_should_break = false;
}

RTResult RTResult::success(std::shared_ptr<Value> success_value)
{
  reset();
  value = success_value;
  return *this;
}

RTResult RTResult::failure(std::shared_ptr<Error> failure_error)
{
  reset();
  error = failure_error;
  return *this;
}

RTResult RTResult::success_return(std::shared_ptr<Value> return_value)
{
  reset();
  this->return_value = return_value;
  return *this;
}

RTResult RTResult::success_continue()
{
  reset();
  loop_should_continue = true;
  return *this;
}

RTResult RTResult::success_break()
{
  reset();
  loop_should_break = true;
  return *this;
}

std::shared_ptr<Value> RTResult::register_result(RTResult result)
{
  error = result.error;
  return_value = result.return_value;
  loop_should_continue = result.loop_should_continue;
  loop_should_break = result.loop_should_break;

  return result.value;
}

bool RTResult::should_return()
{
  return return_value != nullptr || error != nullptr || loop_should_continue || loop_should_break;
}