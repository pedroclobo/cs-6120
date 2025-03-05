#pragma once

#include <array>
#include <optional>

#include "json.h"
#include "type.h"
#include "value.h"
#include "var.h"

enum class Opcode { Const, Br, Jmp, Add, Mul, Sub, Print };

struct Instruction {
  using Json = nlohmann::json;

  virtual ~Instruction() = default;
  virtual Opcode getOpcode() const = 0;
  virtual Json toJson() const = 0;

  virtual const std::vector<Var> getArgs() const { return {}; }
  virtual std::optional<Var> getDest() const { return std::nullopt; }

  static std::unique_ptr<Instruction> fromJson(const Json &json);
};

class ConstInstruction : public Instruction {
  Var m_dest;
  Type m_type;
  Value m_value;

public:
  ConstInstruction(Var dest, Type type, Value value)
      : m_dest(dest), m_type(type), m_value(value) {}

  Opcode getOpcode() const override { return Opcode::Const; }
  std::optional<Var> getDest() const override { return m_dest; }
  Type getType() const { return m_type; }
  Value getValue() const { return m_value; }
  Json toJson() const override;
};

class BranchInstruction : public Instruction {
  std::array<std::string, 2> m_label;

public:
  BranchInstruction(std::array<std::string, 2> label) : m_label(label) {}

  Opcode getOpcode() const override { return Opcode::Br; }
  const std::string &getLabel(size_t i) const { return m_label[i]; }
  Json toJson() const override;
};

class JumpInstruction : public Instruction {
  std::string m_label;

public:
  JumpInstruction(std::string label) : m_label(label) {}

  Opcode getOpcode() const override { return Opcode::Jmp; }
  const std::string &getLabel() const { return m_label; }
  Json toJson() const override;
};

class AddInstruction : public Instruction {
  Var m_dest;
  Type m_type;
  std::vector<Var> m_args;

public:
  AddInstruction(Var dest, Type type, std::vector<Var> args)
      : m_dest(dest), m_type(type), m_args(args) {}

  Opcode getOpcode() const override { return Opcode::Add; }
  std::optional<Var> getDest() const override { return m_dest; }
  Type getType() const { return m_type; }
  const std::vector<Var> getArgs() const override { return m_args; }
  Json toJson() const override;
};

class PrintInstruction : public Instruction {
  std::vector<Var> m_args;

public:
  PrintInstruction(std::vector<Var> args) : m_args(args) {}

  Opcode getOpcode() const override { return Opcode::Print; }
  const std::vector<Var> getArgs() const override { return m_args; }
  Json toJson() const override;
};
