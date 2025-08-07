#include "to-ssa.h"
#include "function.h"
#include "instruction.h"
#include "json.h"
#include <dom-tree.h>
#include <memory>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using Json = nlohmann::json;

using VarStack = std::unordered_map<Var, std::stack<std::string>>;

// TODO: remove unecessary allocations.
std::string ToSSA::new_name(std::string_view var_name) {
  ++m_counter[std::string(var_name)];
  if (m_counter[std::string(var_name)] == 1)
    return std::string(var_name);
  return std::format("{}.{}", var_name, m_counter[std::string(var_name)] - 1);
}

void ToSSA::rename(BasicBlock &bb, const DomTree &dtree) {
  // Keep track of how many pushes to the stack we had for each var.
  std::unordered_map<Var, size_t> pushed_count;

  for (auto &instr : bb) {
    // Replace instruction argument names with the names on top of the stack.
    // Skip `phi` nodes, as argument substitution is performed at a later step.
    if (instr->getOpcode() != Opcode::Phi) {
      for (size_t i = 0, e = instr->getNumArgs(); i < e; ++i) {
        auto &arg = instr->getArg(i);
        assert(!m_stack[arg].empty());
        arg.setName(m_stack[arg].top());
      }
    }

    // Generate a new name for the destination variable and push onto the stack.
    if (instr->getDest()) {
      auto *dest = instr->getDest();
      std::string old_name = dest->getName();
      auto var_name = new_name(old_name);
      dest->setName(var_name);
      m_stack[Var(old_name)].push(std::move(var_name));
      ++pushed_count[Var(std::move(old_name))];
    }
  }

  for (auto *child : dtree.children(bb))
    rename(*child, dtree);

  // Pop all names pushed in this `rename` call off the stack.
  for (const auto &[var, count] : pushed_count)
    for (size_t i = 0; i < count; ++i) {
      assert(!m_stack[var].empty());
      m_stack[var].pop();
    }
}

// TODO: In order to implement this, we may want to use a reaching definitions
// data structure in order to check what definition reaches the particular use.
// This would avoid the `defs_by_block` map and we could maybe combine this into
// the `rename` stage.
static void rename_phis(
    Function &f,
    const std::unordered_map<
        Var, std::unordered_map<BasicBlock *, Instruction *>> &defs_by_block) {
  for (auto &bb : f) {
    for (auto *succ : bb.successors()) {
      for (auto *phi : succ->phis()) {
        for (auto *pred : succ->predecessors()) {
          // Get the original variable name from the phi entry
          auto old_name = phi->getEntry(pred->getName());
          if (!old_name)
            continue;

          Var orig_var((std::string(*old_name)));

          // Look up the instruction defining that variable in the predecessor
          // block
          auto var_iter = defs_by_block.find(orig_var);
          if (var_iter == defs_by_block.end())
            continue;

          auto &bb_defs = var_iter->second;
          auto def_iter = bb_defs.find(pred);
          // No definition in this predecessor block was found.
          // This `phi` instruction will be deleted later.
          if (def_iter == bb_defs.end())
            continue;

          // We found the definition of `var` in the predecessor.
          Instruction *def_instr = def_iter->second;
          if (!def_instr || !def_instr->getDest())
            continue;

          // Rename the `phi` entry of the current block with the name of the
          // instruction in the predecessor block.
          std::string renamed = def_instr->getDest()->getName();
          phi->renameEntry(pred->getName(), std::move(renamed));
        }
      }
    }
  }
}

void ToSSA::run(Function &f) {
  DomTree dtree = DomTree::build(f);
  ToSSA toSSA;

  // Keep track of variable types in order to build phi instructions
  std::unordered_map<Var, Type> var_types;

  // Set of basic blocks where `var` is defined
  std::unordered_map<Var, std::unordered_set<BasicBlock *>> defs;
  std::unordered_map<Var, std::unordered_map<BasicBlock *, Instruction *>>
      defs_by_block;
  std::set<Var> vars;
  for (auto &bb : f)
    for (auto &instr : bb)
      if (instr->getDest()) {
        vars.insert(Var(instr->getDest()->getName()));
        defs[Var(instr->getDest()->getName())].insert(&bb);
        defs_by_block[Var(*instr->getDest())][&bb] = &*instr;
        var_types[Var(instr->getDest()->getName())] = instr->getType();
      }

  // Add phi nodes to the dominance frontier
  for (const auto &var : vars) {
    std::set<BasicBlock *> inserted_phi;
    std::stack<BasicBlock *> w;
    for (auto &bb : defs[Var(var.getName())])
      w.push(bb);

    for (; !w.empty(); w.pop()) {
      auto bb = w.top();
      for (auto *bbf : dtree.dfront(*bb)) {
        // Do not a phi node for the same variable twice
        // TODO: make another pass to eliminate wrong phi's
        if (!inserted_phi.contains(bbf)) {
          auto phi_ptr = std::make_unique<PhiInstruction>(
              Var(std::string(var.getName())), var_types[var]);
          auto phi = phi_ptr.get();
          for (auto *pred : bbf->predecessors())
            phi->addIncoming(Var(var.getName()), std::string(pred->getName()));
          bbf->prependInstruction(std::move(phi_ptr));
          inserted_phi.insert(bbf);

          // The phi node is a new definition site
          if (!defs[Var(var.getName())].contains(bbf)) {
            defs[Var(var.getName())].insert(bbf);
            defs_by_block[Var(var.getName())][bbf] = phi;
            w.push(bbf);
          }
        }
      }
    }
  }

  VarStack stack;
  std::unordered_map<std::string, size_t> name_counters;
  for (const auto &var : vars)
    stack[std::string(var.getName())] = {};
  toSSA.rename(f[0], dtree);
  // Rename phi arguments at a later stage in order to support loops
  rename_phis(f, defs_by_block);
}
