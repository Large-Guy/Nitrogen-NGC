#include "float_node.h"

FloatNode::FloatNode(double value) : value(value) {
}

std::unique_ptr<AstNode> FloatNode::Clone() const {
    return std::make_unique<FloatNode>(value);
}
