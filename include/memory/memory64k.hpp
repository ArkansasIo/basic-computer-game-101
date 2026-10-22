#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
namespace retro {
class Memory64K {
public:
    static constexpr std::size_t size = 65536;
    std::uint8_t read(std::uint16_t address) const noexcept { return bytes_[address]; }
    void write(std::uint16_t address, std::uint8_t value) noexcept { bytes_[address] = value; }
    void clear(std::uint8_t value = 0) noexcept { bytes_.fill(value); }
    const std::array<std::uint8_t, size>& bytes() const noexcept { return bytes_; }
    std::array<std::uint8_t, size>& bytes() noexcept { return bytes_; }
private:
    std::array<std::uint8_t, size> bytes_{};
};
}