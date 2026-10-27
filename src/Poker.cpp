// Poker.cpp: Texas Hold'em dealing, betting, showdown, and bot decisions.
#include "ui/UI.h"

#include "poker/Deck.h"
#include "poker/HandEvaluator.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <iomanip>
#include <sys/select.h>
#include <unistd.h>


using namespace std;
namespace {
std::mt19937& randomEngine() {
    static std::mt19937 rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));
    return rng;
}






double estimateEquity(const std::vector<Card>& hole, const std::vector<Card>& board, int trials = 250) {
    if (board.size() == 5) {
        std::vector<Card> known = hole;
        known.insert(known.end(), board.begin(), board.end());
        std::vector<Card> deck;
        for (int s=0; s<4; ++s) for (int r=2; r<=14; ++r) {
            Card card{static_cast<Suit>(s), r};
            if (std::find(known.begin(), known.end(), card) == known.end()) deck.push_back(card);
        }
        std::shuffle(deck.begin(), deck.end(), randomEngine());
        int wins=0, ties=0;
        for (int i=0; i<trials && deck.size()>=2; ++i) {
            std::vector<Card> mine=known, theirs{deck[i%deck.size()],deck[(i+1)%deck.size()]};
            theirs.insert(theirs.end(), board.begin(), board.end());
            const auto myRank=HandEvaluator::evaluate(mine);
            const auto theirRank=HandEvaluator::evaluate(theirs);
            if (theirRank < myRank) ++wins;
            else if (theirRank == myRank) ++ties;
        }
        return (wins + ties*0.5) / std::max(1,trials);
    }
    int wins=0, ties=0;
    std::vector<Card> known=hole;
    known.insert(known.end(),board.begin(),board.end());
    std::vector<Card> remaining;
    for(int s=0;s<4;++s) for(int r=2;r<=14;++r) {
        Card card{static_cast<Suit>(s),r};
        if(std::find(known.begin(),known.end(),card)==known.end()) remaining.push_back(card);
    }
    for(int t=0;t<trials;++t) {
        std::shuffle(remaining.begin(),remaining.end(),randomEngine());
        if(remaining.size()<7-board.size()) break;
        std::vector<Card> myHole=hole, theirHole{remaining[0],remaining[1]};
        std::vector<Card> runout=board;
        std::size_t idx=2;
        while(runout.size()<5) runout.push_back(remaining[idx++]);
        std::vector<Card> mine=myHole, theirs=theirHole;
        mine.insert(mine.end(),runout.begin(),runout.end());
        theirs.insert(theirs.end(),runout.begin(),runout.end());
        const auto myRank=HandEvaluator::evaluate(mine);
        const auto theirRank=HandEvaluator::evaluate(theirs);
        if(theirRank<myRank) ++wins;
        else if(theirRank==myRank) ++ties;
    }
    return (wins + ties*0.5) / std::max(1,trials);
}
int preflopTier(const std::vector<Card>& hole) {
    const int a=std::max(hole[0].rank,hole[1].rank), b=std::min(hole[0].rank,hole[1].rank);
    const bool pair=a==b, suited=hole[0].suit==hole[1].suit, connected=a-b<=2;
    if(pair && a>=10) return 1;
    if((a==14 && b>=10) || (pair && a>=7) || (a==14 && b>=2 && suited)) return 2;
    if(pair || (suited && connected) || (a>=11 && b>=9)) return 3;
    return 4;
}



}

void runPokerLocal() {
    std::cout << "\nStarting Texas Hold'em...\n";
    int humanStack=1000, botStack=1000;
    int handNumber=0;
    while(humanStack>0 && botStack>0) {
        ++handNumber;
        Deck deck; deck.shuffle();
        std::vector<Card> humanHole{deck.deal(),deck.deal()};
        std::vector<Card> botHole{deck.deal(),deck.deal()};
        std::vector<Card> community;
        int pot=0, humanBet=0, botBet=0;
        const bool humanButton=(handNumber%2==1);
        auto putChips=[&](int& stack,int& bet,int amount) {
            amount=std::max(0,std::min(amount,stack)); stack-=amount; bet+=amount; pot+=amount;
        };
        if(humanButton) {putChips(humanStack,humanBet,10);putChips(botStack,botBet,20);}
        else {putChips(botStack,botBet,10);putChips(humanStack,humanBet,20);}
        bool humanFolded=false, botFolded=false;
        std::string opponentMove = "posts blind";
        for(int street=0;street<4 && !humanFolded && !botFolded;++street) {
            if(street>0) { humanBet=0; botBet=0; }
            if(street==1 || street==2 || street==3) {
                if(street==1) {community.push_back(deck.deal());community.push_back(deck.deal());community.push_back(deck.deal());}
                else community.push_back(deck.deal());
                std::cout << (street==1?"Flop: ":street==2?"Turn: ":"River: ");showCards(community);std::cout<<"\n";
            }
            const int target=std::max(humanBet,botBet);
            const bool humanToAct=true; // Hot-seat local version always prompts the human first.
            if(humanToAct && humanStack>0) {
                const int toCall = std::max(0,target-humanBet);
                const int maxTotalBet = humanBet + humanStack;
                std::vector<int> raiseTotals;
                if(maxTotalBet >= target + 20) raiseTotals.push_back(target + 20);
                if(maxTotalBet >= target + 40) raiseTotals.push_back(target + 40);
                renderPokerTable(handNumber, opponentMove, community, humanHole, pot, humanStack, botStack, toCall);
                int nextOption = 3;
                for(int total : raiseTotals) {
                    std::cout << "  " << nextOption++ << ". Raise to " << total << "\n";
                }
                const int allInOption = nextOption;
                std::cout << "  " << allInOption << ". All-in (" << humanStack << ")\n";
                const int choice = readPokerChoiceTimed(1,allInOption,30);
                if(choice==1) { humanFolded=true; continue; }
                if(choice==2) putChips(humanStack,humanBet,toCall);
                else if(choice>=3 && choice<allInOption) {
                    const int total = raiseTotals[static_cast<std::size_t>(choice-3)];
                    putChips(humanStack,humanBet,total-humanBet);
                } else putChips(humanStack,humanBet,humanStack);
            }
            if(!humanFolded && botStack>0) {
                std::uniform_int_distribution<int> thinkingTime(1200, 5000);
                std::this_thread::sleep_for(std::chrono::milliseconds(thinkingTime(randomEngine())));
                const int currentTarget=std::max(humanBet,botBet);
                const int toCall=std::max(0,currentTarget-botBet);
                const double equity = community.empty()
                    ? (preflopTier(botHole)==1 ? 0.72 : preflopTier(botHole)==2 ? 0.62 : preflopTier(botHole)==3 ? 0.50 : 0.38)
                    : estimateEquity(botHole, community, 220);
                const double potOdds = toCall == 0 ? 0.0 :
                    static_cast<double>(toCall) / std::max(1, pot + toCall);
                std::uniform_real_distribution<double> bluffRoll(0.0,1.0);
                const bool canBluff = community.size() >= 3 && equity < 0.45 &&
                    bluffRoll(randomEngine()) < (community.size()==3 ? 0.16 : 0.09);
                if(toCall > botStack) {
                    putChips(botStack,botBet,botStack);
                    opponentMove="calls all-in";
                } else if(canBluff && botStack > toCall + 40) {
                    putChips(botStack,botBet,std::min(botStack,toCall+40));
                    opponentMove="bluffs / raises";
                } else if(equity >= potOdds + 0.18 && botStack > toCall + 40 &&
                          std::uniform_int_distribution<int>(1,100)(randomEngine()) <= 45) {
                    putChips(botStack,botBet,std::min(botStack,toCall+40));
                    opponentMove="raises for value";
                } else if(toCall == 0) {
                    if(equity >= 0.58 && botStack >= 40 &&
                       std::uniform_int_distribution<int>(1,100)(randomEngine()) <= 30) {
                        putChips(botStack,botBet,40);
                        opponentMove="bets";
                    } else {
                        opponentMove="checks";
                    }
                } else if(equity >= potOdds + 0.02) {
                    putChips(botStack,botBet,toCall);
                    opponentMove="calls";
                } else {
                    botFolded=true;
                    opponentMove="folds";
                }
                std::cout << "\nPokerBot " << opponentMove << ".\n";
            }
            // A single betting exchange per street keeps this lab version intentionally simple.
            if(humanStack==0 || botStack==0) break;
        }
        if(humanFolded) {botStack+=pot;std::cout<<"You folded. Bot wins pot "<<pot<<".\n";}
        else if(botFolded) {humanStack+=pot;std::cout<<"Bot folded. You win pot "<<pot<<".\n";}
        else {
            std::vector<Card> h=humanHole,b=botHole;
            h.insert(h.end(),community.begin(),community.end());b.insert(b.end(),community.begin(),community.end());
            // If someone is all-in early, finish the board.
            while(community.size()<5){community.push_back(deck.deal());}
            h=humanHole;h.insert(h.end(),community.begin(),community.end());
            b=botHole;b.insert(b.end(),community.begin(),community.end());
            auto hr=HandEvaluator::evaluate(h), br=HandEvaluator::evaluate(b);
            std::cout<<"Showdown | Your hand category "<<static_cast<int>(hr.category)
                     <<" | Bot category "<<static_cast<int>(br.category)<<"\n";
            if(br<hr){humanStack+=pot;std::cout<<"You win the pot of "<<pot<<"!\n";}
            else if(hr<br){botStack+=pot;std::cout<<"PokerBot wins the pot of "<<pot<<".\n";}
            else {humanStack+=pot/2;botStack+=pot-pot/2;std::cout<<"Split pot.\n";}
            std::cout<<"Bot cards: ";showCards(botHole);std::cout<<"\n";
        }
        std::cout<<"Stacks — You: "<<humanStack<<" | PokerBot: "<<botStack<<"\n";
        if(humanStack<=0 || botStack<=0) break;
        std::cout<<"Press Enter for next hand, or type q to quit: ";
        std::string next;if(!std::getline(std::cin,next)||next=="q"||next=="Q")break;
    }
    std::cout<<"\nPoker session ended. Final stacks — You: "<<humanStack<<" | PokerBot: "<<botStack<<"\n";
    if(humanStack>botStack)std::cout<<"You win!\n";else if(humanStack<botStack)std::cout<<"PokerBot wins.\n";else std::cout<<"Draw.\n";
}