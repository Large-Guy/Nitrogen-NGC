#include "block.h"

#include <llvm/IR/BasicBlock.h>

Block::Block(llvm::LLVMContext* context) {
    block = llvm::BasicBlock::Create(*context);
}

bool Block::Valid(llvm::Value* value) const {
    // scan backwards, but avoid getting into a circle somehow
    if (moved_values.contains(value)) {
        return false;
    }
    for (auto& pred : predecessors) {
        auto lock = pred.lock();
        if (!lock->Valid(value)) {
            return false;
        }
    }
    return true;
}

void Block::Connect(const std::shared_ptr<Block>& successor) {
    successors.push_back(successor);
    successor->predecessors.push_back(shared_from_this());
}

void Block::Move(llvm::Value* value) {
    moved_values.emplace(value);
}
