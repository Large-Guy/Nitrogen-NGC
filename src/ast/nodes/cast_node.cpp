#include "cast_node.h"

#include "../../memory_utils.h"


CastNode::CastNode(CastNodeType type, std::unique_ptr<TypeNode> target,
                   std::unique_ptr<ExpressionNode> expression) : type(type), target(std::move(target)),
                                                                 expression(std::move(expression)) {
}

std::unique_ptr<AstNode> CastNode::Clone() const {
    return std::make_unique<CastNode>(this->type, UniqueCast<TypeNode>(target->Clone()),
                                      UniqueCast<ExpressionNode>(expression->Clone()));
}
