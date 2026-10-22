# CP/M BASIC Console Emulator

BASIC 2026 now includes a C++23 console application that presents a CP/M-inspired command environment and launches the existing BASIC interpreter as a resident language environment.

## Build

```text
cmake -S . -B build
cmake --build build --config Release
```

Run:

```text
build/cpmbasic
```

On multi-config generators the executable is normally under `build/Release/cpmbasic`.

## Console model

The emulator provides:

- `A:` and `B:` logical disk drives
- persistent host-backed drive directories
- CP/M-style command prompt
- `DIR`
- `TYPE`
- `ERA` / `ERASE`
- `REN` / `RENAME`
- `USER n`
- `BASIC`
- `HELP`
- `VER`
- `EXIT`

The console creates `drives/A`, `drives/B`, and `cartridges` when it starts.

## BASIC mode

Enter:

```text
A:>BASIC
```

The BASIC interpreter becomes the active console until `EXIT` returns to the CP/M command processor.

Example:

```basic
10 PRINT "CP/M BASIC 2026"
20 FOR I = 1 TO 10
30 PRINT I
40 NEXT I
50 END
RUN
```

The CP/M shell and BASIC runtime are deliberately separated. This makes it possible to replace the BASIC front end, add a bytecode VM, or connect the console to a future 8080/Z80 CPU emulator without redesigning the command layer.

## Architecture

```text
+------------------------------+
|        CP/M BASIC Console    |
+------------------------------+
| Command Processor            |
| A:/B: Drive Manager          |
| TYPE / DIR / ERA / REN       |
+------------------------------+
| BASIC Interpreter             |
+------------------------------+
| BASIC 2026 C++23 Runtime      |
| Value | Environment | OOP     |
+------------------------------+
| Host Filesystem / Cartridges  |
+------------------------------+
```

## Next hardware-emulation layer

The next planned layer is a cycle-aware 8-bit machine containing:

1. Intel 8080-compatible CPU core
2. Z80 compatibility mode
3. 64 KiB address space
4. ROM/RAM map
5. CP/M BIOS device interface
6. CP/M BDOS call dispatcher
7. console UART/terminal device
8. disk controller abstraction
9. `.COM` program loader
10. BASIC ROM/cartridge image loader
11. debugger with registers, memory, breakpoints, and single-step execution

This console is the software shell foundation for that hardware emulator.
