#pragma once

#include <vector>
#include <memory>

#include "Value.hpp"
#include "../SymbolTable.hpp"
#include "../Position.hpp"
#include "../RTResult.hpp"
#include "../Error.hpp"

struct BaseFunction : public virtual Value
{
  std::string name;

  BaseFunction(ValueType _type, std::string _name, Position _start, Position _end);

  Context generate_new_context();
  void populate_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args, Context* new_context);
  RTResult check_arg_count(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args);
  RTResult check_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args, Context* new_context);

  virtual RTResult execute(std::vector<std::shared_ptr<Value>> args) = 0;
};