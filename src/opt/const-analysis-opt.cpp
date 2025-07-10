#include <iostream>
#include <sstream>

#include "const-analysis.h"
#include "parser.h"

int main() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();

  auto prog = Parser::parse(buffer.str());

  for (const auto &f : prog)
    ConstAnalysis::run(f);
}
