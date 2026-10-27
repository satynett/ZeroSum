#include "HandEvaluator.h"
#include <algorithm>
#include <map>
#include <stdexcept>

using namespace std;
namespace {
int straightHigh(std::vector<int> ranks) {
    std::sort(ranks.begin(),ranks.end()); ranks.erase(std::unique(ranks.begin(),ranks.end()),ranks.end());
    if(std::find(ranks.begin(),ranks.end(),14)!=ranks.end()) ranks.insert(ranks.begin(),1);
    int run=1;
    for(std::size_t i=1;i<ranks.size();++i) { if(ranks[i]==ranks[i-1]+1) { if(++run>=5) return ranks[i]; } else run=1; }
    return 0;
}
}
HandRank HandEvaluator::evaluate(const std::vector<Card>& cards) {
    if(cards.size()!=7) throw std::invalid_argument("HandEvaluator expects exactly 7 cards");
    std::map<int,int,std::greater<int>> counts; std::map<Suit,std::vector<int>> suits; std::vector<int> ranks;
    for(auto c:cards){++counts[c.rank]; suits[c.suit].push_back(c.rank); ranks.push_back(c.rank);}
    for(auto& s:suits) if(s.second.size()>=5) { int h=straightHigh(s.second); if(h) return {h==14?HandCategory::RoyalFlush:HandCategory::StraightFlush,{h}}; }
    std::vector<int> quads,trips,pairs;
    for(auto [r,n]:counts) { if(n==4) quads.push_back(r); else if(n==3) trips.push_back(r); else if(n==2) pairs.push_back(r); }
    if(!quads.empty()){int k=0;for(auto [r,n]:counts)if(r!=quads[0]){k=r;break;}return {HandCategory::FourOfAKind,{quads[0],k}};}
    if(!trips.empty() && (!pairs.empty() || trips.size()>1)) return {HandCategory::FullHouse,{trips[0],trips.size()>1?trips[1]:pairs[0]}};
    for(auto& s:suits) if(s.second.size()>=5){auto v=s.second;std::sort(v.rbegin(),v.rend());v.resize(5);return {HandCategory::Flush,v};}
    if(int h=straightHigh(ranks)) return {HandCategory::Straight,{h}};
    if(!trips.empty()){std::vector<int> t{trips[0]};for(auto [r,n]:counts)if(r!=trips[0]&&t.size()<3)t.push_back(r);return {HandCategory::ThreeOfAKind,t};}
    if(pairs.size()>=2){int k=0;for(auto [r,n]:counts)if(r!=pairs[0]&&r!=pairs[1]){k=r;break;}return {HandCategory::TwoPair,{pairs[0],pairs[1],k}};}
    if(!pairs.empty()){std::vector<int> t{pairs[0]};for(auto [r,n]:counts)if(r!=pairs[0]&&t.size()<4)t.push_back(r);return {HandCategory::OnePair,t};}
    std::sort(ranks.rbegin(),ranks.rend());ranks.erase(std::unique(ranks.begin(),ranks.end()),ranks.end());ranks.resize(5);return {HandCategory::HighCard,ranks};
}