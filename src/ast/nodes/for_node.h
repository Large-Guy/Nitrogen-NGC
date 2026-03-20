#ifndef NGC_FOR_NODE_H
#define NGC_FOR_NODE_H
#include "expression_node.h"
#include "statement_node.h"


class ForNode : public StatementNode {
public:
    ForNode(std::unique_ptr<StatementNode> init, std::unique_ptr<ExpressionNode> condition,
            std::unique_ptr<ExpressionNode> postfix, std::unique_ptr<StatementNode> body);

    std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<StatementNode> init;
    std::unique_ptr<StatementNode> condition;
    std::unique_ptr<ExpressionNode> postfix;
    std::unique_ptr<StatementNode> body;
};


#endif //NGC_FOR_NODE_H
