#include "generator/program_generator.hpp"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc,char** argv){
    std::size_t count=1,complexity=3;std::string kind="pack",output="generated";
    for(int i=1;i<argc;++i){
        std::string a=argv[i];
        if(a=="--count"&&i+1<argc)count=static_cast<std::size_t>(std::stoul(argv[++i]));
        else if(a=="--complexity"&&i+1<argc)complexity=static_cast<std::size_t>(std::stoul(argv[++i]));
        else if(a=="--kind"&&i+1<argc)kind=argv[++i];
        else if(a=="--out"&&i+1<argc)output=argv[++i];
        else if(a=="--help"){std::cout<<"basicgen --kind pack|algebra|calculus|arithmetic|guess|sequence|dice|walk --count N --complexity N --out PREFIX\n";return 0;}
    }
    retro::generator::ProgramGenerator g;
    using K=retro::generator::ProgramKind;
    K selected=K::Algebra;
    if(kind=="algebra")selected=K::Algebra;else if(kind=="calculus")selected=K::Calculus;else if(kind=="arithmetic")selected=K::Arithmetic;else if(kind=="guess")selected=K::GuessingGame;else if(kind=="sequence")selected=K::NumberSequence;else if(kind=="dice")selected=K::DiceGame;else if(kind=="walk")selected=K::RandomWalk;
    auto programs=kind=="pack"?g.generatePack(count,complexity):std::vector<retro::generator::GeneratedProgram>{g.generate(selected,complexity)};
    for(std::size_t i=0;i<programs.size();++i){
        const auto path=output+"_"+std::to_string(i+1)+".bas";
        std::ofstream f(path);if(!f){std::cerr<<"cannot write "<<path<<"\n";return 2;}
        f<<programs[i].basicSource;
        std::cout<<path<<" — "<<programs[i].name<<"\n";
    }
    return 0;
}
