#ifndef NGC_TUPLE_NODE_H
#define NGC_TUPLE_NODE_H
#include <memory>
#include <vector>

#include "expression_node.h"


class TupleNode : public ExpressionNode {
public:
    TupleNode(std::vector<std::unique_ptr<ExpressionNode> > elements);

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;

    std::vector<std::unique_ptr<ExpressionNode> > elements;
};


#endif //NGC_TUPLE_NODE_H
