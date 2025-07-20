#pragma once

#include <string>

#include "json.h"

class Var {
  using Json = nlohmann::json;

  std::string m_name;

public:
  Var(std::string name) : m_name(std::move(name)) {}

  const std::string &getName() const { return m_name; }
  void setName(std::string newName) { m_name = std::move(newName); }

  bool operator==(const Var &other) const { return m_name == other.m_name; }
  bool operator<(const Var &other) const { return m_name < other.m_name; }

  Json toJson() const { return m_name; }
  static Var fromJson(const Json &json) { return Var(json.get<std::string>()); }

  friend std::ostream &operator<<(std::ostream &os, const Var &var) {
    os << "Var(" << var.m_name << ")";
    return os;
  }
};

namespace std {
template <> struct hash<Var> {
  std::size_t operator()(const Var &v) const noexcept {
    return std::hash<std::string>{}(v.getName());
  }
};
} // namespace std
