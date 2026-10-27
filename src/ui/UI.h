#pragma once
#include "poker/Card.h"
#include <chrono>
#include <string>
#include <vector>

// Shared input and display functions used by the two games.
int readMenuChoice(const std::string& prompt, int low, int high);
bool readMathAnswerTimed(int seconds, int& answer, double& elapsed);
void printMatiksHeader(int mode, int difficulty, int human, int bot, int lives, int streak, int round, std::chrono::steady_clock::time_point blitzEnd);
int readPokerChoiceTimed(int low, int high, int seconds);
std::string shortCard(Card card);
void showCards(const std::vector<Card>& cards);
void renderPokerTable(int handNumber, const std::string& opponentMove, const std::vector<Card>& community, const std::vector<Card>& humanHole, int pot, int humanStack, int botStack, int toCall);
void runGameHubUI();
