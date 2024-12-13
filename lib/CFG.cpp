#include <fstream>

#include <CFG.h>

using Json = nlohmann::json;

CFG CFG::fromBasicBlocks(std::vector<BasicBlock> &bbs) {
  auto edges = std::map<std::string, std::vector<std::string>>();

  for (unsigned i = 0, e = bbs.size(); i < e; ++i) {
    auto &bb = bbs[i];
    edges[bb.getName()] = std::vector<std::string>();
    const auto &last = bb.last();

    if (InstructionUtils::isJmp(*last)) {
      edges[bb.getName()].push_back(last->at("labels")[0]);
    } else if (InstructionUtils::isBr(*last)) {
      edges[bb.getName()].push_back(last->at("labels")[0]);
      edges[bb.getName()].push_back(last->at("labels")[1]);
    } else {
      if (i + 1 < e)
        edges[bb[0]["label"]].push_back(bbs[i + 1][0]["label"]);
    }
  }
  return CFG(std::move(edges));
}

void CFG::writeDot(std::string &&filename) {
  std::ofstream dotfile(std::move(filename));
  dotfile << "digraph G {\n";
  for (auto [k, v] : edges)
    dotfile << "  " << k << ";\n";
  for (auto [k, v] : edges)
    for (auto target : v)
      dotfile << "  " << k << " -> " << target << ";\n";
  dotfile << "}\n";
  dotfile.close();
}

std::ostream &operator<<(std::ostream &os, const CFG &cfg) {
  for (auto [k, v] : cfg.edges) {
    os << k << ": ";
    for (auto target : v)
      os << target << " ";
    os << std::endl;
  }
  return os;
}