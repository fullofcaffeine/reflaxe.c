/*
 * hxrt feature: string-scalar internal decoder.
 *
 * The full owned-string seed and the compiler-selectable scalar slice both
 * decode UTF-8. Keeping the small decoder here prevents two subtly different
 * implementations of malformed-sequence and scalar-width rules. It is an
 * internal header-only implementation detail: generated C calls the public
 * functions in string_scalar.h, never these static helpers.
 */
#ifndef HXRT_STRING_DECODE_H_INCLUDED
#define HXRT_STRING_DECODE_H_INCLUDED

#include "hxrt/string_scalar.h"

typedef struct hxc_utf8_step {
  size_t consumed;
  uint32_t scalar;
  bool valid;
} hxc_utf8_step;

static inline bool hxc_byte_view_has_valid_shape(hxc_byte_view source) {
  return source.data != NULL || source.length == 0u;
}

static inline bool hxc_is_continuation(uint8_t value) {
  return value >= UINT8_C(0x80) && value <= UINT8_C(0xBF);
}

static inline hxc_utf8_step hxc_invalid_step(size_t consumed) {
  hxc_utf8_step step;
  step.consumed = consumed;
  step.scalar = UINT32_C(0xFFFD);
  step.valid = false;
  return step;
}

static inline hxc_utf8_step hxc_valid_step(
  size_t consumed,
  uint32_t scalar
) {
  hxc_utf8_step step;
  step.consumed = consumed;
  step.scalar = scalar;
  step.valid = true;
  return step;
}

/**
 * Decode one maximal UTF-8 unit from a nonempty byte view.
 *
 * A valid result contains one Unicode scalar. An invalid result consumes the
 * maximal ill-formed subpart used by ADR 0004's deterministic U+FFFD rule.
 */
static inline hxc_utf8_step hxc_utf8_read(
  const uint8_t *data,
  size_t length
) {
  const uint8_t first = data[0];
  uint8_t second;
  uint8_t third;
  uint8_t fourth;
  uint32_t scalar;

  if (first <= UINT8_C(0x7F)) {
    return hxc_valid_step(1u, (uint32_t)first);
  }

  if (first >= UINT8_C(0xC2) && first <= UINT8_C(0xDF)) {
    if (length < 2u || !hxc_is_continuation(data[1])) {
      return hxc_invalid_step(1u);
    }
    second = data[1];
    scalar = ((uint32_t)(first & UINT8_C(0x1F)) << 6u)
      | (uint32_t)(second & UINT8_C(0x3F));
    return hxc_valid_step(2u, scalar);
  }

  /* Boundary checks reject overlong encodings and UTF-16 surrogate scalars. */
  if (first >= UINT8_C(0xE0) && first <= UINT8_C(0xEF)) {
    if (length < 2u) {
      return hxc_invalid_step(1u);
    }
    second = data[1];
    if ((first == UINT8_C(0xE0)
        && (second < UINT8_C(0xA0) || second > UINT8_C(0xBF)))
      || (first == UINT8_C(0xED)
        && (second < UINT8_C(0x80) || second > UINT8_C(0x9F)))
      || (first != UINT8_C(0xE0) && first != UINT8_C(0xED)
        && !hxc_is_continuation(second))) {
      return hxc_invalid_step(1u);
    }
    if (length < 3u || !hxc_is_continuation(data[2])) {
      return hxc_invalid_step(2u);
    }
    third = data[2];
    scalar = ((uint32_t)(first & UINT8_C(0x0F)) << 12u)
      | ((uint32_t)(second & UINT8_C(0x3F)) << 6u)
      | (uint32_t)(third & UINT8_C(0x3F));
    return hxc_valid_step(3u, scalar);
  }

  /* F0/F4 bounds keep decoded values within U+10000..U+10FFFF. */
  if (first >= UINT8_C(0xF0) && first <= UINT8_C(0xF4)) {
    if (length < 2u) {
      return hxc_invalid_step(1u);
    }
    second = data[1];
    if ((first == UINT8_C(0xF0)
        && (second < UINT8_C(0x90) || second > UINT8_C(0xBF)))
      || (first == UINT8_C(0xF4)
        && (second < UINT8_C(0x80) || second > UINT8_C(0x8F)))
      || (first != UINT8_C(0xF0) && first != UINT8_C(0xF4)
        && !hxc_is_continuation(second))) {
      return hxc_invalid_step(1u);
    }
    if (length < 3u || !hxc_is_continuation(data[2])) {
      return hxc_invalid_step(2u);
    }
    if (length < 4u || !hxc_is_continuation(data[3])) {
      return hxc_invalid_step(3u);
    }
    third = data[2];
    fourth = data[3];
    scalar = ((uint32_t)(first & UINT8_C(0x07)) << 18u)
      | ((uint32_t)(second & UINT8_C(0x3F)) << 12u)
      | ((uint32_t)(third & UINT8_C(0x3F)) << 6u)
      | (uint32_t)(fourth & UINT8_C(0x3F));
    return hxc_valid_step(4u, scalar);
  }

  return hxc_invalid_step(1u);
}

#endif /* HXRT_STRING_DECODE_H_INCLUDED */
