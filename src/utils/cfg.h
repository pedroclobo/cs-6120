#pragma once

#include <vector>

#include "function.h"

class CFG {
  std::map<std::string, std::vector<std::string>> m_edges;

public:
  CFG(std::map<std::string, std::vector<std::string>> &&edges)
      : m_edges(std::move(edges)) {}
  static CFG build(const Function &f);
  void writeDot(std::ostream &os) const;
  friend std::ostream &operator<<(std::ostream &os, const CFG &cfg);
};
