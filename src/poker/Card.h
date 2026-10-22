#pragma once
#include <ostream>
enum class Suit { Clubs, Diamonds, Hearts, Spades };
struct Card {
    Suit suit = Suit::Clubs;
    int rank = 2; // 2..14 (Ace)
    bool operator==(const Card& other) const { return suit==other.suit && rank==other.rank; }
    bool operator<(const Card& other) const { return rank<other.rank || (rank==other.rank && suit<other.suit); }
};
std::ostream& operator<<(std::ostream& out, const Card& card);
