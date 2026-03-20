#ifndef NGC_FIELD_NODE_H
#define NGC_FIELD_NODE_H
#include <memory>
#include <string>

#include "expression_node.h"


class FieldNode : public ExpressionNode {
public:
    FieldNode(std::unique_ptr<ExpressionNode> object, std::string name);

    std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<ExpressionNode> object;
    std::string name;
};


#endif //NGC_FIELD_NODE_H
