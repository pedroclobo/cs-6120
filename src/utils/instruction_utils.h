#pragma once

#include <json.h>

class InstructionUtils {
  using Json = nlohmann::json;

public:
  static bool isTerminator(const Json &instr);
  static bool isJmp(const Json &instr);
  static bool isBr(const Json &instr);
  static bool isLabel(const Json &instr);
  static std::vector<std::unique_ptr<Json>> getArgs(const Json &instr);
  static bool hasDest(const Json &instr);
  static std::string getDest(const Json &instr);
};
