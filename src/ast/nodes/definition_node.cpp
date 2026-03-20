#include "definition_node.h"

#include <utility>

DefinitionNode::DefinitionNode(std::string name) : name(std::move(name)) {
}

std::unique_ptr<AstNode> DefinitionNode::Clone() const {
    return std::make_unique<DefinitionNode>(*this);
}
