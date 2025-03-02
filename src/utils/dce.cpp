#include <unordered_set>

#include "basic_block.h"
#include "dce.h"
#include "instruction_utils.h"
#include "json.h"

using Json = nlohmann::json;

void LDCE::run(BasicBlock &bb) {
  bool changed = true;
  while (changed) {
    changed = false;
    std::unordered_set<std::string> used;

    for (size_t i = 0; i < bb.size(); ++i) {
      const Json &instr = bb[i];
      for (auto &arg : InstructionUtils::getArgs(instr))
        used.insert(*arg);
    }

    for (int i = static_cast<int>(bb.size()) - 1; i >= 0; --i) {
      const Json &instr = bb[static_cast<size_t>(i)];
      if (InstructionUtils::hasDest(instr)) {
        const auto &dest = InstructionUtils::getDest(instr);
        if (used.find(dest) == used.end()) {
          bb.removeInstruction(static_cast<size_t>(i));
          changed = true;
        }
      }
    }
  }
}
