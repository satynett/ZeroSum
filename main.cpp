#include <iostream>
#include <string>
int main() {
    std::cout<<"C++17 Multiplayer Game Hub\n"
             <<"1. Host (network server module pending)\n"
             <<"2. Join (network client module pending)\n"
             <<"3. Play local Math Game\n"
             <<"4. Poker card/deck smoke test\n"
             <<"Choice: ";
    int choice=0; if(!(std::cin>>choice)) return 0;
    if(choice==3) { std::cout<<"Run tools/math_local sprint|blitz|survival to play.\n"; }
    else if(choice==4) { std::cout<<"Run tools/poker_local to test the deck.\n"; }
    else std::cout<<"Network lobby is not implemented yet.\n";
    return 0;
}