#pragma once
#include "basic2026/core.hpp"
#include <functional>
#include <random>
#include <string_view>
#include <unordered_map>

namespace basic2026 {
class StandardLibrary {
public:
    using Function = std::function<Value(const std::vector<Value>&)>;
    StandardLibrary();
    void registerFunction(std::string name, Function function);
    Value call(std::string_view name, const std::vector<Value>& args) const;
    static StandardLibrary createDefault();
private:
    std::unordered_map<std::string, Function> functions_;
};
}
