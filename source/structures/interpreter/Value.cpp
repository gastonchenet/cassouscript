#include "Value.hpp"

Value::Value(ValueType _type, Position _start, Position _end)
: type(_type), start(_start), end(_end) {}

void Value::set_context(Context* _context)
{
  context = _context;
}