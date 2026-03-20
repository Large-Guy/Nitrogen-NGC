#include "unary_node.h"

#include "../../memory_utils.h"

UnaryNode::UnaryNode(UnaryNodeType type, std::unique_ptr<ExpressionNode> operand) : type(type),
    operand(std::move(operand)) {
}

std::unique_ptr<AstNode> UnaryNode::Clone() const {
    return std::make_unique<UnaryNode>(type, UniqueCast<ExpressionNode>(operand->Clone()));
}
