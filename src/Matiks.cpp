// Matiks.cpp: questions, difficulty, scoring, and MathBot turns.
#include "math/QuestionGenerator.h"
#include "ui/UI.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <sys/select.h>
#include <unistd.h>

// Helper functions used by Matiks to generate bot answers and measure time.

using namespace std;
namespace {
using Clock = std::chrono::steady_clock;

std::mt19937& rng() {
    static std::mt19937 engine(static_cast<unsigned>(
        Clock::now().time_since_epoch().count()));
    return engine;
}





int botAnswer(const Question& q, int accuracy) {
    std::uniform_int_distribution<int> roll(1, 100);
    if (roll(rng()) <= accuracy) return q.answer;
    std::uniform_int_distribution<int> mistake(1, 3);
    return q.answer + (roll(rng()) % 2 ? mistake(rng()) : -mistake(rng()));
}


}

// Run the full Matiks match from the mode menu to the final score.
void runMathArenaLocal() {
    std::cout << "\n          M A T H I K S\n"
              << "      Quick maths. Beat the bot.\n\n"
              << "1. Sprint   - first to 10 points, 8 seconds per question\n"
              << "2. Blitz    - score as much as you can in 2 minutes\n"
              << "3. Survival - 3 lives, difficulty rises every 3 correct answers\n";
    const int mode = readMenuChoice("Choose mode (1-3): ", 1, 3);
    std::cout << "\nBot difficulty:\n1. Easy (60% accuracy)\n"
              << "2. Medium (80% accuracy)\n3. Hard (95% accuracy)\n";
    const int botLevel = readMenuChoice("Choose bot (1-3): ", 1, 3);
    const int accuracy = botLevel == 1 ? 60 : botLevel == 2 ? 80 : 95;

    QuestionGenerator generator;
    int humanScore = 0, botScore = 0, lives = 3, streak = 0, difficulty = 1, round = 0;
    const auto blitzEnd = Clock::now() + std::chrono::minutes(2);
    bool quit = false;

    // Each loop is one question and one chance for the bot to score.
    while (!quit) {
        if (mode == 1 && (humanScore >= 10 || botScore >= 10)) break;
        if (mode == 2 && Clock::now() >= blitzEnd) break;
        if (mode == 3 && lives <= 0) break;

        ++round;
        if (mode == 2) difficulty = std::min(5, 1 + humanScore / 4);
        Question q = generator.generate(difficulty);
        printMatiksHeader(mode, difficulty, humanScore, botScore, lives, streak, round, blitzEnd);
        std::cout << "\n  " << q.text << "\n";
        if (mode == 1 || mode == 3) std::cout << "  You have 8 seconds.\n";
        std::cout << "  Answer (or q to quit): " << std::flush;

        int answer = 0;
        double elapsed = 0.0;
        const bool gotAnswer = readMathAnswerTimed(mode == 2 ? 8 : 8, answer, elapsed);
        if (!gotAnswer) {
            if (!std::cin || elapsed > 8.0) {
                if (mode == 3 && elapsed > 8.0) {
                    --lives; streak = 0;
                    std::cout << "Life lost. Correct answer: " << q.answer << "\n";
                } else if (mode != 2 || elapsed > 8.0) {
                    if (mode == 1) std::cout << "No point this round. Correct answer: " << q.answer << "\n";
                } else {
                    quit = true;
                    break;
                }
            } else {
                quit = true;
                break;
            }
        } else if (answer == q.answer) {
            ++humanScore;
            ++streak;
            std::cout << "Correct! +1 point";
            if (elapsed < 2.0) std::cout << "  (fast!)";
            std::cout << "\n";
            if (mode == 3 && streak % 3 == 0) {
                difficulty = std::min(5, difficulty + 1);
                std::cout << "Difficulty increased to " << difficulty << "!\n";
            }
        } else {
            streak = 0;
            std::cout << "Not quite. Correct answer: " << q.answer << "\n";
            if (mode == 3) {
                --lives;
                std::cout << "Life lost. " << lives << " remaining.\n";
            }
        }

        if (mode == 3 && lives <= 0) break;
        std::uniform_int_distribution<int> think(250, 1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(think(rng())));
        if (botAnswer(q, accuracy) == q.answer) {
            ++botScore;
            std::cout << "MathBot got it right. +1 bot point.\n";
        } else {
            std::cout << "MathBot made a mistake.\n";
        }
        if (mode == 2 && Clock::now() >= blitzEnd) break;
    }

    std::cout << "\n============================================\n"
              << "                FINAL SCORE                 \n"
              << "============================================\n"
              << "You: " << humanScore << "   |   MathBot: " << botScore << "\n";
    if (mode == 3 && lives <= 0)
        std::cout << "Survival over — you ran out of lives.\n";
    if (humanScore > botScore) std::cout << "YOU WIN! 🏆\n";
    else if (humanScore < botScore) std::cout << "MathBot wins this time.\n";
    else std::cout << "It's a draw!\n";
    std::cout << "Press Enter to return to the game hub...";
    std::string ignored;
    std::getline(std::cin, ignored);
}
