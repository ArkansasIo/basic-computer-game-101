#include "math/symbolic_calculus.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace retro::math {
namespace {
using N=std::shared_ptr<ExprNode>;
N node(NodeKind k,N a={},N b={},double v=0.0,std::string name={}){auto n=std::make_shared<ExprNode>();n->kind=k;n->left=std::move(a);n->right=std::move(b);n->value=v;n->name=std::move(name);return n;}
N constant(double x){return node(NodeKind::Constant,{}, {},x);}
N variableNode(std::string n){return node(NodeKind::Variable,{}, {},0,std::move(n));}
bool isConst(const N& n,double v){return n&&n->kind==NodeKind::Constant&&std::abs(n->value-v)<1e-12;}
N simplifyNode(const N& n){
 if(!n)return constant(0);
 N a=n->left?simplifyNode(n->left):N{}, b=n->right?simplifyNode(n->right):N{};
 if(n->kind==NodeKind::Constant||n->kind==NodeKind::Variable)return node(n->kind,{}, {},n->value,n->name);
 if(n->kind==NodeKind::Negate){if(a&&a->kind==NodeKind::Constant)return constant(-a->value);if(a&&a->kind==NodeKind::Negate)return a->left;return node(n->kind,a);}
 if(a&&b&&a->kind==NodeKind::Constant&&b->kind==NodeKind::Constant){
  double x=a->value,y=b->value;
  switch(n->kind){case NodeKind::Add:return constant(x+y);case NodeKind::Subtract:return constant(x-y);case NodeKind::Multiply:return constant(x*y);case NodeKind::Divide:if(y!=0)return constant(x/y);break;case NodeKind::Power:return constant(std::pow(x,y));default:break;}
 }
 switch(n->kind){
  case NodeKind::Add:if(isConst(a,0))return b;if(isConst(b,0))return a;break;
  case NodeKind::Subtract:if(isConst(b,0))return a;if(isConst(a,0))return node(NodeKind::Negate,b);if(a&&b&&a->kind==NodeKind::Variable&&b->kind==NodeKind::Variable&&a->name==b->name)return constant(0);break;
  case NodeKind::Multiply:if(isConst(a,0)||isConst(b,0))return constant(0);if(isConst(a,1))return b;if(isConst(b,1))return a;break;
  case NodeKind::Divide:if(isConst(a,0))return constant(0);if(isConst(b,1))return a;break;
  case NodeKind::Power:if(isConst(b,0))return constant(1);if(isConst(b,1))return a;break;
  default:break;
 }
 return node(n->kind,a,b,n->value,n->name);
}
std::string render(const N& n){
 if(!n)return "0";std::ostringstream o;
 switch(n->kind){
  case NodeKind::Constant:o<<std::setprecision(12)<<n->value;return o.str();
  case NodeKind::Variable:return n->name;
  case NodeKind::Add:case NodeKind::Subtract:case NodeKind::Multiply:case NodeKind::Divide:case NodeKind::Power:
   o<<"("<<render(n->left)<<(n->kind==NodeKind::Add?"+":n->kind==NodeKind::Subtract?"-":n->kind==NodeKind::Multiply?"*":n->kind==NodeKind::Divide?"/":"^")<<render(n->right)<<")";return o.str();
  case NodeKind::Negate:return "(-"+render(n->left)+")";
  case NodeKind::Sin:return "sin("+render(n->left)+")";
  case NodeKind::Cos:return "cos("+render(n->left)+")";
  case NodeKind::Exp:return "exp("+render(n->left)+")";
  case NodeKind::Log:return "log("+render(n->left)+")";
 }
 return "0";
}
double eval(const N& n,const std::function<double(const std::string&)>& lookup){
 switch(n->kind){
  case NodeKind::Constant:return n->value;case NodeKind::Variable:return lookup(n->name);
  case NodeKind::Add:return eval(n->left,lookup)+eval(n->right,lookup);
  case NodeKind::Subtract:return eval(n->left,lookup)-eval(n->right,lookup);
  case NodeKind::Multiply:return eval(n->left,lookup)*eval(n->right,lookup);
  case NodeKind::Divide:{double b=eval(n->right,lookup);if(std::abs(b)<1e-15)throw std::runtime_error("division by zero");return eval(n->left,lookup)/b;}
  case NodeKind::Power:return std::pow(eval(n->left,lookup),eval(n->right,lookup));
  case NodeKind::Negate:return -eval(n->left,lookup);
  case NodeKind::Sin:return std::sin(eval(n->left,lookup));case NodeKind::Cos:return std::cos(eval(n->left,lookup));
  case NodeKind::Exp:return std::exp(eval(n->left,lookup));case NodeKind::Log:{double x=eval(n->left,lookup);if(x<=0)throw std::runtime_error("log domain error");return std::log(x);}
 }
 throw std::runtime_error("unknown expression node");
}
N derivative(const N& n,const std::string& v){
 auto d=[&](const N& x){return derivative(x,v);};
 switch(n->kind){
  case NodeKind::Constant:return constant(0);
  case NodeKind::Variable:return constant(n->name==v?1:0);
  case NodeKind::Add:return node(NodeKind::Add,d(n->left),d(n->right));
  case NodeKind::Subtract:return node(NodeKind::Subtract,d(n->left),d(n->right));
  case NodeKind::Multiply:return node(NodeKind::Add,node(NodeKind::Multiply,d(n->left),n->right),node(NodeKind::Multiply,n->left,d(n->right)));
  case NodeKind::Divide:return node(NodeKind::Divide,node(NodeKind::Subtract,node(NodeKind::Multiply,d(n->left),n->right),node(NodeKind::Multiply,n->left,d(n->right))),node(NodeKind::Power,n->right,constant(2)));
  case NodeKind::Power:
   if(n->right&&n->right->kind==NodeKind::Constant){double p=n->right->value;return node(NodeKind::Multiply,node(NodeKind::Multiply,constant(p),node(NodeKind::Power,n->left,constant(p-1))),d(n->left));}
   return node(NodeKind::Multiply,node(NodeKind::Power,n->left,n->right),node(NodeKind::Add,node(NodeKind::Multiply,d(n->right),node(NodeKind::Log,n->left)),node(NodeKind::Divide,node(NodeKind::Multiply,n->right,d(n->left)),n->left)));
  case NodeKind::Negate:return node(NodeKind::Negate,d(n->left));
  case NodeKind::Sin:return node(NodeKind::Multiply,node(NodeKind::Cos,n->left),d(n->left));
  case NodeKind::Cos:return node(NodeKind::Multiply,node(NodeKind::Negate,node(NodeKind::Sin,n->left)),d(n->left));
  case NodeKind::Exp:return node(NodeKind::Multiply,node(NodeKind::Exp,n->left),d(n->left));
  case NodeKind::Log:return node(NodeKind::Divide,d(n->left),n->left);
 }
 return constant(0);
}
struct Parser {
 const std::string& s;std::size_t p=0;
 void ws(){while(p<s.size()&&std::isspace(static_cast<unsigned char>(s[p])))++p;}
 bool take(char c){ws();if(p<s.size()&&s[p]==c){++p;return true;}return false;}
 N expression(){N n=term();for(;;){if(take('+'))n=node(NodeKind::Add,n,term());else if(take('-'))n=node(NodeKind::Subtract,n,term());else return n;}}
 N term(){N n=power();for(;;){if(take('*'))n=node(NodeKind::Multiply,n,power());else if(take('/'))n=node(NodeKind::Divide,n,power());else return n;}}
 N power(){N n=unary();if(take('^'))n=node(NodeKind::Power,n,power());return n;}
 N unary(){if(take('+'))return unary();if(take('-'))return node(NodeKind::Negate,unary());return primary();}
 N primary(){
  ws();if(take('(')){N n=expression();if(!take(')'))throw std::runtime_error("missing ')'");return n;}
  if(p<s.size()&&(std::isdigit(static_cast<unsigned char>(s[p]))||s[p]=='.')){std::size_t used=0;double v=std::stod(s.substr(p),&used);p+=used;return constant(v);}
  if(p<s.size()&&std::isalpha(static_cast<unsigned char>(s[p]))){std::string n;while(p<s.size()&&(std::isalnum(static_cast<unsigned char>(s[p]))||s[p]=='_'))n+=s[p++];if(take('(')){N arg=expression();if(!take(')'))throw std::runtime_error("missing function ')'");if(n=="sin")return node(NodeKind::Sin,arg);if(n=="cos")return node(NodeKind::Cos,arg);if(n=="exp")return node(NodeKind::Exp,arg);if(n=="log")return node(NodeKind::Log,arg);throw std::runtime_error("unknown function: "+n);}return variableNode(n);}
  throw std::runtime_error("expected number, variable, or parenthesis");
 }
};
}
Expression::Expression():root_(constant(0)){}
Expression::Expression(double c):root_(constant(c)){}
Expression::Expression(std::shared_ptr<ExprNode> n):root_(std::move(n)){}
Expression Expression::variable(std::string n){return Expression(variableNode(std::move(n)));}
Expression Expression::parse(const std::string& s){Parser p{s};auto n=p.expression();p.ws();if(p.p!=s.size())throw std::runtime_error("unexpected token at "+std::to_string(p.p));return Expression(n);}
std::string Expression::str()const{return render(root_);}
double Expression::evaluate(const std::function<double(const std::string&)>& lookup)const{return eval(root_,lookup);}
Expression Expression::simplify()const{return Expression(simplifyNode(root_));}
Expression Expression::differentiate(const std::string& v)const{return Expression(simplifyNode(derivative(root_,v)));}
double Expression::integrateNumeric(const std::string& v,double a,double b,std::size_t intervals,const std::function<double(const std::string&,double)>& setter)const{
 if(intervals<2)intervals=2;if(intervals%2)++intervals;double h=(b-a)/static_cast<double>(intervals),total=0;
 for(std::size_t i=0;i<=intervals;++i){double x=a+h*i;double y=evaluate([&](const std::string& name){if(name==v)return x;return setter(name,x);});total+=(i==0||i==intervals?1:(i%2?4:2))*y;}
 return total*h/3.0;
}
}