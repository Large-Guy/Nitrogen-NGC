#ifndef NGC_MODULE_NODE_H
#define NGC_MODULE_NODE_H
#include <string>

#include "statement_node.h"
#include "../ast_node.h"


class ModuleNode : public StatementNode {
public:
    ModuleNode(std::string path);

    std::unique_ptr<AstNode> Clone() const override;

    std::string path;
};


#endif //NGC_MODULE_NODE_H
