#include "Keyword.hpp"

Keyword::Keyword(KeywordType _name, std::string _value)
: name(_name), value(_value) {}

std::list<Keyword> Keyword::keywords = {
  Keyword(KeywordType::KT_TRUE, "true"),
  Keyword(KeywordType::KT_FALSE, "false"),
  Keyword(KeywordType::KT_SET, "set"),
  Keyword(KeywordType::KT_IF, "if"),
  Keyword(KeywordType::KT_ELSE, "else"),
  Keyword(KeywordType::KT_WHILE, "while"),
  Keyword(KeywordType::KT_FOR, "for"),
  Keyword(KeywordType::KT_NULL, "null"),
  Keyword(KeywordType::KT_BREAK, "break"),
  Keyword(KeywordType::KT_CONTINUE, "continue"),
  Keyword(KeywordType::KT_RETURN, "return"),
  Keyword(KeywordType::KT_FROM, "from"),
  Keyword(KeywordType::KT_TO, "to"),
  Keyword(KeywordType::KT_STEP, "step"),
};

Keyword Keyword::getKeyword(std::string value)
{
  for (Keyword keyword : keywords)
  {
    if (keyword.value == value)
    {
      return keyword;
    }
  }

  throw std::runtime_error("Invalid keyword value");
}

bool Keyword::isKeyword(std::string value)
{
  for (Keyword keyword : keywords)
  {
    if (keyword.value == value)
    {
      return true;
    }
  }

  return false;
}