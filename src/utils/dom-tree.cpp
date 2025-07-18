#include "dom-tree.h"
#include "json.h"
#include <set>

using Json = nlohmann::json;

static std::map<const BasicBlock *, std::set<const BasicBlock *>>
buildDominatorMap(const Function &f) {
  auto dominators =
      std::map<const BasicBlock *, std::set<const BasicBlock *>>();

  // Initialize
  const BasicBlock *entry = &f[0];
  for (const auto &bb : f) {
    dominators[&bb] = {};
    if (&bb == entry) {
      // Entry block dominates itself
      dominators[&bb].insert(&bb);
    } else {
      // All blocks dominate all other blocks
      for (const auto &allBB : f)
        dominators[&bb].insert(&allBB);
    }
  }

  bool changed = true;
  while (changed) {
    for (const auto &[bb, _] : dominators) {
      changed = false;

      auto preds = bb->predecessors();
      if (preds.empty())
        continue;

      std::set<const BasicBlock *> new_doms = dominators[*preds.begin()];

      // Take the intersection of the dominators common to all predecessors
      for (auto pred : preds) {
        std::set<const BasicBlock *> temp;
        std::set_intersection(new_doms.begin(), new_doms.end(),
                              dominators[pred].begin(), dominators[pred].end(),
                              std::inserter(temp, temp.begin()));
        new_doms = std::move(temp);
      }

      // Add the current block to its own dominator set
      new_doms.insert(bb);

      // Check if dominators changed
      if (new_doms != dominators[bb]) {
        dominators[bb] = std::move(new_doms);
        changed = true;
      }
    }
  }

  return dominators;
}

DomTree DomTree::build(const Function &f) {
  return DomTree(buildDominatorMap(f));
}

void DomTree::writeDot(std::ostream &os) const {
  os << "digraph G {\n";
  for (auto [k, v] : m_dominators)
    os << "  " << k->getName() << ";\n";
  for (auto [k, v] : m_dominators)
    for (auto target : v)
      os << "  " << target->getName() << " -> " << k->getName() << ";\n";
  os << "}\n";
}

void DomTree::writeDominators(std::ostream &os) const {
  for (const auto &[bb, doms] : m_dominators) {
    os << bb->getName() << ": ";
    for (const auto &dom : doms) {
      os << dom->getName() << " ";
    }
    os << "\n";
  }
}
