#include "basic2026/oop.hpp"
#include <stdexcept>

namespace basic2026 {
void ClassRegistry::registerClass(std::string name,Factory factory){factories_[std::move(name)]=std::move(factory);}
ObjectPtr ClassRegistry::create(std::string_view name)const{auto it=factories_.find(std::string(name));if(it==factories_.end())throw std::runtime_error("unknown class: "+std::string(name));return it->second();}
bool ClassRegistry::contains(std::string_view name)const{return factories_.contains(std::string(name));}
BasicModule::BasicModule(std::string name):name_(std::move(name)){}
std::string BasicModule::className()const{return "BASIC.MODULE";}
const std::string& BasicModule::name()const noexcept{return name_;}
void BasicModule::setProperty(std::string name,Value value){properties_.set(std::move(name),std::move(value));}
Value BasicModule::property(std::string_view name)const{return properties_.get(name);}
}
