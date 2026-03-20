#include "module_node.h"

ModuleNode::ModuleNode(std::string path) : path(std::move(path)) {
}

std::unique_ptr<AstNode> ModuleNode::Clone() const {
    return std::unique_ptr<AstNode>(new ModuleNode(*this));
}
