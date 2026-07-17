#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* Declarators are semantic grammar trees, not target-code strings. */

struct hxc_opaque;

typedef struct hxc_opaque hxc_opaque;

typedef enum {
  HXC_MODE_FAST = 1,
  HXC_MODE_SAFE = 2
} hxc_mode;

typedef int32_t (*hxc_binary_fn)(const int32_t *restrict left, const int32_t *restrict right);

typedef int32_t (*hxc_matrix_ptr)[4];

typedef int32_t (*hxc_transform_table[3])(int32_t value);

struct hxc_dispatch {
  hxc_binary_fn binary;
  int32_t (*transform)(int32_t value);
  int32_t *items[4];
  int32_t (*matrix)[4];
  _Alignas(16) _Atomic(int32_t) counter;
  union {
    int32_t signed_value;
    uint32_t unsigned_value;
  };
  unsigned long generation;
  bool enabled;
  uint8_t payload[];
};

extern void hxc_visit(size_t rows, size_t columns, int32_t matrix[restrict static rows][*]);

_Static_assert(_Alignof(struct hxc_dispatch) >= 16, "aligned atomic field raises aggregate alignment");

_Static_assert(sizeof(int32_t (*)[4]) > 0, "abstract pointer-to-array declarator is complete");

_Static_assert(offsetof(struct hxc_dispatch, binary) == 0, "first aggregate member starts at offset zero");

static inline int32_t *hxc_select(hxc_binary_fn callback, int32_t *left)
{
  (void)callback;
  return left;
}

int main(void)
{
  struct hxc_dispatch dispatch = { .counter = 0, .enabled = true };
  hxc_select(dispatch.binary, dispatch.items[0]);
  (void)dispatch;
  puts("c-ast-golden: OK");
  return 0;
}
