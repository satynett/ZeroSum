#include "poker/Deck.h"
#include <iostream>
int main() {
    Deck deck; deck.shuffle();
    std::cout<<"Poker card/deck smoke test (game loop is not wired yet)\n";
    for(int i=0;i<5;++i) std::cout<<deck.deal()<<' ';
    std::cout<<'\n';
    return 0;
}