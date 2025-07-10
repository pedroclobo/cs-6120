#include "basic_block.h"
#include "instruction.h"
#include "json.h"

using Json = nlohmann::json;

const std::string &BasicBlock::getName() const { return m_name; }

void BasicBlock::setName(std::string &&newName) { m_name = std::move(newName); }

bool BasicBlock::empty() const { return m_instructions.empty(); }

size_t BasicBlock::size() const { return m_instructions.size(); }

const Instruction &BasicBlock::operator[](size_t i) const {
  assert(i >= 0 && i < m_instructions.size());
  return *m_instructions[i];
}

Instruction &BasicBlock::operator[](size_t i) {
  assert(i >= 0 && i < m_instructions.size());
  return *m_instructions[i];
}

const Instruction &BasicBlock::last() const {
  assert(!m_instructions.empty());
  return *m_instructions.back();
}

void BasicBlock::addInstruction(std::unique_ptr<Instruction> instr) {
  m_instructions.push_back(std::move(instr));
}

void BasicBlock::replaceInstruction(std::unique_ptr<Instruction> instr,
                                    size_t i) {
  assert(i < m_instructions.size());
  m_instructions[i] = std::move(instr);
}

void BasicBlock::removeInstruction(size_t i) {
  assert(i >= 0 && i < m_instructions.size());
  m_instructions.erase(m_instructions.begin() + static_cast<long>(i));
}

void BasicBlock::addPrecedessor(BasicBlock &bb) {
  m_predecessors.push_back(&bb);
}

void BasicBlock::addSucessor(BasicBlock &bb) { m_successors.push_back(&bb); }

Json BasicBlock::toJson() const {
  Json json;
  json["name"] = m_name;
  for (const auto &instr : m_instructions)
    json["instrs"].push_back(instr->toJson());
  return json;
}

std::ostream &operator<<(std::ostream &os, const BasicBlock &bb) {
  os << bb.toJson().dump(2);
  return os;
};
