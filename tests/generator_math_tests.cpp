#include "generator/program_generator.hpp"
#include "math/symbolic_calculus.hpp"
#include <cassert>
#include <cmath>
#include <string>

int main(){
    retro::generator::ProgramGenerator g(1234);
    auto p=g.generate(retro::generator::ProgramKind::Algebra,4);
    assert(!p.basicSource.empty());
    assert(p.basicSource.find("LET Y")!=std::string::npos);

    auto e=retro::math::Expression::parse("3*x^2 + 2*x + 1");
    auto d=e.differentiate("x").simplify();
    double actual=d.evaluate([](const std::string& n){return n=="x"?2.0:0.0;});
    assert(std::abs(actual-14.0)<1e-9);

    auto integral=retro::math::Expression::parse("x^2").integrateNumeric(
        "x",0.0,1.0,100,[](const std::string&,double){return 0.0;});
    assert(std::abs(integral-1.0/3.0)<1e-5);
    return 0;
}
