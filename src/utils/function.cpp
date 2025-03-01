#include <iostream>

#include "function.h"

Json Function::toJson() const {
  Json json;
  json["name"] = m_name;

  for (const auto &bb : m_bbs)
    for (const auto &instr : bb)
      json["instrs"].push_back(instr);

  return json;
}

std::ostream &operator<<(std::ostream &os, const Function &f) {
  os << f.toJson().dump(2);
  return os;
}
