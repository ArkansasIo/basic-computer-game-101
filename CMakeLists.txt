cmake_minimum_required(VERSION 3.20)
project(BasicComputerGame101 VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(basicos
  src/main.cpp
  src/basic/interpreter.cpp
  src/os/filesystem.cpp
  src/os/shell.cpp
  src/cartridge/cartridge.cpp
)
target_include_directories(basicos PRIVATE include)

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

if(MSVC)
  foreach(target basicos basicgen generator_math_tests)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  endforeach()
else()
  foreach(target basicos basicgen generator_math_tests)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endforeach()
endif()

enable_testing()
add_executable(basicos_tests
  tests/smoke.cpp
  src/basic/interpreter.cpp
  src/cartridge/cartridge.cpp
)
target_include_directories(basicos_tests PRIVATE include)
add_test(NAME basicos_smoke COMMAND basicos_tests)
add_test(NAME generator_math_tests COMMAND generator_math_tests)

install(TARGETS basicos basicgen RUNTIME DESTINATION bin)
