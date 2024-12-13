#pragma once

#include <json.h>

using Json = nlohmann::json;

class InstructionUtils {
public:
  static bool isTerminator(const Json &instr);
  static bool isJmp(const Json &instr);
  static bool isBr(const Json &instr);
  static bool isLabel(const Json &instr);
};
