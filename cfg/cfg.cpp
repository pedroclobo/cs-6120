#include <iostream>

#include <CFG.h>

int main() {
  std::string input;
  std::string line;
  while (std::getline(std::cin, line))
    input += line;

  Json prog;
  try {
    prog = Json::parse(input);
  } catch (Json::parse_error &e) {
    std::cerr << "Json parse error: " << e.what() << std::endl;
    return 1;
  }

  auto bbs = BasicBlock::fromJson(prog);
  auto cfg = CFG::fromBasicBlocks(bbs);
  cfg.writeDot("cfg.dot");
  system("dot -Tpdf cfg.dot -o cfg.pdf");
  std::cout << "CFG written to cfg.pdf" << std::endl;

  return 0;
}