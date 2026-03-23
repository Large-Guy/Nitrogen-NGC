#ifndef NGC_UNARY_NODE_H
#define NGC_UNARY_NODE_H
#include <memory>

#include "expression_node.h"
#include "../ast_node.h"

enum class UnaryNodeType {
    NEGATE,
    NOT,
};

class UnaryNode : public ExpressionNode {
public:
    UnaryNode(UnaryNodeType type, std::unique_ptr<ExpressionNode> operand);

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;

    UnaryNodeType type;
    std::unique_ptr<ExpressionNode> operand;
};


#endif //NGC_UNARY_NODE_H
