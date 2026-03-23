#include "LockNode.h"

#include "../../memory_utils.h"

LockNode::LockNode(std::unique_ptr<ExpressionNode> target) : target(std::move(target)) {
}

std::unique_ptr<AstNode> LockNode::Clone() const {
    return std::make_unique<LockNode>(UniqueCast<ExpressionNode>(target->Clone()));
}
