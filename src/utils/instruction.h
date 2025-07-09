#pragma once

#include <array>
#include <optional>

#include "json.h"
#include "type.h"
#include "value.h"
#include "var.h"

enum class Opcode { Const, Br, Jmp, Add, Mul, Sub, Print };

class Instruction {
protected:
  using Json = nlohmann::json;
  std::vector<Var> m_args;

  Instruction(std::vector<Var> args) : m_args(std::move(args)) {}

public:
  virtual ~Instruction() = default;
  virtual Opcode getOpcode() const = 0;
  virtual Json toJson() const = 0;

  virtual Type getType() const { return Type::getVoidType(); }
  // TODO: we can maybe merge `getValue` and `getArgs`
  virtual std::optional<Value> getValue() const { return std::nullopt; }
  virtual const std::vector<Var> getArgs() const { return m_args; }
  virtual size_t getNumArgs() const { return m_args.size(); }
  virtual const Var &getArg(size_t i) const {
    assert(i < m_args.size());
    return m_args[i];
  }
  virtual void setArg(Var &arg, size_t i) {
    assert(i < m_args.size());
    m_args[i] = arg;
  }
  virtual std::optional<Var> getDest() const { return std::nullopt; }

  static std::unique_ptr<Instruction> fromJson(const Json &json);
};

class ConstInstruction : public Instruction {
  Var m_dest;
  Type m_type;
  Value m_value;

public:
  ConstInstruction(Var dest, Type type, Value value)
      : Instruction({}), m_dest(dest), m_type(type), m_value(value) {}

  Opcode getOpcode() const override { return Opcode::Const; }
  std::optional<Var> getDest() const override { return m_dest; }
  Type getType() const override { return m_type; }
  std::optional<Value> getValue() const override { return m_value; }
  Json toJson() const override;
};

class IdInstruction : public Instruction {
  Var m_dest;
  Type m_type;
  Var m_var;

public:
  IdInstruction(Var dest, Type type, Var var)
      : Instruction({}), m_dest(dest), m_type(type), m_var(var) {}

  Opcode getOpcode() const override { return Opcode::Const; }
  std::optional<Var> getDest() const override { return m_dest; }
  Type getType() const override { return m_type; }
  Var getVar() const { return m_var; }
  Json toJson() const override;
};

class BranchInstruction : public Instruction {
  std::array<std::string, 2> m_label;

public:
  BranchInstruction(std::array<std::string, 2> label)
      : Instruction({}), m_label(label) {}

  Opcode getOpcode() const override { return Opcode::Br; }
  const std::string &getLabel(size_t i) const { return m_label[i]; }
  Json toJson() const override;
};

class JumpInstruction : public Instruction {
  std::string m_label;

public:
  JumpInstruction(std::string label) : Instruction({}), m_label(label) {}

  Opcode getOpcode() const override { return Opcode::Jmp; }
  const std::string &getLabel() const { return m_label; }
  Json toJson() const override;
};

class AddInstruction : public Instruction {
  Var m_dest;
  Type m_type;

public:
  AddInstruction(Var dest, Type type, std::vector<Var> args)
      : Instruction(args), m_dest(dest), m_type(type) {}

  Opcode getOpcode() const override { return Opcode::Add; }
  std::optional<Var> getDest() const override { return m_dest; }
  Type getType() const override { return m_type; }
  Json toJson() const override;
};

class MulInstruction : public Instruction {
  Var m_dest;
  Type m_type;

public:
  MulInstruction(Var dest, Type type, std::vector<Var> args)
      : Instruction(args), m_dest(dest), m_type(type) {}

  Opcode getOpcode() const override { return Opcode::Mul; }
  std::optional<Var> getDest() const override { return m_dest; }
  Type getType() const override { return m_type; }
  Json toJson() const override;
};

struct PrintInstruction : public Instruction {
  PrintInstruction(std::vector<Var> args) : Instruction(args) {}

  Opcode getOpcode() const override { return Opcode::Print; }
  Json toJson() const override;
};
