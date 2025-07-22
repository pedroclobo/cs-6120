#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

#include "dom-tree.h"
#include "parser.h"

void help(std::string_view program) {
  std::cerr << "Usage: " << program << " [-o <dot>] [-pdf <pdf>] [-dom]\n"
            << " -o <dot> : Output filename for the .dot file\n"
            << " -pdf <pdf_output>: Output filename for the PDF\n"
            << " -dom: Output dominator relation\n";
}

int main(int argc, char *argv[]) {
  std::optional<std::string> dotFilename;
  std::optional<std::string> pdfFilename;
  bool dominators = false;
  bool immediate_dominators = false;
  bool dominance_frontier = false;

  for (int i = 1; i < argc; ++i) {
    std::string_view arg = argv[i];
    if (arg == "-o" && i + 1 < argc) {
      dotFilename = argv[++i];
    } else if (arg == "-pdf" && i + 1 < argc) {
      pdfFilename = argv[++i];
    } else if (arg == "-dom") {
      dominators = true;
    } else if (arg == "-idom") {
      immediate_dominators = true;
    } else if (arg == "-dfront") {
      dominance_frontier = true;
    } else if (arg == "--help" || arg == "-h") {
      help(argv[0]);
      return 0;
    } else {
      std::cerr << "Error: Unknown argument " << arg << std::endl;
      help(argv[0]);
      return 1;
    }
  }

  std::stringstream buffer;
  buffer << std::cin.rdbuf();

  auto prog = Parser::parse(buffer.str());
  const auto tree = DomTree::build(prog[0]);

  if (dominators) {
    tree.writeDominators(std::cout);
  }
  if (immediate_dominators) {
    tree.writeImmediateDominators(std::cout);
  }
  if (dominance_frontier) {
    tree.writeDominanceFrontier(std::cout);
  }

  if (dominators || immediate_dominators || dominance_frontier)
    return 0;

  if (!dotFilename.has_value() || *dotFilename == "-") {
    tree.writeDot(std::cout);
  } else if (dotFilename.has_value()) {
    std::ofstream dotFile(*dotFilename);
    if (!dotFile) {
      std::cerr << "Error: Could not write to " << *dotFilename << std::endl;
      return 1;
    }
    tree.writeDot(dotFile);
  }

  if (dotFilename.has_value() && pdfFilename.has_value()) {
    std::string dotCmd = "dot -Tpdf " + *dotFilename + " -o " + *pdfFilename;
    if (system(dotCmd.c_str()) != 0) {
      std::cerr << "Error: Failed to generate PDF" << std::endl;
      return 1;
    }
    std::cout << "DomTree written to " << *pdfFilename << std::endl;
  }
}
