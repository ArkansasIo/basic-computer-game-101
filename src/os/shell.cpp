#include "os/shell.hpp"
#include "basic/interpreter.hpp"
#include "cartridge/cartridge.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <utility>
namespace retro {
namespace {std::string trimShell(std::string s){auto ns=[](unsigned char c){return !std::isspace(c);};s.erase(s.begin(),std::find_if(s.begin(),s.end(),ns));s.erase(std::find_if(s.rbegin(),s.rend(),ns).base(),s.end());return s;}std::string upperShell(std::string s){for(char& c:s)c=static_cast<char>(std::toupper(static_cast<unsigned char>(c)));return s;}}
Shell::Shell(FileSystem&a,FileSystem&b,std::string carts):driveA_(a),driveB_(b),cartridgeDirectory_(std::move(carts)){}
void Shell::printHelp() const{std::cout<<"HELP | DIR [A:|B:] | TYPE FILE | ERA FILE | REN OLD NEW | SAVE FILE TEXT... | STAT | CARTLIST | CARTINFO FILE.CRT | BASIC | EXIT\n";}
bool Shell::handle(const std::string& line){
    std::istringstream ss(line);std::string cmd;ss>>cmd;cmd=upperShell(cmd);if(cmd.empty())return true;
    if(cmd=="EXIT"||cmd=="QUIT")return false;if(cmd=="HELP"){printHelp();return true;}
    if(cmd=="DIR"){std::string d;ss>>d;auto&fs=(upperShell(d)=="B:"||upperShell(d)=="B")?driveB_:driveA_;for(const auto&f:fs.list())std::cout<<f<<"\n";return true;}
    if(cmd=="TYPE"){std::string f,body;ss>>f;if(f.empty()){std::cout<<"Usage: TYPE FILE\n";return true;}if(driveA_.readText(f,body))std::cout<<body<<(body.empty()||body.back()=='\n'?"":"\n");else std::cout<<"FILE NOT FOUND\n";return true;}
    if(cmd=="ERA"){std::string f;ss>>f;std::cout<<(driveA_.erase(f)?"DELETED\n":"FILE NOT FOUND\n");return true;}
    if(cmd=="REN"){std::string a,b;ss>>a>>b;if(a.empty()||b.empty()){std::cout<<"Usage: REN OLD NEW\n";return true;}std::cout<<(driveA_.renameFile(a,b)?"RENAMED\n":"RENAME ERROR\n");return true;}
    if(cmd=="SAVE"){std::string f;ss>>f;std::string text;std::getline(ss,text);text=trimShell(text);if(f.empty()){std::cout<<"Usage: SAVE FILE TEXT...\n";return true;}std::cout<<(driveA_.writeText(f,text+"\n")?"SAVED\n":"SAVE ERROR\n");return true;}
    if(cmd=="STAT"){std::cout<<"A: "<<driveA_.root()<<"\nB: "<<driveB_.root()<<"\n";return true;}
    if(cmd=="CARTLIST"){for(const auto&p:Cartridge::list(cartridgeDirectory_))std::cout<<p.filename().string()<<"\n";return true;}
    if(cmd=="CARTINFO"){std::string f;ss>>f;if(f.empty()){std::cout<<"Usage: CARTINFO FILE.CRT\n";return true;}CartridgeImage img;std::string err;if(!Cartridge::load(std::filesystem::path(cartridgeDirectory_)/f,img,err)){std::cout<<"CARTRIDGE ERROR: "<<err<<"\n";return true;}std::cout<<"NAME: "<<img.name<<"\nVERSION: "<<(int)img.version<<"\nLOAD: 0x"<<std::hex<<img.loadAddress<<std::dec<<"\nENTRY: 0x"<<std::hex<<img.entryPoint<<std::dec<<"\nBYTES: "<<img.payload.size()<<"\n";return true;}
    if(cmd=="BASIC"){BasicInterpreter basic([](const std::string&s){std::cout<<s;},[]{std::string s;std::getline(std::cin,s);return s;});basic.repl();return true;}
    std::cout<<"BAD COMMAND - TYPE HELP\n";return true;
}
void Shell::run(){std::cout<<"CP/M-STYLE 8-BIT OS SHELL\nTYPE HELP FOR COMMANDS\n";std::string line;while(std::cout<<"A> "&&std::getline(std::cin,line)){if(!handle(line))break;}}
}