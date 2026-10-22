#pragma once
#include <random>
#include <string>
struct Question { std::string text; int answer = 0; int difficulty = 1; };
class QuestionGenerator {
public:
    QuestionGenerator();
    Question generate(int difficulty);
private:
    std::mt19937 rng_;
};