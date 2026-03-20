#include "expression_node.h"

ExpressionNode::ExpressionNode() = default;

std::unique_ptr<AstNode> ExpressionNode::Clone() const {
    throw std::runtime_error("Not implemented");
}
