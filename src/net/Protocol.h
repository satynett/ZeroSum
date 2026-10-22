#pragma once
#include <iosfwd>
#include <string>
#include <vector>

enum class MsgType {
    Unknown, Join, SelectGame, StartGame, AddBot, Question, Answer, Result,
    Scoreboard, GameOver, PokerAction, State, Error, Chat, Ready, Ping
};
struct Message {
    MsgType type = MsgType::Unknown;
    std::vector<std::string> fields;
};
std::string toString(MsgType type);
std::string serialize(const Message& message);
Message parse(const std::string& line) noexcept;
std::ostream& operator<<(std::ostream& out, const Message& message);