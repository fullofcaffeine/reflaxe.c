#include "hxrt/string.h"

#include <inttypes.h>
#include <stdio.h>

#define HXC_TEST_CHECK(condition) \
  do { \
    if (!(condition)) { \
      (void)fprintf(stderr, "string-runtime: check failed at line %d\n", __LINE__); \
      return 1; \
    } \
  } while (0)

typedef struct hxc_test_arena {
  uint8_t storage[32768];
  size_t cursor;
  size_t allocation_count;
  size_t release_count;
  bool force_failure;
} hxc_test_arena;

typedef struct hxc_lossy_case {
  hxc_byte_view input;
  hxc_byte_view expected;
  size_t expected_scalars;
} hxc_lossy_case;

static hxc_status hxc_test_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  hxc_test_arena *arena = (hxc_test_arena *)context;
  uintptr_t address;
  size_t remainder;
  size_t padding;
  size_t start;
  size_t end;
  hxc_status status;
  if (arena == NULL || out_memory == NULL || size == 0u) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (arena->force_failure) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  address = (uintptr_t)(arena->storage + arena->cursor);
  remainder = (size_t)(address % (uintptr_t)alignment);
  padding = remainder == 0u ? 0u : alignment - remainder;
  status = hxc_size_add(arena->cursor, padding, &start);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_size_add(start, size, &end);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (end > sizeof(arena->storage)) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  *out_memory = arena->storage + start;
  arena->cursor = end;
  arena->allocation_count++;
  return HXC_STATUS_OK;
}

static void hxc_test_release(
  void *context,
  void *memory,
  size_t size,
  size_t alignment
) {
  hxc_test_arena *arena = (hxc_test_arena *)context;
  (void)memory;
  (void)size;
  (void)alignment;
  arena->release_count++;
}

static hxc_allocator hxc_test_allocator(hxc_test_arena *arena) {
  hxc_allocator allocator;
  allocator.context = arena;
  allocator.allocate = hxc_test_allocate;
  allocator.reallocate = NULL;
  allocator.release = hxc_test_release;
  return allocator;
}

static bool hxc_bytes_equal(
  const uint8_t *left,
  const uint8_t *right,
  size_t length
) {
  size_t index;
  for (index = 0u; index < length; index++) {
    if (left[index] != right[index]) {
      return false;
    }
  }
  return true;
}

static int hxc_test_literals_and_scalars(hxc_test_arena *arena) {
  const hxc_string missing = HXC_STRING_INITIALIZER;
  const hxc_string empty = HXC_STRING_EMPTY_INITIALIZER;
  const hxc_string emoji = HXC_STRING_LITERAL("\xF0\x9F\x98\x80");
  const hxc_string embedded = HXC_STRING_LITERAL("a\0b");
  const hxc_string composed = HXC_STRING_LITERAL("\xC3\xA9");
  const hxc_string decomposed = HXC_STRING_LITERAL("e\xCC\x81");
  const hxc_string sequence = HXC_STRING_LITERAL("x\xF0\x9F\x98\x80\xC3\xA9");
  hxc_string slice = HXC_STRING_INITIALIZER;
  size_t length = 99u;
  size_t allocations = arena->allocation_count;
  uint32_t scalar = UINT32_C(0);
  uint32_t hash = UINT32_C(0);
  int32_t order = 7;

  HXC_TEST_CHECK(!hxc_string_is_valid(missing));
  HXC_TEST_CHECK(hxc_string_is_valid(empty));
  HXC_TEST_CHECK(empty.data != NULL && empty.byte_length == 0u);
  HXC_TEST_CHECK(hxc_string_is_valid(emoji));
  HXC_TEST_CHECK(hxc_string_is_valid(embedded));
  HXC_TEST_CHECK(hxc_string_scalar_length(emoji, &length) == HXC_STATUS_OK);
  HXC_TEST_CHECK(length == 1u);
  HXC_TEST_CHECK(hxc_string_scalar_at(emoji, 0u, &scalar) == HXC_STATUS_OK);
  HXC_TEST_CHECK(scalar == UINT32_C(0x1F600));
  HXC_TEST_CHECK(hxc_string_scalar_at(emoji, 1u, &scalar) == HXC_STATUS_OUT_OF_RANGE);
  HXC_TEST_CHECK(hxc_string_scalar_length(embedded, &length) == HXC_STATUS_OK);
  HXC_TEST_CHECK(length == 3u);
  HXC_TEST_CHECK(hxc_string_scalar_length(composed, &length) == HXC_STATUS_OK);
  HXC_TEST_CHECK(length == 1u);
  HXC_TEST_CHECK(hxc_string_scalar_length(decomposed, &length) == HXC_STATUS_OK);
  HXC_TEST_CHECK(length == 2u);
  HXC_TEST_CHECK(hxc_string_compare(composed, decomposed, &order) == HXC_STATUS_OK);
  HXC_TEST_CHECK(order > 0);
  HXC_TEST_CHECK(hxc_string_hash(embedded, &hash) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hash == UINT32_C(284404690));
  HXC_TEST_CHECK(hxc_string_slice(sequence, 1u, 1u, &slice) == HXC_STATUS_OK);
  HXC_TEST_CHECK(slice.byte_length == 4u && !slice.has_trailing_nul);
  HXC_TEST_CHECK(hxc_bytes_equal(slice.data, emoji.data, emoji.byte_length));
  HXC_TEST_CHECK(hxc_string_slice(sequence, 2u, 1u, &slice) == HXC_STATUS_OK);
  HXC_TEST_CHECK(slice.byte_length == 2u && slice.has_trailing_nul);
  HXC_TEST_CHECK(arena->allocation_count == allocations);
  return 0;
}

static int hxc_test_checked_and_lossy(
  hxc_test_arena *arena,
  const hxc_allocator *allocator
) {
  static const uint8_t stray[] = { UINT8_C(0x80) };
  static const uint8_t overlong[] = { UINT8_C(0xC0), UINT8_C(0xAF) };
  static const uint8_t truncated[] = { UINT8_C(0xE2), UINT8_C(0x82) };
  static const uint8_t surrogate[] = {
    UINT8_C(0xED), UINT8_C(0xA0), UINT8_C(0x80)
  };
  static const uint8_t out_of_range[] = {
    UINT8_C(0xF4), UINT8_C(0x90), UINT8_C(0x80), UINT8_C(0x80)
  };
  static const uint8_t partial_then_ascii[] = {
    UINT8_C(0xE1), UINT8_C(0x80), UINT8_C('A')
  };
  static const uint8_t replacement[] = {
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD)
  };
  static const uint8_t replacement_twice[] = {
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD),
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD)
  };
  static const uint8_t replacement_thrice[] = {
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD),
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD),
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD)
  };
  static const uint8_t replacement_four_times[] = {
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD),
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD),
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD),
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD)
  };
  static const uint8_t replacement_then_ascii[] = {
    UINT8_C(0xEF), UINT8_C(0xBF), UINT8_C(0xBD), UINT8_C('A')
  };
  static const uint8_t valid_emoji[] = {
    UINT8_C(0xF0), UINT8_C(0x9F), UINT8_C(0x98), UINT8_C(0x80)
  };
  const hxc_lossy_case cases[] = {
    { { stray, sizeof(stray) }, { replacement, sizeof(replacement) }, 1u },
    { { overlong, sizeof(overlong) }, { replacement_twice, sizeof(replacement_twice) }, 2u },
    { { truncated, sizeof(truncated) }, { replacement, sizeof(replacement) }, 1u },
    { { surrogate, sizeof(surrogate) }, { replacement_thrice, sizeof(replacement_thrice) }, 3u },
    { { out_of_range, sizeof(out_of_range) }, { replacement_four_times, sizeof(replacement_four_times) }, 4u },
    { { partial_then_ascii, sizeof(partial_then_ascii) }, { replacement_then_ascii, sizeof(replacement_then_ascii) }, 2u }
  };
  hxc_owned_string checked = HXC_OWNED_STRING_INITIALIZER;
  hxc_owned_string lossy = HXC_OWNED_STRING_INITIALIZER;
  size_t index;
  size_t scalar_length;
  size_t allocations;

  allocations = arena->allocation_count;
  HXC_TEST_CHECK(
    hxc_string_from_utf8_checked(
      (hxc_byte_view){ valid_emoji, sizeof(valid_emoji) },
      allocator,
      &checked
    ) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(arena->allocation_count == allocations + 1u);
  HXC_TEST_CHECK(checked.value.byte_length == sizeof(valid_emoji));
  HXC_TEST_CHECK(hxc_allocator_same_identity(allocator, &checked.storage.allocator));
  HXC_TEST_CHECK(hxc_owned_string_dispose(&checked) == HXC_STATUS_OK);

  for (index = 0u; index < sizeof(cases) / sizeof(cases[0]); index++) {
    allocations = arena->allocation_count;
    scalar_length = 77u;
    HXC_TEST_CHECK(
      hxc_utf8_validate(cases[index].input, &scalar_length)
        == HXC_STATUS_INVALID_UTF8
    );
    HXC_TEST_CHECK(scalar_length == 77u);
    HXC_TEST_CHECK(
      hxc_string_from_utf8_checked(cases[index].input, allocator, &checked)
        == HXC_STATUS_INVALID_UTF8
    );
    HXC_TEST_CHECK(checked.value.data == NULL);
    HXC_TEST_CHECK(arena->allocation_count == allocations);
    HXC_TEST_CHECK(
      hxc_string_from_utf8_lossy(cases[index].input, allocator, &lossy)
        == HXC_STATUS_OK
    );
    HXC_TEST_CHECK(arena->allocation_count == allocations + 1u);
    HXC_TEST_CHECK(lossy.value.byte_length == cases[index].expected.length);
    HXC_TEST_CHECK(
      hxc_bytes_equal(
        lossy.value.data,
        cases[index].expected.data,
        cases[index].expected.length
      )
    );
    HXC_TEST_CHECK(
      hxc_string_scalar_length(lossy.value, &scalar_length) == HXC_STATUS_OK
    );
    HXC_TEST_CHECK(scalar_length == cases[index].expected_scalars);
    HXC_TEST_CHECK(hxc_owned_string_dispose(&lossy) == HXC_STATUS_OK);
  }
  return 0;
}

static int hxc_test_concat_and_failure(
  hxc_test_arena *arena,
  const hxc_allocator *allocator
) {
  const hxc_string left = HXC_STRING_LITERAL("left\0");
  const hxc_string right = HXC_STRING_LITERAL("\xF0\x9F\x98\x80");
  const hxc_string empty = HXC_STRING_LITERAL("");
  hxc_owned_string concatenated = HXC_OWNED_STRING_INITIALIZER;
  hxc_owned_string empty_result = HXC_OWNED_STRING_INITIALIZER;
  hxc_owned_string failed = HXC_OWNED_STRING_INITIALIZER;
  size_t allocations = arena->allocation_count;
  size_t scalar_length;

  HXC_TEST_CHECK(
    hxc_string_concat(left, right, allocator, &concatenated) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(arena->allocation_count == allocations + 1u);
  HXC_TEST_CHECK(concatenated.storage.size == left.byte_length + right.byte_length + 1u);
  HXC_TEST_CHECK(
    hxc_string_scalar_length(concatenated.value, &scalar_length) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(scalar_length == 6u);
  HXC_TEST_CHECK(hxc_owned_string_dispose(&concatenated) == HXC_STATUS_OK);

  allocations = arena->allocation_count;
  HXC_TEST_CHECK(
    hxc_string_concat(empty, empty, allocator, &empty_result) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(arena->allocation_count == allocations);
  HXC_TEST_CHECK(empty_result.value.byte_length == 0u);
  HXC_TEST_CHECK(empty_result.value.has_trailing_nul);
  HXC_TEST_CHECK(hxc_owned_string_dispose(&empty_result) == HXC_STATUS_OK);

  arena->force_failure = true;
  HXC_TEST_CHECK(
    hxc_string_concat(left, right, allocator, &failed) == HXC_STATUS_OUT_OF_MEMORY
  );
  arena->force_failure = false;
  HXC_TEST_CHECK(failed.value.data == NULL && failed.storage.memory == NULL);
  return 0;
}

static int hxc_test_builder(
  hxc_test_arena *arena,
  const hxc_allocator *allocator
) {
  static const uint8_t invalid[] = { UINT8_C(0xE2), UINT8_C('!') };
  static const uint8_t large[] = {
    UINT8_C('0'), UINT8_C('1'), UINT8_C('2'), UINT8_C('3'), UINT8_C('4'),
    UINT8_C('5'), UINT8_C('6'), UINT8_C('7'), UINT8_C('8'), UINT8_C('9'),
    UINT8_C('a'), UINT8_C('b'), UINT8_C('c'), UINT8_C('d'), UINT8_C('e'),
    UINT8_C('f'), UINT8_C('g'), UINT8_C('h'), UINT8_C('i'), UINT8_C('j')
  };
  const hxc_string prefix = HXC_STRING_LITERAL("abcdefghij");
  hxc_string_buffer buffer = HXC_STRING_BUFFER_INITIALIZER;
  hxc_string_buffer failing = HXC_STRING_BUFFER_INITIALIZER;
  hxc_owned_string finished = HXC_OWNED_STRING_INITIALIZER;
  hxc_string view = HXC_STRING_INITIALIZER;
  hxc_byte_view append;
  size_t allocations = arena->allocation_count;
  size_t before_length;
  size_t scalar_length;

  HXC_TEST_CHECK(hxc_string_buffer_init(allocator, &buffer) == HXC_STATUS_OK);
  HXC_TEST_CHECK(arena->allocation_count == allocations);
  append.data = prefix.data;
  append.length = prefix.byte_length;
  HXC_TEST_CHECK(
    hxc_string_buffer_append_utf8_checked(&buffer, append) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(arena->allocation_count == allocations + 1u);
  HXC_TEST_CHECK(hxc_string_buffer_view(&buffer, &view) == HXC_STATUS_OK);
  append.data = view.data;
  append.length = view.byte_length;
  HXC_TEST_CHECK(
    hxc_string_buffer_append_utf8_checked(&buffer, append) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(hxc_string_buffer_append_scalar(&buffer, UINT32_C(0x1F600)) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_string_buffer_append_scalar(&buffer, UINT32_C(0xD800)) == HXC_STATUS_OK);
  before_length = buffer.byte_length;
  append.data = invalid;
  append.length = sizeof(invalid);
  HXC_TEST_CHECK(
    hxc_string_buffer_append_utf8_checked(&buffer, append) == HXC_STATUS_INVALID_UTF8
  );
  HXC_TEST_CHECK(buffer.byte_length == before_length);
  HXC_TEST_CHECK(hxc_string_buffer_finish(&buffer, &finished) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_string_scalar_length(finished.value, &scalar_length) == HXC_STATUS_OK);
  HXC_TEST_CHECK(scalar_length == 22u);
  HXC_TEST_CHECK(hxc_owned_string_dispose(&finished) == HXC_STATUS_OK);
  HXC_TEST_CHECK(hxc_string_buffer_dispose(&buffer) == HXC_STATUS_OK);

  HXC_TEST_CHECK(hxc_string_buffer_init(allocator, &failing) == HXC_STATUS_OK);
  arena->force_failure = true;
  append.data = large;
  append.length = sizeof(large);
  HXC_TEST_CHECK(
    hxc_string_buffer_append_utf8_checked(&failing, append)
      == HXC_STATUS_OUT_OF_MEMORY
  );
  arena->force_failure = false;
  HXC_TEST_CHECK(failing.byte_length == 0u && failing.storage.memory == NULL);
  HXC_TEST_CHECK(hxc_string_buffer_dispose(&failing) == HXC_STATUS_OK);
  return 0;
}

static int hxc_test_cstrings(
  hxc_test_arena *arena,
  const hxc_allocator *allocator
) {
  const hxc_string stable = HXC_STRING_LITERAL("native");
  const hxc_string embedded = HXC_STRING_LITERAL("a\0b");
  const hxc_string whole = HXC_STRING_LITERAL("slice");
  hxc_string interior = HXC_STRING_INITIALIZER;
  hxc_borrowed_cstring borrowed = HXC_BORROWED_CSTRING_INITIALIZER;
  hxc_owned_cstring owned = HXC_OWNED_CSTRING_INITIALIZER;
  size_t allocations = arena->allocation_count;

  HXC_TEST_CHECK(
    hxc_string_borrow_cstring(&stable, &borrowed) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(borrowed.owner == &stable);
  HXC_TEST_CHECK(borrowed.byte_length == stable.byte_length);
  HXC_TEST_CHECK(borrowed.data[borrowed.byte_length] == '\0');
  HXC_TEST_CHECK(arena->allocation_count == allocations);

  borrowed = (hxc_borrowed_cstring)HXC_BORROWED_CSTRING_INITIALIZER;
  HXC_TEST_CHECK(
    hxc_string_borrow_cstring(&embedded, &borrowed) == HXC_STATUS_EMBEDDED_NUL
  );
  HXC_TEST_CHECK(
    hxc_string_to_cstring_owned(&embedded, allocator, &owned)
      == HXC_STATUS_EMBEDDED_NUL
  );
  HXC_TEST_CHECK(arena->allocation_count == allocations);

  HXC_TEST_CHECK(hxc_string_slice(whole, 0u, 4u, &interior) == HXC_STATUS_OK);
  HXC_TEST_CHECK(!interior.has_trailing_nul);
  HXC_TEST_CHECK(
    hxc_string_borrow_cstring(&interior, &borrowed)
      == HXC_STATUS_BORROW_UNAVAILABLE
  );
  HXC_TEST_CHECK(
    hxc_string_to_cstring_owned(&interior, allocator, &owned) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(arena->allocation_count == allocations + 1u);
  HXC_TEST_CHECK(owned.byte_length == 4u && owned.data[4] == '\0');
  HXC_TEST_CHECK(hxc_allocator_same_identity(allocator, &owned.storage.allocator));
  HXC_TEST_CHECK(hxc_owned_cstring_dispose(&owned) == HXC_STATUS_OK);
  return 0;
}

int main(void) {
  const hxc_string emoji = HXC_STRING_LITERAL("\xF0\x9F\x98\x80");
  const hxc_string embedded = HXC_STRING_LITERAL("a\0b");
  const hxc_string composed = HXC_STRING_LITERAL("\xC3\xA9");
  const hxc_string decomposed = HXC_STRING_LITERAL("e\xCC\x81");
  const hxc_string sequence = HXC_STRING_LITERAL("x\xF0\x9F\x98\x80\xC3\xA9");
  hxc_test_arena arena = {0};
  hxc_allocator allocator = hxc_test_allocator(&arena);
  hxc_string slice = HXC_STRING_INITIALIZER;
  size_t emoji_length;
  size_t embedded_length;
  size_t composed_length;
  size_t decomposed_length;
  uint32_t emoji_scalar;

  HXC_TEST_CHECK(hxc_test_literals_and_scalars(&arena) == 0);
  HXC_TEST_CHECK(hxc_test_checked_and_lossy(&arena, &allocator) == 0);
  HXC_TEST_CHECK(hxc_test_concat_and_failure(&arena, &allocator) == 0);
  HXC_TEST_CHECK(hxc_test_builder(&arena, &allocator) == 0);
  HXC_TEST_CHECK(hxc_test_cstrings(&arena, &allocator) == 0);
  HXC_TEST_CHECK(
    hxc_string_scalar_length(emoji, &emoji_length) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(hxc_string_scalar_at(emoji, 0u, &emoji_scalar) == HXC_STATUS_OK);
  HXC_TEST_CHECK(
    hxc_string_scalar_length(embedded, &embedded_length) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(
    hxc_string_scalar_length(composed, &composed_length) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(
    hxc_string_scalar_length(decomposed, &decomposed_length) == HXC_STATUS_OK
  );
  HXC_TEST_CHECK(hxc_string_slice(sequence, 1u, 1u, &slice) == HXC_STATUS_OK);

  (void)printf(
    "%zu,%" PRIu32 ",%zu,%zu,%zu,",
    emoji_length,
    emoji_scalar,
    embedded_length,
    composed_length,
    decomposed_length
  );
  (void)fwrite(slice.data, 1u, slice.byte_length, stdout);
  (void)putchar('\n');
  return 0;
}
