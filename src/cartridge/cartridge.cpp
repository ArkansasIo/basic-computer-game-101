#include "cartridge/cartridge.hpp"
#include <array>
#include <cstring>
#include <fstream>

namespace retro {
namespace {
void set16(std::array<std::uint8_t,Cartridge::headerSize>&h,std::size_t p,std::uint16_t v){h[p]=static_cast<std::uint8_t>(v&255);h[p+1]=static_cast<std::uint8_t>((v>>8)&255);}
void set32(std::array<std::uint8_t,Cartridge::headerSize>&h,std::size_t p,std::uint32_t v){for(int i=0;i<4;++i)h[p+i]=static_cast<std::uint8_t>((v>>(8*i))&255);}
std::uint16_t read16(const std::array<std::uint8_t,Cartridge::headerSize>&h,std::size_t p){return static_cast<std::uint16_t>(h[p]|(static_cast<std::uint16_t>(h[p+1])<<8));}
std::uint32_t read32(const std::array<std::uint8_t,Cartridge::headerSize>&h,std::size_t p){std::uint32_t n=0;for(int i=0;i<4;++i)n|=static_cast<std::uint32_t>(h[p+i])<<(8*i);return n;}
}
bool Cartridge::load(const std::filesystem::path&path,CartridgeImage&img,std::string&err){
 std::ifstream f(path,std::ios::binary);if(!f){err="cannot open file";return false;}
 std::array<std::uint8_t,headerSize>h{};f.read(reinterpret_cast<char*>(h.data()),static_cast<std::streamsize>(h.size()));
 if(f.gcount()!=static_cast<std::streamsize>(h.size())){err="file shorter than header";return false;}
 if(std::memcmp(h.data(),"CRT1",4)!=0){err="bad cartridge magic";return false;}if(h[4]!=1){err="unsupported cartridge version";return false;}
 auto len=read32(h,8);if(len>65536){err="payload exceeds address space";return false;}
 img.version=h[4];img.flags=h[5];img.loadAddress=read16(h,6);img.entryPoint=read16(h,12);
 img.name.assign(reinterpret_cast<const char*>(h.data()+16),32);auto z=img.name.find('\0');if(z!=std::string::npos)img.name.resize(z);
 if(static_cast<std::uint32_t>(img.loadAddress)+len>65536U){err="payload overflows memory";return false;}
 img.payload.resize(len);if(len){f.read(reinterpret_cast<char*>(img.payload.data()),static_cast<std::streamsize>(len));if(f.gcount()!=static_cast<std::streamsize>(len)){err="truncated payload";return false;}}
 char extra;if(f.get(extra)){err="trailing bytes after payload";return false;}return true;
}
bool Cartridge::save(const std::filesystem::path&path,const CartridgeImage&img,std::string&err){
 if(img.payload.size()>65536){err="payload exceeds address space";return false;}
 if(static_cast<std::uint32_t>(img.loadAddress)+img.payload.size()>65536U){err="payload overflows memory";return false;}
 std::array<std::uint8_t,headerSize>h{};std::memcpy(h.data(),"CRT1",4);h[4]=1;h[5]=img.flags;set16(h,6,img.loadAddress);set32(h,8,static_cast<std::uint32_t>(img.payload.size()));set16(h,12,img.entryPoint);
 auto name=img.name.substr(0,32);std::memcpy(h.data()+16,name.data(),name.size());
 std::ofstream f(path,std::ios::binary);if(!f){err="cannot create file";return false;}f.write(reinterpret_cast<const char*>(h.data()),static_cast<std::streamsize>(h.size()));if(!img.payload.empty())f.write(reinterpret_cast<const char*>(img.payload.data()),static_cast<std::streamsize>(img.payload.size()));if(!f){err="write failed";return false;}return true;
}
std::vector<std::filesystem::path> Cartridge::list(const std::filesystem::path&dir){std::vector<std::filesystem::path>out;if(!std::filesystem::exists(dir))return out;for(const auto&e:std::filesystem::directory_iterator(dir))if(e.is_regular_file()&&e.path().extension()==".crt")out.push_back(e.path());std::sort(out.begin(),out.end());return out;}
}
