#include <stdint.h>

static int32_t take_next(int32_t *cursor)
{
    int32_t value = *cursor;
    *cursor += 1;
    return value;
}

static int32_t combine_pair(int32_t left, int32_t right)
{
    return left * 10 + right;
}

int32_t preserve_call_order(int32_t *cursor)
{
    int32_t left = take_next(cursor);
    int32_t right = take_next(cursor);
    return combine_pair(left, right);
}
