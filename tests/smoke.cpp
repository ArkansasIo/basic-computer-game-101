#include "basic/interpreter.hpp"
#include "cartridge/cartridge.hpp"
#include "memory/memory64k.hpp"
#include <cassert>
#include <filesystem>
#include <string>

int main(){
 retro::Memory64K mem;mem.write(0x1234,0xAB);assert(mem.read(0x1234)==0xAB);
 std::string output;
 retro::BasicInterpreter basic([&](const std::string&s){output+=s;},[]{return std::string("4");});
 basic.executeImmediate("10 PRINT \"HELLO\"");
 basic.executeImmediate("20 LET X = (20+22)");
 basic.executeImmediate("30 PRINT X");
 basic.executeImmediate("RUN");
 assert(output.find("HELLO")!=std::string::npos);assert(output.find("42")!=std::string::npos);
 auto path=std::filesystem::temp_directory_path()/"basiccomputer-smoke.crt";
 retro::CartridgeImage original;original.name="SMOKE";original.loadAddress=0x8000;original.entryPoint=0x8000;original.payload={0xA9,0x2A,0xFF};
 std::string error;assert(retro::Cartridge::save(path,original,error));
 retro::CartridgeImage loaded;assert(retro::Cartridge::load(path,loaded,error));assert(loaded.name=="SMOKE");assert(loaded.payload==original.payload);
 std::filesystem::remove(path);return 0;
}
