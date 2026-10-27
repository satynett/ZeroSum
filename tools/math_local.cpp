// Separation of concerns: this small launcher delegates to runMathArenaLocal() instead of duplicating Matiks gameplay.
// Small launcher for testing Matiks on its own.

using namespace std;
void runMathArenaLocal();
int main() {
    runMathArenaLocal();
    return 0;
}
