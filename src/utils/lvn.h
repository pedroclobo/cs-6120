#pragma once

#include "basic_block.h"

// Local Value Numbering
struct LVN {
  LVN() = default;
  static void run(BasicBlock &bb);
};
