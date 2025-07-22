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
  Type() : m_kind(TypeKind::Void), m_size(0) {}
  Type(TypeKind kind, std::size_t size) : m_kind(kind), m_size(size) {}

  TypeKind getKind() const { return m_kind; }
  bool isVoid() const { return m_kind == TypeKind::Void; }
  bool isInt() const { return m_kind == TypeKind::Int; }
  bool isBool() const { return m_kind == TypeKind::Bool; }
  std::size_t getSize() const { return m_size; }

  static Type getVoidType();
  static Type getIntType(std::size_t size = 4);
  static Type getBoolType(std::size_t size = 1);

  Json toJson() const;
  static Type fromJson(const Json &json);
};
