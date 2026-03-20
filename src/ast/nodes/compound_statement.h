#ifndef NGC_COMPOUND_STATEMENT_H
#define NGC_COMPOUND_STATEMENT_H
#include <memory>
#include <vector>

#include "expression_node.h"


class CompoundStatement : public StatementNode {
public:
    CompoundStatement(std::vector<std::unique_ptr<StatementNode> > statements);

    std::unique_ptr<AstNode> Clone() const override;

    std::vector<std::unique_ptr<StatementNode> > statements;
};


#endif //NGC_COMPOUND_STATEMENT_H
