#include "function.h"
#include "json.h"

using Json = nlohmann::json;

Json Function::toJson() const {
  Json json;
  json["name"] = m_name;
  json["instrs"] = Json::array();

  bool first = true;
  for (const auto &bb : m_bbs) {
    for (const auto &instr : bb.toJson(!first))
      json["instrs"].push_back(instr);
    first = false;
  }

  return json;
}

std::ostream &operator<<(std::ostream &os, const Function &f) {
  os << f.toJson().dump(2);
  return os;
}
