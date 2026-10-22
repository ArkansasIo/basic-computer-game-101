#include "generator/program_generator.hpp"
#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace retro::generator {
ProgramGenerator::ProgramGenerator(std::uint32_t seed) : state_(seed ? seed : 1) {}
std::uint32_t ProgramGenerator::next() { state_ ^= state_ << 13; state_ ^= state_ >> 17; state_ ^= state_ << 5; return state_; }
int ProgramGenerator::pick(int lo,int hi){ return lo + static_cast<int>(next() % static_cast<std::uint32_t>(hi-lo+1)); }
std::string ProgramGenerator::kindName(ProgramKind k) {
 switch(k){case ProgramKind::Arithmetic:return "Arithmetic";case ProgramKind::Algebra:return "Algebra";case ProgramKind::Calculus:return "Calculus";case ProgramKind::GuessingGame:return "Guessing Game";case ProgramKind::NumberSequence:return "Number Sequence";case ProgramKind::DiceGame:return "Dice Game";case ProgramKind::RandomWalk:return "Random Walk";}
 return "Generated BASIC";
}
GeneratedProgram ProgramGenerator::generate(ProgramKind k,std::size_t c){
 c=std::clamp<std::size_t>(c,1,12);
 switch(k){case ProgramKind::Arithmetic:return arithmetic(c);case ProgramKind::Algebra:return algebra(c);case ProgramKind::Calculus:return calculus(c);case ProgramKind::GuessingGame:return guessing(c);case ProgramKind::NumberSequence:return sequence(c);case ProgramKind::DiceGame:return dice(c);case ProgramKind::RandomWalk:return randomWalk(c);}
 throw std::runtime_error("unknown program kind");
}
std::vector<GeneratedProgram> ProgramGenerator::generatePack(std::size_t count,std::size_t c){
 std::vector<GeneratedProgram> out;out.reserve(count);
 constexpr ProgramKind types[]={ProgramKind::Arithmetic,ProgramKind::Algebra,ProgramKind::Calculus,ProgramKind::GuessingGame,ProgramKind::NumberSequence,ProgramKind::DiceGame,ProgramKind::RandomWalk};
 for(std::size_t i=0;i<count;++i)out.push_back(generate(types[i%(sizeof(types)/sizeof(types[0]))],c));
 return out;
}
GeneratedProgram ProgramGenerator::arithmetic(std::size_t c){
 std::ostringstream s;int n=pick(2,10);
 s<<"10 REM GENERATED ARITHMETIC EXERCISE\n20 PRINT \"ARITHMETIC WORKSHOP\"\n";
 s<<"30 LET A = "<<pick(2,30)<<"\n40 LET B = "<<pick(2,30)<<"\n50 PRINT \"A+B=\";A+B\n60 PRINT \"A*B=\";A*B\n";
 for(std::size_t i=0;i<c;++i)s<<(70+10*i)<<" LET R"<<i<<" = (A+"<<pick(1,9)<<")*("<<n<<"-"<<(i%std::max(1,n-1))<<")\n";
 s<<(70+10*c)<<" PRINT \"DONE\"\n"<<(80+10*c)<<" END\n";
 return {"Arithmetic Workshop","Generated integer arithmetic expressions.",s.str()};
}
GeneratedProgram ProgramGenerator::algebra(std::size_t c){
 int a=pick(1,9),b=pick(1,15),x=pick(1,12);
 std::ostringstream s;s<<"10 REM GENERATED ALGEBRA EXERCISE\n20 PRINT \"SOLVE "<<a<<"*X+"<<b<<"\"\n30 PRINT \"WHAT IS THE VALUE WHEN X="<<x<<"?\"\n40 LET X = "<<x<<"\n50 LET Y = "<<a<<"*X+"<<b<<"\n60 PRINT \"Y=\";Y\n70 PRINT \"REARRANGED: X=(Y-"<<b<<")/"<<a<<"\"\n80 PRINT \"CHECK THE RESULT BY HAND\"\n90 END\n";
 for(std::size_t i=0;i<c;++i)s.insert(s.str().find("90 END"),"");
 return {"Algebra Lab","Linear equations and substitution using generated coefficients.",s.str()};
}
GeneratedProgram ProgramGenerator::calculus(std::size_t c){
 int a=pick(1,8),b=pick(0,12),x=pick(1,7),h=pick(1,3);
 std::ostringstream s;s<<"10 REM NUMERICAL CALCULUS EXERCISE\n20 PRINT \"FINITE DIFFERENCE APPROXIMATION\"\n30 LET A = "<<a<<"\n40 LET B = "<<b<<"\n50 LET X = "<<x<<"\n60 LET H = "<<h<<"\n";
 s<<"70 LET F1 = A*X*X+B\n80 LET F2 = A*(X+H)*(X+H)+B\n90 LET D = (F2-F1)/H\n100 PRINT \"APPROX DERIVATIVE=\";D\n";
 s<<"110 PRINT \"ANALYTIC DERIVATIVE IS 2*A*X\"\n120 PRINT \"ANALYTIC VALUE=\";2*A*X\n";
 for(std::size_t i=0;i<c;++i)s<<(130+10*i)<<" REM EXTRA CALCULUS STEP "<<i+1<<"\n";
 s<<(130+10*c)<<" END\n";
 return {"Calculus: Finite Difference","Approximates derivative using (f(x+h)-f(x))/h and compares to a known derivative.",s.str()};
}
GeneratedProgram ProgramGenerator::guessing(std::size_t c){
 int secret=pick(1,99);
 std::ostringstream s;s<<"10 REM GENERATED NUMBER GUESSING GAME\n20 PRINT \"GUESS THE NUMBER FROM 1 TO 99\"\n30 LET SECRET = "<<secret<<"\n40 INPUT G\n50 IF G = SECRET THEN 100\n60 IF G < SECRET THEN 80\n70 PRINT \"TOO HIGH\"\n75 GOTO 40\n80 PRINT \"TOO LOW\"\n90 GOTO 40\n100 PRINT \"CORRECT!\"\n110 END\n";
 if(c>5)s<<"REM COMPLEXITY LEVEL "<<c<<"\n";
 return {"Number Guess","A generated number guessing game.",s.str()};
}
GeneratedProgram ProgramGenerator::sequence(std::size_t c){
 int first=pick(0,8),step=pick(1,6);
 std::ostringstream s;s<<"10 REM GENERATED ARITHMETIC SEQUENCE\n20 LET N = "<<first<<"\n30 LET STEP = "<<step<<"\n40 LET I = 1\n50 PRINT N\n60 LET N = N+STEP\n70 LET I = I+1\n80 IF I < "<<(c+5)<<" THEN 50\n90 END\n";
 return {"Number Sequence","Prints a generated arithmetic progression.",s.str()};
}
GeneratedProgram ProgramGenerator::dice(std::size_t c){
 std::ostringstream s;s<<"10 REM GENERATED DICE SIMULATION\n20 PRINT \"DICE ROLL SIMULATOR\"\n30 INPUT N\n40 IF N < 1 THEN 100\n50 LET D1 = INT(RND(1)*6)+1\n60 LET D2 = INT(RND(1)*6)+1\n70 PRINT \"TOTAL=\";D1+D2\n80 LET N = N-1\n90 IF N > 0 THEN 50\n100 END\n";
 (void)c;
 return {"Dice Simulator","Repeated dice-throw example. Requires INT/RND runtime support to execute as written.",s.str()};
}
GeneratedProgram ProgramGenerator::randomWalk(std::size_t c){
 std::ostringstream s;s<<"10 REM GENERATED RANDOM WALK MODEL\n20 LET POS = 0\n30 LET I = 1\n40 INPUT STEPS\n50 IF I > STEPS THEN 100\n60 LET POS = POS+1\n70 PRINT \"POSITION=\";POS\n80 LET I = I+1\n90 GOTO 50\n100 END\n";
 (void)c;
 return {"Random Walk","A simple generated state-transition simulation.",s.str()};
}
}
