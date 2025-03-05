#pragma once

#include <string>

#include "json.h"

class Var {
  using Json = nlohmann::json;

  std::string m_name;

public:
  Var(std::string &&name) : m_name(std::move(name)) {}

  const std::string &getName() const { return m_name; }
  void setName(std::string &&newName) { m_name = std::move(newName); }

  Json toJson() const { return m_name; }
  static Var fromJson(const Json &json) { return Var(json.get<std::string>()); }
};
