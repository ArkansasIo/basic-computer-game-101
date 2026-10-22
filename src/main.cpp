#include "basic/interpreter.hpp"
#include "memory/memory64k.hpp"
#include "os/filesystem.hpp"
#include "os/shell.hpp"
#include <filesystem>
#include <iostream>
#include <string>
int main(){
 std::cout<<"========================================\n         8-BIT BASIC COMPUTER OS\n========================================\nCPU: 8-BIT SIMULATOR | ADDRESS SPACE: 64K\nBASIC 1.0 | CP/M-STYLE SHELL | CRT1\n\n";
 retro::Memory64K memory;(void)memory;
 std::filesystem::create_directories("drives/A");std::filesystem::create_directories("drives/B");std::filesystem::create_directories("cartridges");
 retro::FileSystem a("drives/A"),b("drives/B");std::cout<<"Enter OS or BASIC: ";std::string mode;if(!std::getline(std::cin,mode))return 0;
 if(mode=="BASIC"||mode=="basic"){retro::BasicInterpreter basic([](const std::string&s){std::cout<<s;},[]{std::string s;std::getline(std::cin,s);return s;});basic.repl();}
 else {retro::Shell shell(a,b,"cartridges");shell.run();}
 return 0;
}