#include "parser.h"

#include <functional>
#include <iostream>
#include <unordered_set>

Parser::Parser(Lexer lexer) : lexer_(std::move(lexer)) {
    if (rules_.empty())
        rules_ = BuildRules();
    Advance();
}

std::unordered_map<TokenType, Parser::ParseRule> Parser::rules_ = {};

class Expressions {
public:
    // Special expression statement
    static std::unique_ptr<AstNode> AppendAssign(Parser& parser, std::unique_ptr<AstNode> target) {
        auto assignment = parser.current_;
        switch (assignment.type) {
            case TokenType::TOKEN_TYPE_EQUAL: {
                parser.Advance();
                auto assign = AstNode::New(AstNodeType::ASSIGN);
                assign->AddNode(std::move(target));
                assign->AddNode(std::move(parser.Expression()));
                return assign;
            }
            default: {
                return target;
            }
        }
    }
    
    static std::unique_ptr<AstNode> Number(Parser& parser, bool canAssign) {
        auto num = parser.previous_;
        if (parser.previous_.type == TokenType::TOKEN_TYPE_FLOAT) {
            return AstNode::New(AstNodeType::FLOAT, num);
        }
        return AstNode::New(AstNodeType::INT, num);
    }

    static std::unique_ptr<AstNode> String(Parser& parser, bool canAssign) {
    }

    static std::unique_ptr<AstNode> Group(Parser& parser, bool canAssign) {
        auto node = parser.Expression();
        if (parser.Match(TokenType::TOKEN_TYPE_COMMA)) {
            auto list = AstNode::New(AstNodeType::LIST);
            list->AddNode(std::move(node));
            do {
                auto next = parser.Expression();
                list->AddNode(std::move(next));
            } while (parser.Match(TokenType::TOKEN_TYPE_COMMA));

            node = std::move(list);
        }

        parser.Consume(TokenType::TOKEN_TYPE_RIGHT_PAREN, "Expected closing ')'");

        return node;
    }

    static std::unique_ptr<AstNode> Unary(Parser& parser, bool canAssign) {
        auto unary = parser.previous_;
        auto operand = parser.ParsePrecedence(Parser::Precedence::UNARY);
        switch (unary.type) {
            case TokenType::TOKEN_TYPE_MINUS: {
                auto negate = AstNode::New(AstNodeType::NEGATE);
                negate->AddNode(std::move(operand));
                return negate;
            }
            case TokenType::TOKEN_TYPE_AND: {
                auto address = AstNode::New(AstNodeType::ADDRESS);
                address->AddNode(std::move(operand));
                return address;
            }
            case TokenType::TOKEN_TYPE_STAR: {
                auto negate = AstNode::New(AstNodeType::LOCK);
                negate->AddNode(std::move(operand));
                return negate;
            }
            case TokenType::TOKEN_TYPE_BANG: {
                auto negate = AstNode::New(AstNodeType::NOT);
                negate->AddNode(std::move(operand));
                return negate;
            }
                
            default: {
                parser.Error(unary, "Unsupported unary expression type");
                return nullptr;
            }
        }
    }

    static std::unique_ptr<AstNode> Cast(Parser& parser, bool canAssign) {
    }

    static std::unique_ptr<AstNode> Heap(Parser& parser, bool canAssign) {
        auto node = AstNode::New(AstNodeType::HEAP);
        auto value = parser.Expression();
        parser.Consume(TokenType::TOKEN_TYPE_RIGHT_BRACKET, "Expected closing ']'");
        node->AddNode(std::move(value));
        return node;
    }

    static std::unique_ptr<AstNode> Variable(Parser& parser, bool canAssign) {
        auto variable = parser.previous_;
        auto variable_name = AstNode::New(AstNodeType::NAME, variable);
        if (canAssign) {
            //TODO: assign
            return AppendAssign(parser, std::move(variable_name));
        }
        return variable_name;
    }

    static std::unique_ptr<AstNode> Literal(Parser& parser, bool canAssign) {
        auto token = parser.previous_;
        switch (token.type) {
            case TokenType::TOKEN_TYPE_NULL:
                return AstNode::New(AstNodeType::_NULL);
            case TokenType::TOKEN_TYPE_TRUE:
                return AstNode::New(AstNodeType::TRUE);
            case TokenType::TOKEN_TYPE_FALSE:
                return AstNode::New(AstNodeType::FALSE);
            default: {
                parser.Error(token, "Unsupported literal type");
                return nullptr;
            }
        }
    }

    static std::unique_ptr<AstNode> Binary(Parser& parser, std::unique_ptr<AstNode> left, bool canAssign) {
        auto op = parser.previous_;

        auto rule = parser.Rule(op.type);

        auto right = parser.ParsePrecedence(static_cast<Parser::Precedence>(static_cast<int>(rule.precedence) + 1));

        std::unique_ptr<AstNode> node = nullptr;

        switch (op.type) {
            case TokenType::TOKEN_TYPE_PLUS: {
                node = AstNode::New(AstNodeType::ADD);
                break;
            }
            case TokenType::TOKEN_TYPE_MINUS: {
                node = AstNode::New(AstNodeType::SUBTRACT);
                break;
            }
            case TokenType::TOKEN_TYPE_STAR: {
                node = AstNode::New(AstNodeType::MULTIPLY);
                break;
            }
            case TokenType::TOKEN_TYPE_SLASH: {
                node = AstNode::New(AstNodeType::DIVIDE);
                break;
            }
            case TokenType::TOKEN_TYPE_STAR_STAR: {
                node = AstNode::New(AstNodeType::EXPONENT);
                break;
            }
            case TokenType::TOKEN_TYPE_PERCENT: {
                node = AstNode::New(AstNodeType::MODULO);
                break;
            }
            case TokenType::TOKEN_TYPE_PIPE: {
                node = AstNode::New(AstNodeType::BITWISE_OR);
                break;
            }
            case TokenType::TOKEN_TYPE_CARET: {
                node = AstNode::New(AstNodeType::BITWISE_XOR);
                break;
            }
            case TokenType::TOKEN_TYPE_AND: {
                node = AstNode::New(AstNodeType::BITWISE_AND);
                break;
            }
            case TokenType::TOKEN_TYPE_GREATER: {
                node = AstNode::New(AstNodeType::GREATER);
                break;
            }
            case TokenType::TOKEN_TYPE_GREATER_EQUAL: {
                node = AstNode::New(AstNodeType::GREATER_EQUAL);
                break;
            }
            case TokenType::TOKEN_TYPE_LESS: {
                node = AstNode::New(AstNodeType::LESS);
                break;
            }
            case TokenType::TOKEN_TYPE_LESS_EQUAL: {
                node = AstNode::New(AstNodeType::LESS_EQUAL);
                break;
            }
            case TokenType::TOKEN_TYPE_GREATER_GREATER: {
                node = AstNode::New(AstNodeType::SHIFT_RIGHT);
                break;
            }
            case TokenType::TOKEN_TYPE_LESS_LESS: {
                node = AstNode::New(AstNodeType::SHIFT_LEFT);
                break;
            }
            case TokenType::TOKEN_TYPE_AND_AND: {
                node = AstNode::New(AstNodeType::AND);
                break;
            }
            case TokenType::TOKEN_TYPE_PIPE_PIPE: {
                node = AstNode::New(AstNodeType::OR);
                break;
            }
            default: {
                parser.Error(op, "Unsupported binary expression type");
                return nullptr;
            }
        }

        node->AddNode(std::move(left));
        node->AddNode(std::move(right));

        return node;
    }

    static std::unique_ptr<AstNode> Interval(Parser& parser, std::unique_ptr<AstNode> left, bool canAssign) {
    }

    static std::unique_ptr<AstNode> Include(Parser& parser, std::unique_ptr<AstNode> left, bool canAssign) {
    }

    static std::unique_ptr<AstNode> Call(Parser& parser, std::unique_ptr<AstNode> left, bool canAssign) {
    }

    static std::unique_ptr<AstNode> Index(Parser& parser, std::unique_ptr<AstNode> left, bool canAssign) {
    }

    static std::unique_ptr<AstNode> Field(Parser& parser, std::unique_ptr<AstNode> left, bool canAssign) {
    }

    static std::unique_ptr<AstNode> Ternary(Parser& parser, std::unique_ptr<AstNode> left, bool canAssign) {
    }
};

std::unordered_map<TokenType, Parser::ParseRule> Parser::BuildRules() {
    std::unordered_map<TokenType, ParseRule> rules{};
    rules[TokenType::TOKEN_TYPE_LEFT_PAREN] = {Expressions::Group, Expressions::Call, Precedence::CALL};
    rules[TokenType::TOKEN_TYPE_LEFT_BRACKET] = {Expressions::Heap, Expressions::Index, Precedence::CALL};
    rules[TokenType::TOKEN_TYPE_DOT] = {nullptr, Expressions::Field, Precedence::CALL};
    rules[TokenType::TOKEN_TYPE_PLUS] = {nullptr, Expressions::Binary, Precedence::TERM};
    rules[TokenType::TOKEN_TYPE_MINUS] = {Expressions::Unary, Expressions::Binary, Precedence::TERM};
    rules[TokenType::TOKEN_TYPE_STAR] = {Expressions::Unary, Expressions::Binary, Precedence::FACTOR};
    rules[TokenType::TOKEN_TYPE_STAR_STAR] = {nullptr, Expressions::Binary, Precedence::EXPONENT};
    rules[TokenType::TOKEN_TYPE_STAR_EQUAL] = {nullptr, nullptr, Precedence::NONE};
    rules[TokenType::TOKEN_TYPE_SLASH] = {nullptr, Expressions::Binary, Precedence::FACTOR};
    rules[TokenType::TOKEN_TYPE_BANG] = {Expressions::Unary, nullptr, Precedence::TERM};
    rules[TokenType::TOKEN_TYPE_BANG_EQUAL] = {nullptr, Expressions::Binary, Precedence::EQUALITY};
    rules[TokenType::TOKEN_TYPE_EQUAL_EQUAL] = {nullptr, Expressions::Binary, Precedence::EQUALITY};
    rules[TokenType::TOKEN_TYPE_GREATER] = {nullptr, Expressions::Binary, Precedence::COMPARISON};
    rules[TokenType::TOKEN_TYPE_GREATER_GREATER] = {nullptr, Expressions::Binary, Precedence::SHIFT};
    rules[TokenType::TOKEN_TYPE_GREATER_EQUAL] = {nullptr, Expressions::Binary, Precedence::COMPARISON};
    rules[TokenType::TOKEN_TYPE_LESS] = {nullptr, Expressions::Binary, Precedence::COMPARISON};
    rules[TokenType::TOKEN_TYPE_LESS_LESS] = {nullptr, Expressions::Binary, Precedence::SHIFT};
    rules[TokenType::TOKEN_TYPE_LESS_EQUAL] = {nullptr, Expressions::Binary, Precedence::COMPARISON};
    rules[TokenType::TOKEN_TYPE_AND] = {Expressions::Unary, Expressions::Binary, Precedence::BITWISE_AND};
    rules[TokenType::TOKEN_TYPE_AND_AND] = {nullptr, Expressions::Binary, Precedence::AND};
    rules[TokenType::TOKEN_TYPE_PIPE] = {nullptr, Expressions::Binary, Precedence::BITWISE_OR};
    rules[TokenType::TOKEN_TYPE_PIPE_PIPE] = {nullptr, Expressions::Binary, Precedence::OR};
    rules[TokenType::TOKEN_TYPE_PERCENT] = {nullptr, Expressions::Binary, Precedence::MODULO};
    rules[TokenType::TOKEN_TYPE_CARET] = {nullptr, Expressions::Binary, Precedence::BITWISE_XOR};
    rules[TokenType::TOKEN_TYPE_TILDE] = {Expressions::Unary, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_STRING_LITERAL] = {Expressions::String, nullptr, Precedence::NONE};
    rules[TokenType::TOKEN_TYPE_CHARACTER] = {Expressions::Number, nullptr, Precedence::NONE};
    rules[TokenType::TOKEN_TYPE_INTEGER] = {Expressions::Number, nullptr, Precedence::NONE};
    rules[TokenType::TOKEN_TYPE_FLOAT] = {Expressions::Number, nullptr, Precedence::NONE};
    rules[TokenType::TOKEN_TYPE_IDENTIFIER] = {Expressions::Variable, nullptr, Precedence::NONE};
    rules[TokenType::TOKEN_TYPE_I8] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_I16] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_I32] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_I64] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_U8] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_U16] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_U32] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_U64] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_F32] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_F64] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_ISIZE] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_USIZE] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_BOOL] = {Expressions::Cast, nullptr, Precedence::UNARY};
    rules[TokenType::TOKEN_TYPE_NULL] = {Expressions::Literal, nullptr, Precedence::NONE};
    rules[TokenType::TOKEN_TYPE_TRUE] = {Expressions::Literal, nullptr, Precedence::NONE};
    rules[TokenType::TOKEN_TYPE_FALSE] = {Expressions::Literal, nullptr, Precedence::NONE};
    rules[TokenType::TOKEN_TYPE_IN] = {nullptr, Expressions::Include, Precedence::CALL};
    rules[TokenType::TOKEN_TYPE_QUESTION] = {nullptr, Expressions::Ternary, Precedence::TERNARY};
    return rules;
};

std::vector<std::unique_ptr<AstNode> > Parser::Parse() {
    std::vector<std::unique_ptr<AstNode> > nodes = {};
    while (!Match(TokenType::TOKEN_TYPE_EOF)) {
        nodes.push_back(Statement());
    }
    return nodes;
}

Parser::ParseRule Parser::Rule(const TokenType& type) {
    if (rules_.find(type) != rules_.end()) {
        return rules_[type];
    }
    return {};
}

std::unique_ptr<AstNode> Parser::ParsePrecedence(Precedence precedence) {
    Advance();
    auto prefix = Rule(previous_.type).prefix;
    if (prefix == nullptr) {
        Error(previous_, "Expected type");
        return nullptr;
    }

    bool can_assign = precedence <= Precedence::ASSIGNMENT;
    auto result = prefix(*this, can_assign);

    while (precedence <= Rule(current_.type).precedence) {
        Advance();
        auto infix = Rule(previous_.type).infix;
        if (infix == nullptr) {
            Error(previous_, "Expected type");
        }
        result = infix(*this, std::move(result), can_assign);
    }

    return result;
}

std::unique_ptr<AstNode> Parser::Expression(Precedence start) {
    return ParsePrecedence(start);
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

std::unique_ptr<AstNode> Parser::BuildType(std::unique_ptr<AstNode> base) {
    if (Match(TokenType::TOKEN_TYPE_LESS)) {
        auto size = Expression(Precedence::SHIFT);
        Consume(TokenType::TOKEN_TYPE_GREATER, "Expected closing '>'");
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
    if (Match(TokenType::TOKEN_TYPE_RETURN)) {
        return ReturnStatement();
    }
    return ExpressionStatement();
}

std::unique_ptr<AstNode> Parser::ReturnStatement() {
    auto ret = AstNode::New(AstNodeType::RETURN);
    if (Match(TokenType::TOKEN_TYPE_SEMICOLON)) {
        return ret;
    }
    auto what = Expression();
    ret->AddNode(std::move(what));
    Consume(TokenType::TOKEN_TYPE_SEMICOLON, "Expected semicolon");
    return ret;
}

std::unique_ptr<AstNode> Parser::ExpressionStatement() {
    auto expression = Expression();
    Consume(TokenType::TOKEN_TYPE_SEMICOLON, "Expected semicolon");
    return expression;
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
        if (Match(TokenType::TOKEN_TYPE_EQUAL)) {
            definition->AddNode(std::move(Expression()));
        }
        Consume(TokenType::TOKEN_TYPE_SEMICOLON, "Expected ';'");
    } else if (definition->type == AstNodeType::FUNCTION) {
        definition->AddNode(Statement());
    }
    return definition;
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
    return false;
}

bool Parser::Match(TokenType type) {
    if (Check(type)) {
        Advance();
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

void Parser::Consume(TokenType type, const std::string& error) {
    if (Check(type)) {
        Advance();
        return;
    }
    Error(current_, error);
}
