#pragma once

#include <vector>

#include "basic_block.h"

class Function {
  using Json = nlohmann::json;

  std::string m_name;
  std::vector<BasicBlock> m_bbs;

public:
  Function(std::string &&name, std::vector<BasicBlock> &&bbs)
      : m_name(name), m_bbs(std::move(bbs)){};

  const std::string &getName() const { return m_name; }
  void setName(std::string &&newName) { m_name = std::move(newName); }

  bool empty() const { return m_bbs.empty(); }
  size_t size() const { return m_bbs.size(); }
  const BasicBlock &operator[](size_t i) const {
    assert(i >= 0 && i < m_bbs.size());
    return m_bbs[i];
  }

  const BasicBlock &last() const { return m_bbs[m_bbs.size() - 1]; }

  auto begin() { return m_bbs.begin(); }
  auto end() { return m_bbs.end(); }
  auto begin() const { return m_bbs.begin(); }
  auto end() const { return m_bbs.end(); }

  Json toJson() const;
  friend std::ostream &operator<<(std::ostream &os, const Function &f);
};
