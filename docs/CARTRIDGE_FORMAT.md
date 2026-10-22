# Cartridge image format (CRT1)

All integer fields are little-endian. Header size is 48 bytes.

| Offset | Size | Meaning |
|---:|---:|---|
| 0 | 4 | ASCII magic `CRT1` |
| 4 | 1 | format version (1) |
| 5 | 1 | flags |
| 6 | 2 | load address |
| 8 | 4 | payload length |
| 12 | 2 | entry point |
| 14 | 2 | reserved |
| 16 | 32 | NUL-padded display name |
| 48 | N | payload |

The loader validates magic/version/length and address-space bounds. It does not execute machine code on its own; an emulator must map the payload and define the cartridge CPU/runtime.
