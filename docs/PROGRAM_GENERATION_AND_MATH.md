# Program Generation, Algebra, and Calculus

## Program generator

`ProgramGenerator` creates BASIC source from algorithmic templates. The PRNG is seeded, so generated programs can be reproduced by supplying the same seed. Current template families:

- arithmetic drills;
- linear algebra by substitution;
- finite-difference derivative demo;
- number-guessing game;
- arithmetic sequences;
- dice simulation;
- simple state simulation.

Run the generator CLI after building it:

```sh
./basicgen --kind algebra --count 5 --complexity 4 --out generated/algebra
./basicgen --kind calculus --out generated/calculus
./basicgen --kind pack --count 21 --complexity 5 --out generated/program
```

The initial generated BASIC dialect supports only the statements already implemented by the interpreter. Some templates that mention functions such as `RND` or `INT` are currently instructional templates and need math-function support added to the interpreter before full execution.

## Symbolic expression engine

Expression syntax includes constants, variables, `+`, `-`, `*`, `/`, `^`, parentheses, and `sin()`, `cos()`, `exp()`, `log()`.

Example:

```cpp
auto f = retro::math::Expression::parse("3*x^2 + 2*x + 1");
auto df = f.differentiate("x").simplify();
std::cout << df.str(); // simplified symbolic derivative
```

Rules used include the sum, difference, product, quotient, constant-power, and elementary chain rules.

## Numerical integration

`integrateNumeric` uses composite Simpson's rule. It requires an even number of intervals and will round an odd interval count upward. This is a numerical approximation, not symbolic antiderivative generation.

For `f(x)=x^2), the integral from 0 to 1 is approximately 1/3.

## Limitations

- Algebra simplification is local rule-based simplification, not a full computer algebra system.
- Numerical integration assumes the integrand is well-behaved across the interval.
- Floating-point math is for the host-side authoring/toolchain; the simulated 8-bit BASIC runtime remains integer-first.
