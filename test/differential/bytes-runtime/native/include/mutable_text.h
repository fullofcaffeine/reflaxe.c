#ifndef HXC_TEST_MUTABLE_TEXT_H_INCLUDED
#define HXC_TEST_MUTABLE_TEXT_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Replace caller-owned text in place without retaining the pointer.
 *
 * `capacity` includes the final NUL byte. The function returns false without
 * writing when the input is not the expected fixture value or is too small.
 */
bool fixture_replace_mutable_text(char *text, int32_t capacity);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXC_TEST_MUTABLE_TEXT_H_INCLUDED */
