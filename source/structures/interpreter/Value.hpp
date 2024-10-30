#pragma once

#include <iostream>
#include <string>
#include <memory>

struct Context;
struct RTResult;

#include "../Position.hpp"
#include "../Context.hpp"
#include "../RTResult.hpp"
#include "../Error.hpp"
#include "../../util/color.hpp"

enum class ValueType
{
  NUMBER,
  STRING,
  BOOLEAN,
  FUNCTION,
  BUILTIN_FUNCTION,
  LIST,
  MAP,
  NULL_VALUE
};

struct Value
{
  ValueType type;
  Position start;
  Position end;
  Context* context = nullptr;

  Value(ValueType _type, Position _start, Position _end);
  virtual ~Value() = default;

  void set_context(Context* _context);

  virtual bool is_true() = 0;

  virtual RTResult to_positive() = 0;
  virtual RTResult to_negative() = 0;
  virtual RTResult to_not() = 0;

  virtual RTResult add(std::shared_ptr<Value> other) = 0;
  virtual RTResult subtract(std::shared_ptr<Value> other) = 0;
  virtual RTResult multiply(std::shared_ptr<Value> other) = 0;
  virtual RTResult divide(std::shared_ptr<Value> other) = 0;
  virtual RTResult modulo(std::shared_ptr<Value> other) = 0;
  virtual RTResult power(std::shared_ptr<Value> other) = 0;
  virtual RTResult equal(std::shared_ptr<Value> other) = 0;
  virtual RTResult not_equal(std::shared_ptr<Value> other) = 0;
  virtual RTResult greater_than(std::shared_ptr<Value> other) = 0;
  virtual RTResult less_than(std::shared_ptr<Value> other) = 0;
  virtual RTResult greater_than_or_equal(std::shared_ptr<Value> other) = 0;
  virtual RTResult less_than_or_equal(std::shared_ptr<Value> other) = 0;
  virtual RTResult and_op(std::shared_ptr<Value> other) = 0;
  virtual RTResult or_op(std::shared_ptr<Value> other) = 0;

  virtual std::string to_string(int depth = 0) = 0;
};