#include <cstddef>

#include "json.h"
#include "type.h"

using Json = nlohmann::json;

Type Type::getVoidType() { return Type(TypeKind::Void, 0); }
Type Type::getIntType(std::size_t size) { return Type(TypeKind::Int, size); }
Type Type::getBoolType(std::size_t size) { return Type(TypeKind::Bool, size); }

Json Type::toJson() const {
  switch (m_kind) {
  case TypeKind::Void:
    return "void";
  case TypeKind::Int:
    return "int";
  case TypeKind::Bool:
    return "bool";
  }
}

Type Type::fromJson(const Json &json) {
  assert(json.is_string() && "Types are represented as strings in JSON");
  std::string kind = json.get<std::string>();
  if (kind == "void") {
    return Type::getVoidType();
  } else if (kind == "int") {
    return Type::getIntType();
  } else if (kind == "bool") {
    return Type::getBoolType();
  }
  assert(false && "Unknown type kind");
}
