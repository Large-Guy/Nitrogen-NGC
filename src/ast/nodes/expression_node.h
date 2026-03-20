#ifndef NGC_EXPRESSION_NODE_H
#define NGC_EXPRESSION_NODE_H
#include "statement_node.h"
#include "../ast_node.h"


class ExpressionNode : public StatementNode {
public:
    ExpressionNode();

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;
};


#endif //NGC_EXPRESSION_NODE_H
