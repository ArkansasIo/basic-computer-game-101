#include "basic2026/core.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace basic2026 {
Integer Value::asInteger() const { if(type_==Type::Integer)return integer_; if(type_==Type::Real)return static_cast<Integer>(real_); if(type_==Type::Boolean)return boolean_?1:0; throw std::runtime_error("value is not numeric"); }
double Value::asReal() const { if(type_==Type::Real)return real_; if(type_==Type::Integer)return static_cast<double>(integer_); if(type_==Type::Boolean)return boolean_?1.0:0.0; throw std::runtime_error("value is not numeric"); }
const std::string& Value::asString() const { if(type_!=Type::String)throw std::runtime_error("value is not a string"); return string_; }
bool Value::asBoolean() const { if(type_==Type::Boolean)return boolean_; if(type_==Type::Integer)return integer_!=0; if(type_==Type::Real)return real_!=0.0; throw std::runtime_error("value is not boolean"); }
std::string Value::toString() const { std::ostringstream out; switch(type_){case Type::Integer:return std::to_string(integer_);case Type::Real:out<<std::setprecision(15)<<real_;return out.str();case Type::String:return string_;case Type::Boolean:return boolean_?"TRUE":"FALSE";case Type::None:return "NONE";}return {}; }
void Environment::set(std::string name,Value value){ values_[std::move(name)]=std::move(value); }
bool Environment::contains(std::string_view name)const{return values_.contains(std::string(name));}
Value Environment::get(std::string_view name)const{auto it=values_.find(std::string(name));if(it==values_.end())throw std::runtime_error("undefined variable: "+std::string(name));return it->second;}
void Environment::clear(){values_.clear();}
void Program::add(LineNumber line,StatementPtr statement){statements_.emplace_back(line,std::move(statement));std::ranges::sort(statements_,[](const auto&a,const auto&b){return a.first<b.first;});}
void Program::clear(){statements_.clear();}
void Program::run(Environment& env){for(auto& [line,statement]:statements_){(void)line;statement->execute(env);}}
}
