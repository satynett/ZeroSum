#include "QuestionGenerator.h"
#include <algorithm>
#include <chrono>
#include <stdexcept>
QuestionGenerator::QuestionGenerator() : rng_(static_cast<unsigned>(
    std::chrono::steady_clock::now().time_since_epoch().count())) {}
Question QuestionGenerator::generate(int difficulty) {
    difficulty = std::max(1, std::min(5, difficulty));
    const int maxN = difficulty * 10;
    std::uniform_int_distribution<int> number(0, maxN), op(0, difficulty < 3 ? 1 : 3);
    int a = number(rng_), b = number(rng_), ans = 0;
    const int choice = op(rng_);
    char symbol = '+';
    if (choice == 0) { ans = a + b; symbol = '+'; }
    else if (choice == 1) { if (a < b) std::swap(a,b); ans = a-b; symbol='-'; }
    else if (choice == 2) { ans = a*b; symbol='*'; }
    else { b = std::max(1,b); ans = a; a = a*b; symbol='/'; }
    return {std::to_string(a)+" "+symbol+" "+std::to_string(b)+" = ?", ans, difficulty};
}