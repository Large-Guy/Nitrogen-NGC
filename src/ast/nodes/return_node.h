#ifndef NGC_RETURN_STATEMENT_H
#define NGC_RETURN_STATEMENT_H
#include "expression_node.h"
#include "statement_node.h"


class ReturnNode : public StatementNode {
public:
    ReturnNode(std::unique_ptr<ExpressionNode> value);

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<ExpressionNode> value;
};


#endif //NGC_RETURN_STATEMENT_H
