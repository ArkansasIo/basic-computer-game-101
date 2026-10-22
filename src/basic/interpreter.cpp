#include "basic/interpreter.hpp"
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace retro {
BasicInterpreter::BasicInterpreter(Output output, Input input)
    : output_(std::move(output)), input_(std::move(input)) {}

std::string BasicInterpreter::trim(std::string s) const {
    auto notSpace = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
    return s;
}
std::string BasicInterpreter::upper(std::string s) const {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

long long BasicInterpreter::eval(const std::string& raw) const {
    struct Parser {
        const std::string& s; const std::unordered_map<std::string,long long>& vars; std::size_t p=0;
        void ws(){while(p<s.size()&&std::isspace(static_cast<unsigned char>(s[p])))++p;}
        bool take(char c){ws();if(p<s.size()&&s[p]==c){++p;return true;}return false;}
        long long expression(){auto n=term();while(true){if(take('+'))n+=term();else if(take('-'))n-=term();else break;}return n;}
        long long term(){auto n=factor();while(true){if(take('*'))n*=factor();else if(take('/')){auto d=factor();if(!d)throw std::runtime_error("division by zero");n/=d;}else if(take('%')){auto d=factor();if(!d)throw std::runtime_error("division by zero");n%=d;}else break;}return n;}
        long long factor(){
            ws();if(take('+'))return factor();if(take('-'))return -factor();
            if(take('(')){auto n=expression();if(!take(')'))throw std::runtime_error("missing )");return n;}
            if(p<s.size()&&std::isdigit(static_cast<unsigned char>(s[p]))){long long n=0;while(p<s.size()&&std::isdigit(static_cast<unsigned char>(s[p])))n=n*10+(s[p++]-'0');return n;}
            if(p<s.size()&&std::isalpha(static_cast<unsigned char>(s[p]))){std::string k;while(p<s.size()&&(std::isalnum(static_cast<unsigned char>(s[p]))||s[p]=='$'))k+=static_cast<char>(std::toupper(static_cast<unsigned char>(s[p++])));auto it=vars.find(k);return it==vars.end()?0:it->second;}
            throw std::runtime_error("invalid expression");
        }
    } parser{raw, variables_};
    auto result=parser.expression();parser.ws();
    if(parser.p!=raw.size())throw std::runtime_error("invalid expression near: "+raw.substr(parser.p));
    return result;
}

void BasicInterpreter::executeStatement(const std::string& raw,int& nextLine,int currentLine) {
    std::string s=trim(raw);if(s.empty())return;std::string u=upper(s);
    if(u.rfind("REM",0)==0)return;
    if(u=="END"||u=="STOP"){stopRequested_=true;return;}
    if(u=="CLS"){output_("\x1b[2J\x1b[H");return;}
    if(u=="HELP"){output_("BASIC: PRINT INPUT LET IF...THEN GOTO GOSUB RETURN FOR/NEXT REM END LIST RUN NEW CLEAR EXIT\n");return;}

    if(u.rfind("PRINT",0)==0&&(u.size()==5||std::isspace(static_cast<unsigned char>(u[5])))){
        std::string arg=trim(s.substr(5));std::ostringstream out;
        std::size_t start=0;bool quoted=false;std::vector<std::string> parts;
        for(std::size_t i=0;i<arg.size();++i){if(arg[i]=='"')quoted=!quoted;else if(arg[i]==';'&&!quoted){parts.push_back(arg.substr(start,i-start));start=i+1;}}
        parts.push_back(arg.substr(start));
        for(auto p:parts){p=trim(p);if(p.size()>=2&&p.front()=='"'&&p.back()=='"')out<<p.substr(1,p.size()-2);else if(!p.empty())out<<eval(p);}
        out<<"\n";output_(out.str());return;
    }
    if(u.rfind("INPUT ",0)==0){
        auto name=upper(trim(s.substr(6)));output_(name+"? ");auto text=trim(input_());
        try{variables_[name]=std::stoll(text);}catch(...){variables_[name]=0;output_("?NUMBER EXPECTED; USING 0\n");}return;
    }
    if(u.rfind("GOTO ",0)==0){nextLine=static_cast<int>(eval(s.substr(5)));return;}
    if(u.rfind("GOSUB ",0)==0){throw std::runtime_error("GOSUB requires the upcoming full control-flow implementation");}
    if(u=="RETURN")throw std::runtime_error("RETURN without GOSUB");

    if(u.rfind("IF ",0)==0){
        auto thenPos=u.find(" THEN ");if(thenPos==std::string::npos)throw std::runtime_error("IF requires THEN");
        std::string cond=trim(s.substr(3,thenPos-3)), target=trim(s.substr(thenPos+6)), op;std::size_t pos=std::string::npos;
        for(const auto& candidate:{std::string("<="),std::string(">="),std::string("<>"),std::string("="),std::string("<"),std::string(">")}){pos=cond.find(candidate);if(pos!=std::string::npos){op=candidate;break;}}
        if(pos==std::string::npos)throw std::runtime_error("invalid IF comparison");
        long long a=eval(cond.substr(0,pos)),b=eval(cond.substr(pos+op.size()));
        bool yes=op=="="?a==b:op=="<>"?a!=b:op=="<"?a<b:op==">"?a>b:op=="<="?a<=b:a>=b;
        if(yes){std::string tu=upper(target);if(tu.rfind("GOTO ",0)==0)nextLine=static_cast<int>(eval(target.substr(5)));else if(!target.empty()&&std::isdigit(static_cast<unsigned char>(target[0])))nextLine=static_cast<int>(eval(target));else executeStatement(target,nextLine,currentLine);}
        return;
    }

    std::string assignment=s;if(u.rfind("LET ",0)==0)assignment=trim(s.substr(4));
    auto eq=assignment.find('=');if(eq!=std::string::npos){
        auto name=upper(trim(assignment.substr(0,eq)));if(name.empty()||!std::isalpha(static_cast<unsigned char>(name[0])))throw std::runtime_error("invalid variable");
        variables_[name]=eval(assignment.substr(eq+1));return;
    }
    throw std::runtime_error("syntax error: "+s);
}
void BasicInterpreter::executeImmediate(const std::string& raw){
    std::string line=trim(raw);if(line.empty())return;std::istringstream ss(line);int n;std::string rest;
    if(ss>>n){std::getline(ss,rest);rest=trim(rest);if(rest.empty())lines_.erase(n);else lines_[n]=rest;return;}
    auto cmd=upper(line);
    if(cmd=="LIST"){listProgram();return;}if(cmd=="RUN"){runProgram();return;}if(cmd=="NEW"||cmd=="CLEAR"){clearProgram();return;}
    if(cmd=="HELP"){output_("Commands: LIST RUN NEW CLEAR HELP EXIT; numbered lines edit program.\n");return;}
    if(cmd=="EXIT"||cmd=="QUIT"){stopRequested_=true;return;}
    int next=0;try{executeStatement(line,next,0);}catch(const std::exception&e){output_("?"+std::string(e.what())+"\n");}
}
void BasicInterpreter::listProgram() const{for(const auto&[n,s]:lines_)output_(std::to_string(n)+" "+s+"\n");}
void BasicInterpreter::clearProgram(){lines_.clear();variables_.clear();}
void BasicInterpreter::runProgram(){
    variables_.clear();stopRequested_=false;std::vector<int> nums;for(const auto&[n,s]:lines_){(void)s;nums.push_back(n);}
    std::size_t ip=0,steps=0;constexpr std::size_t limit=100000;
    while(ip<nums.size()&&!stopRequested_&&steps++<limit){int current=nums[ip],next=current;
        try{executeStatement(lines_.at(current),next,current);}catch(const std::exception&e){output_("?"+std::string(e.what())+" IN "+std::to_string(current)+"\n");return;}
        if(stopRequested_)break;
        if(next!=current){auto it=std::lower_bound(nums.begin(),nums.end(),next);if(it==nums.end()||*it!=next){output_("?UNDEFINED LINE "+std::to_string(next)+"\n");return;}ip=static_cast<std::size_t>(it-nums.begin());}
        else ++ip;
    }
    if(steps>=limit)output_("?STEP LIMIT\n");
}
void BasicInterpreter::repl(){
    output_("BASIC 1.0 - TYPE HELP FOR COMMANDS\nREADY.\n");std::string line;
    while(true){output_("> ");if(!std::getline(std::cin,line))break;auto u=upper(trim(line));if(u=="EXIT"||u=="QUIT")break;stopRequested_=false;executeImmediate(line);if(stopRequested_)break;}
}
}
