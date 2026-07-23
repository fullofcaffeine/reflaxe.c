/*
 * Verify the public Raylib clock supplied by the Memory platform.
 *
 * This is deliberately a native C integration fixture: it tests the behavior
 * of the third-party library before haxe.c or Caxecraft uses it.  A volatile
 * accumulator keeps the bounded CPU work observable so an optimizing compiler
 * cannot remove the interval that the clock must measure.
 */

#include <stdint.h>
#include <stdio.h>

#include "raylib.h"

int main(void)
{
    volatile uint64_t accumulator = UINT64_C(0);

    SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "raylib Memory clock probe");

    const double before = GetTime();
    for (uint32_t index = UINT32_C(0); index < UINT32_C(10000000); ++index)
    {
        accumulator ^= ((uint64_t)index + UINT64_C(0x9e3779b97f4a7c15));
    }
    const double after = GetTime();

    CloseWindow();
    if ((before < 0.0) || (after < before) || (after == before))
    {
        (void)fprintf(stderr, "raylib-memory-clock: invalid monotonic interval\n");
        return 1;
    }

    (void)printf("raylib-memory-clock: OK\n");
    return 0;
}
