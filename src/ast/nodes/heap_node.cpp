//
// Created by Ravi Lebgue on 3/19/26.
//

#include "heap_node.h"

#include "../../memory_utils.h"

HeapNode::HeapNode(std::unique_ptr<ExpressionNode> expression) : expression(std::move(expression)) {
}

std::unique_ptr<AstNode> HeapNode::Clone() const {
    return std::make_unique<HeapNode>(UniqueCast<ExpressionNode>(expression->Clone()));
}
