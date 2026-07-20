#include <stdint.h>

int32_t classify_until(const int32_t *values, int32_t count)
{
    int32_t score = 0;

    for (int32_t index = 0; index < count; ++index) {
        int32_t value = values[index];
        if (value < 0) {
            break;
        }

        switch (value) {
            case 0:
                score += 1;
                break;
            case 1:
                score += 2;
                break;
            default:
                score += 3;
                break;
        }
    }

    return score;
}
