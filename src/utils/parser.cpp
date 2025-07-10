#include <utility>

#include "instruction.h"
#include "json.h"
#include "parser.h"

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

    auto getBB = [](std::string bb_name,
                    std::vector<BasicBlock> &bbs) -> BasicBlock * {
      for (auto &bb : bbs) {
        if (bb_name == bb.getName())
          return &bb;
      }
      std::unreachable();
    };

    // Set basic block successors and predecessors
    for (size_t i = 0, e = bbs.size(); i < e; ++i) {
      auto &bb = bbs[i];
      const auto &last = bb.last();
      if (auto *bi = dynamic_cast<const BranchInstruction *>(&last)) {
        auto &first_succ = *getBB(bi->getLabel(0), bbs);
        auto &second_succ = *getBB(bi->getLabel(1), bbs);
        bb.addSucessor(first_succ);
        bb.addSucessor(second_succ);
        first_succ.addPrecedessor(bb);
        second_succ.addPrecedessor(bb);
      } else if (auto *ji = dynamic_cast<const JumpInstruction *>(&last)) {
        auto &succ = *getBB(ji->getLabel(), bbs);
        bb.addSucessor(succ);
        succ.addPrecedessor(bb);
      } else {
        // Fallthrough to next block
        if (i + 1 < bbs.size()) {
          bb.addSucessor(bbs[i + 1]);
          bbs[i + 1].addPrecedessor(bb);
        }
      }
    }

    functions.emplace_back(std::move(func[0]["name"]), std::move(bbs));
  }

  return Program(std::move(functions));
}
