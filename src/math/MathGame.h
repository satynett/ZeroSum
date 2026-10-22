#pragma once
#include "core/Game.h"
#include "QuestionGenerator.h"
#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <vector>

class MathGame : public Game {
public:
    explicit MathGame(std::string mode);
    std::string name() const override;
    Outbox start(const std::vector<Player>& players) override;
    Outbox handleInput(int playerId, const Message& msg) override;
    Outbox update() override;
    Outbox onPlayerDisconnected(int playerId) override;
    bool isFinished() const override { return finished_; }
    std::vector<Player> finalStandings() const override;
protected:
    void nextQuestion();
    Outbox broadcastQuestion(int playerId = -1);
    Outbox scoreboard() const;
    std::string mode_;
    QuestionGenerator generator_;
    std::vector<Player> players_;
    std::map<int,int> difficulty_, lives_, correct_;
    Question current_;
    std::chrono::steady_clock::time_point deadline_, gameDeadline_;
    bool finished_ = false;
    int questionIndex_ = 0;
    int currentPlayer_ = 0;
};
class SprintMode final : public MathGame { public: SprintMode():MathGame("sprint"){} };
class BlitzMode final : public MathGame { public: BlitzMode():MathGame("blitz"){} };
class SurvivalMode final : public MathGame { public: SurvivalMode():MathGame("survival"){} };
std::unique_ptr<Game> makeMathGame(const std::string& mode);