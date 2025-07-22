#pragma once

#include <vector>

#include "function.h"
#include "json.h"

class Program {
  using Json = nlohmann::json;

  std::vector<Function> m_funcs;

public:
  Program(std::vector<Function> &&bbs) : m_funcs(std::move(bbs)) {}
  const Function &operator[](size_t i) const {
    assert(i >= 0 && i < m_funcs.size());
    return m_funcs[i];
  }
  Function &operator[](size_t i) {
    assert(i >= 0 && i < m_funcs.size());
    return m_funcs[i];
  }
  auto begin() { return m_funcs.begin(); }
  auto end() { return m_funcs.end(); }
  auto begin() const { return m_funcs.begin(); }
  auto end() const { return m_funcs.end(); }
  Json toJson() const;
  friend std::ostream &operator<<(std::ostream &os, const Program &bb);
};
