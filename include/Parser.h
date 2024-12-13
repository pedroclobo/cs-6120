#pragma once

#include <Json.h>

using Json = nlohmann::json;

class Parser {
  static Json parse();
};
