#include <memory>
#include <set>

#include "basic_block.h"
#include "function.h"
#include "gdce.h"
#include "instruction.h"
#include "json.h"

using Json = nlohmann::json;

void GDCE::run(Function &f) {
  std::map<std::string, std::set<Var>> use, def;

  // Compute uses and definitions in each basic block.
  for (const auto &bb : f) {
    std::set<Var> bb_use, bb_def;
    for (const auto &instr : bb) {
      for (const auto &arg : instr->getArgs())
        bb_use.insert(arg);
      if (instr->getDest())
        bb_def.insert(*instr->getDest());
    }
    use[bb.getName()] = std::move(bb_use);
    def[bb.getName()] = std::move(bb_def);
  }

  // Fixpoint backwards iteration
  std::map<std::string, std::set<Var>> in, out;
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto bb_iter = f.rbegin(); bb_iter != f.rend(); ++bb_iter) {
      const auto &bb = *bb_iter;
      auto &in_bb = in[bb.getName()];
      auto &out_bb = out[bb.getName()];

      // out[bb] = union of all in[S] for all successors S of bb
      std::set<Var> new_out;
      for (const auto &succ : bb.successors())
        new_out.insert(in[succ->getName()].begin(), in[succ->getName()].end());

      // in[bb] = (out[bb] - defs[bb]) union uses[bb]
      std::set<Var> new_in = use[bb.getName()];
      for (const auto &v : new_out)
        if (!def[bb.getName()].contains(v))
          new_in.insert(v);

      if (new_in != in_bb || new_out != out_bb) {
        in_bb = std::move(new_in);
        out_bb = std::move(new_out);
        changed = true;
      }
    }
  }

  for (auto &bb : f) {
    std::set<Var> live = out[bb.getName()];

    auto it = bb.rbegin();
    while (it != bb.rend()) {
      Instruction *instr = it->get();
      auto *dest = instr->getDest();

      // Check if instruction result is used
      bool is_live = !dest || live.contains(*dest);

      // If not live and instruction has no side effects, remove it
      if (dest && !is_live) {
        it = decltype(it){bb.eraseInstruction(std::next(it).base())};
        continue;
      }

      // If it defines a variable, remove it from live set
      if (dest)
        live.erase(*dest);

      // Add used variables to live set
      for (const auto &arg : instr->getArgs())
        live.insert(arg);

      ++it;
    }
  }
}
