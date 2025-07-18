#pragma once

#include <set>

#include "function.h"

class DomTree {
  std::map<const BasicBlock *, std::set<const BasicBlock *>> m_doms;
  std::map<const BasicBlock *, const BasicBlock *> m_idoms;
  std::map<const BasicBlock *, std::set<const BasicBlock *>> m_dfront;

public:
  DomTree(std::map<const BasicBlock *, std::set<const BasicBlock *>> &&doms,
          std::map<const BasicBlock *, const BasicBlock *> &&idoms,
          std::map<const BasicBlock *, std::set<const BasicBlock *>> &&dfront)
      : m_doms(doms), m_idoms(idoms), m_dfront(dfront) {}
  static DomTree build(const Function &f);
  void writeDot(std::ostream &os) const;
  void writeDominators(std::ostream &os) const;
  void writeImmediateDominators(std::ostream &os) const;
  void writeDominanceFrontier(std::ostream &os) const;
};
