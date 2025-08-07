#include <iostream>
#include <sstream>

#include "gdce.h"
#include "parser.h"
#include "to-ssa.h"

int main() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();

  auto prog = Parser::parse(buffer.str());

  for (auto &f : prog) {
    ToSSA::run(f);
    GDCE::run(f);
  }

  std::cout << prog;
}
