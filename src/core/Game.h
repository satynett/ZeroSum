#pragma once
#include <string>
#include <vector>
#include "net/Protocol.h"

struct Player {
    int id = -1;
    std::string name;
    bool connected = true;
    bool ready = false;
    int score = 0;
};
struct Outgoing { int toPlayer = -1; Message msg; };
using Outbox = std::vector<Outgoing>;

class Game {
public:
    virtual ~Game() = default;
    virtual std::string name() const = 0;
    virtual Outbox start(const std::vector<Player>& players) = 0;
    virtual Outbox handleInput(int playerId, const Message& msg) = 0;
    virtual Outbox update() = 0;
    virtual Outbox onPlayerDisconnected(int playerId) = 0;
    virtual bool isFinished() const = 0;
    virtual std::vector<Player> finalStandings() const = 0;
};