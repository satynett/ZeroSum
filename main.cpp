#include "src/local/LocalGames.h"
#include <iostream>
#include <string>
int main() {
    for (;;) {
        std::cout<<"\n=== C++17 Terminal Game Hub ===\n"
                 <<"1. Math Game vs bot\n"
                 <<"2. Texas Hold'em vs bot\n"
                 <<"0. Exit\nChoice: ";
        std::string choice;
        if(!std::getline(std::cin,choice)) return 0;
        if(choice=="1") runMathLocal();
        else if(choice=="2") runPokerLocal();
        else if(choice=="0" || choice=="q") return 0;
        else std::cout<<"Choose 1, 2, or 0.\n";
    }
}