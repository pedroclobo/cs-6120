#include <iostream>
#include <sstream>

#include "dce.h"
#include "parser.h"

int main() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();

  auto prog = Parser::parse(buffer.str());

  for (auto &f : prog)
    for (auto &bb : f)
      LDCE::run(bb);

  std::cout << prog;
}
