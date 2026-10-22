# 101 BASIC Computer

C++23 retro-computer project: line-numbered BASIC interpreter, CP/M-inspired disk command shell, 64 KiB memory abstraction, cartridge image format, algorithmic BASIC generator, and a host-side symbolic algebra/calculus engine.

This is an original implementation inspired by classic 8-bit computers; it does not bundle CP/M source code or copyrighted book listings.

## Build

```sh
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Windows: run `build.bat`, then `run.bat`.

At startup, choose `OS` or `BASIC`. The shell supports `HELP`, `DIR`, `TYPE`, `ERA`, `REN`, `SAVE`, `STAT`, `CARTLIST`, `CARTINFO`, `BASIC`, and `EXIT`.

## Generate new BASIC programs

`basicgen` creates new source programs from deterministic algorithmic templates.

```sh
./build/basicgen --kind algebra --count 5 --complexity 4 --out generated/algebra
./build/basicgen --kind calculus --out generated/calculus
./build/basicgen --kind pack --count 21 --complexity 5 --out generated/program
```

Available kinds: `pack`, `arithmetic`, `algebra`, `calculus`, `guess`, `sequence`, `dice`, `walk`.

## Algebra and calculus API

The host-side expression library parses algebraic expressions, simplifies them, evaluates them, produces symbolic derivatives, and approximates definite integrals using composite Simpson's rule.

```cpp
auto f = retro::math::Expression::parse("3*x^2 + 2*x + 1");
auto df = f.differentiate("x").simplify();
auto area = f.integrateNumeric("x", 0.0, 1.0, 100,
    [](const std::string&, double) { return 0.0; });
```

See [docs/PROGRAM_GENERATION_AND_MATH.md](docs/PROGRAM_GENERATION_AND_MATH.md).

## Cartridge format

See `docs/CARTRIDGE_FORMAT.md` for the project-specific `CRT1` format. Images can be validated and inspected; executing machine-code cartridges requires a specified ISA and runtime mapping.
