#ifndef NCC_AST_NODE_H
#define NCC_AST_NODE_H
#include <memory>
#include <optional>
#include <vector>

#include "lexer.h"

enum class AstNodeType {
    //types
    NULL_,
    VOID,
    BORROW,
    OWNER,
    OPTIONAL,
    ARRAY,
    MAP,
    SIMD,

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

    INFER, // for let statements

    SEQUENCE, // essentially equivalent to a compound statement

    NAME,

    MODULE,
    GET,
    
    FUNCTION,
    VARIABLE,
};

const char* AstNodeTypeToString(AstNodeType e);

class AstNode {
public:
    static std::unique_ptr<AstNode> New(AstNodeType type, std::optional<Token> token = std::nullopt);

    AstNode(const AstNode&) = delete;
    AstNode& operator=(const AstNode&) = delete;
    AstNode(AstNode&&) = default;
    AstNode& operator=(AstNode&&) = default;
    ~AstNode();

    [[nodiscard]] AstNode* parent() const;
    AstNode* operator[](size_t index) const;
    void AddNode(std::unique_ptr<AstNode> node);
    void RemoveNode(const AstNode* node);
    
    void Debug(uint32_t depth = 0) const;

    AstNodeType type;
    std::optional<Token> token;
private:
    AstNode(AstNodeType type, std::optional<Token> token);

    AstNode* parent_;
    std::vector<std::unique_ptr<AstNode>> children_;
};


#endif //NCC_AST_NODE_H