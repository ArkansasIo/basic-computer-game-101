#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace retro {
class FileSystem {
public:
    explicit FileSystem(std::filesystem::path root);
    std::vector<std::string> list() const;
    bool readText(const std::string& name, std::string& content) const;
    bool writeText(const std::string& name, const std::string& content);
    bool erase(const std::string& name);
    bool renameFile(const std::string& from, const std::string& to);
    const std::filesystem::path& root() const noexcept { return root_; }
private:
    std::filesystem::path root_;
    std::filesystem::path safePath(const std::string& name) const;
};
}
