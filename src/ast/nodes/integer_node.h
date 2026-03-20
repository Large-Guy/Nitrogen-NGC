#ifndef NGC_INTEGER_NODE_H
#define NGC_INTEGER_NODE_H
#include <variant>

#include "expression_node.h"


class IntegerNode : public ExpressionNode {
public:
    IntegerNode(int64_t value);

    std::unique_ptr<AstNode> Clone() const override;

    int64_t value;
};


#endif //NGC_INTEGER_NODE_H
