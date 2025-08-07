#pragma once

#include <deque>
#include <vector>

#include "instruction.h"
#include "json.h"

class BasicBlock {
  using Json = nlohmann::json;

  std::string m_name;
  std::vector<BasicBlock *> m_predecessors;
  std::vector<BasicBlock *> m_successors;
  std::deque<std::unique_ptr<Instruction>> m_instructions;

public:
  using iterator = std::deque<std::unique_ptr<Instruction>>::iterator;
  using const_iterator =
      std::deque<std::unique_ptr<Instruction>>::const_iterator;

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
  auto rbegin() { return m_instructions.rbegin(); }
  auto rend() { return m_instructions.rend(); }
  auto rbegin() const { return m_instructions.rbegin(); }
  auto rend() const { return m_instructions.rend(); }

  void appendInstruction(std::unique_ptr<Instruction> instr);
  void prependInstruction(std::unique_ptr<Instruction> instr);
  void replaceInstruction(std::unique_ptr<Instruction> instr, size_t i);
  void removeInstruction(size_t i);
  iterator eraseInstruction(iterator pos);

  void addPrecedessor(BasicBlock &bb);
  void addSucessor(BasicBlock &bb);

  const std::vector<BasicBlock *> &successors() const { return m_successors; }
  std::vector<BasicBlock *> &successors() { return m_successors; }

  const std::vector<BasicBlock *> &predecessors() const {
    return m_predecessors;
  }
  std::vector<BasicBlock *> &predecessors() { return m_predecessors; }
  std::vector<PhiInstruction *> phis() {
    std::vector<PhiInstruction *> phis;
    for (auto &instr : m_instructions)
      if (instr->getOpcode() == Opcode::Phi)
        phis.push_back(dynamic_cast<PhiInstruction *>(instr.get()));
    return phis;
  }

  friend std::ostream &operator<<(std::ostream &os, const BasicBlock &bb);
  Json toJson(bool emitLabel = true) const;
};
