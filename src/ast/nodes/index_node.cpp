#include "index_node.h"

#include "../../memory_utils.h"

IndexNode::IndexNode(std::unique_ptr<ExpressionNode> data,
                     std::unique_ptr<ExpressionNode> index) : data(std::move(data)), index(std::move(index)) {
}

std::unique_ptr<AstNode> IndexNode::Clone() const {
    return std::make_unique<IndexNode>(UniqueCast<ExpressionNode>(data->Clone()),
                                       UniqueCast<ExpressionNode>(index->Clone()));
}
