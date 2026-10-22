#pragma once
#include "Card.h"
#include <vector>
enum class HandCategory { HighCard, OnePair, TwoPair, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraightFlush, RoyalFlush };
struct HandRank {
    HandCategory category=HandCategory::HighCard;
    std::vector<int> tieBreak;
    bool operator==(const HandRank& o) const { return category==o.category && tieBreak==o.tieBreak; }
    bool operator<(const HandRank& o) const { return category<o.category || (category==o.category && tieBreak<o.tieBreak); }
};
class HandEvaluator {
public:
    static HandRank evaluate(const std::vector<Card>& seven);
};