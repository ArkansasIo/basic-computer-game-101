# BASIC 2026

BASIC 2026 is the next-generation architecture for this project. It keeps the readable, line-oriented BASIC heritage while moving the implementation toward modern C++23 object-oriented software design.

## Architecture

```text
BASIC 2026
├── Language
│   ├── Lexer / Parser
│   ├── AST
│   ├── Statements
│   └── Expressions
├── Runtime
│   ├── Value
│   ├── Environment
│   ├── Program
│   └── Interpreter
├── Standard Library
│   ├── ABS / SQRT
│   ├── SIN / COS
│   ├── EXP / LOG
│   ├── POW
│   ├── LEN
│   └── RND
├── Object System
│   ├── Object
│   ├── ClassRegistry
│   └── BasicModule
├── Mathematics
│   ├── Algebra
│   ├── Symbolic differentiation
│   └── Numerical integration
├── I/O
│   ├── Console
│   ├── Files
│   └── Cartridges
└── Tools
    └── BASIC program generator
```

## C++23 design

The implementation is intended to use modern C++ features where they improve safety and clarity:

- `std::unique_ptr` for AST/statement ownership;
- `std::shared_ptr` for runtime objects whose lifetime is shared;
- `std::string_view` for non-owning names and source views;
- `std::unordered_map` for environments and registries;
- `std::ranges` algorithms for ordered program statements;
- strongly typed `enum class` values;
- RAII rather than manual allocation;
- `std::function` for extensible library functions and class factories;
- `constexpr`/`consteval` opportunities for language constants;
- exceptions at the host API boundary for malformed programs and runtime errors.

## Object-oriented BASIC

The runtime introduces a small object model. `Object` is the polymorphic base class. `ClassRegistry` maps BASIC class names to C++ factories. `BasicModule` provides a named object with BASIC properties.

This allows future BASIC syntax such as:

```basic
CLASS Player
  NAME$ = "PLAYER 1"
  SCORE = 0
END CLASS

P = NEW Player
PRINT P.NAME$
```

The current C++ object layer is the foundation; parsing and execution of this class syntax should be added as the language frontend evolves.

## Standard library

`StandardLibrary::createDefault()` provides host functions for mathematical and utility operations. The BASIC interpreter can progressively expose these functions without hard-coding every function into the parser.

## Compatibility philosophy

The project should retain classic numbered BASIC programs while adding structured constructs gradually. Existing 101 BASIC examples can remain valid while BASIC 2026 gains modules, stronger types, object-oriented facilities, modern math, collections, and reusable libraries.

## Suggested future language features

1. `FUNCTION ... END FUNCTION`
2. `SUB ... END SUB`
3. `CLASS ... END CLASS`
4. `NEW` and object properties
5. arrays and dictionaries
6. structured `IF / ELSE / END IF`
7. `FOR / NEXT`, `WHILE / WEND`, and `DO / LOOP`
8. exceptions with `TRY / CATCH`
9. modules with `IMPORT`
10. file and cartridge APIs
11. asynchronous/task APIs for the host runtime
12. a bytecode VM for faster execution
