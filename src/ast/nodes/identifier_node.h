#ifndef NGC_IDENTIFIER_NODE_H
#define NGC_IDENTIFIER_NODE_H
#include <string>

#include "expression_node.h"


class IdentifierNode : public ExpressionNode {
public:
    IdentifierNode(std::string identifier);

    std::unique_ptr<AstNode> Clone() const override;

    std::string identifier;
};


#endif //NGC_IDENTIFIER_NODE_H
