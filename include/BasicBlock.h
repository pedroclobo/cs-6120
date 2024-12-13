#pragma once

#include <Json.h>
#include <vector>

using Json = nlohmann::json;

class BasicBlock {
  std::string name;
  std::vector<Json *> instructions;

public:
  BasicBlock() : name("unnamed") {}
  BasicBlock(std::string name) : name(name) {}

  std::string getName() const;
  void setName(std::string &newName);

  void addInstruction(Json *instr);

  Json &operator[](int i) const;
  Json *last() const;

  static std::vector<BasicBlock> fromJson(Json &prog);

  friend std::ostream &operator<<(std::ostream &os, const BasicBlock &bb);
};
