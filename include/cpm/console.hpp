#pragma once

#include "basic/interpreter.hpp"

#include <filesystem>
#include <string>

namespace cpm {

class Console {
public:
    Console(std::filesystem::path driveA,
            std::filesystem::path driveB,
            std::filesystem::path cartridges);

    int run();

private:
    std::filesystem::path driveA_;
    std::filesystem::path driveB_;
    std::filesystem::path cartridges_;
    char currentDrive_ = 'A';
    int user_ = 0;
    bool running_ = true;
    retro::BasicInterpreter basic_;

    std::filesystem::path currentPath() const;
    void printBanner() const;
    void prompt() const;
    void executeCommand(const std::string& command);
    void commandDir(const std::string& argument) const;
    void commandType(const std::string& argument) const;
    void commandEra(const std::string& argument) const;
    void commandRen(const std::string& argument) const;
    void commandDrive(const std::string& command);
    void commandBasic();
    void commandHelp() const;
    static std::string trim(std::string value);
    static std::string upper(std::string value);
};

} // namespace cpm
