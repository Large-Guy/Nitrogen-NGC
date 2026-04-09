#ifndef NGC_BACKEND_H
#define NGC_BACKEND_H
#include <vector>

#include "ast/ast_node.h"
#include "ast/nodes/function_node.h"


class Backend {
public:
    virtual ~Backend() = default;

    virtual void Generate(std::vector<std::unique_ptr<AstNode> > nodes) = 0;

    static int64_t EvaluateInt(ExpressionNode* unique);

    static size_t EvaluateSize(const TypeNode* type_node);

protected:
    template<typename T, typename U>
    static constexpr T* is(U* object) {
        return dynamic_cast<T*>(object);
    }
};


#endif //NGC_BACKEND_H
