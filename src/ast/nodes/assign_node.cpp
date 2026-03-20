#include "assign_node.h"

#include "../../memory_utils.h"

AssignNode::AssignNode(std::unique_ptr<ExpressionNode> target,
                       std::unique_ptr<ExpressionNode> value) : target(std::move(target)), value(std::move(value)) {
}

std::unique_ptr<AstNode> AssignNode::Clone() const {
    return std::make_unique<AssignNode>(UniqueCast<ExpressionNode>(target->Clone()),
                                        UniqueCast<ExpressionNode>(value->Clone()));
}
