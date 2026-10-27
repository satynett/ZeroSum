// Compile-time polymorphism: overloading operator<< for Card lets a Card use stream-printing syntax like built-in types.
#include "Card.h"
#include <string>

using namespace std;
// Overload for Card: this is compile-time polymorphism (operator overloading).
std::ostream& operator<<(std::ostream& out,const Card& c) {
    static const char* ranks="??23456789TJQKA";
    static const char* suits[]={"♣","♦","♥","♠"};
    if(c.rank>=2 && c.rank<=14) out<<ranks[c.rank];
    out<<suits[static_cast<int>(c.suit)]; return out;
}