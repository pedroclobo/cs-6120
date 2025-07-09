#include <memory>
#include <optional>

#include "basic_block.h"
#include "instruction.h"
#include "json.h"
#include "lvn.h"

using Json = nlohmann::json;

namespace lvn {
class Env {
public:
  std::unordered_map<Var, size_t> m_env;

  void insert(const std::string &var, size_t index) { m_env[var] = index; }
  size_t operator[](const Var &var) const {
    assert(m_env.contains(var));
    return m_env.at(var);
  }
};

struct Val {
  virtual ~Val() = default;
  virtual bool equals(const Val &other) const = 0;
  bool operator==(const Val &other) const { return equals(other); }
  virtual std::unique_ptr<Val> clone() const = 0;
};

class ConstVal : public Val {
  const uint64_t m_value;

public:
  ConstVal(uint64_t value) : m_value(value) {};

  bool equals(const Val &other) const override {
    if (const auto *o = dynamic_cast<const ConstVal *>(&other))
      return m_value == o->m_value;
    return false;
  }

  std::unique_ptr<Val> clone() const override {
    return std::make_unique<ConstVal>(m_value);
  }
};

struct AddVal : public Val {
  const Var m_lhs;
  const Var m_rhs;

  AddVal(Var lhs, Var rhs) : m_lhs(lhs), m_rhs(rhs) {};

  bool equals(const Val &other) const override {
    if (const auto *o = dynamic_cast<const AddVal *>(&other))
      return m_lhs == o->m_lhs && m_rhs == o->m_rhs;
    return false;
  }

  std::unique_ptr<Val> clone() const override {
    return std::make_unique<AddVal>(m_lhs, m_rhs);
  }
};

struct MulVal : public Val {
  const Var m_lhs;
  const Var m_rhs;

  MulVal(Var lhs, Var rhs) : m_lhs(lhs), m_rhs(rhs) {};

  bool equals(const Val &other) const override {
    if (const auto *o = dynamic_cast<const MulVal *>(&other))
      return m_lhs == o->m_lhs && m_rhs == o->m_rhs;
    return false;
  }

  std::unique_ptr<Val> clone() const override {
    return std::make_unique<MulVal>(m_lhs, m_rhs);
  }
};

class LVNTable {
public:
  std::vector<std::pair<std::unique_ptr<Val>, std::string>> table;

  size_t add(std::unique_ptr<Val> val, std::string var) {
    table.emplace_back(std::move(val), var);
    return table.size() - 1;
  }

  std::optional<std::pair<size_t, std::string>> lookup(const Val &val) const {
    for (size_t i = 0; i < table.size(); ++i) {
      if (*table[i].first == val) {
        return std::make_pair(i, table[i].second);
      }
    }
    return std::nullopt;
  }

  std::unique_ptr<Val> value(const size_t index) const {
    assert(index < table.size());
    return table[index].first->clone();
  }

  std::string at(const size_t index) const {
    assert(index < table.size());
    return table[index].second;
  }
};

} // namespace lvn

std::unique_ptr<lvn::Val> getLVNValue(const Instruction &instr,
                                      const lvn::LVNTable &table,
                                      const lvn::Env &env) {
  if (auto ci = dynamic_cast<const ConstInstruction *>(&instr)) {
    uint64_t value = ci->getValue()->getValue();
    return std::make_unique<lvn::ConstVal>(value);
  } else if (auto ii = dynamic_cast<const IdInstruction *>(&instr)) {
    return std::unique_ptr(table.value(env[ii->getVar()]));
  } else if (auto ai = dynamic_cast<const AddInstruction *>(&instr)) {
    const auto &args = ai->getArgs();
    return std::make_unique<lvn::AddVal>(args[0], args[1]);
  } else if (auto mi = dynamic_cast<const MulInstruction *>(&instr)) {
    const auto &args = mi->getArgs();
    return std::make_unique<lvn::MulVal>(args[0], args[1]);
  }
  std::unreachable();
}

void LVN::run(BasicBlock &bb) {
  lvn::LVNTable table;
  lvn::Env env;
  for (size_t i = 0, e = bb.size(); i < e; ++i) {
    Instruction &instr = bb[i];
    // Replace instruction arguments by canonical argument
    for (size_t idx = 0, num_args = instr.getNumArgs(); idx < num_args; ++idx) {
      auto new_arg = Var(table.at(env[instr.getArg(idx)]));
      instr.setArg(new_arg, idx);
    }

    // Do not bother with instruction that do not produce a value
    if (!instr.getDest().has_value())
      continue;

    const auto var = instr.getDest().value().getName();
    auto lvn_value = getLVNValue(instr, table, env);
    const auto lookup_result = table.lookup(*lvn_value);
    if (lookup_result.has_value()) {
      // If we found a matching LVNValue, create an id instruction pointing to
      // the canonical variable.
      env.insert(var, lookup_result.value().first);
      const auto &new_instr =
          IdInstruction(var, instr.getType(), lookup_result.value().second);
      bb.replaceInstruction(std::make_unique<IdInstruction>(new_instr), i);
    } else {
      env.insert(var, table.add(std::move(lvn_value), var));
    }
  }
}
