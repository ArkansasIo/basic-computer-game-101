# 101 BASIC Computer

C++23 retro-computer project: line-numbered BASIC interpreter, CP/M-inspired disk command shell, 64 KiB memory abstraction, and project-specific cartridge image format.

This is an original implementation inspired by classic 8-bit computers; it does not bundle CP/M source code or copyrighted book listings.

## Build

```sh
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Windows: run `build.bat`, then `run.bat`.

At startup, choose `OS` or `BASIC`.

The shell supports `HELP`, `DIR`, `TYPE`, `ERA`, `REN`, `SAVE`, `STAT`, `CARTLIST`, `CARTINFO`, `BASIC`, and `EXIT`. The BASIC interpreter supports numbered lines, `PRINT`, `INPUT`, `LET`, `IF...THEN`, `GOTO`, `GOSUB`, `RETURN`, loops, `LIST`, `RUN`, and `NEW`.

See `docs/CARTRIDGE_FORMAT.md` for the `CRT1` cartridge format. Cartridge images can be validated and inspected; executing machine-code cartridges needs a defined ISA and runtime mapping.
