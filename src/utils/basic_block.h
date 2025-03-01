#pragma once

#include <json.h>
#include <vector>

using Json = nlohmann::json;

class BasicBlock {
  std::string m_name;
  std::vector<Json> m_instructions;

public:
  BasicBlock() = default;
  BasicBlock(std::string &&name) : m_name(std::move(name)) {}

  const std::string &getName() const;
  void setName(std::string &&newName);

  bool empty() const;
  size_t size() const;
  const Json &operator[](size_t i) const;
  const Json &last() const;

  auto begin() { return m_instructions.begin(); }
  auto end() { return m_instructions.end(); }
  auto begin() const { return m_instructions.begin(); }
  auto end() const { return m_instructions.end(); }

  void addInstruction(const Json &instr);
  void removeInstruction(size_t i);

  friend std::ostream &operator<<(std::ostream &os, const BasicBlock &bb);
  Json toJson() const;
};
