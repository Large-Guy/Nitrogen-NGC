#ifndef NGC_ASSIGN_NODE_H
#define NGC_ASSIGN_NODE_H
#include <memory>

#include "expression_node.h"


class AssignNode : public ExpressionNode {
public:
    AssignNode(std::unique_ptr<ExpressionNode> target, std::unique_ptr<ExpressionNode> value);

    std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<ExpressionNode> target;
    std::unique_ptr<ExpressionNode> value;
};


#endif //NGC_ASSIGN_NODE_H
