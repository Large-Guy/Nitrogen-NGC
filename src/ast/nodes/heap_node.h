//
// Created by Ravi Lebgue on 3/19/26.
//

#ifndef NGC_HEAP_NODE_H
#define NGC_HEAP_NODE_H
#include <memory>

#include "expression_node.h"


class HeapNode : public ExpressionNode {
public:
    HeapNode(std::unique_ptr<ExpressionNode> expression);

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;


    std::unique_ptr<ExpressionNode> expression;
};


#endif //NGC_HEAP_NODE_H
