#ifndef NGC_INDEX_NODE_H
#define NGC_INDEX_NODE_H
#include "expression_node.h"


class IndexNode : public ExpressionNode {
public:
    IndexNode(std::unique_ptr<ExpressionNode> data, std::unique_ptr<ExpressionNode> index);

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<ExpressionNode> data;
    std::unique_ptr<ExpressionNode> index;
};


#endif //NGC_INDEX_NODE_H
