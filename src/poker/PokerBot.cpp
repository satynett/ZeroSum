#include "PokerBot.h"
#include <chrono>
PokerBot::PokerBot():rng_(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())){}
void PokerBot::onMessage(const Message& m) {
    if(m.type!=MsgType::State || m.fields.size()<6) return;
    // State contract: phase, pot, community cards, your cards, player summary, whose turn.
    const std::string& phase=m.fields[0];
    const std::string& hole=m.fields[3];
    std::string action="check";
    std::uniform_int_distribution<int> roll(1,100),delay(300,900);
    const bool bluff=roll(rng_)<=12;
    if(phase=="preflop") {
        // Simple text heuristic; card parsing is intentionally kept separate from game rules.
        bool high=hole.find('A')!=std::string::npos || hole.find('K')!=std::string::npos || hole.find('Q')!=std::string::npos;
        bool pair=hole.size()>=4 && hole[0]==hole[2];
        bool suited=hole.size()>=4 && hole[1]==hole[3];
        if(pair || (high && hole.find('A')!=std::string::npos && hole.find('K')!=std::string::npos)) action="raise|40";
        else if(high || suited || bluff) action="call";
        else action="fold";
    } else {
        if(bluff) action="raise|40";
        else if(hole.find('A')!=std::string::npos || hole.find('K')!=std::string::npos) action="call";
        else action="check";
    }
    due_=std::chrono::steady_clock::now()+std::chrono::milliseconds(delay(rng_));
    pending_={{MsgType::PokerAction,{action}}}; scheduled_=true;
}
std::vector<Message> PokerBot::update(){if(!scheduled_||std::chrono::steady_clock::now()<due_)return {};scheduled_=false;return pending_;}