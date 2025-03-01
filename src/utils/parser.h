#pragma once

#include "program.h"
#include <json.h>

using Json = nlohmann::json;

struct Parser {
  static Program parse(std::string_view prog);
};
