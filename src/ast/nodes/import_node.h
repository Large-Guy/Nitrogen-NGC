#ifndef NGC_IMPORT_NODE_H
#define NGC_IMPORT_NODE_H
#include <string>

#include "../ast_node.h"


class ImportNode : public AstNode {
public:
    ImportNode(std::string name);

    [[nodiscard]] std::unique_ptr<AstNode> Clone() const override;

    std::string name;
};


#endif //NGC_IMPORT_NODE_H
