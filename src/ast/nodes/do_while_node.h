#ifndef NGC_DO_WHILE_NODE_H
#define NGC_DO_WHILE_NODE_H
#include "expression_node.h"
#include "statement_node.h"


class DoWhileNode : public StatementNode {
public:
    DoWhileNode(std::unique_ptr<StatementNode> body, std::unique_ptr<ExpressionNode> condition);

    std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<StatementNode> body;
    std::unique_ptr<ExpressionNode> condition;
};


#endif //NGC_DO_WHILE_NODE_H
