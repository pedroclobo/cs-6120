#include "dom-tree.h"
#include "json.h"
#include <set>

using Json = nlohmann::json;

static std::map<const BasicBlock *, std::set<const BasicBlock *>>
buildDominatorMap(const Function &f) {
  auto doms = std::map<const BasicBlock *, std::set<const BasicBlock *>>();

  // All blocks have all other blocks as dominators
  for (const auto &bb : f) {
    doms[&bb] = {};
    for (const auto &allBB : f)
      doms[&bb].insert(&allBB);
  }

  bool changed = true;
  while (changed) {
    changed = false;
    for (const auto &[bb, _] : doms) {
      auto preds = bb->predecessors();
      std::set<const BasicBlock *> new_doms = {bb};
      if (!preds.empty()) {
        // Take the intersection of the dominators common to all predecessors
        new_doms = doms[*preds.begin()];
        for (auto pred : preds) {
          std::set<const BasicBlock *> temp;
          std::set_intersection(new_doms.begin(), new_doms.end(),
                                doms[pred].begin(), doms[pred].end(),
                                std::inserter(temp, temp.begin()));
          new_doms = std::move(temp);
        }
        new_doms.insert(bb);
      }

      // Check if dominators changed
      if (new_doms != doms[bb]) {
        doms[bb] = std::move(new_doms);
        changed = true;
      }
    }
  }

  return doms;
}

// The dominance tree is given by the immediate dominators only.
// Iterate over the dominators map and rule out every dominator that is not
// the immediate dominator.
static std::map<const BasicBlock *, const BasicBlock *>
buildImmediateDominatorMap(
    const Function &f,
    const std::map<const BasicBlock *, std::set<const BasicBlock *>>
        &dominators) {
  std::map<const BasicBlock *, const BasicBlock *> idoms;
  for (const auto &bb : f) {
    // The entry block has no immediate dominator
    if (&bb == &f[0]) {
      idoms[&bb] = nullptr;
      continue;
    }

    const auto &doms = dominators.at(&bb);
    std::set<const BasicBlock *> strict_doms(doms.begin(), doms.end());
    strict_doms.erase(&bb);

    // `d` immediately dominates `bb` iff `d` dominates `bb` but `d` does not
    // strictly dominate any `other` node that strictly dominates `bb`
    const BasicBlock *idom = nullptr;
    for (const BasicBlock *d : strict_doms) {
      bool is_idom = true;
      for (const BasicBlock *other : strict_doms) {
        if (other == d)
          continue;
        if (dominators.at(other).count(d)) {
          is_idom = false;
          break;
        }
      }
      if (is_idom) {
        idom = d;
        break;
      }
    }

    idoms[&bb] = idom;
  }

  return idoms;
}

DomTree DomTree::build(const Function &f) {
  auto doms = buildDominatorMap(f);
  auto idoms = buildImmediateDominatorMap(f, doms);
  return DomTree(std::move(doms), std::move(idoms));
}

void DomTree::writeDot(std::ostream &os) const {
  os << "digraph G {\n";
  for (const auto &[bb, _] : m_idoms)
    os << "  \"" << bb->getName() << "\";\n";
  for (const auto &[bb, dom] : m_idoms)
    if (dom)
      os << "  \"" << dom->getName() << "\" -> \"" << bb->getName() << "\";\n";
  os << "}\n";
}

void DomTree::writeDominators(std::ostream &os) const {
  for (const auto &[bb, doms] : m_doms) {
    os << '"' << bb->getName() << "\": ";
    for (const auto &dom : doms) {
      os << '"' << dom->getName() << "\" ";
    }
    os << "\n";
  }
}

void DomTree::writeImmediateDominators(std::ostream &os) const {
  for (const auto &[bb, dom] : m_idoms)
    if (dom)
      os << '"' << bb->getName() << "\" - \"" << dom->getName() << "\"\n";
}
