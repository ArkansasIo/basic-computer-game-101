#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace retro::generator {
enum class ProgramKind { Arithmetic, Algebra, Calculus, GuessingGame, NumberSequence, DiceGame, RandomWalk };

struct GeneratedProgram {
    std::string name;
    std::string description;
    std::string basicSource;
};

class ProgramGenerator {
public:
    explicit ProgramGenerator(std::uint32_t seed = 0xB451C);
    GeneratedProgram generate(ProgramKind kind, std::size_t complexity = 3);
    std::vector<GeneratedProgram> generatePack(std::size_t count, std::size_t complexity = 3);
    static std::string kindName(ProgramKind kind);
private:
    std::uint32_t state_;
    std::uint32_t next();
    int pick(int min, int max);
    GeneratedProgram arithmetic(std::size_t complexity);
    GeneratedProgram algebra(std::size_t complexity);
    GeneratedProgram calculus(std::size_t complexity);
    GeneratedProgram guessing(std::size_t complexity);
    GeneratedProgram sequence(std::size_t complexity);
    GeneratedProgram dice(std::size_t complexity);
    GeneratedProgram randomWalk(std::size_t complexity);
};
}