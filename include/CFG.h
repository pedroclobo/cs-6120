#pragma once

#include <iostream>
#include <vector>

#include <BasicBlock.h>
#include <InstructionUtils.h>
#include <Json.h>

using Json = nlohmann::json;

class CFG {
  std::map<std::string, std::vector<std::string>> edges;

public:
  CFG(std::map<std::string, std::vector<std::string>> &&edges)
      : edges(std::move(edges)) {}
  static CFG fromBasicBlocks(std::vector<BasicBlock> &bbs);
  void writeDot(std::string &&filename);
  friend std::ostream &operator<<(std::ostream &os, const CFG &cfg);
};
