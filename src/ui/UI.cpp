// UI.cpp: shared menus, input prompts, and terminal game screens.
#include "UI.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/select.h>
#include <unistd.h>

using Clock = std::chrono::steady_clock;

// These functions start the actual game logic.
void runMathArenaLocal();
void runPokerLocal();

int readMenuChoice(const std::string& prompt, int low, int high) {
    for (;;) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return low;
        std::istringstream input(line);
        int value; char extra;
        if (input >> value && !(input >> extra) && value >= low && value <= high)
            return value;
        std::cout << "Enter a number from " << low << " to " << high << ".\n";
    }
}

bool readMathAnswerTimed(int seconds, int& answer, double& elapsed) {
    const auto started = Clock::now();
    const auto deadline = started + std::chrono::seconds(seconds);
    std::string line;
    for (;;) {
        const auto now = Clock::now();
        if (now >= deadline) {
            elapsed = std::chrono::duration<double>(now - started).count();
            std::cout << "\nTime's up!\n";
            return false;
        }
        const auto remaining = std::chrono::duration_cast<std::chrono::seconds>(
            deadline - now);
        timeval wait{static_cast<long>(std::min<std::int64_t>(1, remaining.count())), 0};
        fd_set inputSet;
        FD_ZERO(&inputSet);
        FD_SET(STDIN_FILENO, &inputSet);
        const int ready = select(STDIN_FILENO + 1, &inputSet, nullptr, nullptr, &wait);
        if (ready > 0) {
            if (!std::getline(std::cin, line)) {
                elapsed = std::chrono::duration<double>(Clock::now() - started).count();
                return false;
            }
            elapsed = std::chrono::duration<double>(Clock::now() - started).count();
            if (line == "q" || line == "Q") return false;
            std::istringstream parsed(line);
            char extra;
            if (parsed >> answer && !(parsed >> extra)) return true;
            std::cout << "Enter a whole number, or q to quit: ";
        } else if (ready < 0) {
            elapsed = std::chrono::duration<double>(Clock::now() - started).count();
            return false;
        }
    }
}

void printMatiksHeader(int mode, int difficulty, int human, int bot, int lives,
                 int streak, int round, Clock::time_point blitzEnd) {
    std::cout << "\n============================================\n"
              << "              M A T H I K S                 \n"
              << "============================================\n";
    const char* modes[] = {"", "SPRINT", "BLITZ", "SURVIVAL"};
    std::cout << modes[mode] << "  | Round " << round
              << "  | Difficulty " << difficulty << "\n"
              << "You " << human << "  :  " << bot << " MathBot";
    if (mode == 3) std::cout << "  | Lives " << lives << "/3";
    if (mode == 3) std::cout << "  | Streak " << streak;
    if (mode == 2) {
        const auto left = std::chrono::duration_cast<std::chrono::seconds>(
            blitzEnd - Clock::now()).count();
        std::cout << "  | Time left " << std::max<std::int64_t>(0, left) << "s";
    }
    std::cout << "\n--------------------------------------------\n";
}

int readPokerChoiceTimed(int low, int high, int seconds) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
    std::string line;
    for (;;) {
        if (std::chrono::steady_clock::now() >= deadline) {
            std::cout << "\nTime expired. Checking/calling.\n";
            return std::min(2, high);
        }
        fd_set input;
        FD_ZERO(&input);
        FD_SET(STDIN_FILENO, &input);
        timeval wait{1, 0};
        const int ready = select(STDIN_FILENO + 1, &input, nullptr, nullptr, &wait);
        if (ready > 0) {
            if (!std::getline(std::cin, line)) return std::min(2, high);
            std::istringstream in(line);
            int choice; char extra;
            if (in >> choice && !(in >> extra) && choice >= low && choice <= high) return choice;
            std::cout << "Choose " << low << "-" << high << ".\n";
        } else if (ready < 0) {
            return std::min(2, high);
        }
    }
}

std::string shortCard(Card c) {
    static const char* ranks[]={"?","?","2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    static const char* suits[]={"♣","♦","♥","♠"};
    const std::string rank = (c.rank >= 2 && c.rank <= 14) ? ranks[c.rank] : "?";
    const std::string suit = suits[static_cast<int>(c.suit)];
    const bool red = c.suit == Suit::Diamonds || c.suit == Suit::Hearts;
    return std::string(red ? "\033[31m" : "\033[37m") + "[ " + rank + " " + suit + " ]\033[0m";
}

void showCards(const std::vector<Card>& cards) {
    for (auto c:cards) std::cout << shortCard(c) << ' ';
}

void renderPokerTable(int handNumber, const std::string& opponentMove,
                      const std::vector<Card>& community, const std::vector<Card>& humanHole,
                      int pot, int humanStack, int botStack, int toCall) {
    std::cout << "\033[2J\033[H";
    std::cout << "\033[1;32m";
    std::cout << "╭────────────────────────────────────────────────────╮\n"
              << "│                 ♠  TEXAS HOLD'EM  ♥                │\n"
              << "╰────────────────────────────────────────────────────╯\033[0m\n";
    std::cout << "HAND " << handNumber << "                         🤖 PokerBot: " << opponentMove << "\n\n";
    std::cout << "                 OPPONENT  ·  $" << botStack << "\n";
    std::cout << "                 [ 🂠 ] [ 🂠 ]\n\n";
    std::cout << "          ";
    if (community.empty()) std::cout << "[  ·  ] [  ·  ] [  ·  ] [  ·  ] [  ·  ]";
    else {
        showCards(community);
        for (std::size_t i=community.size(); i<5; ++i) std::cout << "[  ·  ] ";
    }
    std::cout << "\n\n";
    std::cout << "                    ┌──────────┐\n"
              << "                    │  POT $" << pot;
    if (pot < 100) std::cout << "   ";
    else if (pot < 1000) std::cout << "  ";
    else std::cout << " ";
    std::cout << "│\n                    └──────────┘\n\n";
    std::cout << "                      YOU  ·  $" << humanStack << "\n";
    std::cout << "                 ";
    showCards(humanHole);
    std::cout << "\n\n";
    std::cout << "\033[1;33m";
    std::cout << "  1  FOLD       2  " << (toCall == 0 ? "CHECK" : "CALL $" + std::to_string(std::min(toCall,humanStack))) << "\n";
    std::cout << "\033[0m";
}

// Keep the main menu separate from the rules of either game.
void runGameHubUI() {
    for (;;) {
        std::cout << "\n=== C++17 Terminal Game Hub ===\n"
                  << "1. Matiks vs MathBot\n"
                  << "2. Texas Hold'em vs bot\n"
                  << "0. Exit\nChoice: ";
        std::string choice;
        if (!std::getline(std::cin, choice)) return;

        if (choice == "1") runMathArenaLocal();
        else if (choice == "2") runPokerLocal();
        else if (choice == "0" || choice == "q") return;
        else std::cout << "Choose 1, 2, or 0.\n";
    }
}
