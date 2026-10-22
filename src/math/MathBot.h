#pragma once
#include "net/Protocol.h"
#include <chrono>
#include <random>
#include <string>
#include <vector>
class MathBot {
public:
    explicit MathBot(std::string difficulty = "medium");
    void onMessage(const Message& message);
    std::vector<Message> update();
private:
    std::string difficulty_;
    std::mt19937 rng_;
    std::chrono::steady_clock::time_point due_{};
    int answer_ = 0;
    bool pending_ = false;
};