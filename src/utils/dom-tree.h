#pragma once

#include <set>

#include "function.h"

class DomTree {
  std::map<const BasicBlock *, std::set<const BasicBlock *>> m_doms;
  std::map<const BasicBlock *, const BasicBlock *> m_idoms;

public:
  DomTree(std::map<const BasicBlock *, std::set<const BasicBlock *>> &&doms,
          std::map<const BasicBlock *, const BasicBlock *> &&idoms)
      : m_doms(doms), m_idoms(idoms) {}
  static DomTree build(const Function &f);
  void writeDot(std::ostream &os) const;
  void writeDominators(std::ostream &os) const;
  void writeImmediateDominators(std::ostream &os) const;
};
