#include "BaseFunction.hpp"

BaseFunction::BaseFunction(ValueType _type, std::string _name, Position _start, Position _end)
: Value(_type, _start, _end), name(_name) {}

Context BaseFunction::generate_new_context()
{
  Context new_context = Context(name, context, &start);
  new_context.set_symbol_table(new SymbolTable(new_context.parent->symbol_table));

  return new_context;
}

void BaseFunction::populate_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args, Context* new_context)
{
  for (int i = 0; i < arg_names.size(); i++)
  {
    std::string arg_name = arg_names[i];
    std::shared_ptr<Value> arg_value = args[i];

    arg_value->set_context(new_context);
    new_context->symbol_table->set(arg_name, arg_value);
  }
}

RTResult BaseFunction::check_arg_count(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args)
{
  RTResult result = RTResult();

  if (arg_names.size() != args.size())
  {
    return result.failure(std::make_shared<RTError>(
      "Expected " + std::to_string(arg_names.size()) + " arguments, got " + std::to_string(args.size()),
      start,
      end,
      context
    ));
  }

  return result.success(nullptr);
}

RTResult BaseFunction::check_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args, Context* context)
{
  RTResult result = RTResult();

  result.register_result(check_arg_count(arg_names, args));
  if (result.should_return()) return result;

  populate_args(arg_names, args, context);
  return result.success(nullptr);
}