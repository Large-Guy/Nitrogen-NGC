#include "identifier_node.h"

#include <utility>

IdentifierNode::IdentifierNode(std::string identifier) : identifier(std::move(identifier)) {
}

std::unique_ptr<AstNode> IdentifierNode::Clone() const {
    return std::make_unique<IdentifierNode>(*this);
}
