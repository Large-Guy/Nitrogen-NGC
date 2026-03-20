//
// Created by Ravi Lebgue on 3/18/26.
//

#include "lambda_node.h"

#include "../../memory_utils.h"

LambdaNode::LambdaNode(std::unique_ptr<TypeNode> return_type, std::vector<std::unique_ptr<DefinitionNode> > args,
                       std::unique_ptr<StatementNode> body) : ExpressionNode(), return_type(std::move(return_type)),
                                                              args(std::move(args)), body(std::move(body)) {
}

std::unique_ptr<AstNode> LambdaNode::Clone() const {
    std::vector<std::unique_ptr<DefinitionNode> > arguments(args.capacity());
    for (auto& arg: args) {
        arguments.push_back(UniqueCast<DefinitionNode>(arg->Clone()));
    }

    return std::make_unique<LambdaNode>(UniqueCast<TypeNode>(return_type->Clone()), std::move(arguments),
                                        UniqueCast<StatementNode>(body->Clone()));
}
