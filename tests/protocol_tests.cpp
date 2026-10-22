#include "net/Protocol.h"
#include <cassert>
int main() {
    auto m = parse("ANSWER|42");
    assert(m.type == MsgType::Answer);
    assert(m.fields.size() == 1 && m.fields[0] == "42");
    assert(serialize(m) == "ANSWER|42");
    assert(parse("not-a-message").type == MsgType::Unknown);
    assert(parse("").type == MsgType::Unknown);
    assert(parse(std::string(1025, 'x')).type == MsgType::Unknown);
    auto p = parse("POKERACTION|raise|100");
    assert(p.type == MsgType::PokerAction && p.fields.size() == 2);
}