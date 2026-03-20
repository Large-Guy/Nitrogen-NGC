#include "while_node.h"

#include "../../memory_utils.h"

WhileNode::WhileNode(std::unique_ptr<ExpressionNode> condition,
                     std::unique_ptr<StatementNode> body) : condition(std::move(condition)), body(std::move(body)) {
}

std::unique_ptr<AstNode> WhileNode::Clone() const {
    return std::make_unique<WhileNode>(UniqueCast<ExpressionNode>(condition->Clone()),
                                       UniqueCast<StatementNode>(body->Clone()));
}
