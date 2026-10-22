#pragma once
#include "basic2026/core.hpp"
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace basic2026 {
class Object {
public:
    virtual ~Object() = default;
    virtual std::string className() const = 0;
};
using ObjectPtr = std::shared_ptr<Object>;

class ClassRegistry {
public:
    using Factory = std::function<ObjectPtr()>;
    void registerClass(std::string name, Factory factory);
    ObjectPtr create(std::string_view name) const;
    bool contains(std::string_view name) const;
private:
    std::unordered_map<std::string, Factory> factories_;
};

class BasicModule : public Object {
public:
    explicit BasicModule(std::string name);
    std::string className() const override;
    const std::string& name() const noexcept;
    void setProperty(std::string name, Value value);
    Value property(std::string_view name) const;
private:
    std::string name_;
    Environment properties_;
};
}
