#pragma once

#include "program.h"

struct Parser {
  static Program parse(std::string_view prog);
};
