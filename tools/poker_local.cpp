// Separation of concerns: this small launcher delegates to runPokerLocal() instead of duplicating poker gameplay.
#include "../src/local/LocalGames.h"

using namespace std;
int main() { runPokerLocal(); return 0; }