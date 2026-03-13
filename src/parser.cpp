#include "parser.h"

#include <iostream>

Parser::Parser(Lexer lexer) : lexer_(std::move(lexer)) {
    Advance();
}

std::unique_ptr<AstNode> Parser::Expression() {
    
}

std::unique_ptr<AstNode> Parser::Statement() {
    if (Match(TokenType::TOKEN_TYPE_MODULE)) {
        return ModuleStatement();
    }
    if (MatchType()) {
        return DeclarationStatement();
    }
    if (Match(TokenType::TOKEN_TYPE_LEFT_BRACE)) {
        return Block();
    }
    Error(current_, "Unsupported instruction");
    return nullptr;
}

std::unique_ptr<AstNode> Parser::ModuleStatement() {
    auto node = AstNode::New(AstNodeType::MODULE);
    std::unique_ptr<AstNode> name = nullptr;
    do {
        Consume(TokenType::TOKEN_TYPE_IDENTIFIER, "Expected identifier");
        if (!name) {
            name = AstNode::New(AstNodeType::NAME, previous_);
        } else {
            auto get = AstNode::New(AstNodeType::GET);
            get->AddNode(std::move(name));
            get->AddNode(AstNode::New(AstNodeType::NAME, previous_));
            name = std::move(get);
        }
    }
    while (Match(TokenType::TOKEN_TYPE_DOT));

    Consume(TokenType::TOKEN_TYPE_SEMICOLON, "Expected semicolon after statement");

    node->AddNode(std::move(name));

    return node;
}

std::unique_ptr<AstNode> Parser::BuildType(std::unique_ptr<AstNode> base) {
    if (Match(TokenType::TOKEN_TYPE_GREATER)) {
        auto size = Expression();
        Consume(TokenType::TOKEN_TYPE_LESS, "Expected closing '>'");
        auto simd = AstNode::New(AstNodeType::SIMD);
        simd->AddNode(std::move(base));
        simd->AddNode(std::move(size));
        return BuildType(std::move(simd));
    }
    if (Match(TokenType::TOKEN_TYPE_STAR)) {
        auto owner = AstNode::New(AstNodeType::OWNER);
        owner->AddNode(std::move(base));
        return BuildType(std::move(owner));
    }
    if (Match(TokenType::TOKEN_TYPE_AND)) {
        auto borrow = AstNode::New(AstNodeType::BORROW);
        borrow->AddNode(std::move(base));
        return BuildType(std::move(borrow));
    }
    if (Match(TokenType::TOKEN_TYPE_QUESTION)) {
        auto borrow = AstNode::New(AstNodeType::OPTIONAL);
        borrow->AddNode(std::move(base));
        return BuildType(std::move(borrow));
    }
    if (Match(TokenType::TOKEN_TYPE_LEFT_BRACKET)) {
        Consume(TokenType::TOKEN_TYPE_RIGHT_BRACKET, "Expected closing ']'");
        auto dynamic_array = AstNode::New(AstNodeType::ARRAY);
        dynamic_array->AddNode(std::move(base));
        return BuildType(std::move(dynamic_array));
    }
    return base;
}

std::unique_ptr<AstNode> Parser::NodeFromType(const Token& token) {
    switch (token.type) {
        case TokenType::TOKEN_TYPE_I8:
            return AstNode::New(AstNodeType::I8);
        case TokenType::TOKEN_TYPE_I16:
            return AstNode::New(AstNodeType::I16);
        case TokenType::TOKEN_TYPE_I32:
            return AstNode::New(AstNodeType::I32);
        case TokenType::TOKEN_TYPE_I64:
            return AstNode::New(AstNodeType::I64);
        case TokenType::TOKEN_TYPE_U8:
            return AstNode::New(AstNodeType::U8);
        case TokenType::TOKEN_TYPE_U16:
            return AstNode::New(AstNodeType::U16);
        case TokenType::TOKEN_TYPE_U32:
            return AstNode::New(AstNodeType::U32);
        case TokenType::TOKEN_TYPE_U64:
            return AstNode::New(AstNodeType::U64);
        case TokenType::TOKEN_TYPE_F32:
            return AstNode::New(AstNodeType::F32);
        case TokenType::TOKEN_TYPE_F64:
            return AstNode::New(AstNodeType::F64);
        case TokenType::TOKEN_TYPE_BOOL:
            return AstNode::New(AstNodeType::BOOL);
        default: {
            Error(token, "Unsupported type");
            return nullptr;
        }
    }
}

std::unique_ptr<AstNode> Parser::Declaration() {
    auto type_token = previous_;
    auto base_node = NodeFromType(type_token);
    auto type_node = BuildType(std::move(base_node));

    Consume(TokenType::TOKEN_TYPE_IDENTIFIER, "Expected name after definition");
    auto name = AstNode::New(AstNodeType::NAME, previous_);

    if (Match(TokenType::TOKEN_TYPE_LEFT_PAREN)) {
        auto function = AstNode::New(AstNodeType::FUNCTION);
        function->AddNode(std::move(name));
        function->AddNode(std::move(type_node));
        do {
            if (!MatchType())
                break;
            function->AddNode(std::move(Declaration()));
        }
        while (Match(TokenType::TOKEN_TYPE_COMMA));
        Consume(TokenType::TOKEN_TYPE_RIGHT_PAREN, "Expected ')'");

        return function;
    }

    auto variable = AstNode::New(AstNodeType::VARIABLE);
    variable->AddNode(std::move(name));
    variable->AddNode(std::move(type_node));
    return variable;
}

std::unique_ptr<AstNode> Parser::Block() {
    auto sequence = AstNode::New(AstNodeType::SEQUENCE);
    while (!Match(TokenType::TOKEN_TYPE_RIGHT_BRACE)) {
        sequence->AddNode(Statement());
    }
    return sequence;
}

std::unique_ptr<AstNode> Parser::DeclarationStatement() {
    auto definition = Declaration();
    if (definition->type == AstNodeType::VARIABLE) {
        Consume(TokenType::TOKEN_TYPE_SEMICOLON, "Expected ';'");
    } else if (definition->type == AstNodeType::FUNCTION) {
        definition->AddNode(Statement());
    }
    return definition;
}

std::vector<std::unique_ptr<AstNode> > Parser::Parse() {
    std::vector<std::unique_ptr<AstNode> > nodes = {};
    while (!Match(TokenType::TOKEN_TYPE_EOF)) {
        nodes.push_back(Statement());
    }
    return nodes;
}

void Parser::Error(const Token& token, const std::string& message) {
    std::cerr << message << " at line: " << token.line << " at: " << token.value << std::endl;
    throw std::exception();
}

Token Parser::Advance() {
    previous_ = current_;
    while (true) {
        current_ = lexer_.Next();
        if (current_.type != TokenType::TOKEN_TYPE_ERROR)
            break;

        Error(current_, "Unexpected error");
        break;
    }
    std::cout << TokenTypeToString(current_.type) << std::endl;
    return current_;
}

Token Parser::Peek() {
    return current_;
}

bool Parser::Check(TokenType type) const {
    return current_.type == type;
}

bool Parser::CheckType() const {
    if (Check(TokenType::TOKEN_TYPE_I8)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_I16)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_I32)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_I64)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_U8)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_U16)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_U32)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_U64)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_F32)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_F64)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_BOOL)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_STRING)) {
        return true;
    }
    if (Check(TokenType::TOKEN_TYPE_IDENTIFIER)) {
        return true;
    }
    return false;
}

bool Parser::MatchType() {
    if (CheckType()) {
        Advance();
        return true;
    }
    return false;
}

bool Parser::Match(TokenType type) {
    if (Check(type)) {
        Advance();
        return true;
    }
    return false;
}

void Parser::Consume(TokenType type, const std::string& error) {
    if (Check(type)) {
        Advance();
        return;
    }
    Error(current_, error);
}
