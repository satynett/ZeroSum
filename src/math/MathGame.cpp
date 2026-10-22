#include "MathGame.h"
#include <algorithm>
#include <stdexcept>

MathGame::MathGame(std::string mode): mode_(std::move(mode)) {}
std::string MathGame::name() const { return "math:"+mode_; }
void MathGame::nextQuestion() {
    ++questionIndex_;
    if (players_.empty()) { finished_=true; return; }
    if (mode_ == "blitz") {
        for (const auto& p:players_) difficulty_[p.id]=std::min(5,1+p.score/3);
        current_=generator_.generate(1);
        deadline_=gameDeadline_;
    } else {
        currentPlayer_=(currentPlayer_+1)%static_cast<int>(players_.size());
        current_=generator_.generate(difficulty_[players_[currentPlayer_].id]);
        deadline_=std::chrono::steady_clock::now()+std::chrono::seconds(mode_=="sprint"?8:10);
    }
}
Outbox MathGame::broadcastQuestion(int id) {
    return {{id,{MsgType::Question,{std::to_string(questionIndex_),current_.text,
        mode_=="blitz"?"120000":mode_=="sprint"?"8000":"10000"}}}};
}
Outbox MathGame::scoreboard() const {
    std::string s;
    for (const auto& p:players_) { if(!s.empty()) s+='|'; s+=p.name+":"+std::to_string(p.score); }
    return {{-1,{MsgType::Scoreboard,{s}}}};
}
Outbox MathGame::start(const std::vector<Player>& players) {
    players_=players; finished_=players_.empty(); questionIndex_=0; currentPlayer_=-1;
    for (const auto& p:players_) { difficulty_[p.id]=1; lives_[p.id]=3; correct_[p.id]=0; }
    gameDeadline_=std::chrono::steady_clock::now()+std::chrono::minutes(2);
    if (!finished_) { currentPlayer_=-1; nextQuestion(); }
    return broadcastQuestion();
}
Outbox MathGame::handleInput(int id,const Message& msg) {
    if (finished_ || msg.type!=MsgType::Answer || msg.fields.size()!=1) return {};
    auto it=std::find_if(players_.begin(),players_.end(),[id](const Player& p){return p.id==id && p.connected;});
    if(it==players_.end()) return {};
    int answer;
    try { std::size_t n=0; answer=std::stoi(msg.fields[0],&n); if(n!=msg.fields[0].size()) return {}; } catch(...) { return {}; }
    const bool correct=answer==current_.answer;
    if(correct) { ++it->score; ++correct_[id]; if(mode_=="survival" && correct_[id]%3==0) difficulty_[id]=std::min(5,difficulty_[id]+1); }
    if(mode_=="survival" && !correct) --lives_[id];
    Outbox out{{-1,{MsgType::Result,{std::to_string(id),correct?"1":"0",correct?"1":"0"}}}};
    if(mode_=="sprint" && correct && it->score>=10) { finished_=true; out.push_back({-1,{MsgType::GameOver,{it->name}}}); return out; }
    if(mode_=="survival" && std::count_if(players_.begin(),players_.end(),[&](const Player&p){return p.connected && lives_[p.id]>0;})<=1) {
        finished_=true; auto alive=std::find_if(players_.begin(),players_.end(),[&](const Player&p){return p.connected && lives_[p.id]>0;});
        out.push_back({-1,{MsgType::GameOver,{alive==players_.end()?"Nobody":alive->name}}}); return out;
    }
    if(mode_=="sprint" && correct) { nextQuestion(); auto q=broadcastQuestion(); out.insert(out.end(),q.begin(),q.end()); }
    else if(mode_=="blitz") { nextQuestion(); auto q=broadcastQuestion(id); out.insert(out.end(),q.begin(),q.end()); }
    else if(mode_=="survival" && id==players_[currentPlayer_].id) { nextQuestion(); auto q=broadcastQuestion(); out.insert(out.end(),q.begin(),q.end()); }
    auto sb=scoreboard(); out.insert(out.end(),sb.begin(),sb.end()); return out;
}
Outbox MathGame::update() {
    if(finished_) return {};
    const auto now=std::chrono::steady_clock::now();
    if(mode_=="blitz" && now>=gameDeadline_) {
        finished_=true; auto standings=finalStandings();
        return {{-1,{MsgType::GameOver,{standings.empty()?"Nobody":standings.front().name}}}};
    }
    if(now>=deadline_) {
        if(mode_=="survival" && !players_.empty()) --lives_[players_[currentPlayer_].id];
        nextQuestion(); return broadcastQuestion();
    }
    return {};
}
Outbox MathGame::onPlayerDisconnected(int id) {
    for(auto& p:players_) if(p.id==id) p.connected=false;
    if(std::none_of(players_.begin(),players_.end(),[](const Player&p){return p.connected;})) finished_=true;
    return {};
}
std::vector<Player> MathGame::finalStandings() const {
    auto out=players_; std::stable_sort(out.begin(),out.end(),[](const Player&a,const Player&b){return a.score>b.score;}); return out;
}
std::unique_ptr<Game> makeMathGame(const std::string& mode) {
    if(mode=="sprint") return std::make_unique<SprintMode>();
    if(mode=="blitz") return std::make_unique<BlitzMode>();
    if(mode=="survival") return std::make_unique<SurvivalMode>();
    throw std::invalid_argument("Unknown math mode: "+mode);
}