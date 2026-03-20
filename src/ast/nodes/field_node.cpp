#include "field_node.h"

#include "../../memory_utils.h"

FieldNode::FieldNode(std::unique_ptr<ExpressionNode> object, std::string name) : object(std::move(object)),
    name(std::move(name)) {
}

std::unique_ptr<AstNode> FieldNode::Clone() const {
    return std::make_unique<FieldNode>(UniqueCast<ExpressionNode>(object->Clone()), name);
}
