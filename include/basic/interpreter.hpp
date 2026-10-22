#pragma once
#include <functional>
#include <map>
#include <string>
#include <unordered_map>

namespace retro {
class BasicInterpreter {
public:
    using Output = std::function<void(const std::string&)>;
    using Input = std::function<std::string()>;
    BasicInterpreter(Output output, Input input);
    void repl();
    void executeImmediate(const std::string& line);
    void runProgram();
    void listProgram() const;
    void clearProgram();
private:
    Output output_;
    Input input_;
    std::map<int, std::string> lines_;
    std::unordered_map<std::string, long long> variables_;
    bool stopRequested_ = false;
    void executeStatement(const std::string& statement, int& nextLine, int currentLine);
    long long eval(const std::string& expression) const;
    std::string trim(std::string text) const;
    std::string upper(std::string text) const;
};
}