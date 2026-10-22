#include "cpm/console.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace cpm {

Console::Console(std::filesystem::path driveA,
                 std::filesystem::path driveB,
                 std::filesystem::path cartridges)
    : driveA_(std::move(driveA)),
      driveB_(std::move(driveB)),
      cartridges_(std::move(cartridges)),
      basic_([](const std::string& text) { std::cout << text; }, [] {
          std::string input;
          std::getline(std::cin, input);
          return input;
      }) {}

std::filesystem::path Console::currentPath() const {
    return currentDrive_ == 'A' ? driveA_ : driveB_;
}

std::string Console::trim(std::string value) {
    auto notSpace = [](unsigned char c) { return !std::isspace(c); };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), notSpace));
    value.erase(std::find_if(value.rbegin(), value.rend(), notSpace).base(), value.end());
    return value;
}

std::string Console::upper(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });
    return value;
}

void Console::printBanner() const {
    std::cout << "\nCP/M BASIC CONSOLE 2.0\n"
              << "8080-compatible console environment | 64K logical memory\n"
              << "BASIC computer language | drives A: and B:\n"
              << "Type HELP for commands. Type BASIC to enter BASIC.\n\n";
}

void Console::prompt() const {
    std::cout << currentDrive_ << ":>" << std::flush;
}

void Console::commandDir(const std::string& argument) const {
    (void)argument;
    std::cout << "Directory of " << currentDrive_ << ":\n";
    std::size_t count = 0;
    if (!std::filesystem::exists(currentPath())) return;
    for (const auto& entry : std::filesystem::directory_iterator(currentPath())) {
        if (!entry.is_regular_file()) continue;
        const auto size = std::filesystem::file_size(entry.path());
        std::cout << "  " << entry.path().filename().string() << "\t" << size << " bytes\n";
        ++count;
    }
    std::cout << count << " file(s)\n";
}

void Console::commandType(const std::string& argument) const {
    const auto name = trim(argument);
    if (name.empty()) {
        std::cout << "Usage: TYPE filename\n";
        return;
    }
    const auto path = currentPath() / name;
    std::ifstream input(path);
    if (!input) {
        std::cout << "File not found: " << name << "\n";
        return;
    }
    std::cout << input.rdbuf();
    std::cout << '\n';
}

void Console::commandEra(const std::string& argument) const {
    const auto name = trim(argument);
    if (name.empty()) {
        std::cout << "Usage: ERA filename\n";
        return;
    }
    const auto path = currentPath() / name;
    if (std::filesystem::remove(path)) std::cout << "Deleted " << name << "\n";
    else std::cout << "File not found: " << name << "\n";
}

void Console::commandRen(const std::string& argument) const {
    std::istringstream stream(argument);
    std::string oldName;
    std::string newName;
    stream >> oldName >> newName;
    if (oldName.empty() || newName.empty()) {
        std::cout << "Usage: REN newname=oldname\n";
        return;
    }
    if (oldName.find('=') != std::string::npos) {
        const auto equal = oldName.find('=');
        newName = oldName.substr(0, equal);
        oldName = oldName.substr(equal + 1);
    }
    std::error_code error;
    std::filesystem::rename(currentPath() / oldName, currentPath() / newName, error);
    if (error) std::cout << "Rename failed: " << error.message() << "\n";
    else std::cout << "Renamed " << oldName << " to " << newName << "\n";
}

void Console::commandDrive(const std::string& command) {
    if (command.size() >= 2 && command[1] == ':') {
        const char drive = static_cast<char>(std::toupper(static_cast<unsigned char>(command[0])));
        if (drive == 'A' || drive == 'B') {
            currentDrive_ = drive;
            std::cout << currentDrive_ << ":\n";
        } else {
            std::cout << "Invalid drive. Use A: or B:\n";
        }
    }
}

void Console::commandBasic() {
    std::cout << "BASIC 2026 compatibility console. Type EXIT to return to CP/M.\n";
    basic_.repl();
}

void Console::commandHelp() const {
    std::cout << "CP/M commands:\n"
              << "  DIR             List files\n"
              << "  TYPE FILE       Display a text file\n"
              << "  ERA FILE        Delete a file\n"
              << "  REN NEW=OLD     Rename a file\n"
              << "  A: / B:         Select drive\n"
              << "  USER n          Select CP/M user area\n"
              << "  BASIC           Enter BASIC\n"
              << "  HELP            Show this help\n"
              << "  VER             Display version\n"
              << "  EXIT            Exit emulator\n";
}

void Console::executeCommand(const std::string& command) {
    const auto line = trim(command);
    if (line.empty()) return;
    const auto space = line.find_first_of(" \t");
    const auto verb = upper(line.substr(0, space));
    const auto argument = space == std::string::npos ? std::string{} : trim(line.substr(space + 1));

    if (line.size() == 2 && line[1] == ':') { commandDrive(line); return; }
    if (verb == "DIR") commandDir(argument);
    else if (verb == "TYPE") commandType(argument);
    else if (verb == "ERA" || verb == "ERASE") commandEra(argument);
    else if (verb == "REN" || verb == "RENAME") commandRen(argument);
    else if (verb == "BASIC") commandBasic();
    else if (verb == "HELP" || verb == "?") commandHelp();
    else if (verb == "VER") std::cout << "CP/M BASIC Console 2.0 / BASIC 2026 / C++23\n";
    else if (verb == "USER") {
        try { user_ = std::stoi(argument); std::cout << "User " << user_ << "\n"; }
        catch (...) { std::cout << "Invalid user number.\n"; }
    } else if (verb == "EXIT" || verb == "QUIT") running_ = false;
    else std::cout << "Bad command or file name: " << line << "\n";
}

int Console::run() {
    std::filesystem::create_directories(driveA_);
    std::filesystem::create_directories(driveB_);
    std::filesystem::create_directories(cartridges_);
    printBanner();
    while (running_ && std::cin.good()) {
        prompt();
        std::string command;
        if (!std::getline(std::cin, command)) break;
        executeCommand(command);
    }
    std::cout << "\nCP/M halted.\n";
    return 0;
}

} // namespace cpm
