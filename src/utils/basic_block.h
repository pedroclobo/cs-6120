#pragma once

#include <vector>

#include "instruction.h"
#include "json.h"

class BasicBlock {
  using Json = nlohmann::json;

  std::string m_name;
  std::vector<std::unique_ptr<Instruction>> m_instructions;

public:
  BasicBlock() = default;
  BasicBlock(std::string &&name) : m_name(std::move(name)) {}
  BasicBlock(const BasicBlock &) = delete;
  BasicBlock &operator=(const BasicBlock &) = delete;
  BasicBlock(BasicBlock &&) = default;
  BasicBlock &operator=(BasicBlock &&) = default;

  const std::string &getName() const;
  void setName(std::string &&newName);

  bool empty() const;
  size_t size() const;
  const Instruction &operator[](size_t i) const;
  const Instruction &last() const;

  auto begin() { return m_instructions.begin(); }
  auto end() { return m_instructions.end(); }
  auto begin() const { return m_instructions.begin(); }
  auto end() const { return m_instructions.end(); }

  void addInstruction(std::unique_ptr<Instruction> instr);
  void removeInstruction(size_t i);

  friend std::ostream &operator<<(std::ostream &os, const BasicBlock &bb);
  Json toJson() const;
};
