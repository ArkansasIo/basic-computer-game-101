#pragma once
#include <functional>
#include <memory>
#include <string>

namespace retro::math {
enum class NodeKind { Constant, Variable, Add, Subtract, Multiply, Divide, Power, Negate, Sin, Cos, Exp, Log };
struct ExprNode {
    NodeKind kind;
    double value = 0.0;
    std::string name;
    std::shared_ptr<ExprNode> left;
    std::shared_ptr<ExprNode> right;
};
class Expression {
public:
    Expression();
    explicit Expression(double constant);
    static Expression variable(std::string name);
    static Expression parse(const std::string& source);
    std::string str() const;
    double evaluate(const std::function<double(const std::string&)>& variableLookup) const;
    Expression simplify() const;
    Expression differentiate(const std::string& variable) const;
    double integrateNumeric(const std::string& variable, double a, double b, std::size_t intervals,
                            const std::function<double(const std::string&, double)>& variableSetter) const;
    const std::shared_ptr<ExprNode>& root() const { return root_; }
private:
    explicit Expression(std::shared_ptr<ExprNode> node);
    std::shared_ptr<ExprNode> root_;
};
}