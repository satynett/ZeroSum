#include "math/QuestionGenerator.h"
#include "math/MathGame.h"
#include <cassert>
#include <stdexcept>
int main() {
    QuestionGenerator g;
    for (int d=1; d<=5; ++d) for(int i=0;i<500;++i) {
        auto q=g.generate(d);
        assert(q.difficulty==d);
        assert(q.answer>=0);
        assert(!q.text.empty());
    }
    auto sprint=makeMathGame("sprint");
    auto blitz=makeMathGame("blitz");
    auto survival=makeMathGame("survival");
    assert(sprint->name()=="math:sprint");
    assert(blitz->name()=="math:blitz");
    assert(survival->name()=="math:survival");
    bool threw=false; try { (void)makeMathGame("unknown"); } catch(const std::invalid_argument&) { threw=true; }
    assert(threw);
    Player p{1,"Tester",true,true,0};
    auto out=sprint->start({p});
    assert(!out.empty() && out[0].msg.type==MsgType::Question);
}