#ifndef NGC_CAST_NODE_H
#define NGC_CAST_NODE_H
#include <memory>

#include "expression_node.h"
#include "type_node.h"

enum class CastNodeType {
    STATIC,
    REINTERPRET,
};

class CastNode : public ExpressionNode {
public:
    CastNode(CastNodeType type, std::unique_ptr<TypeNode> target, std::unique_ptr<ExpressionNode> expression);

    std::unique_ptr<AstNode> Clone() const override;

    CastNodeType type;
    std::unique_ptr<TypeNode> target;
    std::unique_ptr<ExpressionNode> expression;
};


#endif //NGC_CAST_NODE_H
