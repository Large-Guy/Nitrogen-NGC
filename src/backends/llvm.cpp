#include "llvm.h"

#include <iostream>

void LlvmBackend::Generate(std::vector<std::unique_ptr<AstNode>> nodes) {
    context_ = std::make_unique<LLVMContext>();
    builder_ = std::make_unique<IRBuilder<>>(*context_);

    for (const auto& ptr : nodes) {
        AstNode& node = *ptr.get();
        if (node.type == AstNodeType::MODULE) {
            const auto name = node[0];
            if (module_ != nullptr) {
                std::cout << "Module already generated" << std::endl;
            }
            module_ = std::make_unique<Module>(name->token->value, *context_);
            std::cout << "Module generated" << std::endl;
        }
        if (node.type == AstNodeType::FUNCTION) {
            const auto name = node[0];
            const auto return_type = node[1];
            std::vector<AstNode*> args;
            for (auto i = 2; i < node.ChildrenCount() - 1; ++i) {
                args.push_back(node[i]);
            }
            const auto body = node[node.ChildrenCount() - 1];
            std::vector<Type*> ir_args(args.size());
        }
    }
}

