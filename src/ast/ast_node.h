#ifndef NGC_AST_NODE_H
#define NGC_AST_NODE_H
#include <memory>


class AstNode {
public:
    AstNode() = default;

    AstNode(const AstNode&) = default;

    AstNode(AstNode&&) = default;

    AstNode& operator=(AstNode&&) = default;

    virtual ~AstNode() = default;

    [[nodiscard]] virtual std::unique_ptr<AstNode> Clone() const = 0;
};


#endif //NGC_AST_NODE_H
