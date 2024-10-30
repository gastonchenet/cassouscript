#include "Error.hpp"

Error::Error(std::string _error_name, std::string _details, Position _start, Position _end)
: error_name(_error_name), details(_details), start(_start), end(_end) {}

IllegalCharError::IllegalCharError(std::string _details, Position _start, Position _end)
: Error("Illegal Character Error", _details, _start, _end) {}

IllegalCharError::IllegalCharError(std::string _details, Position _pos)
: Error("Illegal Character Error", _details, _pos, _pos)
{
  Position _end = _pos.copy();
  _end.advance();
  end = _end;
}

InvalidSyntaxError::InvalidSyntaxError(std::string _details, Position _start, Position _end)
: Error("Syntax Error", _details, _start, _end) {}

InvalidSyntaxError::InvalidSyntaxError(std::string _details, Position _pos)
: Error("Syntax Error", _details, _pos, _pos)
{
  Position _end = _pos.copy();
  _end.advance();
  end = _end;
}

RTError::RTError(std::string _details, Position _start, Position _end, Context* _context)
: Error("Runtime Error", _details, _start, _end), context(_context) {}

RTError::RTError(std::string _details, Position _pos, Context* _context)
: Error("Runtime Error", _details, _pos, _pos), context(_context)
{
  Position _end = _pos.copy();
  _end.advance();
  end = _end;
}

std::string IllegalCharError::to_string()
{
  return "\033[1;91m" + error_name + "\033[0m\n" + "\033[0;30mFile " + start.to_string() + "\033[0m" + "\n" + details;
}

std::string InvalidSyntaxError::to_string()
{
  return "\033[1;91m" + error_name + "\033[0m\n" + "\033[0;30mFile " + start.to_string() + "\033[0m" + "\n" + details;
}


std::string RTError::to_string()
{
  std::string res = "\033[1;91m" + error_name + "\033[0m\n";

  if (context != nullptr)
  {
    Context* current_context = context;
    res += "Traceback (most recent call last):";
    
    while (current_context->parent != nullptr)
    {
      res += util::color::colorize("\n  In File " + current_context->parent_entry_pos->to_string(), util::color::black);
      current_context = current_context->parent;
    }

    res += util::color::colorize("\n  In File " + start.to_string(), util::color::black);
    res += "\n\n";
  }

  res += details;

  return res;
}