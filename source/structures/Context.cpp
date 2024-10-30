#include "Context.hpp"

Context::Context(std::string _display_name, Context* _parent, Position* _parent_entry_pos)
: display_name(_display_name), parent(_parent), parent_entry_pos(_parent_entry_pos) {}

void Context::set_symbol_table(SymbolTable* _symbol_table)
{
  symbol_table = _symbol_table;
}