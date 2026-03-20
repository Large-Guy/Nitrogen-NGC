#ifndef NGC_IF_NODE_H
#define NGC_IF_NODE_H
#include "expression_node.h"
#include "statement_node.h"


class IfNode : public StatementNode {
public:
    IfNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<StatementNode> then,
           std::unique_ptr<StatementNode> otherwise);

    std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<StatementNode> then;
    std::unique_ptr<StatementNode> otherwise;
};


#endif //NGC_IF_NODE_H
