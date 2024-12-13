#include <iostream>

#include <parser.h>

Json Parser::parse() {
  std::stringstream rawProg;
  std::string line;

  while (std::getline(std::cin, line))
    rawProg << line << "\n";

  return Json::parse(rawProg);
}
