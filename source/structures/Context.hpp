#pragma once

#include <string>

struct SymbolTable;

#include "Position.hpp"
#include "SymbolTable.hpp"

struct Context
{
  std::string display_name;
  Context* parent;
  Position* parent_entry_pos;
  SymbolTable* symbol_table = nullptr;

  Context(std::string _display_name, Context* _parent = nullptr, Position* _parent_entry_pos = nullptr);

  void set_symbol_table(SymbolTable* _symbol_table);
};