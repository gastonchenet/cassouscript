#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Position.hpp"
#include "Token.hpp"
#include "../constants/Keyword.hpp"
#include "Error.hpp"

struct Lexer
{
private:
  std::shared_ptr<IllegalCharError> make_identifier();
  std::shared_ptr<IllegalCharError> make_number();
  std::shared_ptr<IllegalCharError> make_string();
  std::shared_ptr<IllegalCharError> make_equals();
  std::shared_ptr<IllegalCharError> make_or();
  std::shared_ptr<IllegalCharError> make_and();
  std::shared_ptr<IllegalCharError> make_not();
  std::shared_ptr<IllegalCharError> make_lower_than();
  std::shared_ptr<IllegalCharError> make_greater_than_or_comment();
  std::shared_ptr<IllegalCharError> make_plus();
  std::shared_ptr<IllegalCharError> make_arrow_or_minus();
  std::shared_ptr<IllegalCharError> make_mul();
  std::shared_ptr<IllegalCharError> make_div();
  std::shared_ptr<IllegalCharError> make_mod();
  std::shared_ptr<IllegalCharError> make_power();

public:
  std::string input;
  Position position;
  std::vector<Token> tokens = {};

  Lexer(std::string _input, std::string _filename);

  void advance();
  std::shared_ptr<IllegalCharError> make_tokens();
  void print_tokens();
};