#include "instruction.h"
#include "json.h"

using Json = nlohmann::json;

std::unique_ptr<Instruction> Instruction::fromJson(const Json &instr) {
  auto parse_opcode([](std::string_view op) {
    if (op == "const")
      return Opcode::Const;
    if (op == "id")
      return Opcode::Id;
    if (op == "br")
      return Opcode::Br;
    if (op == "jmp")
      return Opcode::Jmp;
    if (op == "add")
      return Opcode::Add;
    if (op == "sub")
      return Opcode::Sub;
    if (op == "mul")
      return Opcode::Mul;
    if (op == "print")
      return Opcode::Print;
    if (op == "eq")
      return Opcode::Eq;
    if (op == "gt")
      return Opcode::Gt;
    if (op == "lt")
      return Opcode::Lt;
    if (op == "phi")
      return Opcode::Phi;

    assert(false && "Invalid opcode when parsing");
  });

  auto opcode = parse_opcode(instr.at("op").get<std::string_view>());
  switch (opcode) {
  case Opcode::Const:
    return std::make_unique<ConstInstruction>(
        Var::fromJson(instr.at("dest")), Type::fromJson(instr.at("type")),
        Value::fromJson(instr.at("value")));
  case Opcode::Id:
    return std::make_unique<IdInstruction>(Var::fromJson(instr.at("dest")),
                                           Type::fromJson(instr.at("type")),
                                           Var::fromJson(instr.at("args")[0]));
  case Opcode::Br:
    return std::make_unique<BranchInstruction>(
        Var::fromJson(instr.at("args").back()),
        std::array<std::string, 2>{
            instr.at("labels").at(0).get<std::string>(),
            instr.at("labels").at(1).get<std::string>()});
  case Opcode::Jmp:
    return std::make_unique<JumpInstruction>(
        instr.at("labels").at(0).get<std::string>());
  case Opcode::Add:
    return std::make_unique<AddInstruction>(
        Var::fromJson(instr.at("dest")), Type::fromJson(instr.at("type")),
        std::vector<Var>(instr.at("args").begin(), instr.at("args").end()));
  case Opcode::Sub:
    return std::make_unique<SubInstruction>(
        Var::fromJson(instr.at("dest")), Type::fromJson(instr.at("type")),
        std::vector<Var>(instr.at("args").begin(), instr.at("args").end()));
  case Opcode::Mul:
    return std::make_unique<MulInstruction>(
        Var::fromJson(instr.at("dest")), Type::fromJson(instr.at("type")),
        std::vector<Var>(instr.at("args").begin(), instr.at("args").end()));
  case Opcode::Print:
    return std::make_unique<PrintInstruction>(
        std::vector<Var>(instr.at("args").begin(), instr.at("args").end()));
  case Opcode::Eq:
  case Opcode::Gt:
  case Opcode::Lt:
    return std::make_unique<CmpInstruction>(
        Var::fromJson(instr.at("dest")), Type::fromJson(instr.at("type")),
        std::vector<Var>(instr.at("args").begin(), instr.at("args").end()),
        opcode);
  case Opcode::Phi:
    return std::make_unique<PhiInstruction>(
        Var::fromJson(instr.at("dest")), Type::fromJson(instr.at("type")),
        std::vector<Var>(instr.at("args").begin(), instr.at("args").end()),
        std::vector<std::string>(instr.at("labels").begin(),
                                 instr.at("labels").end()));
  default:
    assert(false && "Unknown opcode");
  }
}

Json ConstInstruction::toJson() const {
  return {{"dest", m_dest.value().toJson()},
          {"op", "const"},
          {"type", m_type.toJson()},
          {"value", m_value.toJson()}};
}

Json IdInstruction::toJson() const {
  auto args = Json::array();
  args.push_back(m_var.toJson());

  return {{"dest", m_dest.value().toJson()},
          {"op", "id"},
          {"type", m_type.toJson()},
          {"args", args}};
}

Json BranchInstruction::toJson() const {
  auto args = Json::array();
  args.push_back(m_args[0].toJson());
  return {{"op", "br"}, {"labels", m_labels}, {"args", args}};
}

Json JumpInstruction::toJson() const {
  return {{"op", "jmp"}, {"labels", {m_label}}};
}

Json AddInstruction::toJson() const {
  auto args = Json::array();
  for (const auto &arg : m_args)
    args.push_back(arg.toJson());

  return {{"args", args},
          {"dest", m_dest.value().toJson()},
          {"op", "add"},
          {"type", m_type.toJson()}};
}

Json SubInstruction::toJson() const {
  auto args = Json::array();
  for (const auto &arg : m_args)
    args.push_back(arg.toJson());

  return {{"args", args},
          {"dest", m_dest.value().toJson()},
          {"op", "sub"},
          {"type", m_type.toJson()}};
}

Json MulInstruction::toJson() const {
  auto args = Json::array();
  for (const auto &arg : m_args)
    args.push_back(arg.toJson());

  return {{"args", args},
          {"dest", m_dest.value().toJson()},
          {"op", "mul"},
          {"type", m_type.toJson()}};
}

Json CmpInstruction::toJson() const {
  auto args = Json::array();
  for (const auto &arg : m_args)
    args.push_back(arg.toJson());

  return {{"args", args},
          {"dest", m_dest.value().toJson()},
          {"op", m_opcode},
          {"type", m_type.toJson()}};
}

Json PrintInstruction::toJson() const {
  auto args = Json::array();
  for (const auto &arg : m_args)
    args.push_back(arg.toJson());

  return {{"op", "print"}, {"args", args}};
}

Json PhiInstruction::toJson() const {
  auto args = Json::array();
  for (const auto &arg : m_args)
    args.push_back(arg.toJson());

  auto labels = Json::array();
  for (const auto &label : m_labels)
    labels.push_back(label);

  return {{"op", "phi"},
          {"dest", m_dest.value().toJson()},
          {"type", m_type.toJson()},
          {"args", args},
          {"labels", labels}};
}
