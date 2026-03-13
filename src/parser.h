#ifndef NCC_PARSER_H
#define NCC_PARSER_H
#include "ast_node.h"
#include "lexer.h"


class Parser {
public:
    Parser(Lexer lexer);

private:
    std::unique_ptr<AstNode> Expression();

    std::unique_ptr<AstNode> Statement();

    std::vector<std::unique_ptr<AstNode> > Parse();

    std::unique_ptr<AstNode> ModuleStatement();

    std::unique_ptr<AstNode> BuildType(std::unique_ptr<AstNode> base);

    std::unique_ptr<AstNode> NodeFromType(const Token& token);

    std::unique_ptr<AstNode> Declaration();

    std::unique_ptr<AstNode> Block();

    std::unique_ptr<AstNode> DeclarationStatement();

    void Error(const Token& token, const std::string& message);

    Token Advance();

    Token Peek();

    [[nodiscard]] bool Check(TokenType type) const;

    [[nodiscard]] bool CheckType() const;

    bool MatchType();

    bool Match(TokenType type);

    void Consume(TokenType type, const std::string& error);

    Lexer lexer_;

    Token current_;
    Token previous_;
};


#endif //NCC_PARSER_H
