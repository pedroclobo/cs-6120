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
