#ifndef NGC_TYPENODE_H
#define NGC_TYPENODE_H
#include <memory>

#include "expression_node.h"
#include "../ast_node.h"

enum class TypeNodeType {
    VOID,
    BORROW,
    OWNER,
    OPTIONAL,
    ARRAY,
    MAP,
    SIMD,
    TUPLE,

    BOOL,

    I8,
    I16,
    I32,
    I64,

    U8,
    U16,
    U32,
    U64,

    F32,
    F64,

    TYPE_COUNT, // used for implicit cast table
    INFERED
};

class TypeNode : public AstNode {
public:
    TypeNode(TypeNodeType type, std::unique_ptr<TypeNode> subtype = nullptr,
             std::unique_ptr<ExpressionNode> capacity = nullptr);

    std::unique_ptr<AstNode> Clone() const override;

    TypeNodeType type;
    std::unique_ptr<TypeNode> subtype;
    std::unique_ptr<ExpressionNode> capacity;
};


#endif //NGC_TYPENODE_H
