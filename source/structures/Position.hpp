#pragma once

#include <string>

struct Position
{
private:
  std::string code;

public:
  int index;
  int line;
  int column;
  std::string filename;

  Position(std::string _code, int _index, int _line, int _column, std::string _filename);
  Position(std::string _code, std::string _filename);
  Position();

  void advance();
  Position copy();
  std::string to_string();

  bool operator==(const Position& other) const;
};