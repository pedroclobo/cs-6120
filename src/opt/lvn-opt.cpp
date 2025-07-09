#include <iostream>
#include <sstream>

#include "dce.h"
#include "lvn.h"
#include "parser.h"

int main() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();

  auto prog = Parser::parse(buffer.str());

  for (auto &f : prog)
    for (auto &bb : f) {
      LVN::run(bb);
      LDCE::run(bb);
    }

  std::cout << prog;
}
