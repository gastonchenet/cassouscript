#pragma once

#include <vector>
#include <memory>

#include "BaseFunction.hpp"
#include "Value.hpp"
#include "Null.hpp"
#include "Boolean.hpp"
#include "../nodes/Node.hpp"
#include "../SymbolTable.hpp"
#include "../Position.hpp"
#include "../RTResult.hpp"
#include "../Interpreter.hpp"

struct Function : public virtual BaseFunction
{
  std::shared_ptr<Node> body_node;
  std::vector<std::string> arg_names;
  bool should_auto_return;

  Function(std::string _name, Position _start, Position _end, std::shared_ptr<Node> _body_node, std::vector<std::string> _arg_names, bool _should_auto_return, Context* _context);

  RTResult execute(std::vector<std::shared_ptr<Value>> args) override;

  bool is_true() override;
  
  RTResult to_positive() override;
  RTResult to_negative() override;
  RTResult to_not() override;

  RTResult add(std::shared_ptr<Value> other) override;
  RTResult subtract(std::shared_ptr<Value> other) override;
  RTResult multiply(std::shared_ptr<Value> other) override;
  RTResult divide(std::shared_ptr<Value> other) override;
  RTResult modulo(std::shared_ptr<Value> other) override;
  RTResult power(std::shared_ptr<Value> other) override;
  RTResult equal(std::shared_ptr<Value> other) override;
  RTResult not_equal(std::shared_ptr<Value> other) override;
  RTResult greater_than(std::shared_ptr<Value> other) override;
  RTResult less_than(std::shared_ptr<Value> other) override;
  RTResult greater_than_or_equal(std::shared_ptr<Value> other) override;
  RTResult less_than_or_equal(std::shared_ptr<Value> other) override;
  RTResult and_op(std::shared_ptr<Value> other) override;
  RTResult or_op(std::shared_ptr<Value> other) override;

  std::string to_string(int depth = 0) override;
  Function copy();
};