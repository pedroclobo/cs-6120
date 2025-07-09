#include "instruction.h"
#include "json.h"

using Json = nlohmann::json;

std::unique_ptr<Instruction> Instruction::fromJson(const Json &instr) {
  auto opcode = instr.at("op").get<std::string_view>();
  if (opcode == "const") {
    return std::make_unique<ConstInstruction>(
        Var::fromJson(instr.at("dest")), Type::fromJson(instr.at("type")),
        Value::fromJson(instr.at("value")));
  } else if (opcode == "id") {
    return std::make_unique<IdInstruction>(Var::fromJson(instr.at("dest")),
                                           Type::fromJson(instr.at("type")),
                                           Var::fromJson(instr.at("args")[0]));
  } else if (opcode == "br") {
    return std::make_unique<BranchInstruction>(std::array<std::string, 2>{
        instr.at("labels").at(0).get<std::string>(),
        instr.at("labels").at(1).get<std::string>()});
  } else if (opcode == "jmp") {
    return std::make_unique<JumpInstruction>(
        instr.at("labels").at(0).get<std::string>());
  } else if (opcode == "add") {
    return std::make_unique<AddInstruction>(
        Var::fromJson(instr.at("dest")), Type::fromJson(instr.at("type")),
        std::vector<Var>(instr.at("args").begin(), instr.at("args").end()));
  } else if (opcode == "mul") {
    return std::make_unique<MulInstruction>(
        Var::fromJson(instr.at("dest")), Type::fromJson(instr.at("type")),
        std::vector<Var>(instr.at("args").begin(), instr.at("args").end()));
  } else if (opcode == "print") {
    return std::make_unique<PrintInstruction>(
        std::vector<Var>(instr.at("args").begin(), instr.at("args").end()));
  }
  assert(false && "Unknown opcode");
}

Json ConstInstruction::toJson() const {
  return {{"dest", m_dest.toJson()},
          {"op", "const"},
          {"type", m_type.toJson()},
          {"value", m_value.toJson()}};
}

Json IdInstruction::toJson() const {
  auto args = Json::array();
  args.push_back(m_var.toJson());

  return {{"dest", m_dest.toJson()},
          {"op", "id"},
          {"type", m_type.toJson()},
          {"args", args}};
}

Json BranchInstruction::toJson() const {
  return {{"op", "br"}, {"label", m_label}};
}

Json JumpInstruction::toJson() const {
  return {{"op", "jmp"}, {"label", m_label}};
}

Json AddInstruction::toJson() const {
  auto args = Json::array();
  for (const auto &arg : m_args)
    args.push_back(arg.toJson());

  return {{"args", args},
          {"dest", m_dest.toJson()},
          {"op", "add"},
          {"type", m_type.toJson()}};
}

Json MulInstruction::toJson() const {
  auto args = Json::array();
  for (const auto &arg : m_args)
    args.push_back(arg.toJson());

  return {{"args", args},
          {"dest", m_dest.toJson()},
          {"op", "mul"},
          {"type", m_type.toJson()}};
}

Json PrintInstruction::toJson() const {
  auto args = Json::array();
  for (const auto &arg : m_args)
    args.push_back(arg.toJson());

  return {{"op", "print"}, {"args", args}};
}
