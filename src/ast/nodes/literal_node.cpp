#include "literal_node.h"

LiteralNode::LiteralNode(LiteralNodeType type) : type(type) {
}

std::unique_ptr<AstNode> LiteralNode::Clone() const {
    return std::make_unique<LiteralNode>(*this);
}
