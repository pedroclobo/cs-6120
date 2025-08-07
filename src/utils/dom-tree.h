#pragma once

#include <set>

#include "function.h"

class DomTree {
  std::map<BasicBlock *, std::set<BasicBlock *>> m_doms;
  std::map<BasicBlock *, BasicBlock *> m_idoms;
  std::map<BasicBlock *, std::set<BasicBlock *>> m_dfront;

public:
  DomTree(std::map<BasicBlock *, std::set<BasicBlock *>> &&doms,
          std::map<BasicBlock *, BasicBlock *> &&idoms,
          std::map<BasicBlock *, std::set<BasicBlock *>> &&dfront)
      : m_doms(doms), m_idoms(idoms), m_dfront(dfront) {}
  static DomTree build(Function &f);
  std::set<BasicBlock *> children(BasicBlock &bb) const;
  std::set<BasicBlock *> dfront(BasicBlock &bb) const;
  void writeDot(std::ostream &os) const;
  void writeDominators(std::ostream &os) const;
  void writeImmediateDominators(std::ostream &os) const;
  void writeDominanceFrontier(std::ostream &os) const;
};
