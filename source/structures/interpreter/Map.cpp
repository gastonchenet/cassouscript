#include "Map.hpp"

#define MAX_WIDTH_LIST 80

Map::Map(std::map<std::string, std::shared_ptr<Value>> _values, Position _start, Position _end, Context* _context)
: Value(ValueType::MAP, _start, _end), values(_values)
{
  set_context(_context);
}

bool Map::is_true()
{
  return values.size() != 0;
}

RTResult Map::to_positive()
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot convert a map to a number",
    start,
    end,
    context
  ));
}

RTResult Map::to_negative()
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot convert a map to a number",
    start,
    end,
    context
  ));
}

RTResult Map::to_not()
{
  return RTResult().success(std::make_shared<Boolean>(values.size() == 0, start, end, context));
}

RTResult Map::add(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot add to a map",
    start,
    end,
    context
  ));
}

RTResult Map::subtract(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot subtract from a map",
    start,
    end,
    context
  ));
}

RTResult Map::multiply(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot multiply a map",
    start,
    end,
    context
  ));
}

RTResult Map::divide(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot divide a map",
    start,
    end,
    context
  ));
}

RTResult Map::modulo(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot modulo a map",
    start,
    end,
    context
  ));
}

RTResult Map::power(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot power a map",
    start,
    end,
    context
  ));
}

RTResult Map::equal(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(
    values == dynamic_cast<Map&>(*other).values,
    start,
    end,
    context
  ));
}

RTResult Map::not_equal(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(
    values != dynamic_cast<Map&>(*other).values,
    start,
    end,
    context
  ));
}

RTResult Map::greater_than(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot compare a map",
    start,
    end,
    context
  ));
}

RTResult Map::less_than(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot compare a map",
    start,
    end,
    context
  ));
}

RTResult Map::greater_than_or_equal(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot compare a map",
    start,
    end,
    context
  ));
}

RTResult Map::less_than_or_equal(std::shared_ptr<Value> other)
{
  return RTResult().failure(std::make_shared<RTError>(
    "Cannot compare a map",
    start,
    end,
    context
  ));
}

RTResult Map::and_op(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(
    is_true() && other->is_true(),
    start,
    end,
    context
  ));
}

RTResult Map::or_op(std::shared_ptr<Value> other)
{
  return RTResult().success(std::make_shared<Boolean>(
    is_true() || other->is_true(),
    start,
    end,
    context
  ));
}

int calculate_line_size(std::map<std::string, std::shared_ptr<Value>> values, int depth)
{
  int size = 2 * depth + 6 * values.size() + 1;

  for (int i = 0; i < values.size(); i++)
  {
    auto [key, value] = *std::next(values.begin(), i);
    size += key.size() + value->to_string(depth).size();
  }

  return size;
}

std::string Map::to_string(int depth)
{
  depth++;
  std::string res = "";
  int size = values.size();

  if (size == 0)
  {
    return util::color::colorize("{}", util::color::black);
  }

  if (calculate_line_size(values, depth) < (util::getTerminalWidth() < 80 ? util::getTerminalWidth() : MAX_WIDTH_LIST))
  {
    res += util::color::colorize("{", util::color::black) + " ";

    for (int i = 0; i < values.size(); i++)
    {
      auto [key, value] = *std::next(values.begin(), i);

      res += util::color::colorize("\"" + key + "\"", util::color::green) +
        " " +
        util::color::colorize("=>", util::color::black) +
        " " +
        value->to_string(depth) +
        (i < values.size() - 1 ? util::color::colorize(",", util::color::black) : "") +
        " ";
    }

    res += util::color::colorize("}", util::color::black);
  }
  else
  {
    res += util::color::colorize("{\n", util::color::black);

    for (int i = 0; i < values.size(); i++)
    {
      auto [key, value] = *std::next(values.begin(), i);

      res += std::string(2 * depth, ' ') +
        util::color::colorize("\"" + key + "\"", util::color::green) +
        " " +
        util::color::colorize("=>", util::color::black) +
        " " +
        value->to_string(depth) +
        (i < values.size() - 1 ? util::color::colorize(",", util::color::black) : "") +
        "\n";
    }

    res += std::string(2 * (depth - 1), ' ') +
      util::color::colorize("}", util::color::black);
  }
  
  return res;
}