#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

#include <cfg.h>

void help(std::string_view program) {
  std::cerr << "Usage: " << program << " [-o <dot>] [-pdf <pdf>]\n"
            << " -o <dot> : Output filename for the .dot file\n"
            << " -pdf <pdf_output>: Output filename for the PDF\n";
}

int main(int argc, char *argv[]) {
  std::optional<std::string> dotFilename;
  std::optional<std::string> pdfFilename;

  for (int i = 1; i < argc; ++i) {
    std::string_view arg = argv[i];
    if (arg == "-o" && i + 1 < argc) {
      dotFilename = argv[++i];
    } else if (arg == "-pdf" && i + 1 < argc) {
      pdfFilename = argv[++i];
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

  const auto prog = Json::parse(buffer.str());
  const auto bbs = BasicBlock::fromJson(prog);
  const auto cfg = CFG::fromBasicBlocks(bbs);

  if (!dotFilename.has_value() || *dotFilename == "-") {
    cfg.writeDot(std::cout);
  } else if (dotFilename.has_value()) {
    std::ofstream dotFile(*dotFilename);
    if (!dotFile) {
      std::cerr << "Error: Could not write to " << *dotFilename << std::endl;
      return 1;
    }
    cfg.writeDot(dotFile);
  }

  if (dotFilename.has_value() && pdfFilename.has_value()) {
    std::string dotCmd = "dot -Tpdf " + *dotFilename + " -o " + *pdfFilename;
    if (system(dotCmd.c_str()) != 0) {
      std::cerr << "Error: Failed to generate PDF" << std::endl;
      return 1;
    }
    std::cout << "CFG written to " << *pdfFilename << std::endl;
  }
}
