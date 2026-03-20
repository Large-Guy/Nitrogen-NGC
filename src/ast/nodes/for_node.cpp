#include "for_node.h"

#include "../../memory_utils.h"

ForNode::ForNode(std::unique_ptr<StatementNode> init, std::unique_ptr<ExpressionNode> condition,
                 std::unique_ptr<ExpressionNode> postfix, std::unique_ptr<StatementNode> body) : init(std::move(init)),
    condition(std::move(condition)), postfix(std::move(postfix)), body(std::move(body)) {
}

std::unique_ptr<AstNode> ForNode::Clone() const {
    return std::make_unique<ForNode>(UniqueCast<StatementNode>(init->Clone()),
                                     UniqueCast<ExpressionNode>(condition->Clone()),
                                     UniqueCast<ExpressionNode>(postfix->Clone()),
                                     UniqueCast<StatementNode>(body->Clone()));
}
