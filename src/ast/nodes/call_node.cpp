#include "call_node.h"

#include "../../memory_utils.h"

CallNode::CallNode(std::unique_ptr<ExpressionNode> callable,
                   std::vector<std::unique_ptr<ExpressionNode> > args) : callable(std::move(callable)),
                                                                         args(std::move(args)) {
}

std::unique_ptr<AstNode> CallNode::Clone() const {
    std::vector<std::unique_ptr<ExpressionNode> > arguments(args.capacity());
    for (const auto& arg: args) {
        arguments.push_back(UniqueCast<ExpressionNode>(arg->Clone()));
    }
    return std::make_unique<CallNode>(UniqueCast<ExpressionNode>(callable->Clone()), std::move(arguments));
}
