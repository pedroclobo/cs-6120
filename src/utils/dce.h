#pragma once

#include "basic_block.h"

// Local Dead Code Elimination (operates on basic blocks)
struct LDCE {
  LDCE() = default;
  static void run(BasicBlock &bb);
};
