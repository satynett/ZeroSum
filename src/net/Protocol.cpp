#include "Protocol.h"
#include <algorithm>
#include <sstream>

std::string toString(MsgType t) {
    switch (t) {
#define CASE(x, s) case MsgType::x: return s
        CASE(Unknown,"UNKNOWN"); CASE(Join,"JOIN"); CASE(SelectGame,"SELECTGAME");
        CASE(StartGame,"STARTGAME"); CASE(AddBot,"ADDBOT"); CASE(Question,"QUESTION");
        CASE(Answer,"ANSWER"); CASE(Result,"RESULT"); CASE(Scoreboard,"SCOREBOARD");
        CASE(GameOver,"GAMEOVER"); CASE(PokerAction,"POKERACTION"); CASE(State,"STATE");
        CASE(Error,"ERROR"); CASE(Chat,"CHAT"); CASE(Ready,"READY"); CASE(Ping,"PING");
#undef CASE
    }
    return "UNKNOWN";
}
std::string serialize(const Message& m) {
    std::string out = toString(m.type);
    for (const auto& f : m.fields) { out += '|'; for (char c : f) out += (c == '\n' || c == '\r' ? ' ' : c); }
    return out;
}
Message parse(const std::string& line) noexcept {
    Message m;
    if (line.empty() || line.size() > 1024) return m;
    std::istringstream in(line);
    std::string token;
    if (!std::getline(in, token, '|')) return m;
    const std::string key = token;
#define MATCH(s, x) if (key == s) m.type = MsgType::x; else
    MATCH("JOIN",Join) MATCH("SELECTGAME",SelectGame) MATCH("STARTGAME",StartGame)
    MATCH("ADDBOT",AddBot) MATCH("QUESTION",Question) MATCH("ANSWER",Answer)
    MATCH("RESULT",Result) MATCH("SCOREBOARD",Scoreboard) MATCH("GAMEOVER",GameOver)
    MATCH("POKERACTION",PokerAction) MATCH("STATE",State) MATCH("ERROR",Error)
    MATCH("CHAT",Chat) MATCH("READY",Ready) MATCH("PING",Ping)
    { m.type = MsgType::Unknown; }
#undef MATCH
    if (m.type == MsgType::Unknown) return m;
    while (std::getline(in, token, '|')) m.fields.push_back(token);
    return m;
}
std::ostream& operator<<(std::ostream& out, const Message& m) { return out << serialize(m); }