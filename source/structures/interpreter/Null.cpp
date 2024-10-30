#include "Null.hpp"

Null::Null(Position _start, Position _end, Context* _context)
: Value(ValueType::NULL_VALUE, _start, _end)
{
  set_context(_context);
}

bool Null::is_true()
{
  return false;
}

RTResult Null::to_positive()
{
  return RTResult().failure(std::make_shared<RTError>("Cannot convert null to a number", start, end, context));
}

RTResult Null::to_negative()
{
  return RTResult().failure(std::make_shared<RTError>("Cannot convert null to a number", start, end, context));
}

RTResult Null::to_not()
{
  return RTResult().success(std::make_shared<Boolean>(true, start, end, context));
}

RTResult Null::add(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot add null", start, end, context));
}

RTResult Null::subtract(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot subtract null", start, end, context));
}

RTResult Null::multiply(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot multiply null", start, end, context));
}

RTResult Null::divide(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot divide null", start, end, context));
}

RTResult Null::modulo(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot modulo null", start, end, context));
}

RTResult Null::power(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot power null", start, end, context));
}

RTResult Null::equal(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(other->type == ValueType::NULL_VALUE, start, end, context));
}

RTResult Null::not_equal(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(other->type != ValueType::NULL_VALUE, start, end, context));
}

RTResult Null::greater_than(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot compare null", start, end, context));
}

RTResult Null::less_than(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot compare null", start, end, context));
}

RTResult Null::greater_than_or_equal(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot compare null", start, end, context));
}

RTResult Null::less_than_or_equal(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot compare null", start, end, context));
}

RTResult Null::and_op(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(false, start, end, context));
}

RTResult Null::or_op(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(other->is_true(), start, end, context));
}

std::string Null::to_string(int depth)
{
  return util::color::colorize("null", util::color::black);
}