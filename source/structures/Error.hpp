#pragma once

#include <string>

#include "Position.hpp"
#include "Context.hpp"
#include "../util/color.hpp"

struct Error
{
  std::string error_name;
  std::string details;
  Position start;
  Position end;

  Error(std::string _error_name, std::string _details, Position _start, Position _end);

  virtual std::string to_string() = 0;
};

struct IllegalCharError : public virtual Error
{
  IllegalCharError(std::string _details, Position _start, Position _end);
  IllegalCharError(std::string _details, Position _pos);

  std::string to_string() override;
};

struct InvalidSyntaxError : public virtual Error
{
  InvalidSyntaxError(std::string _details, Position _start, Position _end);
  InvalidSyntaxError(std::string _details, Position _pos);

  std::string to_string() override;
};

struct RTError : public virtual Error
{
  Context* context = nullptr;

  RTError(std::string _details, Position _start, Position _end, Context* _context = nullptr);
  RTError(std::string _details, Position _pos, Context* _context = nullptr);

  std::string to_string() override;
};