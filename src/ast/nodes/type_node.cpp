#include "type_node.h"
#include "../../memory_utils.h"

TypeNode::TypeNode(TypeNodeType type, std::unique_ptr<TypeNode> subtype,
                   std::unique_ptr<ExpressionNode> capacity) : type(type), subtype(std::move(subtype)),
                                                               capacity(std::move(capacity)) {
}

std::unique_ptr<AstNode> TypeNode::Clone() const {
    std::unique_ptr<TypeNode> sub = nullptr;
    if (subtype != nullptr) {
        sub = UniqueCast<TypeNode>(subtype->Clone());
    }
    return std::make_unique<TypeNode>(type, std::move(sub), UniqueCast<ExpressionNode>(capacity->Clone()));
}
