#include <iostream>

#include <Parser.h>

Json Parser::parse() {
  std::stringstream rawProg;
  std::string line;

  while (std::getline(std::cin, line))
    rawProg << line << "\n";

  Json prog;
  try {
    prog = Json::parse(rawProg);
  } catch (Json::parse_error &e) {
    std::cerr << "Json parse error: " << e.what() << std::endl;
  }

  return prog;
}