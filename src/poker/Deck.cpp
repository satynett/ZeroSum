#include "Deck.h"
#include <algorithm>
#include <chrono>
#include <stdexcept>
Deck::Deck():rng_(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())) {
    for(int s=0;s<4;++s) for(int r=2;r<=14;++r) cards_.push_back({static_cast<Suit>(s),r});
}
void Deck::shuffle(){ std::shuffle(cards_.begin(),cards_.end(),rng_); next_=0; }
Card Deck::deal(){ if(next_>=cards_.size()) throw std::out_of_range("Deck is empty"); return cards_[next_++]; }