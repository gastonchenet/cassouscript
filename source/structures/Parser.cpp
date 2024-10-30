#include "Parser.hpp"

Parser::Parser(std::vector<Token> _tokens)
: tokens(_tokens)
{
  if (!tokens.empty())
  {
    current_token = tokens[token_index];
  }
  else
  {
    throw std::invalid_argument("No tokens");
  }
}

ParseResult Parser::atom()
{
  ParseResult result = ParseResult();

  if (current_token->match(TokenType::TT_NUMBER))
  {
    std::shared_ptr<Node> node = std::make_shared<NumberNode>(*current_token);
    result.register_advancement();
    advance();
    return result.success(node);
  } else if (current_token->match(TokenType::TT_STRING))
  {
    std::shared_ptr<Node> node = std::make_shared<StringNode>(*current_token);
    result.register_advancement();
    advance();
    return result.success(node);
  }
  else if (current_token->match(TokenType::TT_KEYWORD, {"true", "false"}))
  {
    std::shared_ptr<Node> node = std::make_shared<BooleanNode>(*current_token);
    result.register_advancement();
    advance();
    return result.success(node);
  }
  else if (current_token->match(TokenType::TT_KEYWORD, {"null"}))
  {
    std::shared_ptr<Node> node = std::make_shared<NullNode>(*current_token);
    result.register_advancement();
    advance();
    return result.success(node);
  }
  else if (current_token->match(TokenType::TT_LPAREN))
  {
    result.register_advancement();
    advance();

    std::shared_ptr<Node> node = result.register_result(expr());
    if (result.error != nullptr) return result;

    if (!current_token->match(TokenType::TT_RPAREN))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected ')'", current_token->start, current_token->end));
    }

    result.register_advancement();
    advance();
    return result.success(node);
  }
  else if (current_token->match(TokenType::TT_IDENTIFIER))
  {
    Token var_tok = current_token->copy();
    result.register_advancement();
    advance();

    if (current_token->match(TokenType::TT_ASSIGN))
    {
      result.register_advancement();
      advance();

      std::shared_ptr<Node> content = result.register_result(expr());
      if (result.error != nullptr) return result;

      return result.success(std::make_shared<VarReAssignNode>(var_tok, content));
    }
    else if (current_token->match({TokenType::TT_ADD_ASSIGN,TokenType::TT_SUB_ASSIGN,TokenType::TT_DIV_ASSIGN,TokenType::TT_MUL_ASSIGN,TokenType::TT_MOD_ASSIGN,TokenType::TT_POW_ASSIGN}))
    {
      Token op_tok = current_token->copy();
      result.register_advancement();
      advance();

      std::shared_ptr<Node> content = result.register_result(expr());
      if (result.error != nullptr) return result;

      return result.success(std::make_shared<BinaryOpAssignNode>(var_tok, op_tok, content));
    }
    else if (current_token->match({TokenType::TT_INCR,TokenType::TT_DECR}))
    {
      Token op_tok = current_token->copy();
      result.register_advancement();
      advance();

      return result.success(std::make_shared<UnaryOpAssignNode>(op_tok, var_tok));
    }
    
    return result.success(std::make_shared<VarAccessNode>(var_tok));
  }
  else if (current_token->match(TokenType::TT_LSQUARE))
  {
    std::shared_ptr<Node> node = result.register_result(list_expr());
    if (result.error != nullptr) return result;
    return result.success(node);
  }
  else if (current_token->match(TokenType::TT_LBRACKET))
  {
    std::shared_ptr<Node> node = result.register_result(map_expr());
    if (result.error != nullptr) return result;
    return result.success(node);
  }
  else if (current_token->match(TokenType::TT_KEYWORD, {"if"}))
  {
    std::shared_ptr<Node> expression = result.register_result(if_expr());
    if (result.error != nullptr) return result;
    return result.success(expression);
  }
  else if (current_token->match(TokenType::TT_KEYWORD, {"for"}))
  {
    std::shared_ptr<Node> expression = result.register_result(for_expr());
    if (result.error != nullptr) return result;
    return result.success(expression);
  }
  else if (current_token->match(TokenType::TT_KEYWORD, {"while"}))
  {
    std::shared_ptr<Node> expression = result.register_result(while_expr());
    if (result.error != nullptr) return result;
    return result.success(expression);
  }
  else if (current_token->match(TokenType::TT_KEYWORD, {"from"}))
  {
    std::shared_ptr<Node> expression = result.register_result(num_list_expr());
    if (result.error != nullptr) return result;
    return result.success(expression);
  }
  else
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected int, float, identifier, '+', '-', '(', '[', '{', 'true', 'false', 'null'", current_token->start, current_token->end));
  }
}

ParseResult Parser::crawl()
{
  ParseResult result = ParseResult();

  std::shared_ptr<Node> node = result.register_result(atom());
  if (result.error != nullptr) return result;

  while (current_token->match(TokenType::TT_LSQUARE))
  {
    result.register_advancement();
    advance();

    std::shared_ptr<Node> index = result.register_result(expr());
    if (result.error != nullptr) return result;

    if (!current_token->match(TokenType::TT_RSQUARE))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected ']'", current_token->start, current_token->end));
    }

    result.register_advancement();
    advance();

    node = std::make_shared<AccessNode>(node, index);
  }

  return result.success(node);
}

ParseResult Parser::call()
{
  ParseResult result = ParseResult();

  std::shared_ptr<Node> node = result.register_result(crawl());
  if (result.error != nullptr) return result;

  if (current_token->match(TokenType::TT_LPAREN))
  {
    result.register_advancement();
    advance();

    std::vector<std::shared_ptr<Node>> args = {};

    if (!current_token->match(TokenType::TT_RPAREN))
    {
      std::shared_ptr<Node> arg = result.register_result(expr());
      if (result.error != nullptr) return result;

      args.push_back(arg);

      while (current_token->match(TokenType::TT_COMMA))
      {
        result.register_advancement();
        advance();

        arg = result.register_result(expr());
        if (result.error != nullptr) return result;

        args.push_back(arg);
      }

      if (!current_token->match(TokenType::TT_RPAREN))
      {
        return result.failure(std::make_shared<InvalidSyntaxError>("Expected ')'", current_token->start, current_token->end));
      }
    }

    result.register_advancement();
    advance();

    return result.success(std::make_shared<CallNode>(node, args));
  }

  return result.success(node);
}

ParseResult Parser::power()
{
  return bin_op(FunctionType::CALL, {TokenType::TT_POW}, FunctionType::FACTOR);
}

ParseResult Parser::factor()
{
  ParseResult result = ParseResult();

  if (current_token->match(TokenType::TT_NOT))
  {
    Token op_tok = current_token->copy();
    result.register_advancement();
    advance();

    std::shared_ptr<Node> node = result.register_result(comp_expr());
    if (result.error != nullptr) return result;

    return result.success(std::make_shared<UnaryOpNode>(op_tok, node));
  }
  else if (current_token->match({TokenType::TT_PLUS, TokenType::TT_MINUS}))
  {
    Token op_tok = current_token->copy();
    result.register_advancement();
    advance();

    std::shared_ptr<Node> node = result.register_result(factor());
    if (result.error != nullptr) return result;

    return result.success(std::make_shared<UnaryOpNode>(op_tok, node));
  }
  else
  {
    return power();
  }
}

ParseResult Parser::term()
{
  return bin_op(FunctionType::FACTOR, {TokenType::TT_MUL, TokenType::TT_DIV, TokenType::TT_MOD}, FunctionType::FACTOR);
}

ParseResult Parser::arith_expr()
{
  return bin_op(FunctionType::TERM, {TokenType::TT_PLUS, TokenType::TT_MINUS}, FunctionType::TERM);
}

ParseResult Parser::comp_expr()
{
  return bin_op(FunctionType::ARITH_EXPR, {
    TokenType::TT_EQUALS,
    TokenType::TT_NEQUALS,
    TokenType::TT_LT,
    TokenType::TT_GT,
    TokenType::TT_LTE,
    TokenType::TT_GTE
  }, FunctionType::ARITH_EXPR);
}

ParseResult Parser::bin_op(FunctionType funca, std::list<TokenType> ops, FunctionType funcb)
{
  ParseResult result = ParseResult();

  std::shared_ptr<Node> left;

  switch (funca)
  {
    case FunctionType::ATOM:
      left = result.register_result(atom());
      break;

    case FunctionType::CRAWL:
      left = result.register_result(crawl());
      break;

    case FunctionType::CALL:
      left = result.register_result(call());
      break;

    case FunctionType::FACTOR:
      left = result.register_result(factor());
      break;

    case FunctionType::TERM:
      left = result.register_result(term());
      break;

    case FunctionType::ARITH_EXPR:
      left = result.register_result(arith_expr());
      break;

    case FunctionType::COMP_EXPR:
      left = result.register_result(comp_expr());
      break;

    default:
      throw std::invalid_argument("Invalid function");
  }

  if (result.error != nullptr) return result;

  while (std::find(ops.begin(), ops.end(), current_token->type) != ops.end())
  {
    Token op_tok = current_token->copy();
    result.register_advancement();
    advance();

    std::shared_ptr<Node> right;

    switch (funcb)
    {
      case FunctionType::ATOM:
        right = result.register_result(atom());
        break;

      case FunctionType::CRAWL:
        right = result.register_result(crawl());
        break;

      case FunctionType::CALL:
        right = result.register_result(call());
        break;

      case FunctionType::FACTOR:
        right = result.register_result(factor());
        break;

      case FunctionType::TERM:
        right = result.register_result(term());
        break;

      case FunctionType::ARITH_EXPR:
        right = result.register_result(arith_expr());
        break;

      case FunctionType::COMP_EXPR:
        right = result.register_result(comp_expr());
        break;

      default:
        throw std::invalid_argument("Invalid function");
    }

    if (result.error != nullptr) return result;
    left = std::make_shared<BinaryOpNode>(left, op_tok, right);
  }

  return result.success(left);
}

ParseResult Parser::if_expr()
{
  ParseResult result = ParseResult();

  if (!current_token->match(TokenType::TT_KEYWORD, {"if"}))
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected 'if'", current_token->start, current_token->end));
  }

  result.register_advancement();
  advance();

  std::shared_ptr<Node> condition = result.register_result(expr());
  if (result.error != nullptr) return result;

  std::shared_ptr<Node> if_body;

  if (current_token->match(TokenType::TT_LBRACKET))
  {
    result.register_advancement();
    advance();

    if_body = result.register_result(statements());
    if (result.error != nullptr) return result;

    if (!current_token->match(TokenType::TT_RBRACKET))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected '}'", current_token->start, current_token->end));
    }

    result.register_advancement();
    advance();
  }
  else if (current_token->match(TokenType::TT_ARROW))
  {
    result.register_advancement();
    advance();

    if_body = result.register_result(statement());
    if (result.error != nullptr) return result;
  }
  else
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected '{' or '->'", current_token->start, current_token->end));
  }
  
  if (!current_token->match(TokenType::TT_KEYWORD, {"else"}))
  {
    return result.success(std::make_shared<IfNode>(condition, if_body, nullptr));
  }

  result.register_advancement();
  advance();

  std::shared_ptr<Node> else_body;

  if (current_token->match(TokenType::TT_LBRACKET))
  {
    result.register_advancement();
    advance();

    else_body = result.register_result(statements());
    if (result.error != nullptr) return result;

    if (!current_token->match(TokenType::TT_RBRACKET))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected '}'", current_token->start, current_token->end));
    }

    result.register_advancement();
    advance();
  }
  else if (current_token->match(TokenType::TT_ARROW))
  {
    result.register_advancement();
    advance();

    else_body = result.register_result(statement());
    if (result.error != nullptr) return result;
  }
  else if (current_token->match(TokenType::TT_KEYWORD, {"if"}))
  {
    else_body = result.register_result(if_expr());
    if (result.error != nullptr) return result;
  }
  else
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected '{' or '->'", current_token->start, current_token->end));
  }

  return result.success(std::make_shared<IfNode>(condition, if_body, else_body));
}

ParseResult Parser::for_expr()
{
  ParseResult result = ParseResult();

  if (!current_token->match(TokenType::TT_KEYWORD, {"for"}))
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected 'for'", current_token->start, current_token->end));
  }

  result.register_advancement();
  advance();

  if (!current_token->match(TokenType::TT_IDENTIFIER))
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected identifier", current_token->start, current_token->end));
  }

  Token var_name = current_token->copy();
  result.register_advancement();
  advance();

  if (!current_token->match(TokenType::TT_ASSIGN))
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected '='", current_token->start, current_token->end));
  }
  
  result.register_advancement();
  advance();

  std::shared_ptr<Node> iterable = result.register_result(expr());
  if (result.error != nullptr) return result;

  std::shared_ptr<Node> body;
  bool should_return_null = true;

  if (current_token->match(TokenType::TT_LBRACKET))
  {
    result.register_advancement();
    advance();

    body = result.register_result(statements());
    if (result.error != nullptr) return result;

    if (!current_token->match(TokenType::TT_RBRACKET))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected '}'", current_token->start, current_token->end));
    }

    result.register_advancement();
    advance();
  }
  else if (current_token->match(TokenType::TT_ARROW))
  {
    result.register_advancement();
    advance();

    body = result.register_result(statement());
    should_return_null = false;
    if (result.error != nullptr) return result;
  }
  else
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected '{' or '->'", current_token->start, current_token->end));
  }

  return result.success(std::make_shared<ForNode>(var_name, iterable, body, should_return_null));
}

ParseResult Parser::while_expr()
{
  ParseResult result = ParseResult();

  if (!current_token->match(TokenType::TT_KEYWORD, {"while"}))
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected 'while'", current_token->start, current_token->end));
  }

  result.register_advancement();
  advance();

  std::shared_ptr<Node> condition = result.register_result(expr());
  if (result.error != nullptr) return result;
  
  std::shared_ptr<Node> body;
  bool should_return_null = true;

  if (current_token->match(TokenType::TT_LBRACKET))
  {
    result.register_advancement();
    advance();

    body = result.register_result(statements());
    if (result.error != nullptr) return result;

    if (!current_token->match(TokenType::TT_RBRACKET))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected '}'", current_token->start, current_token->end));
    }

    result.register_advancement();
    advance();
  }
  else if (current_token->match(TokenType::TT_ARROW))
  {
    result.register_advancement();
    advance();

    body = result.register_result(statement());
    should_return_null = false;
    if (result.error != nullptr) return result;
  }
  else
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected '{' or '->'", current_token->start, current_token->end));
  }

  return result.success(std::make_shared<WhileNode>(condition, body, should_return_null));
}

ParseResult Parser::list_expr()
{
  ParseResult result = ParseResult();

  if (!current_token->match(TokenType::TT_LSQUARE))
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected '['", current_token->start, current_token->end));
  }

  Position start = current_token->start.copy();
  result.register_advancement();
  advance();

  std::vector<std::shared_ptr<Node>> elements = {};

  if (!current_token->match(TokenType::TT_RSQUARE))
  {
    std::shared_ptr<Node> element = result.register_result(expr());
    if (result.error != nullptr) return result;

    elements.push_back(element);

    while (current_token->match(TokenType::TT_COMMA))
    {
      result.register_advancement();
      advance();

      element = result.register_result(expr());
      if (result.error != nullptr) return result;

      elements.push_back(element);
    }

    if (!current_token->match(TokenType::TT_RSQUARE))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected ']'", current_token->start, current_token->end));
    }
  }

  result.register_advancement();
  advance();

  return result.success(std::make_shared<ListNode>(elements, start, current_token->end.copy()));
}

ParseResult Parser::num_list_expr()
{
  ParseResult result = ParseResult();

  if (!current_token->match(TokenType::TT_KEYWORD, {"from"}))
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected 'from'", current_token->start, current_token->end));
  }
  
  result.register_advancement();
  advance();

  std::shared_ptr<Node> start = result.register_result(expr());
  if (result.error != nullptr) return result;

  if (!current_token->match(TokenType::TT_KEYWORD, {"to"}))
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected 'to'", current_token->start, current_token->end));
  }

  result.register_advancement();
  advance();

  std::shared_ptr<Node> end = result.register_result(expr());
  if (result.error != nullptr) return result;

  std::shared_ptr<Node> step = nullptr;

  if (current_token->match(TokenType::TT_KEYWORD, {"step"}))
  {
    result.register_advancement();
    advance();

    step = result.register_result(expr());
    if (result.error != nullptr) return result;
  }

  return result.success(std::make_shared<NumListNode>(start, end, step));
}

ParseResult Parser::map_expr()
{
  ParseResult result = ParseResult();

  if (!current_token->match(TokenType::TT_LBRACKET))
  {
    return result.failure(std::make_shared<InvalidSyntaxError>("Expected '{'", current_token->start, current_token->end));
  }

  Position start = current_token->start.copy();
  result.register_advancement();
  advance();

  std::map<std::shared_ptr<Node>, std::shared_ptr<Node>> elements = {};

  if (!current_token->match(TokenType::TT_RBRACKET))
  {
    std::shared_ptr<Node> key = result.register_result(expr());
    if (result.error != nullptr) return result;

    if (!current_token->match(TokenType::TT_BARROW))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected '=>'", current_token->start, current_token->end));
    }

    result.register_advancement();
    advance();

    std::shared_ptr<Node> value = result.register_result(expr());
    if (result.error != nullptr) return result;

    elements[key] = value;

    while (current_token->match(TokenType::TT_COMMA))
    {
      result.register_advancement();
      advance();

      key = result.register_result(expr());
      if (result.error != nullptr) return result;

      if (!current_token->match(TokenType::TT_BARROW))
      {
        return result.failure(std::make_shared<InvalidSyntaxError>("Expected '=>'", current_token->start, current_token->end));
      }

      result.register_advancement();
      advance();

      value = result.register_result(expr());
      if (result.error != nullptr) return result;

      elements[key] = value;
    }

    if (!current_token->match(TokenType::TT_RBRACKET))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected '}'", current_token->start, current_token->end));
    }
  }

  result.register_advancement();
  advance();

  return result.success(std::make_shared<MapNode>(elements, start, current_token->end.copy()));
}

ParseResult Parser::expr()
{
  ParseResult result = ParseResult();

  if (current_token->match(TokenType::TT_KEYWORD, {"set"}))
  {
    result.register_advancement();
    advance();

    Token var_tok = current_token->copy();
    result.register_advancement();
    advance();

    if (current_token->match(TokenType::TT_LPAREN))
    {
      result.register_advancement();
      advance();

      std::vector<Token> args = {};

      if (!current_token->match(TokenType::TT_RPAREN))
      {
        Token arg_tok = current_token->copy();
        args.push_back(arg_tok);
        result.register_advancement();
        advance();

        while (current_token->match(TokenType::TT_COMMA))
        {
          result.register_advancement();
          advance();

          Token arg_tok = current_token->copy();
          args.push_back(arg_tok);
          result.register_advancement();
          advance();
        }

        if (!current_token->match(TokenType::TT_RPAREN))
        {
          return result.failure(std::make_shared<InvalidSyntaxError>("Expected ')'", current_token->start, current_token->end));
        }
      }

      result.register_advancement();
      advance();

      std::shared_ptr<Node> body;
      bool should_auto_return = false;

      if (current_token->match(TokenType::TT_LBRACKET))
      {
        result.register_advancement();
        advance();

        body = result.register_result(statements());
        if (result.error != nullptr) return result;

        if (!current_token->match(TokenType::TT_RBRACKET))
        {
          return result.failure(std::make_shared<InvalidSyntaxError>("Expected '}'", current_token->start, current_token->end));
        }

        result.register_advancement();
        advance();
      }
      else if (current_token->match(TokenType::TT_ARROW))
      {
        result.register_advancement();
        advance();

        body = result.register_result(statement());
        should_auto_return = true;
        if (result.error != nullptr) return result;
      }
      else
      {
        return result.failure(std::make_shared<InvalidSyntaxError>("Expected '{' or '->'", current_token->start, current_token->end));
      }

      return result.success(std::make_shared<FuncDefNode>(var_tok, args, body, should_auto_return));
    }
    else if (current_token->match(TokenType::TT_ASSIGN))
    {
      result.register_advancement();
      advance();

      std::shared_ptr<Node> content = result.register_result(expr());
      if (result.error != nullptr) return result;

      return result.success(std::make_shared<VarAssignNode>(var_tok, content));
    }

    return result.failure(std::make_shared<InvalidSyntaxError>("Expected '(' or '='", current_token->start, current_token->end));
  }

  return bin_op(FunctionType::COMP_EXPR, {TokenType::TT_OR, TokenType::TT_AND}, FunctionType::COMP_EXPR);
}

ParseResult Parser::statement()
{
  ParseResult result = ParseResult();

  if (current_token->match(TokenType::TT_KEYWORD, {"break"}))
  {
    Position start = current_token->start.copy();
    Position end = current_token->end.copy();
    result.register_advancement();
    advance();
    return result.success(std::make_shared<BreakNode>(start, end));
  }

  if (current_token->match(TokenType::TT_KEYWORD, {"continue"}))
  {
    Position start = current_token->start.copy();
    Position end = current_token->end.copy();
    result.register_advancement();
    advance();
    return result.success(std::make_shared<ContinueNode>(start, end));
  }

  if (current_token->match(TokenType::TT_KEYWORD, {"return"}))
  {
    Position start = current_token->start.copy();

    result.register_advancement();
    advance();

    std::shared_ptr<Node> node = result.register_result(expr());
    if (result.error != nullptr) return result;

    return result.success(std::make_shared<ReturnNode>(start, node));
  }

  std::shared_ptr<Node> node = result.register_result(expr());
  if (result.error != nullptr) return result;
  
  return result.success(node);
}

ParseResult Parser::statements()
{
  ParseResult result = ParseResult();
  std::vector<std::shared_ptr<Node>> curr_statements = {};
  Position start = current_token->start.copy();

  while (true)
  {
    if (current_token->match(TokenType::TT_EOF) || current_token->match(TokenType::TT_RBRACKET))
    {
      break;
    }

    std::shared_ptr<Node> curr_statement = result.register_result(statement());
    if (result.error != nullptr) return result;

    if (!current_token->match(TokenType::TT_SEMICOLON))
    {
      return result.failure(std::make_shared<InvalidSyntaxError>("Expected ';'", current_token->start, current_token->end));
    }

    result.register_advancement();
    advance();

    curr_statements.push_back(curr_statement);
  }

  return result.success(std::make_shared<ListNode>(curr_statements, start, current_token->end.copy()));
}

ParseResult Parser::parse()
{
  ParseResult result = ParseResult();
  std::shared_ptr<Node> node = result.register_result(statements());
  return result.success(node);
}

void Parser::print_node(ParseResult* result)
{
  std::cout << result->node->to_string() << std::endl;
}

void Parser::advance()
{
  if (!current_token->match(TokenType::TT_EOF))
  {
    token_index++;

    if (token_index < tokens.size())
    {
      current_token = tokens[token_index];
    }
    else
    {
      current_token = std::nullopt;
    }
  }
}

void Parser::reverse(int num)
{
  token_index -= num;

  if (token_index > 0)
  {
    current_token = tokens[token_index];
  }
  else
  {
    current_token = std::nullopt;
  }
}