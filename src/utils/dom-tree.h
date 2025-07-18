#pragma once

#include <set>

#include "function.h"

class DomTree {
  std::map<const BasicBlock *, std::set<const BasicBlock *>> m_dominators;

public:
  DomTree(
      std::map<const BasicBlock *, std::set<const BasicBlock *>> &&dominators)
      : m_dominators(dominators) {}
  static DomTree build(const Function &f);
  void writeDot(std::ostream &os) const;
  void writeDominators(std::ostream &os) const;
};
