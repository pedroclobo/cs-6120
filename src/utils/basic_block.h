#pragma once

#include <deque>
#include <vector>

#include "instruction.h"
#include "json.h"

class BasicBlock {
  using Json = nlohmann::json;

  std::string m_name;
  std::deque<std::unique_ptr<Instruction>> m_instructions;
  std::vector<BasicBlock *> m_predecessors;
  std::vector<BasicBlock *> m_successors;

public:
  BasicBlock() = default;
  BasicBlock(std::string &&name)
      : m_name(std::move(name)), m_predecessors({}), m_successors({}) {}
  BasicBlock(const BasicBlock &) = delete;
  BasicBlock &operator=(const BasicBlock &) = delete;
  BasicBlock(BasicBlock &&) = default;
  BasicBlock &operator=(BasicBlock &&) = default;

  const std::string &getName() const;
  void setName(std::string &&newName);

  bool empty() const;
  size_t size() const;
  const Instruction &operator[](size_t i) const;
  Instruction &operator[](size_t i);
  const Instruction &last() const;

  auto begin() { return m_instructions.begin(); }
  auto end() { return m_instructions.end(); }
  auto begin() const { return m_instructions.begin(); }
  auto end() const { return m_instructions.end(); }

  void appendInstruction(std::unique_ptr<Instruction> instr);
  void prependInstruction(std::unique_ptr<Instruction> instr);
  void replaceInstruction(std::unique_ptr<Instruction> instr, size_t i);
  void removeInstruction(size_t i);

  void addPrecedessor(BasicBlock &bb);
  void addSucessor(BasicBlock &bb);

  const std::vector<BasicBlock *> &successors() const { return m_successors; }
  std::vector<BasicBlock *> &successors() { return m_successors; }

  const std::vector<BasicBlock *> &predecessors() const {
    return m_predecessors;
  }
  std::vector<BasicBlock *> &predecessors() { return m_predecessors; }

  friend std::ostream &operator<<(std::ostream &os, const BasicBlock &bb);
  Json toJson() const;
};
