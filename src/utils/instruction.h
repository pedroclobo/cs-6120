#pragma once

#include <array>
#include <optional>

#include "json.h"
#include "type.h"
#include "value.h"
#include "var.h"

enum class Opcode { Const, Id, Br, Jmp, Add, Mul, Sub, Print, Eq, Lt, Gt, Phi };

class Instruction {
protected:
  using Json = nlohmann::json;
  std::vector<Var> m_args;
  std::optional<Var> m_dest;
  Type m_type;

  Instruction(std::vector<Var> args)
      : m_args(std::move(args)), m_dest(std::nullopt),
        m_type(Type::getVoidType()) {}
  Instruction(std::vector<Var> args, Var &&var)
      : m_args(std::move(args)), m_dest(var), m_type(Type::getVoidType()) {}
  Instruction(std::vector<Var> args, Var &&var, Type type)
      : m_args(std::move(args)), m_dest(var), m_type(type) {}

public:
  virtual ~Instruction() = default;
  virtual Opcode getOpcode() const = 0;
  virtual Json toJson() const = 0;

  virtual Type getType() const { return m_type; }
  // TODO: we can maybe merge `getValue` and `getArgs`
  virtual std::optional<Value> getValue() const { return std::nullopt; }
  virtual const std::vector<Var> getArgs() const { return m_args; }
  virtual size_t getNumArgs() const { return m_args.size(); }
  virtual const Var &getArg(size_t i) const {
    assert(i < m_args.size());
    return m_args[i];
  }
  virtual Var &getArg(size_t i) {
    assert(i < m_args.size());
    return m_args[i];
  }
  virtual void setArg(Var &&arg, size_t i) {
    assert(i < m_args.size());
    m_args[i] = arg;
  }
  virtual const Var *getDest() const {
    return m_dest.has_value() ? &m_dest.value() : nullptr;
  }
  virtual Var *getDest() {
    return m_dest.has_value() ? &m_dest.value() : nullptr;
  }
  virtual void setDest(Var &&dest) { m_dest = dest; }

  static std::unique_ptr<Instruction> fromJson(const Json &json);
};

class ConstInstruction : public Instruction {
  Value m_value;

public:
  ConstInstruction(Var dest, Type type, Value value)
      : Instruction({}, std::move(dest), type), m_value(value) {}

  Opcode getOpcode() const override { return Opcode::Const; }
  std::optional<Value> getValue() const override { return m_value; }
  Json toJson() const override;
};

class IdInstruction : public Instruction {
  Var m_var;

public:
  IdInstruction(Var dest, Type type, Var var)
      : Instruction({}, std::move(dest), type), m_var(var) {}

  Opcode getOpcode() const override { return Opcode::Id; }
  Var getVar() const { return m_var; }
  Json toJson() const override;
};

class BranchInstruction : public Instruction {
  std::array<std::string, 2> m_labels;

public:
  BranchInstruction(Var &&cond, std::array<std::string, 2> labels)
      : Instruction({cond}), m_labels(labels) {}

  Opcode getOpcode() const override { return Opcode::Br; }
  const std::string &getLabel(size_t i) const { return m_labels[i]; }
  Json toJson() const override;
};

class JumpInstruction : public Instruction {
  std::string m_label;

public:
  JumpInstruction(std::string label)
      : Instruction({}), m_label(std::move(label)) {}

  Opcode getOpcode() const override { return Opcode::Jmp; }
  const std::string &getLabel() const { return m_label; }
  Json toJson() const override;
};

struct AddInstruction : public Instruction {
  AddInstruction(Var dest, Type type, std::vector<Var> args)
      : Instruction(args, std::move(dest), type) {}

  Opcode getOpcode() const override { return Opcode::Add; }
  Json toJson() const override;
};

struct SubInstruction : public Instruction {
  SubInstruction(Var dest, Type type, std::vector<Var> args)
      : Instruction(args, std::move(dest), type) {}

  Opcode getOpcode() const override { return Opcode::Sub; }
  Json toJson() const override;
};

struct MulInstruction : public Instruction {
  MulInstruction(Var dest, Type type, std::vector<Var> args)
      : Instruction(args, std::move(dest), type) {}

  Opcode getOpcode() const override { return Opcode::Mul; }
  Json toJson() const override;
};

class CmpInstruction : public Instruction {
  Opcode m_opcode;

public:
  CmpInstruction(Var dest, Type type, std::vector<Var> args, Opcode opcode)
      : Instruction(args, std::move(dest), type), m_opcode(opcode) {}

  Opcode getOpcode() const override { return m_opcode; }
  Json toJson() const override;
};

struct PrintInstruction : public Instruction {
  PrintInstruction(std::vector<Var> args) : Instruction(args) {}

  Opcode getOpcode() const override { return Opcode::Print; }
  Json toJson() const override;
};

class PhiInstruction : public Instruction {
  std::vector<std::string> m_labels;

public:
  PhiInstruction(Var dest, Type type, std::vector<Var> args,
                 std::vector<std::string> labels)
      : Instruction(args, std::move(dest), type), m_labels(labels) {}

  Opcode getOpcode() const override { return Opcode::Phi; }
  Json toJson() const override;
};
