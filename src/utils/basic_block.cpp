#include "basic_block.h"
#include "instruction_utils.h"

const std::string &BasicBlock::getName() const { return m_name; }

void BasicBlock::setName(std::string &&newName) { m_name = std::move(newName); }

bool BasicBlock::empty() const { return m_instructions.empty(); }

const Json &BasicBlock::operator[](size_t i) const {
  assert(i >= 0 && i < m_instructions.size());
  return m_instructions[i];
}

const Json &BasicBlock::last() const {
  assert(!m_instructions.empty());
  return m_instructions.back();
}

void BasicBlock::addInstruction(const Json &instr) {
  m_instructions.push_back(std::move(instr));
}

std::vector<BasicBlock> BasicBlock::fromJson(const Json &prog) {
  auto bbs = std::vector<BasicBlock>();
  std::string name = "main";

  for (const auto &func : prog["functions"]) {
    BasicBlock bb("entry");
    for (const auto &instr : func["instrs"]) {
      if (InstructionUtils::isLabel(instr)) {
        if (!bb.m_instructions.empty()) {
          bb.setName(std::move(name));
          bbs.push_back(bb);
        }
        bb = BasicBlock();
        name = instr["label"];
      } else {
        bb.addInstruction(instr);
        if (InstructionUtils::isTerminator(instr)) {
          bbs.push_back(bb);
          bb = BasicBlock();
        }
      }
    }
    if (!bb.m_instructions.empty()) {
      bb.setName(std::move(name));
      bbs.push_back(bb);
    }
  }

  return bbs;
}

std::ostream &operator<<(std::ostream &os, const BasicBlock &bb) {
  os << bb.m_name << ": ";
  for (const auto &instr : bb.m_instructions)
    os << "\n  " << instr;
  os << std::endl;
  return os;
};
