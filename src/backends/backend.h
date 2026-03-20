#ifndef NGC_BACKEND_H
#define NGC_BACKEND_H
#include <vector>

#include "../ast/ast_node.h"


class Backend {
public:
    virtual ~Backend() = default;

    virtual void Generate(std::vector<std::unique_ptr<AstNode> > nodes) = 0;
};


#endif //NGC_BACKEND_H
