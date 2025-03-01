#include <instruction_utils.h>

bool InstructionUtils::isTerminator(const Json &instr) {
  return instr.contains("op") && instr["op"] == "ret";
}

bool InstructionUtils::isJmp(const Json &instr) {
  return instr.contains("op") && instr["op"] == "jmp";
}

bool InstructionUtils::isBr(const Json &instr) {
  return instr.contains("op") && instr["op"] == "br";
}

bool InstructionUtils::isLabel(const Json &instr) {
  return instr.contains("label");
}

std::vector<std::unique_ptr<Json>>
InstructionUtils::getArgs(const Json &instr) {
  std::vector<std::unique_ptr<Json>> args;
  if (instr.contains("args") && instr["args"].is_array()) {
    for (const auto &arg : instr["args"]) {
      args.push_back(std::make_unique<Json>(arg));
    }
  }
  return args;
}

bool InstructionUtils::hasDest(const Json &instr) {
  return instr.contains("dest");
}

std::string InstructionUtils::getDest(const Json &instr) {
  if (hasDest(instr))
    return instr["dest"];
  return nullptr;
}
