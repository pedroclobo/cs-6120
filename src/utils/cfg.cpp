#include "cfg.h"
#include "instruction_utils.h"
#include "json.h"

using Json = nlohmann::json;

CFG CFG::build(const Function &f) {
  auto edges = std::map<std::string, std::vector<std::string>>();

  for (size_t i = 0, e = f.size(); i < e; ++i) {
    auto &bb = f[i];
    edges[bb.getName()] = std::vector<std::string>();
    const auto &last = bb.last();

    if (InstructionUtils::isJmp(last)) {
      edges[bb.getName()].push_back(last["labels"][0]);
    } else if (InstructionUtils::isBr(last)) {
      edges[bb.getName()].push_back(last["labels"][0]);
      edges[bb.getName()].push_back(last["labels"][1]);
    } else {
      if (i + 1 < e)
        edges[bb[0]["label"]].push_back(f[i + 1][0]["label"]);
    }
  }
  return CFG(std::move(edges));
}

void CFG::writeDot(std::ostream &os) const {
  os << "digraph G {\n";
  for (auto [k, v] : m_edges)
    os << "  " << k << ";\n";
  for (auto [k, v] : m_edges)
    for (auto target : v)
      os << "  " << k << " -> " << target << ";\n";
  os << "}\n";
}

std::ostream &operator<<(std::ostream &os, const CFG &cfg) {
  for (auto [k, v] : cfg.m_edges) {
    os << k << ": ";
    for (auto target : v)
      os << target << " ";
    os << std::endl;
  }
  return os;
}
