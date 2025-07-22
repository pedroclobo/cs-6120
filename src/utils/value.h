#pragma once

#include <cstdint>

#include "json.h"
#include "type.h"

class Value {
  using Json = nlohmann::json;

  uint64_t m_value;
  Type m_type;

public:
  Value(uint64_t value, Type type) : m_value(value), m_type(type) {}

  uint64_t getValue() const { return m_value; }

  Json toJson() const {
    if (m_type.isBool())
      return Json(static_cast<bool>(m_value != 0));
    return m_value;
  }
  static Value fromJson(const Json &json) {
    assert(json.is_boolean() ||
           json.is_number_integer() && "Invalid JSON for Value");
    if (json.is_boolean())
      return Value(json.get<bool>(), Type::getBoolType());
    return Value(json.get<uint64_t>(), Type::getIntType());
  }
};
