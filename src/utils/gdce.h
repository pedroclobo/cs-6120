#pragma once

#include "function.h"

struct GDCE {
  GDCE() = default;
  static void run(Function &f);
};
