#ifndef NGC_SCOPE_H
#define NGC_SCOPE_H
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../ast/nodes/type_node.h"

template <typename Value>
class Scope {
public:
    void PushScope() {
        stack.push_back({});
    }
    void PopScope() {
        stack.pop_back();
    }

    void Declare(const std::string& name, Value* value, std::unique_ptr<TypeNode> type) {
        stack.back()[name] = std::pair(value, std::move(type));
    }

    Value* Lookup(const std::string& name) {
        for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return found->second.first;
            }
        }
        return nullptr;
    }

    TypeNode* Type(const std::string& name) {
        for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return found->second.second.get();
            }
        }
        return nullptr;
    }
private:

    std::vector<std::unordered_map<std::string, std::pair<Value*, std::unique_ptr<TypeNode>>>> stack;
};


#endif //NGC_SCOPE_H