#include "Lexer.hpp"

Lexer::Lexer(std::string _input, std::string _filename)
: input(_input), position(_input, _filename) {}

void Lexer::advance()
{
  position.advance();
}

std::shared_ptr<IllegalCharError> Lexer::make_identifier()
{
  Position start = position.copy();
  std::string identifier = "";

  while (
    position.index < input.length() &&
    (
      ('a' <= input[position.index] && input[position.index] <= 'z') ||
      ('A' <= input[position.index] && input[position.index] <= 'Z') ||
      ('0' <= input[position.index] && input[position.index] <= '9') ||
      input[position.index] == '_'
    )
  )
  {
    identifier += input[position.index];
    advance();
  }

  if (Keyword::isKeyword(identifier))
  {
    tokens.push_back(Token(TokenType::TT_KEYWORD, identifier, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_IDENTIFIER, identifier, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_number()
{
  Position start = position.copy();
  std::string number = "";
  bool dot = false;

  while (position.index < input.length() && ('0' <= input[position.index] && input[position.index] <= '9' || input[position.index] == '.' || input[position.index] == '_'))
  {
    if (input[position.index] == '_')
    {
      advance();
      continue;
    }

    if (input[position.index] == '.')
    {
      if (dot)
      {
        return std::make_shared<IllegalCharError>("Unexpected token '.'", position.copy());
      }

      dot = true;
      number += '.';
    }
    else
    {
      number += input[position.index];
    }

    advance();
  }

  tokens.push_back(Token(TokenType::TT_NUMBER, number, start, position.copy()));

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_string()
{
  Position start = position.copy();
  std::string string = "";
  char quote = input[position.index];
  bool escape = false;
  advance();

  while (position.index < input.length() && input[position.index] != quote)
  {
    if (escape)
    {
      switch (input[position.index])
      {
        case 'n':
          string += '\n';
          break;

        case 't':
          string += '\t';
          break;

        default:
          string += input[position.index];
          break;
      }

      escape = false;
    }
    else
    {
      if (input[position.index] == '\\')
      {
        escape = true;
      }
      else
      {
        string += input[position.index];
      }
    }

    advance();
  }

  advance();

  tokens.push_back(Token(TokenType::TT_STRING, string, start, position.copy()));

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_equals()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_EQUALS, start, position.copy()));
  }
  else if (position.index < input.length() && input[position.index] == '>')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_BARROW, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_ASSIGN, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_or()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '|')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_OR, start, position.copy()));
  }
  else
  {
    return std::make_shared<IllegalCharError>("Unexpected token '|'", position.copy());
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_and()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '&')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_AND, start, position.copy()));
  }
  else
  {
    return std::make_shared<IllegalCharError>("Unexpected token '&'", position.copy());
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_not()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_NEQUALS, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_NOT, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_lower_than()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_LTE, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_LT, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_greater_than_or_comment()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_GTE, start, position.copy()));
  }
  else if (position.index < input.length() && input[position.index] == '>')
  {
    advance();
    
    while (position.index < input.length() && input[position.index] != '\n')
    {
      advance();
    }

    advance();
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_GT, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_plus()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_ADD_ASSIGN, start, position.copy()));
  }
  else if (position.index < input.length() && input[position.index] == '+')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_INCR, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_PLUS, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_arrow_or_minus()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '>')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_ARROW, start, position.copy()));
  }
  else if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_SUB_ASSIGN, start, position.copy()));
  }
  else if (position.index < input.length() && input[position.index] == '-')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_DECR, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_MINUS, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_mul()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_MUL_ASSIGN, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_MUL, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_div()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_DIV_ASSIGN, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_DIV, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_mod()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_MOD_ASSIGN, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_MOD, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_power()
{
  Position start = position.copy();
  advance();

  if (position.index < input.length() && input[position.index] == '=')
  {
    advance();
    tokens.push_back(Token(TokenType::TT_POW_ASSIGN, start, position.copy()));
  }
  else
  {
    tokens.push_back(Token(TokenType::TT_POW, start, position.copy()));
  }

  return nullptr;
}

std::shared_ptr<IllegalCharError> Lexer::make_tokens()
{
  while (position.index < input.length())
  {
    char current_char = input[position.index];

    if (current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\r')
    {
      advance();
      continue;
    }
    else if (('a' <= current_char && current_char <= 'z') || ('A' <= current_char && current_char <= 'Z') || current_char == '_')
    {
      std::shared_ptr<IllegalCharError> error = make_identifier();
      if (error != nullptr) return error;
    }
    else if ('0' <= current_char && current_char <= '9')
    {
      std::shared_ptr<IllegalCharError> error = make_number();
      if (error != nullptr) return error;
    }
    else if (current_char == '"'|| current_char == '\'')
    {
      std::shared_ptr<IllegalCharError> error = make_string();
      if (error != nullptr) return error;
    }
    else if (current_char == '=')
    {
      std::shared_ptr<IllegalCharError> error = make_equals();
      if (error != nullptr) return error;
    } else if (current_char == '|')
    {
      std::shared_ptr<IllegalCharError> error = make_or();
    } else if (current_char == '&')
    {
      std::shared_ptr<IllegalCharError> error = make_and();
      if (error != nullptr) return error;
    }
    else if (current_char == '!')
    {
      std::shared_ptr<IllegalCharError> error = make_not();
      if (error != nullptr) return error;
    }
    else if (current_char == '<')
    {
      std::shared_ptr<IllegalCharError> error = make_lower_than();
      if (error != nullptr) return error;
    }
    else if (current_char == '>')
    {
      std::shared_ptr<IllegalCharError> error = make_greater_than_or_comment();
      if (error != nullptr) return error;
    }
    else if (current_char == '+')
    {
      std::shared_ptr<IllegalCharError> error = make_plus();
      if (error != nullptr) return error;
    }
    else if (current_char == '-')
    {
      std::shared_ptr<IllegalCharError> error = make_arrow_or_minus();
      if (error != nullptr) return error;
    }
    else if (current_char == '*')
    {
      std::shared_ptr<IllegalCharError> error = make_mul();
      if (error != nullptr) return error;
    }
    else if (current_char == '/')
    {
      std::shared_ptr<IllegalCharError> error = make_div();
      if (error != nullptr) return error;
    }
    else if (current_char == '%')
    {
      std::shared_ptr<IllegalCharError> error = make_mod();
      if (error != nullptr) return error;
    }
    else if (current_char == '^')
    {
      std::shared_ptr<IllegalCharError> error = make_power();
      if (error != nullptr) return error;
    }
    else if (current_char == '(')
    {
      tokens.push_back(Token(TokenType::TT_LPAREN, position.copy()));
      advance();
    }
    else if (current_char == ')')
    {
      tokens.push_back(Token(TokenType::TT_RPAREN, position.copy()));
      advance();
    }
    else if (current_char == '{')
    {
      tokens.push_back(Token(TokenType::TT_LBRACKET, position.copy()));
      advance();
    }
    else if (current_char == '}')
    {
      tokens.push_back(Token(TokenType::TT_RBRACKET, position.copy()));
      advance();
    }
    else if (current_char == '[')
    {
      tokens.push_back(Token(TokenType::TT_LSQUARE, position.copy()));
      advance();
    }
    else if (current_char == ']')
    {
      tokens.push_back(Token(TokenType::TT_RSQUARE, position.copy()));
      advance();
    }
    else if (current_char == ',')
    {
      tokens.push_back(Token(TokenType::TT_COMMA, position.copy()));
      advance();
    }
    else if (current_char == ';')
    {
      tokens.push_back(Token(TokenType::TT_SEMICOLON, position.copy()));
      advance();
    }
    else
    {
      return std::make_shared<IllegalCharError>("Unexpected token '" + std::string(1, current_char) + "'", position.copy());
    }
  }

  advance();
  
  tokens.push_back(Token(TokenType::TT_EOF, position.copy(), position.copy()));

  return nullptr;
}

void Lexer::print_tokens()
{
  std::cout << "Tokens(" << std::endl;

  for (int i = 0; i < tokens.size(); i++)
  {
    Token token = tokens[i];
    std::cout << "  " + token.to_string() + (i < tokens.size() - 1 ? "," : "") << std::endl;
  }

  std::cout << ")" << std::endl;
}