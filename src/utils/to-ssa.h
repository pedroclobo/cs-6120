#pragma once

#include "dom-tree.h"
#include "function.h"
#include <stack>

class ToSSA {
public:
  using VarStack = std::unordered_map<Var, std::stack<std::string>>;
  using NameCounter = std::unordered_map<std::string, size_t>;

private:
  VarStack m_stack;
  NameCounter m_counter;
  std::string new_name(std::string_view var_name);
  void rename(BasicBlock &bb, const DomTree &dtree);

public:
  ToSSA() = default;
  static void run(Function &f);
};
