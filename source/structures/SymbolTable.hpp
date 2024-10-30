#pragma once

#include <string>
#include <memory>
#include <map>

struct Value;

#include "interpreter/Value.hpp"

struct SymbolTable
{
  std::map<std::string, std::shared_ptr<Value>> table;
  SymbolTable* parent;

  SymbolTable(SymbolTable* _parent = nullptr);

  std::shared_ptr<Value> get(std::string key);
  bool contains(std::string key);
  void set(std::string key, std::shared_ptr<Value> value);
  void remove(std::string key);

  std::string to_string(int depth = 0);
};