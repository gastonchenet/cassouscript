#pragma once

#include <iostream>
#include <string>
#include <list>

enum class KeywordType
{
  KT_SET,
  KT_IF,
  KT_ELSE,
  KT_WHILE,
  KT_FOR,
  KT_TRUE,
  KT_FALSE,
  KT_NULL,
  KT_BREAK,
  KT_CONTINUE,
  KT_RETURN,
  KT_FROM,
  KT_TO,
  KT_STEP,
};

struct Keyword
{
  KeywordType name;
  std::string value;

  Keyword(KeywordType _name, std::string _value);

  static std::list<Keyword> keywords;
  static Keyword getKeyword(std::string value);
  static bool isKeyword(std::string value);
};