#pragma once
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
namespace retro {
struct CartridgeImage {
    std::uint8_t version = 1;
    std::uint8_t flags = 0;
    std::uint16_t loadAddress = 0x8000;
    std::uint16_t entryPoint = 0x8000;
    std::string name;
    std::vector<std::uint8_t> payload;
};
class Cartridge {
public:
    static constexpr std::size_t headerSize = 48;
    static bool load(const std::filesystem::path& path, CartridgeImage& image, std::string& error);
    static bool save(const std::filesystem::path& path, const CartridgeImage& image, std::string& error);
    static std::vector<std::filesystem::path> list(const std::filesystem::path& directory);
};
}