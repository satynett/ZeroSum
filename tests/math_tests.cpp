#include "math/QuestionGenerator.h"
#include <cassert>

int main() {
    QuestionGenerator generator;

    // Check that every difficulty level produces valid questions.
    for (int difficulty = 1; difficulty <= 5; ++difficulty) {
        for (int i = 0; i < 500; ++i) {
            const auto question = generator.generate(difficulty);
            assert(question.difficulty == difficulty);
            assert(question.answer >= 0);
            assert(!question.text.empty());
        }
    }
}
