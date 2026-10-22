#pragma once
#include "os/filesystem.hpp"
#include <string>
namespace retro {
class Shell {
public:
    Shell(FileSystem& driveA, FileSystem& driveB, std::string cartridgeDirectory);
    void run();
private:
    FileSystem& driveA_;
    FileSystem& driveB_;
    std::string cartridgeDirectory_;
    bool handle(const std::string& line);
    void printHelp() const;
};
}