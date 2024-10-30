#include "SymbolTable.hpp"

SymbolTable::SymbolTable(SymbolTable* _parent)
: parent(_parent) {}

std::shared_ptr<Value> SymbolTable::get(std::string key)
{
  if (table.find(key) != table.end())
  {
    return table[key];
  }
  
  if (parent != nullptr)
  {
    return parent->get(key);
  }

  return nullptr;
}

bool SymbolTable::contains(std::string key)
{
  return table.find(key) != table.end();
}

void SymbolTable::set(std::string key, std::shared_ptr<Value> value)
{
  table.insert_or_assign(key, value);
}

void SymbolTable::remove(std::string key)
{
  if (table.find(key) != table.end())
  {
    table.erase(key);
  }
}

std::string SymbolTable::to_string(int depth)
{
  depth++;
  std::string res = "";
  
  res += "SymbolTable(\n";
  for (auto& [key, value] : table)
  {
    res += std::string(2 * depth, ' ') + key + ": " + value->to_string() + ",\n";
  }
  res += std::string(2 * (depth - 1), ' ') + ")";

  return res;
}