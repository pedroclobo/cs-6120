#include <iostream>

#include "json.h"
#include "program.h"

using Json = nlohmann::json;

Json Program::toJson() const {
  Json json;

  for (const auto &func : m_funcs)
    json["functions"].push_back(func.toJson());

  return json;
}

std::ostream &operator<<(std::ostream &os, const Program &p) {
  os << p.toJson().dump(2);
  return os;
}
