#include "parser.h"
#include "instruction_utils.h"

Program Parser::parse(std::string_view prog) {
  const auto json = Json::parse(prog);

  std::vector<Function> functions;
  for (const auto &func : json) {
    std::vector<BasicBlock> bbs;

    BasicBlock bb("entry");
    std::string bb_name = "entry";
    for (const auto &instr : func[0]["instrs"]) {
      if (InstructionUtils::isLabel(instr)) {
        if (!bb.empty()) {
          bb.setName(std::move(bb_name));
          bbs.push_back(bb);
        }
        bb = BasicBlock();
        bb_name = instr["label"];
      } else {
        bb.addInstruction(instr);
        if (InstructionUtils::isTerminator(instr)) {
          bbs.push_back(bb);
          bb = BasicBlock();
        }
      }
    }

    if (!bb.empty()) {
      bb.setName(std::move(bb_name));
      bbs.push_back(bb);
    }

    functions.emplace_back(std::move(func[0]["name"]), std::move(bbs));
  }

  return Program(std::move(functions));
}
