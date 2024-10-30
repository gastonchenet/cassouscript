#pragma once

#include <string>
#include <memory>
#include <vector>

#include "RTResult.hpp"
#include "SymbolTable.hpp"
#include "Token.hpp"
#include "nodes/Node.hpp"
#include "interpreter/Value.hpp"
#include "interpreter/Number.hpp"
#include "interpreter/String.hpp"
#include "interpreter/Boolean.hpp"
#include "interpreter/List.hpp"
#include "interpreter/Map.hpp"
#include "interpreter/Null.hpp"
#include "interpreter/Function.hpp"
#include "interpreter/BuiltInFunction.hpp"
#include "nodes/NumberNode.hpp"
#include "nodes/StringNode.hpp"
#include "nodes/BooleanNode.hpp"
#include "nodes/NullNode.hpp"
#include "nodes/BinaryOpNode.hpp"
#include "nodes/UnaryOpNode.hpp"
#include "nodes/VarAssignNode.hpp"
#include "nodes/VarReAssignNode.hpp"
#include "nodes/VarAccessNode.hpp"
#include "nodes/CallNode.hpp"
#include "nodes/NullNode.hpp"
#include "nodes/ReturnNode.hpp"
#include "nodes/MapNode.hpp"
#include "nodes/ListNode.hpp"
#include "nodes/IfNode.hpp"
#include "nodes/NumListNode.hpp"
#include "nodes/WhileNode.hpp"
#include "nodes/ForNode.hpp"
#include "nodes/FuncDefNode.hpp"
#include "nodes/BinaryOpAssignNode.hpp"
#include "nodes/UnaryOpAssignNode.hpp"
#include "nodes/AccessNode.hpp"

struct Interpreter
{
private:
  RTResult visit_number(NumberNode node, Context* context);
  RTResult visit_string(StringNode node, Context* context);
  RTResult visit_boolean(BooleanNode node, Context* context);
  RTResult visit_null(NullNode node, Context* context);
  RTResult visit_binary_op(BinaryOpNode node, Context* context);
  RTResult visit_unary_op(UnaryOpNode node, Context* context);
  RTResult visit_var_assign(VarAssignNode node, Context* context);
  RTResult visit_var_reassign(VarReAssignNode node, Context* context);
  RTResult visit_var_access(VarAccessNode node, Context* context);
  RTResult visit_access(AccessNode node, Context* context);
  RTResult visit_call(CallNode node, Context* context);
  RTResult visit_return(ReturnNode node, Context* context);
  RTResult visit_list(ListNode node, Context* context);
  RTResult visit_map(MapNode node, Context* context);
  RTResult visit_if(IfNode node, Context* context);
  RTResult visit_num_list(NumListNode node, Context* context);
  RTResult visit_while(WhileNode node, Context* context);
  RTResult visit_for(ForNode node, Context* context);
  RTResult visit_func_def(FuncDefNode node, Context* context);
  RTResult visit_binary_op_assign(BinaryOpAssignNode node, Context* context);
  RTResult visit_unary_op_assign(UnaryOpAssignNode node, Context* context);

public:
  Context* context;

  std::shared_ptr<Node> node;
  RTResult visit(std::shared_ptr<Node> node, Context* context);
};