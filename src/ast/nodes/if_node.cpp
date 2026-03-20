#include "if_node.h"

#include "../../memory_utils.h"

IfNode::IfNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<StatementNode> then,
               std::unique_ptr<StatementNode> otherwise) : condition(std::move(condition)), then(std::move(then)),
                                                           otherwise(std::move(otherwise)) {
}

std::unique_ptr<AstNode> IfNode::Clone() const {
    return std::make_unique<IfNode>(UniqueCast<ExpressionNode>(condition->Clone()),
                                    UniqueCast<StatementNode>(then->Clone()),
                                    UniqueCast<StatementNode>(otherwise->Clone()));
}
