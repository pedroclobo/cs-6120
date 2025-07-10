#pragma once

#include "function.h"

// Constant Analysis
struct ConstAnalysis {
  ConstAnalysis() = default;
  static void run(const Function &f);
};
