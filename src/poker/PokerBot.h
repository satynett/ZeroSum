#pragma once
#include "net/Protocol.h"
#include <chrono>
#include <random>
#include <vector>
class PokerBot {
public:
    PokerBot();
    void onMessage(const Message& message);
    std::vector<Message> update();
private:
    std::mt19937 rng_;
    std::chrono::steady_clock::time_point due_{};
    std::vector<Message> pending_;
    bool scheduled_=false;
};