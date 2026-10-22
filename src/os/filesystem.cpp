#include "os/filesystem.hpp"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <stdexcept>

namespace retro {
FileSystem::FileSystem(std::filesystem::path root):root_(std::filesystem::absolute(std::move(root)).lexically_normal()){std::filesystem::create_directories(root_);}
std::filesystem::path FileSystem::safePath(const std::string& name) const {
    std::filesystem::path rel(name);
    if(rel.empty()||rel.is_absolute())throw std::invalid_argument("invalid filename");
    for(const auto& part:rel)if(part==".."||part==".")throw std::invalid_argument("path traversal denied");
    auto full=(root_/rel).lexically_normal();
    auto base=root_.generic_string(),candidate=full.generic_string();
    if(candidate.size()<base.size()||candidate.compare(0,base.size(),base)!=0||(candidate.size()>base.size()&&candidate[base.size()]!='/'))throw std::invalid_argument("path outside drive");
    return full;
}
std::vector<std::string> FileSystem::list() const{std::vector<std::string> out;for(const auto&e:std::filesystem::directory_iterator(root_))out.push_back(e.path().filename().string());std::sort(out.begin(),out.end());return out;}
bool FileSystem::readText(const std::string& name,std::string& content) const{try{std::ifstream f(safePath(name),std::ios::binary);if(!f)return false;content.assign(std::istreambuf_iterator<char>(f),{});return true;}catch(...){return false;}}
bool FileSystem::writeText(const std::string& name,const std::string& content){try{auto p=safePath(name);std::filesystem::create_directories(p.parent_path());std::ofstream f(p,std::ios::binary);if(!f)return false;f<<content;return static_cast<bool>(f);}catch(...){return false;}}
bool FileSystem::erase(const std::string& name){try{return std::filesystem::remove(safePath(name));}catch(...){return false;}}
bool FileSystem::renameFile(const std::string& from,const std::string& to){try{std::filesystem::rename(safePath(from),safePath(to));return true;}catch(...){return false;}}
}
