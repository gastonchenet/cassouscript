#include "Number.hpp"

Number::Number(double _value, Position _start, Position _end, Context* _context)
: Value(ValueType::NUMBER, _start, _end), value(_value)
{
  set_context(_context);
}

bool Number::is_true()
{
  return value != 0;
}

RTResult Number::to_positive()
{
  return RTResult().success(std::make_shared<Number>(value, start, end, context));
}

RTResult Number::to_negative()
{
  return RTResult().success(std::make_shared<Number>(-value, start, end, context));
}

RTResult Number::to_not()
{
  return RTResult().success(std::make_shared<Boolean>(!value, start, end, context));
}

RTResult Number::add(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }

  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Number>(value + number.value, start, end, context));
}

RTResult Number::subtract(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Number>(value - number.value, start, end, context));
}

RTResult Number::multiply(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type == ValueType::NUMBER)
  {
    Number number = dynamic_cast<Number&>(*other);
    return result.success(std::make_shared<Number>(value * number.value, start, end, context));
  }
  else if (other->type == ValueType::STRING)
  {
    std::string content = "";
    String string = dynamic_cast<String&>(*other);

    for (int i = 0; i < value; i++)
    {
      content += string.value;
    }

    return result.success(std::make_shared<String>(content, start, end, context));
  }
  else if (other->type == ValueType::LIST)
  {
    List list = dynamic_cast<List&>(*other);

    if (value < 0)
    {
      return result.failure(std::make_shared<RTError>(
        "Cannot multiply a list by a negative number",
        start,
        end,
        context
      ));
    }

    std::vector<std::shared_ptr<Value>> new_values;

    for (int i = 0; i < value; i++)
    {
      for (int j = 0; j < list.values.size(); j++)
      {
        new_values.push_back(list.values[j]);
      }
    }

    return result.success(std::make_shared<List>(new_values, start, end, context));
  }

  return result.failure(std::make_shared<RTError>(
    "Expected number, string or list",
    start,
    end,
    context
  ));
}

RTResult Number::divide(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);

  if (number.value == 0)
  {
    return result.failure(std::make_shared<RTError>("Division by zero", start, end, context));
  }

  return result.success(std::make_shared<Number>(value / number.value, start, end, context));
}

RTResult Number::modulo(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Number>(std::fmod(value, number.value), start, end, context));
}

RTResult Number::power(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Number>(std::pow(value, number.value), start, end, context));
}

RTResult Number::equal(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Boolean>(value == number.value, start, end, context));
}

RTResult Number::not_equal(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Boolean>(value != number.value, start, end, context));
}

RTResult Number::greater_than(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Boolean>(value > number.value, start, end, context));
}

RTResult Number::less_than(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Boolean>(value < number.value, start, end, context));
}

RTResult Number::greater_than_or_equal(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Boolean>(value >= number.value, start, end, context));
}

RTResult Number::less_than_or_equal(std::shared_ptr<Value> other)
{
  RTResult result = RTResult();

  if (other->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>(
      "Expected number",
      start,
      end,
      context
    ));
  }
  
  Number number = dynamic_cast<Number&>(*other);
  return result.success(std::make_shared<Boolean>(value <= number.value, start, end, context));
}

RTResult Number::and_op(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(is_true() && other->is_true(), start, end, context));
}

RTResult Number::or_op(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(is_true() || other->is_true(), start, end, context));
}

std::string Number::to_string(int depth)
{
  std::string str = std::to_string(value);
  str.erase(str.find_last_not_of('0') + 1, std::string::npos);
  str.erase(str.find_last_not_of('.') + 1, std::string::npos);
  
  return util::color::colorize(str, util::color::yellow);
}