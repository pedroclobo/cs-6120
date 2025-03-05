#include "parser.h"
#include "instruction.h"
#include "json.h"

using Json = nlohmann::json;

static bool isLabel(const Json &instr) { return instr.contains("label"); }

static bool isTerminator(const Json &instr) {
  return instr.contains("op") && instr["op"] == "ret";
}

Program Parser::parse(std::string_view prog) {
  const auto json = Json::parse(prog);

  std::vector<Function> functions;
  for (const auto &func : json) {
    std::vector<BasicBlock> bbs;

    BasicBlock bb("entry");
    std::string bb_name = "entry";
    for (const auto &instr : func[0]["instrs"]) {
      if (isLabel(instr)) {
        if (!bb.empty()) {
          bb.setName(std::move(bb_name));
          bbs.push_back(std::move(bb));
        }
        bb = BasicBlock();
        bb_name = instr["label"];
      } else {
        bb.addInstruction(Instruction::fromJson(instr));
        if (isTerminator(instr)) {
          bbs.push_back(std::move(bb));
          bb = BasicBlock();
        }
      }
    }

    if (!bb.empty()) {
      bb.setName(std::move(bb_name));
      bbs.push_back(std::move(bb));
    }

    functions.emplace_back(std::move(func[0]["name"]), std::move(bbs));
  }

  return Program(std::move(functions));
}
