#ifndef NGC_MEMORY_UTILS_H
#define NGC_MEMORY_UTILS_H
#include <memory>
#include "string"


template<typename T, typename U>
static std::unique_ptr<T> UniqueCast(std::unique_ptr<U>& from) {
    if (auto casted = dynamic_cast<T*>(from.get())) {
        from.release();
        return std::unique_ptr<T>(casted);
    }
    return nullptr;
}

template<typename T, typename U>
static std::unique_ptr<T> UniqueCast(std::unique_ptr<U>&& from) {
    if (auto casted = dynamic_cast<T*>(from.get())) {
        from.release();
        return std::unique_ptr<T>(casted);
    }
    return nullptr;
}

#endif //NGC_MEMORY_UTILS_H
