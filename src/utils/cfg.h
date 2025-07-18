#pragma once

#include <vector>

#include "function.h"

class CFG {
  std::map<const BasicBlock *, std::vector<const BasicBlock *>> m_edges;

public:
  CFG(std::map<const BasicBlock *, std::vector<const BasicBlock *>> &&edges)
      : m_edges(std::move(edges)) {}
  static CFG build(const Function &f);
  void writeDot(std::ostream &os) const;
  friend std::ostream &operator<<(std::ostream &os, const CFG &cfg);
};
