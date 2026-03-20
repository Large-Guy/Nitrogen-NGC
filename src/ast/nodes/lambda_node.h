#ifndef NGC_LAMBDA_NODE_H
#define NGC_LAMBDA_NODE_H
#include <memory>
#include <vector>

#include "definition_node.h"
#include "expression_node.h"
#include "type_node.h"


class LambdaNode : public ExpressionNode {
public:
    LambdaNode(std::unique_ptr<TypeNode> return_type, std::vector<std::unique_ptr<DefinitionNode> > args,
               std::unique_ptr<StatementNode> body);

    std::unique_ptr<AstNode> Clone() const override;

    std::unique_ptr<TypeNode> return_type;
    std::vector<std::unique_ptr<DefinitionNode> > args;
    std::unique_ptr<StatementNode> body;
};


#endif //NGC_LAMBDA_NODE_H
