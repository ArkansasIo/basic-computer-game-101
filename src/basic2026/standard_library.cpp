#include "basic2026/standard_library.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace basic2026 {
StandardLibrary::StandardLibrary() = default;
void StandardLibrary::registerFunction(std::string name,Function function){functions_[std::move(name)]=std::move(function);}
Value StandardLibrary::call(std::string_view name,const std::vector<Value>& args)const{auto it=functions_.find(std::string(name));if(it==functions_.end())throw std::runtime_error("unknown function: "+std::string(name));return it->second(args);}
StandardLibrary StandardLibrary::createDefault(){
 StandardLibrary lib;
 lib.registerFunction("ABS",[](const auto&a){if(a.size()!=1)throw std::runtime_error("ABS expects 1 argument");return Value(std::llabs(a[0].asInteger()));});
 lib.registerFunction("SQRT",[](const auto&a){if(a.size()!=1)throw std::runtime_error("SQRT expects 1 argument");return Value(std::sqrt(a[0].asReal()));});
 lib.registerFunction("SIN",[](const auto&a){if(a.size()!=1)throw std::runtime_error("SIN expects 1 argument");return Value(std::sin(a[0].asReal()));});
 lib.registerFunction("COS",[](const auto&a){if(a.size()!=1)throw std::runtime_error("COS expects 1 argument");return Value(std::cos(a[0].asReal()));});
 lib.registerFunction("EXP",[](const auto&a){if(a.size()!=1)throw std::runtime_error("EXP expects 1 argument");return Value(std::exp(a[0].asReal()));});
 lib.registerFunction("LOG",[](const auto&a){if(a.size()!=1)throw std::runtime_error("LOG expects 1 argument");return Value(std::log(a[0].asReal()));});
 lib.registerFunction("POW",[](const auto&a){if(a.size()!=2)throw std::runtime_error("POW expects 2 arguments");return Value(std::pow(a[0].asReal(),a[1].asReal()));});
 lib.registerFunction("LEN",[](const auto&a){if(a.size()!=1)throw std::runtime_error("LEN expects 1 argument");return Value(static_cast<Integer>(a[0].asString().size()));});
 lib.registerFunction("RND",[](const auto&){static thread_local std::mt19937_64 rng{std::random_device{}()};std::uniform_real_distribution<double>d(0.0,1.0);return Value(d(rng));});
 return lib;
}
}
