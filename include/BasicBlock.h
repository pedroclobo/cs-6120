#pragma once

#include <Json.h>
#include <vector>

using Json = nlohmann::json;

class BasicBlock {
  std::string name;
  std::vector<std::shared_ptr<Json>> instructions;

public:
  BasicBlock() = default;
  BasicBlock(std::string &&name) : name(std::move(name)) {}

  const std::string &getName() const;
  void setName(std::string &&newName);

  void addInstruction(std::shared_ptr<Json> instr);

  const Json &operator[](int i) const;
  const std::shared_ptr<Json> last() const;

  static std::vector<BasicBlock> fromJson(const Json &prog);

  friend std::ostream &operator<<(std::ostream &os, const BasicBlock &bb);
};
