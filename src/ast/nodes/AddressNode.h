#ifndef NGC_ADDRESSNODE_H
#define NGC_ADDRESSNODE_H
#include "expression_node.h"


class AddressNode : public ExpressionNode {
public:
    std::unique_ptr<ExpressionNode> target;

    AddressNode(std::unique_ptr<ExpressionNode> target);

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;
};


#endif //NGC_ADDRESSNODE_H