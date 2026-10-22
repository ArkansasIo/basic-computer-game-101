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
if(MSVC)
  target_compile_options(basicos PRIVATE /W4 /permissive-)
else()
  target_compile_options(basicos PRIVATE -Wall -Wextra -Wpedantic)
endif()
enable_testing()
add_executable(basicos_tests tests/smoke.cpp src/basic/interpreter.cpp src/cartridge/cartridge.cpp)
target_include_directories(basicos_tests PRIVATE include)
add_test(NAME basicos_smoke COMMAND basicos_tests)
install(TARGETS basicos RUNTIME DESTINATION bin)
