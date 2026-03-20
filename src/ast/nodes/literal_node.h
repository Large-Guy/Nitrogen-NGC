#ifndef NGC_LITERAL_NODE_H
#define NGC_LITERAL_NODE_H
#include "expression_node.h"

enum class LiteralNodeType {
    _NULL,
    FALSE,
    TRUE
};

class LiteralNode : public ExpressionNode {
public:
    LiteralNode(LiteralNodeType type);

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;

    LiteralNodeType type;
};

#endif //NGC_LITERAL_NODE_H
