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
  const Json &operator[](size_t i) const;
  const Json &last() const;

  void addInstruction(const Json &instr);

  static std::vector<BasicBlock> fromJson(const Json &prog);

  friend std::ostream &operator<<(std::ostream &os, const BasicBlock &bb);
};
