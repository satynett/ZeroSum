#include "math/MathGame.h"
#include <chrono>
#include <iostream>
#include <thread>
int main(int argc,char** argv) {
    std::string mode=argc>1?argv[1]:"sprint";
    auto game=makeMathGame(mode);
    Player p{1,"You",true,true,0};
    auto print=[](const Outbox& out) {
        for(const auto& o:out) {
            const auto& m=o.msg;
            if(m.type==MsgType::Question && m.fields.size()>=2) std::cout<<"Q"<<m.fields[0]<<": "<<m.fields[1]<<"\n";
            else std::cout<<serialize(m)<<"\n";
        }
    };
    print(game->start({p}));
    std::string line;
    while(!game->isFinished()) {
        if(std::cin.rdbuf()->in_avail()>0 && std::getline(std::cin,line)) print(game->handleInput(1,parse("ANSWER|"+line)));
        print(game->update());
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if(!std::cin && std::cin.eof()) break;
    }
    return 0;
}