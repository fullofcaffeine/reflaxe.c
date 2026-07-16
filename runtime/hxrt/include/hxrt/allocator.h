#ifndef HXRT_ALLOCATOR_H_INCLUDED
#define HXRT_ALLOCATOR_H_INCLUDED

#include "hxrt/status.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef void *(*hxc_allocate_fn)(void *context, size_t size, size_t alignment);
typedef void *(*hxc_reallocate_fn)(
  void *context,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment
);
typedef void (*hxc_release_fn)(void *context, void *memory, size_t size, size_t alignment);

typedef struct hxc_allocator {
  void *context;
  hxc_allocate_fn allocate;
  hxc_reallocate_fn reallocate;
  hxc_release_fn release;
} hxc_allocator;

/** Hosted default allocator. Callbacks are null in `HXC_FREESTANDING` builds. */
HXC_API hxc_allocator hxc_default_allocator(void);

HXC_API bool hxc_allocator_is_valid(const hxc_allocator *allocator);
HXC_API hxc_status hxc_alloc(
  const hxc_allocator *allocator,
  size_t size,
  size_t alignment,
  void **out_memory
);
HXC_API hxc_status hxc_realloc(
  const hxc_allocator *allocator,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment,
  void **out_memory
);
HXC_API hxc_status hxc_free(
  const hxc_allocator *allocator,
  void *memory,
  size_t size,
  size_t alignment
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_ALLOCATOR_H_INCLUDED */
