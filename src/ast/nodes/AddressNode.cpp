#include "AddressNode.h"

#include "../../memory_utils.h"

AddressNode::AddressNode(std::unique_ptr<ExpressionNode> target) : target(std::move(target)) {
}

std::unique_ptr<AstNode> AddressNode::Clone() const {
    return std::make_unique<AddressNode>(UniqueCast<ExpressionNode>(target->Clone()));
}
