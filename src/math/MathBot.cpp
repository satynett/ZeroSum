#include "MathBot.h"
#include <chrono>
#include <sstream>
MathBot::MathBot(std::string d):difficulty_(std::move(d)),rng_(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())){}
void MathBot::onMessage(const Message& m) {
    if(m.type!=MsgType::Question || m.fields.size()<2) return;
    std::string t=m.fields[1]; char op=0; int a=0,b=0; std::istringstream in(t); std::string sym,eq;
    if(!(in>>a>>sym>>b)) return; op=sym.empty()?0:sym[0];
    if(op=='+') answer_=a+b; else if(op=='-') answer_=a-b; else if(op=='*') answer_=a*b; else if(op=='/') {if(b==0)return; answer_=a/b;} else return;
    int chance=difficulty_=="easy"?60:difficulty_=="hard"?95:80;
    std::uniform_int_distribution<int> roll(1,100),delay(250,1600);
    if(roll(rng_)>chance) answer_+=(roll(rng_)%2?1:-1);
    due_=std::chrono::steady_clock::now()+std::chrono::milliseconds(delay(rng_)); pending_=true;
}
std::vector<Message> MathBot::update() {
    if(!pending_ || std::chrono::steady_clock::now()<due_) return {};
    pending_=false; return {{MsgType::Answer,{std::to_string(answer_)}}};
}