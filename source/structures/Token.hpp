#pragma once

#include <iostream>
#include <string>
#include <list>

#include "Position.hpp"

enum class TokenType
{
  TT_EOF,
  TT_IDENTIFIER,
  TT_KEYWORD,
  TT_NUMBER,
  TT_STRING,
  TT_ASSIGN,
  TT_EQUALS,
  TT_NEQUALS,
  TT_PLUS,
  TT_MINUS,
  TT_MUL,
  TT_DIV,
  TT_MOD,
  TT_LPAREN,
  TT_RPAREN,
  TT_NOT,
  TT_OR,
  TT_AND,
  TT_LT,
  TT_GT,
  TT_LTE,
  TT_GTE,
  TT_POW,
  TT_COMMA,
  TT_SEMICOLON,
  TT_LBRACKET,
  TT_RBRACKET,
  TT_ARROW,
  TT_INCR,
  TT_DECR,
  TT_ADD_ASSIGN,
  TT_SUB_ASSIGN,
  TT_MUL_ASSIGN,
  TT_DIV_ASSIGN,
  TT_MOD_ASSIGN,
  TT_POW_ASSIGN,
  TT_LSQUARE,
  TT_RSQUARE,
  TT_BARROW
};

struct Token
{
  TokenType type;
  std::string value;
  Position start;
  Position end;

  Token(TokenType _type, std::string _value, Position _start, Position _end);
  Token(TokenType _type, Position _start, Position _end);
  Token(TokenType _type, Position _pos);

  bool match(TokenType correct_type, std::list<std::string> values = {});
  bool match(std::list<TokenType> types);

  Token copy();

  std::string to_string();
  std::string to_string_type();
};