//
// Created by Ravi Lebgue on 3/18/26.
//

#include "binary_node.h"

#include "../../memory_utils.h"

BinaryNode::BinaryNode(BinaryNodeType type, std::unique_ptr<ExpressionNode> left,
                       std::unique_ptr<ExpressionNode> right) : ExpressionNode(),
                                                                type(type),
                                                                left(std::move(left)),
                                                                right(std::move(right)) {
}

std::unique_ptr<AstNode> BinaryNode::Clone() const {
    return std::make_unique<BinaryNode>(type, UniqueCast<ExpressionNode>(left->Clone()),
                                        UniqueCast<ExpressionNode>(right->Clone()));
}
