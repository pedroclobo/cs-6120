#include "BasicBlock.h"
#include "InstructionUtils.h"
#include <memory>

const std::string &BasicBlock::getName() const { return name; }

void BasicBlock::setName(std::string &&newName) { name = std::move(newName); }

void BasicBlock::addInstruction(std::shared_ptr<Json> instr) {
  instructions.push_back(std::move(instr));
}

const Json &BasicBlock::operator[](int i) const {
  if (i < 0 || i >= instructions.size())
    throw std::out_of_range("Index out of range");
  return *instructions[i];
}

const std::shared_ptr<Json> BasicBlock::last() const {
  if (instructions.empty())
    return nullptr;
  return instructions.back();
}

std::vector<BasicBlock> BasicBlock::fromJson(const Json &prog) {
  auto bbs = std::vector<BasicBlock>();
  std::string name = "main";

  for (auto &func : prog["functions"]) {
    BasicBlock bb("entry");
    for (auto &instr : func["instrs"]) {
      if (InstructionUtils::isLabel(instr)) {
        if (!bb.instructions.empty()) {
          bb.setName(std::move(name));
          bbs.push_back(bb);
        }
        bb = BasicBlock();
        name = instr["label"];
      } else {
        bb.addInstruction(std::make_shared<Json>(instr));
        if (InstructionUtils::isTerminator(instr)) {
          bbs.push_back(bb);
          bb = BasicBlock();
        }
      }
    }
    if (!bb.instructions.empty()) {
      bb.setName(std::move(name));
      bbs.push_back(bb);
    }
  }

  return bbs;
}

std::ostream &operator<<(std::ostream &os, const BasicBlock &bb) {
  os << bb.name << ": ";
  for (auto instr : bb.instructions)
    os << "\n  " << *instr;
  os << std::endl;
  return os;
};
