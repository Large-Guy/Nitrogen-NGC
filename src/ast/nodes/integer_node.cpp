#include "integer_node.h"

IntegerNode::IntegerNode(int64_t value) : value(value) {
}

std::unique_ptr<AstNode> IntegerNode::Clone() const {
    return std::make_unique<IntegerNode>(value);
}
