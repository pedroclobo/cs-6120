#pragma once

#include <json.h>

using Json = nlohmann::json;

class Parser {
  static Json parse();
};
