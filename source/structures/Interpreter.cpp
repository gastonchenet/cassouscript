#include "Interpreter.hpp"

RTResult Interpreter::crawl(std::shared_ptr<Node> node, std::vector<std::shared_ptr<Node>> path, Context* context)
{
  RTResult result = RTResult();

  
}

RTResult Interpreter::visit_number(NumberNode node, Context* context)
{
  return RTResult().success(std::make_shared<Number>(std::stod(node.value_tok.value), node.start, node.end, context));
}

RTResult Interpreter::visit_string(StringNode node, Context* context)
{
  return RTResult().success(std::make_shared<String>(node.value_tok.value, node.start, node.end, context));
}

RTResult Interpreter::visit_boolean(BooleanNode node, Context* context)
{
  return RTResult().success(std::make_shared<Boolean>(node.value_tok.value == "true", node.start, node.end, context));
}

RTResult Interpreter::visit_list(ListNode node, Context* context)
{
  RTResult result = RTResult();
  std::vector<std::shared_ptr<Value>> values = {};

  for (auto& elem_node : node.nodes)
  { 
    std::shared_ptr<Value> value = result.register_result(visit(elem_node, context));
    if (result.should_return()) return result;

    values.push_back(value);
  }

  return result.success(std::make_shared<List>(values, node.start, node.end, context));
}

RTResult Interpreter::visit_map(MapNode node, Context* context)
{
  RTResult result = RTResult();
  std::map<std::string, std::shared_ptr<Value>> values = {};

  for (auto& pair : node.pairs)
  {
    std::shared_ptr<Value> key = result.register_result(visit(pair.first, context));
    if (result.should_return()) return result;

    if (key->type != ValueType::STRING)
    {
      return result.failure(std::make_shared<RTError>("Map key must be a string", pair.first->start, pair.first->end, context));
    }

    std::shared_ptr<Value> value = result.register_result(visit(pair.second, context));
    if (result.should_return()) return result;

    values[dynamic_cast<String&>(*key).value] = value;
  }

  return result.success(std::make_shared<Map>(values, node.start, node.end, context));
}

RTResult Interpreter::visit_null(NullNode node, Context* context)
{
  return RTResult().success(std::make_shared<Null>(node.start, node.end, context));
}

RTResult Interpreter::visit_unary_op(UnaryOpNode node, Context* context)
{
  RTResult result = RTResult();

  std::shared_ptr<Value> operand = result.register_result(visit(node.operand, context));
  if (result.should_return()) return result;

  std::shared_ptr<Value> value;

  switch (node.op_tok.type)
  {
    case TokenType::TT_PLUS:
      value = result.register_result(operand->to_positive());
      break;

    case TokenType::TT_MINUS:
      value = result.register_result(operand->to_negative());
      break;

    case TokenType::TT_NOT:
      value = result.register_result(operand->to_not());
      break;

    default:
      throw std::invalid_argument("Invalid unary operator: " + node.op_tok.to_string_type());
  }

  if (result.should_return()) return result;

  return result.success(value);
}

RTResult Interpreter::visit_binary_op(BinaryOpNode node, Context* context)
{
  RTResult result = RTResult();

  std::shared_ptr<Value> left = result.register_result(visit(node.left, context));
  if (result.should_return()) return result;

  std::shared_ptr<Value> right = result.register_result(visit(node.right, context));
  if (result.should_return()) return result;

  std::shared_ptr<Value> value;

  switch (node.op_tok.type)
  {
    case TokenType::TT_PLUS:
      value = result.register_result(left->add(right));
      break;

    case TokenType::TT_MINUS:
      value = result.register_result(left->subtract(right));
      break;

    case TokenType::TT_MUL:
      value = result.register_result(left->multiply(right));
      break;

    case TokenType::TT_DIV:
      value = result.register_result(left->divide(right));
      break;

    case TokenType::TT_POW:
      value = result.register_result(left->power(right));
      break;

    case TokenType::TT_EQUALS:
      value = result.register_result(left->equal(right));
      break;

    case TokenType::TT_NEQUALS:
      value = result.register_result(left->not_equal(right));
      break;

    case TokenType::TT_LT:
      value = result.register_result(left->less_than(right));
      break;

    case TokenType::TT_GT:
      value = result.register_result(left->greater_than(right));
      break;

    case TokenType::TT_LTE:
      value = result.register_result(left->less_than_or_equal(right));
      break;

    case TokenType::TT_GTE:
      value = result.register_result(left->greater_than_or_equal(right));
      break;

    case TokenType::TT_AND:
      value = result.register_result(left->and_op(right));
      break;

    case TokenType::TT_OR:
      value = result.register_result(left->or_op(right));
      break;

    default:
      throw std::invalid_argument("Invalid binary operator: " + node.op_tok.to_string_type());
  }

  if (result.should_return()) return result;

  return result.success(value);
}

RTResult Interpreter::visit_var_assign(VarAssignNode node, Context* context)
{
  RTResult result = RTResult();

  std::string var_name = node.var_name.value;
  std::shared_ptr<Value> value = result.register_result(visit(node.value, context));

  if (context->symbol_table->contains(var_name))
  {
    return result.failure(std::make_shared<RTError>("Variable '" + var_name + "' is already defined", node.start, node.end, context));
  }

  context->symbol_table->set(var_name, value);

  return result.success(value);
}

RTResult Interpreter::visit_var_reassign(VarReAssignNode node, Context* context)
{
  RTResult result = RTResult();

  std::string var_name = node.var_name.value;
  std::shared_ptr<Value> value = result.register_result(visit(node.value, context));

  if (!context->symbol_table->contains(var_name))
  {
    result.failure(std::make_shared<RTError>("Variable '" + var_name + "' is not defined", node.start, node.end, context));
  }

  context->symbol_table->set(var_name, value);
  return result.success(value);
}

RTResult Interpreter::visit_var_access(VarAccessNode node, Context* context)
{
  RTResult result = RTResult();

  std::string var_name = node.var_name.value;
  std::shared_ptr<Value> value = context->symbol_table->get(var_name);

  if (value == nullptr)
  {
    return result.failure(std::make_shared<RTError>("Variable '" + var_name + "' is not defined", node.start, node.end, context));
  }

  return result.success(value);
}

RTResult Interpreter::visit_access(AccessNode node, Context* context)
{
  RTResult result = RTResult();

  std::shared_ptr<Value> var = result.register_result(visit(node.var_node, context));
  if (result.should_return()) return result;

  if (var->type == ValueType::LIST)
  {
    List list = dynamic_cast<List&>(*var);
    std::shared_ptr<Value> index = result.register_result(visit(node.index_node, context));
    if (result.should_return()) return result;

    if (index->type != ValueType::NUMBER)
    {
      return result.failure(std::make_shared<RTError>("Index must be a number", node.index_node->start, node.index_node->end, context));
    }

    int idx = dynamic_cast<Number&>(*index).value;

    if (idx < 0 || idx >= list.values.size())
    {
      return result.failure(std::make_shared<RTError>("Index out of bounds", node.index_node->start, node.index_node->end, context));
    }

    return result.success(list.values[idx]);
  }
  else if (var->type == ValueType::MAP)
  {
    Map map = dynamic_cast<Map&>(*var);
    std::shared_ptr<Value> key = result.register_result(visit(node.index_node, context));
    if (result.should_return()) return result;

    if (key->type != ValueType::STRING)
    {
      return result.failure(std::make_shared<RTError>("Key must be a string", node.index_node->start, node.index_node->end, context));
    }

    std::string key_str = dynamic_cast<String&>(*key).value;

    if (map.values.find(key_str) == map.values.end())
    {
      return result.failure(std::make_shared<RTError>("Key not found", node.index_node->start, node.index_node->end, context));
    }

    return result.success(map.values[key_str]);
  }

  return result.failure(std::make_shared<RTError>("Invalid access", node.start, node.end, context));
}

RTResult Interpreter::visit_call(CallNode node, Context* context)
{
  RTResult result = RTResult();

  std::shared_ptr<Value> func_name = result.register_result(visit(node.func_name, context));
  if (result.should_return()) return result;
  std::vector<std::shared_ptr<Value>> args = {};

  for (std::shared_ptr<Node> arg_node : node.args)
  {
    args.push_back(result.register_result(visit(arg_node, context)));
    if (result.should_return()) return result;
  }

  std::shared_ptr<Value> return_value;

  if (func_name->type == ValueType::FUNCTION)
  {
    Function func = dynamic_cast<Function&>(*func_name);
    return_value = result.register_result(func.execute(args));
  }
  else if (func_name->type == ValueType::BUILTIN_FUNCTION)
  {
    BuiltInFunction func = dynamic_cast<BuiltInFunction&>(*func_name);
    return_value = result.register_result(func.execute(args));
  }
  else
  {
    return result.failure(std::make_shared<RTError>("Variable '" + func_name->to_string() + "' is not function", node.func_name->start, node.func_name->end, context));
  }

  if (result.should_return()) return result;

  return result.success(return_value);
}

RTResult Interpreter::visit_return(ReturnNode node, Context* context)
{
  RTResult result = RTResult();
  std::shared_ptr<Value> value = std::make_shared<Null>(node.start, node.end, context);

  if (node.node)
  {
    value = result.register_result(visit(node.node, context));
    if (result.should_return()) return result;
  }

  return result.success_return(value);
}

RTResult Interpreter::visit_if(IfNode node, Context* context)
{
  RTResult result = RTResult();

  std::shared_ptr<Value> condition = result.register_result(visit(node.if_cond, context));

  if (condition->is_true())
  {
    std::shared_ptr<Value> value = result.register_result(visit(node.if_body, context));
    if (result.should_return()) return result;

    return result.success(value);
  }
  else if (node.else_body != nullptr)
  {
    std::shared_ptr<Value> value = result.register_result(visit(node.else_body, context));
    if (result.should_return()) return result;

    return result.success(value);
  }

  return result.success(std::make_shared<Null>(node.start, node.end, context));
}

RTResult Interpreter::visit_num_list(NumListNode node, Context* context)
{
  RTResult result = RTResult();

  std::shared_ptr<Value> start_value = result.register_result(visit(node.start_node, context));
  if (result.should_return()) return result;
  
  std::shared_ptr<Value> end_value = result.register_result(visit(node.end_node, context));
  if (result.should_return()) return result;

  int start_val = dynamic_cast<Number&>(*start_value).value;
  int end_val = dynamic_cast<Number&>(*end_value).value;

  int step_val = 1;

  if (node.step_node != nullptr)
  {
    std::shared_ptr<Value> step_value = result.register_result(visit(node.step_node, context));
    if (result.should_return()) return result;

    step_val = dynamic_cast<Number&>(*step_value).value;
  }

  std::vector<std::shared_ptr<Value>> values = {};

  for (int i = start_val; i < end_val; i += step_val)
  {
    values.push_back(std::make_shared<Number>(i, node.start, node.end, context));
  }

  return result.success(std::make_shared<List>(values, node.start, node.end, context));
}

RTResult Interpreter::visit_while(WhileNode node, Context* context)
{
  RTResult result = RTResult();
  std::vector<std::shared_ptr<Value>> results = {};

  while (true)
  {
    std::shared_ptr<Value> condition = result.register_result(visit(node.condition, context));
    if (result.should_return()) return result;
    if (!condition->is_true()) break;

    std::shared_ptr<Value> value = result.register_result(visit(node.body, context));
    if (result.should_return() && !result.loop_should_continue && !result.loop_should_break) return result;

    if (result.loop_should_continue) continue;
    if (result.loop_should_break) break;

    results.push_back(value);
  }

  if (node.should_return_null)
  {
    return result.success(std::make_shared<Null>(node.start, node.end, context));
  }

  return result.success(std::make_shared<Null>(node.start, node.end, context));
}

RTResult Interpreter::visit_for(ForNode node, Context* context)
{
  RTResult result = RTResult();
  std::vector<std::shared_ptr<Value>> results = {};

  if (node.iterable->type != NodeType::NUM_LIST && node.iterable->type != NodeType::LIST)
  {
    return result.failure(std::make_shared<RTError>("Expected a list", node.iterable->start, node.iterable->end, context));
  }

  std::shared_ptr<Value> iterable = result.register_result(visit(node.iterable, context));
  if (result.should_return()) return result;

  if (iterable->type != ValueType::LIST)
  {
    return result.failure(std::make_shared<RTError>("Expected a list", node.iterable->start, node.iterable->end, context));
  }

  Context new_context = Context("<for-loop>", context, &node.start);
  new_context.set_symbol_table(new SymbolTable(new_context.parent->symbol_table));

  List list = dynamic_cast<List&>(*iterable);
  std::vector<std::shared_ptr<Value>> values = list.values;

  for (auto& list_value : values)
  {
    new_context.symbol_table->set(node.name_token.value, list_value);

    std::shared_ptr<Value> value = result.register_result(visit(node.body, &new_context));
    if (result.should_return() && !result.loop_should_continue && !result.loop_should_break) return result;

    if (result.loop_should_continue) continue;
    if (result.loop_should_break) break;
    
    results.push_back(value);
  }

  if (node.should_return_null)
  {
    return result.success(std::make_shared<Null>(node.start, node.end, context));
  }

  return result.success(std::make_shared<List>(results, node.start, node.end, context));
}

RTResult Interpreter::visit_func_def(FuncDefNode node, Context* context)
{
  RTResult result = RTResult();

  std::vector<std::string> arg_names = {};

  for (auto& arg_name : node.arg_name_toks)
  {
    arg_names.push_back(arg_name.value);
  }

  Function func_value = Function(node.name_tok.value, node.start, node.end, node.body_node, arg_names, node.should_auto_return, context);
  func_value.set_context(context);

  if (node.name_tok.value != "" && context->symbol_table != nullptr)
  {
    context->symbol_table->set(node.name_tok.value, std::make_shared<Function>(func_value));
  }

  return result.success(std::make_shared<Function>(func_value));
}

RTResult Interpreter::visit_binary_op_assign(BinaryOpAssignNode node, Context* context)
{
  RTResult result = RTResult();

  std::shared_ptr<Value> value = context->symbol_table->get(node.left.value);

  if (value == nullptr)
  {
    return result.failure(std::make_shared<RTError>("Variable '" + node.left.value + "' is not defined", node.start, node.end, context));
  }

  std::shared_ptr<Value> right = result.register_result(visit(node.right, context));

  switch (node.op_tok.type)
  {
    case TokenType::TT_ADD_ASSIGN:
      value = result.register_result(value->add(right));
      break;

    case TokenType::TT_SUB_ASSIGN:
      value = result.register_result(value->subtract(right));
      break;

    case TokenType::TT_MUL_ASSIGN:
      value = result.register_result(value->multiply(right));
      break;

    case TokenType::TT_DIV_ASSIGN:
      value = result.register_result(value->divide(right));
      break;

    case TokenType::TT_MOD_ASSIGN:
      value = result.register_result(value->modulo(right));
      break;

    case TokenType::TT_POW_ASSIGN:
      value = result.register_result(value->power(right));
      break;

    default:
      throw std::invalid_argument("Invalid binary operator: " + node.op_tok.to_string_type());
  }

  if (result.should_return()) return result;

  context->symbol_table->set(node.left.value, value);

  return result.success(value);
}

RTResult Interpreter::visit_unary_op_assign(UnaryOpAssignNode node, Context* context)
{
  RTResult result = RTResult();

  std::shared_ptr<Value> value = context->symbol_table->get(node.var_name_tok.value);

  if (value == nullptr)
  {
    return result.failure(std::make_shared<RTError>("Variable '" + node.var_name_tok.value + "' is not defined", node.start, node.end, context));
  }

  if (value->type != ValueType::NUMBER)
  {
    return result.failure(std::make_shared<RTError>("Variable '" + node.var_name_tok.value + "' is not a number", node.start, node.end, context));
  }

  switch (node.op_tok.type)
  {
    case TokenType::TT_INCR:
      value = result.register_result(value->add(std::make_shared<Number>(1, node.start, node.end, context)));
      break;

    case TokenType::TT_DECR:
      value = result.register_result(value->subtract(std::make_shared<Number>(1, node.start, node.end, context)));
      break;

    default:
      throw std::invalid_argument("Invalid unary operator: " + node.op_tok.to_string_type());
  }

  if (result.should_return()) return result;

  context->symbol_table->set(node.var_name_tok.value, value);

  return result.success(value);
}

RTResult Interpreter::visit(std::shared_ptr<Node> node, Context* context)
{
  switch (node->type)
  {
    case NodeType::NUMBER:
      return visit_number(dynamic_cast<NumberNode&>(*node), context);

    case NodeType::STRING:
      return visit_string(dynamic_cast<StringNode&>(*node), context);

    case NodeType::BOOLEAN:
      return visit_boolean(dynamic_cast<BooleanNode&>(*node), context);

    case NodeType::LIST:
      return visit_list(dynamic_cast<ListNode&>(*node), context);

    case NodeType::MAP:
      return visit_map(dynamic_cast<MapNode&>(*node), context);

    case NodeType::NULL_NODE:
      return visit_null(dynamic_cast<NullNode&>(*node), context);

    case NodeType::BINARY_OPERATION:
      return visit_binary_op(dynamic_cast<BinaryOpNode&>(*node), context);

    case NodeType::UNARY_OPERATION:
      return visit_unary_op(dynamic_cast<UnaryOpNode&>(*node), context);

    case NodeType::VAR_ASSIGN:
      return visit_var_assign(dynamic_cast<VarAssignNode&>(*node), context);

    case NodeType::VAR_REASSIGN:
      return visit_var_reassign(dynamic_cast<VarReAssignNode&>(*node), context);

    case NodeType::VAR_ACCESS:
      return visit_var_access(dynamic_cast<VarAccessNode&>(*node), context);

    case NodeType::CALL:
      return visit_call(dynamic_cast<CallNode&>(*node), context);

    case NodeType::ACCESS:
      return visit_access(dynamic_cast<AccessNode&>(*node), context);

    case NodeType::RETURN:
      return visit_return(dynamic_cast<ReturnNode&>(*node), context);

    case NodeType::IF:
      return visit_if(dynamic_cast<IfNode&>(*node), context);

    case NodeType::NUM_LIST:
      return visit_num_list(dynamic_cast<NumListNode&>(*node), context);

    case NodeType::WHILE:
      return visit_while(dynamic_cast<WhileNode&>(*node), context);

    case NodeType::FOR:
      return visit_for(dynamic_cast<ForNode&>(*node), context);

    case NodeType::FUNC_DEF:
      return visit_func_def(dynamic_cast<FuncDefNode&>(*node), context);

    case NodeType::BINARY_OP_ASSIGN:
      return visit_binary_op_assign(dynamic_cast<BinaryOpAssignNode&>(*node), context);

    case NodeType::UNARY_OP_ASSIGN:
      return visit_unary_op_assign(dynamic_cast<UnaryOpAssignNode&>(*node), context);

    case NodeType::BREAK:
      return RTResult().success_break();

    case NodeType::CONTINUE:
      return RTResult().success_continue();

    default:
      throw std::invalid_argument("Invalid node type: " + node->to_string(1));
  }
}