#include "cpm/console.hpp"

int main() {
    cpm::Console console("drives/A", "drives/B", "cartridges");
    return console.run();
}
