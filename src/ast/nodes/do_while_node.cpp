#include "do_while_node.h"

#include "../../memory_utils.h"

DoWhileNode::DoWhileNode(std::unique_ptr<StatementNode> body,
                         std::unique_ptr<ExpressionNode> condition) : body(std::move(body)),
                                                                      condition(std::move(condition)) {
}

std::unique_ptr<AstNode> DoWhileNode::Clone() const {
    return std::make_unique<DoWhileNode>(UniqueCast<StatementNode>(body->Clone()),
                                         UniqueCast<ExpressionNode>(condition->Clone()));
}
