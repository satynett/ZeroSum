#include "Card.h"
#include <string>
std::ostream& operator<<(std::ostream& out,const Card& c) {
    static const char* ranks="??23456789TJQKA";
    static const char* suits[]={"♣","♦","♥","♠"};
    if(c.rank>=2 && c.rank<=14) out<<ranks[c.rank];
    out<<suits[static_cast<int>(c.suit)]; return out;
}