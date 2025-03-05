#pragma once

#include <cstddef>

#include "json.h"

enum class TypeKind {
  Void,
  Int,
  Bool,
};

class Type {
  using Json = nlohmann::json;

  TypeKind m_kind;
  std::size_t m_size;

public:
  Type(TypeKind kind, std::size_t size) : m_kind(kind), m_size(size) {}

  TypeKind getKind() const { return m_kind; }
  std::size_t getSize() const { return m_size; }

  static Type getVoidType();
  static Type getIntType(std::size_t size = 4);
  static Type getBoolType(std::size_t size = 1);

  Json toJson() const;
  static Type fromJson(const Json &json);
};
