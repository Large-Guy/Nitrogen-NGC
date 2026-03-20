#include "compound_statement.h"

#include "../../memory_utils.h"

CompoundStatement::CompoundStatement(std::vector<std::unique_ptr<StatementNode> > statements) : statements(
    std::move(statements)) {
}

std::unique_ptr<AstNode> CompoundStatement::Clone() const {
    std::vector<std::unique_ptr<StatementNode> > ops(statements.size());
    for (const auto& statement: statements) {
        ops.push_back(UniqueCast<StatementNode>(statement->Clone()));
    }
    return std::make_unique<CompoundStatement>(std::move(ops));
}
