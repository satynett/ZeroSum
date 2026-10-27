#include "poker/HandEvaluator.h"
#include <cassert>
#include <vector>

using namespace std;
using C=Card;
static std::vector<C> hand(std::initializer_list<C> x){return std::vector<C>(x);}
int main(){
 auto royal=HandEvaluator::evaluate(hand({{Suit::Spades,10},{Suit::Spades,11},{Suit::Spades,12},{Suit::Spades,13},{Suit::Spades,14},{Suit::Clubs,2},{Suit::Diamonds,3}}));
 auto wheel=HandEvaluator::evaluate(hand({{Suit::Spades,14},{Suit::Clubs,2},{Suit::Hearts,3},{Suit::Diamonds,4},{Suit::Spades,5},{Suit::Clubs,9},{Suit::Diamonds,11}}));
 auto pair=HandEvaluator::evaluate(hand({{Suit::Spades,2},{Suit::Clubs,2},{Suit::Hearts,4},{Suit::Diamonds,7},{Suit::Spades,9},{Suit::Clubs,11},{Suit::Diamonds,13}}));
 auto high=HandEvaluator::evaluate(hand({{Suit::Spades,3},{Suit::Clubs,5},{Suit::Hearts,7},{Suit::Diamonds,9},{Suit::Spades,11},{Suit::Clubs,13},{Suit::Diamonds,14}}));
 assert(high<pair); assert(pair<wheel); assert(wheel<royal);
 assert(royal==HandEvaluator::evaluate(hand({{Suit::Spades,10},{Suit::Spades,11},{Suit::Spades,12},{Suit::Spades,13},{Suit::Spades,14},{Suit::Clubs,4},{Suit::Diamonds,6}})));
 auto flush=HandEvaluator::evaluate(hand({{Suit::Hearts,2},{Suit::Hearts,4},{Suit::Hearts,6},{Suit::Hearts,8},{Suit::Hearts,10},{Suit::Clubs,3},{Suit::Diamonds,5}}));
 assert(wheel<flush);
 // Additional category ordering checks.
 auto twoPair=HandEvaluator::evaluate(hand({{Suit::Spades,3},{Suit::Clubs,3},{Suit::Hearts,8},{Suit::Diamonds,8},{Suit::Spades,10},{Suit::Clubs,12},{Suit::Diamonds,14}}));
 assert(pair<twoPair); assert(twoPair<flush); assert(royal==royal);
 // At least 15 pairwise ordering checks, including the wheel straight.
 assert(high<twoPair); assert(high<wheel); assert(high<flush); assert(high<royal);
 assert(pair<wheel); assert(pair<flush); assert(pair<royal);
 assert(twoPair<wheel); assert(twoPair<royal); assert(wheel<royal);
 assert(flush<royal); assert(twoPair<flush); assert(pair<twoPair);
 assert(wheel<flush); assert(high<pair);
}