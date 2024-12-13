#pragma once

#include <iostream>
#include <vector>

#include <basic_block.h>
#include <instruction_utils.h>
#include <json.h>

using Json = nlohmann::json;

class CFG {
  std::map<std::string, std::vector<std::string>> m_edges;

public:
  CFG(std::map<std::string, std::vector<std::string>> &&edges)
      : m_edges(std::move(edges)) {}
  static CFG fromBasicBlocks(const std::vector<BasicBlock> &bbs);
  void writeDot(std::ostream &os) const;
  friend std::ostream &operator<<(std::ostream &os, const CFG &cfg);
};
