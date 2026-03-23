#ifndef NGC_LOCKNODE_H
#define NGC_LOCKNODE_H
#include "expression_node.h"


class LockNode : public ExpressionNode {
public:
    std::unique_ptr<ExpressionNode> target;

    LockNode(std::unique_ptr<ExpressionNode> target);

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;
};


#endif //NGC_LOCKNODE_H