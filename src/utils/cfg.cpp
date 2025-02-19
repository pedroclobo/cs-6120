#include <cfg.h>

using Json = nlohmann::json;

CFG CFG::fromBasicBlocks(const std::vector<BasicBlock> &bbs) {
  auto edges = std::map<std::string, std::vector<std::string>>();

  for (size_t i = 0, e = bbs.size(); i < e; ++i) {
    auto &bb = bbs[i];
    edges[bb.getName()] = std::vector<std::string>();
    const auto &last = bb.last();

    if (InstructionUtils::isJmp(last)) {
      edges[bb.getName()].push_back(last["labels"][0]);
    } else if (InstructionUtils::isBr(last)) {
      edges[bb.getName()].push_back(last["labels"][0]);
      edges[bb.getName()].push_back(last["labels"][1]);
    } else {
      if (i + 1 < e)
        edges[bb[0]["label"]].push_back(bbs[i + 1][0]["label"]);
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
