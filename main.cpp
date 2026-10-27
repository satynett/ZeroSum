// OOP/design note: the entry point delegates to the UI layer, keeping startup separate from game behavior. This file itself does not demonstrate inheritance or polymorphism.
// main.cpp: start the game hub UI.
#include "src/ui/UI.h"


using namespace std;
int main() {
    runGameHubUI();
    return 0;
}
