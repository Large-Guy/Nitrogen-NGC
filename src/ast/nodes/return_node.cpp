#include "return_node.h"

#include "../../memory_utils.h"

ReturnNode::ReturnNode(std::unique_ptr<ExpressionNode> value) : value(std::move(value)) {
}

std::unique_ptr<AstNode> ReturnNode::Clone() const {
    return std::make_unique<ReturnNode>(UniqueCast<ExpressionNode>(value->Clone()));
}
