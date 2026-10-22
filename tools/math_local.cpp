#include "math/MathGame.h"
#include <chrono>
#include <iostream>
#include <thread>
int main(int argc,char** argv) {
    auto game=makeMathGame(argc>1?argv[1]:"sprint");
    Player p{1,"You",true,true,0};
    auto print=[](const Outbox& out) {
        for(const auto& o:out) {
            if(o.msg.type==MsgType::Question && o.msg.fields.size()>=2)
                std::cout<<"Q"<<o.msg.fields[0]<<": "<<o.msg.fields[1]<<"\n";
            else std::cout<<serialize(o.msg)<<"\n";
        }
    };
    print(game->start({p}));
    std::string line;
    while(!game->isFinished()) {
        std::cout<<"Answer (or Ctrl-D to quit): "<<std::flush;
        if(!std::getline(std::cin,line)) break;
        print(game->handleInput(1,parse("ANSWER|"+line)));
        print(game->update());
    }
    return 0;
}