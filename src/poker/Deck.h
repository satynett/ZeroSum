#pragma once
#include "Card.h"
#include <random>
#include <vector>
class Deck {
public:
    Deck();
    void shuffle();
    Card deal();
    std::size_t remaining() const { return cards_.size()-next_; }
private:
    std::vector<Card> cards_;
    std::size_t next_=0;
    std::mt19937 rng_;
};