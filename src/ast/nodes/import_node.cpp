#include "import_node.h"

ImportNode::ImportNode(std::string name) : name(std::move(name)) {
}

std::unique_ptr<AstNode> ImportNode::Clone() const {
    return std::make_unique<ImportNode>(name);
}
