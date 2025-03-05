#pragma once

#include <cstdint>

#include "json.h"

class Value {
  using Json = nlohmann::json;

  uint64_t m_value;

public:
  Value(uint64_t value) : m_value(value) {}

  uint64_t getValue() const { return m_value; }

  Json toJson() const { return m_value; }
  static Value fromJson(const Json &json) {
    assert(json.is_boolean() ||
           json.is_number_integer() && "Invalid JSON for Value");
    if (json.is_boolean())
      return Value(json.get<bool>());
    return Value(json.get<uint64_t>());
  }
};
