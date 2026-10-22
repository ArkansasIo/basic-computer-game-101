cmake_minimum_required(VERSION 3.20)
project(BasicComputerGame101 VERSION 2026.2.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(basic2026
  src/basic2026/core.cpp
  src/basic2026/standard_library.cpp
  src/basic2026/oop.cpp
)
target_include_directories(basic2026 PUBLIC include)
target_compile_features(basic2026 PUBLIC cxx_std_23)

add_executable(basicos
  src/main.cpp
  src/basic/interpreter.cpp
  src/os/filesystem.cpp
  src/os/shell.cpp
  src/cartridge/cartridge.cpp
)
target_include_directories(basicos PRIVATE include)
target_link_libraries(basicos PRIVATE basic2026)

add_executable(cpmbasic
  tools/cpmbasic.cpp
  src/cpm/console.cpp
  src/basic/interpreter.cpp
)
target_include_directories(cpmbasic PRIVATE include)
target_link_libraries(cpmbasic PRIVATE basic2026)

a dd_executable(basicgen
  tools/basicgen.cpp
  src/generator/program_generator.cpp
)
target_include_directories(basicgen PRIVATE include)
target_link_libraries(basicgen PRIVATE basic2026)

add_executable(generator_math_tests
  tests/generator_math_tests.cpp
  src/generator/program_generator.cpp
  src/math/symbolic_calculus.cpp
)
target_include_directories(generator_math_tests PRIVATE include)
target_link_libraries(generator_math_tests PRIVATE basic2026)

if(MSVC)
  foreach(target basicos cpmbasic basicgen generator_math_tests basic2026)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  endforeach()
else()
  foreach(target basicos cpmbasic basicgen generator_math_tests basic2026)
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
target_link_libraries(basicos_tests PRIVATE basic2026)
add_test(NAME basicos_smoke COMMAND basicos_tests)
add_test(NAME generator_math_tests COMMAND generator_math_tests)

install(TARGETS basicos cpmbasic basicgen basic2026
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
