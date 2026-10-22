# Included by CMakeLists.txt to build basicgen and generator/math tests.
add_executable(basicgen
  tools/basicgen.cpp
  src/generator/program_generator.cpp
)
target_include_directories(basicgen PRIVATE include)

add_executable(generator_math_tests
  tests/generator_math_tests.cpp
  src/generator/program_generator.cpp
  src/math/symbolic_calculus.cpp
)
target_include_directories(generator_math_tests PRIVATE include)
add_test(NAME generator_math_tests COMMAND generator_math_tests)
