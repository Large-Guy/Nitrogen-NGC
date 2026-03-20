#ifndef NGC_WHILE_STATEMENT_H
#define NGC_WHILE_STATEMENT_H
#include "expression_node.h"
#include "statement_node.h"


class WhileNode : public StatementNode {
public:
    WhileNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<StatementNode> body);

    std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<StatementNode> body;
};


#endif //NGC_WHILE_STATEMENT_H
