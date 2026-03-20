#ifndef NGC_CALL_NODE_H
#define NGC_CALL_NODE_H
#include <memory>
#include <vector>

#include "expression_node.h"


class CallNode : public ExpressionNode {
public:
    CallNode(std::unique_ptr<ExpressionNode> callable, std::vector<std::unique_ptr<ExpressionNode> > args);

    std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<ExpressionNode> callable;
    std::vector<std::unique_ptr<ExpressionNode> > args;
};


#endif //NGC_CALL_NODE_H
