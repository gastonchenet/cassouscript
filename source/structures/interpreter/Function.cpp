#include "Function.hpp"

Function::Function(std::string _name, Position _start, Position _end, std::shared_ptr<Node> _body_node, std::vector<std::string> _arg_names, bool _should_auto_return, Context* _context)
: Value(ValueType::FUNCTION, _start, _end), BaseFunction(ValueType::FUNCTION, _name, _start, _end), body_node(_body_node), arg_names(_arg_names), should_auto_return(_should_auto_return)
{
  context = _context;
}

RTResult Function::execute(std::vector<std::shared_ptr<Value>> args)
{
  RTResult result = RTResult();
  Context exec_context = generate_new_context();
  Interpreter interpreter = Interpreter();

  check_args(arg_names, args, &exec_context);
  std::shared_ptr<Value> value = result.register_result(interpreter.visit(body_node, &exec_context));

  if (result.should_return() && result.return_value == nullptr)
  {
    return result;
  }

  std::shared_ptr<Value> return_value;

  if (should_auto_return)
  {
    return_value = value;
  }
  else if (result.return_value != nullptr)
  {
    return_value = result.return_value;
  }
  else
  {
    return_value = std::make_shared<Null>(start, end, context);
  }

  return result.success(return_value);
}

bool Function::is_true()
{
  return true;
}

RTResult Function::to_positive()
{
  return RTResult().failure(std::make_shared<RTError>("Cannot convert function to positive", start, end, context));
}

RTResult Function::to_negative()
{
  return RTResult().failure(std::make_shared<RTError>("Cannot convert function to negative", start, end, context));
}

RTResult Function::to_not()
{
  return RTResult().success(std::make_shared<Boolean>(false, start, end, context));
}

RTResult Function::add(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot add function", start, end, context));
}

RTResult Function::subtract(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot subtract function", start, end, context));
}

RTResult Function::multiply(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot multiply function", start, end, context));
}

RTResult Function::divide(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot divide function", start, end, context));
}

RTResult Function::modulo(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot modulo function", start, end, context));
}

RTResult Function::power(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot power function", start, end, context));
}

RTResult Function::equal(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(false, start, end, context));
}

RTResult Function::not_equal(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(true, start, end, context));
}

RTResult Function::greater_than(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot compare function", start, end, context));
}

RTResult Function::less_than(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot compare function", start, end, context));
}

RTResult Function::greater_than_or_equal(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot compare function", start, end, context));
}

RTResult Function::less_than_or_equal(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>("Cannot compare function", start, end, context));
}

RTResult Function::and_op(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(false, start, end, context));
}

RTResult Function::or_op(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(other->is_true(), start, end, context));
}

std::string Function::to_string(int depth)
{
  return util::color::colorize("<function " + name + ">", util::color::cyan);
}

Function Function::copy()
{
  return Function(name, start, end, body_node, arg_names, should_auto_return, context);
}