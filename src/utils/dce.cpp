#include <unordered_set>

#include "basic_block.h"
#include "dce.h"
#include "json.h"

using Json = nlohmann::json;

void LDCE::run(BasicBlock &bb) {
  bool changed = true;
  while (changed) {
    changed = false;
    std::unordered_set<std::string> used;
    std::map<std::string, bool> seen_def;

    for (size_t i = 0; i < bb.size(); ++i) {
      const auto &instr = bb[i];
      for (const auto &arg : instr.getArgs())
        used.insert(arg.getName());
    }

    for (int i = static_cast<int>(bb.size()) - 1; i >= 0; --i) {
      const auto &instr = bb[static_cast<size_t>(i)];
      bool removed = false;
      if (instr.getDest()) {
        const auto dest = instr.getDest()->getName();
        if (seen_def[dest]) {
          // We've already seen a declaration for this variable that succeeds
          // the current declaration. This is a dead declaration. Remove it.
          seen_def[dest] = false;
          bb.removeInstruction(static_cast<size_t>(i));
          removed = true;
          changed = true;
        } else {
          // This is the first time we've seen a declaration for this variable.
          seen_def[dest] = true;
        }
      }

      // Don't remove the instruction twice.
      if (removed)
        continue;

      // If the value produced by the instruction is not used, remove it.
      if (instr.getDest()) {
        const auto dest = instr.getDest()->getName();
        if (used.find(dest) == used.end()) {
          bb.removeInstruction(static_cast<size_t>(i));
          changed = true;
        }
      }
    }
  }
}
