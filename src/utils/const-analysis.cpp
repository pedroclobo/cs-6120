#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <stack>

#include "basic_block.h"
#include "const-analysis.h"
#include "function.h"
#include "instruction.h"
#include "json.h"

using Json = nlohmann::json;

// initial value: {}
// transfer: { constants in the current basic block }
// merge: set union
void ConstAnalysis::run(const Function &f) {
  std::stack<const BasicBlock *> worklist;
  for (const auto &bb : f)
    worklist.push(&bb);

  std::map<std::string, std::set<Var>> in;
  std::map<std::string, std::set<Var>> out;
  for (size_t i = 0, e = f.size(); i < e; ++i) {
    auto bb_name = f[i].getName();
    if (i == 0)
      in[bb_name] = {};
    out[bb_name] = {};
  }

  while (!worklist.empty()) {
    const auto &bb = worklist.top();
    worklist.pop();
    std::string bb_name = bb->getName();

    for (const auto &pred : bb->predecessors())
      in[bb_name].insert(out[pred->getName()].begin(),
                         out[pred->getName()].end());

    for (const auto &in : in[bb_name]) {
      std::cout << bb->getName() << " " << in.getName() << "\n";
    }

    for (const auto &instr : *bb) {
      if (auto *ci = dynamic_cast<const ConstInstruction *>(instr.get())) {
        in[bb_name].insert(*ci->getDest());
      } else {
        bool all = true;
        for (const auto &var : instr->getArgs())
          all &= in[bb_name].contains(var);
        if (all && instr->getDest())
          in[bb_name].insert(*instr->getDest());
      }
    }

    auto old_size = out[bb_name].size();
    out[bb_name].insert(in[bb_name].begin(), in[bb_name].end());
    if (old_size != out[bb_name].size())
      for (const auto &succ : bb->successors())
        worklist.push(succ);
  }

  for (const auto &bb : f) {
    std::cout << bb.getName() << ": ";
    for (const auto &var : out[bb.getName()]) {
      std::cout << var.getName() << " ";
    }
    std::cout << "\n";
  }
}
