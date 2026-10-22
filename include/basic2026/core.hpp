#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace basic2026 {

using Integer = std::int64_t;
using LineNumber = std::uint32_t;

class Value {
public:
    enum class Type { Integer, Real, String, Boolean, None };
    Value() = default;
    explicit Value(Integer v) : type_(Type::Integer), integer_(v) {}
    explicit Value(double v) : type_(Type::Real), real_(v) {}
    explicit Value(std::string v) : type_(Type::String), string_(std::move(v)) {}
    explicit Value(bool v) : type_(Type::Boolean), boolean_(v) {}
    Type type() const noexcept { return type_; }
    Integer asInteger() const;
    double asReal() const;
    const std::string& asString() const;
    bool asBoolean() const;
    std::string toString() const;
private:
    Type type_{Type::None};
    Integer integer_{};
    double real_{};
    std::string string_{};
    bool boolean_{};
};

class Environment {
public:
    void set(std::string name, Value value);
    bool contains(std::string_view name) const;
    Value get(std::string_view name) const;
    void clear();
private:
    std::unordered_map<std::string, Value> values_;
};

class Statement {
public:
    virtual ~Statement() = default;
    virtual void execute(Environment&) = 0;
    virtual std::string debug() const = 0;
};

using StatementPtr = std::unique_ptr<Statement>;

class Program {
public:
    void add(LineNumber line, StatementPtr statement);
    void clear();
    void run(Environment& environment);
    const std::vector<std::pair<LineNumber, StatementPtr>>& statements() const noexcept { return statements_; }
private:
    std::vector<std::pair<LineNumber, StatementPtr>> statements_;
};

class Interpreter {
public:
    virtual ~Interpreter() = default;
    virtual Program parse(std::string_view source) const = 0;
    virtual void execute(Program&, Environment&) const = 0;
};

}
