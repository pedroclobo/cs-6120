#include "cfg.h"
#include "json.h"

using Json = nlohmann::json;

CFG CFG::build(const Function &f) {
  auto edges = std::map<const BasicBlock *, std::vector<const BasicBlock *>>();

  for (size_t i = 0, e = f.size(); i < e; ++i) {
    auto &bb = f[i];
    edges[&bb] = std::vector<const BasicBlock *>();
    const auto &last = bb.last();

    if (const auto *JmpInst = dynamic_cast<const JumpInstruction *>(&last)) {
      edges[&bb].push_back(&f.bb(JmpInst->getLabel()));
    } else if (const auto *BrInst =
                   dynamic_cast<const BranchInstruction *>(&last)) {
      edges[&bb].push_back(&f.bb(BrInst->getLabel(0)));
      edges[&bb].push_back(&f.bb(BrInst->getLabel(1)));
    } else {
      if (i + 1 < e)
        edges[&bb].push_back(&f[i + 1]);
    }
  }
  return CFG(std::move(edges));
}

void CFG::writeDot(std::ostream &os) const {
  os << "digraph G {\n";
  for (const auto &[in, _] : m_edges)
    os << "  \"" << in->getName() << "\";\n";
  for (const auto &[in, outs] : m_edges)
    for (const auto &out : outs)
      os << "  \"" << in->getName() << "\" -> \"" << out->getName() << "\";\n";
  os << "}\n";
}

std::ostream &operator<<(std::ostream &os, const CFG &cfg) {
  for (const auto &[in, outs] : cfg.m_edges) {
    os << in->getName() << ": ";
    for (const auto &out : outs)
      os << out->getName() << " ";
    os << std::endl;
  }
  return os;
}
