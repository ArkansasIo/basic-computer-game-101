# Architecture

- **Memory64K** is a 16-bit address-space abstraction.
- **BASIC interpreter** stores numbered lines, evaluates integer expressions, and executes statements.
- **OS shell** offers CP/M-inspired commands over sandboxed host directories `drives/A` and `drives/B`.
- **Cartridge** is a validated project-specific binary image.
- **BIOS/kernel** are represented by boot setup and machine services. This is a hosted simulator, not a bootable PC OS.

## Logical memory map

| Range | Purpose |
|---|---|
| 0000–00FF | Zero page / vectors |
| 0100–01FF | Stack |
| 0200–1FFF | OS/runtime workspace |
| 2000–5FFF | BASIC program/data |
| 6000–7FFF | cartridge RAM window |
| 8000–FFFF | ROM/cartridge window |
