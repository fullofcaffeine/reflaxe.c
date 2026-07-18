/*
 * Fixture-only custom-allocator contract probe.
 *
 * The native harness uses this bounded arena to test freestanding operation,
 * alignment, overflow, callback identity, injected failure, and publication
 * atomicity without relying on libc allocation. It is not runtime source and is
 * never emitted or linked merely because generated Haxe mentions allocation.
 */
#include "hxrt/allocator.h"

typedef struct hxc_test_arena {
  unsigned char storage[2048];
  size_t cursor;
  size_t allocation_count;
  size_t release_count;
  void *last_released;
  size_t last_release_size;
  size_t last_release_alignment;
  bool force_failure;
  bool force_misalignment;
} hxc_test_arena;

static hxc_status hxc_test_arena_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  hxc_test_arena *arena = (hxc_test_arena *)context;
  uintptr_t address;
  size_t aligned_cursor;
  size_t end;
  size_t padding;
  size_t remainder;
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
  status = hxc_size_add(arena->cursor, padding, &aligned_cursor);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_size_add(aligned_cursor, size, &end);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (end > sizeof(arena->storage)) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }

  if (arena->force_misalignment) {
    if (end == sizeof(arena->storage)) {
      return HXC_STATUS_OUT_OF_MEMORY;
    }
    *out_memory = arena->storage + aligned_cursor + 1u;
    end++;
  } else {
    *out_memory = arena->storage + aligned_cursor;
  }
  arena->cursor = end;
  arena->allocation_count++;
  return HXC_STATUS_OK;
}

static void hxc_test_arena_release(
  void *context,
  void *memory,
  size_t size,
  size_t alignment
) {
  hxc_test_arena *arena = (hxc_test_arena *)context;
  arena->release_count++;
  arena->last_released = memory;
  arena->last_release_size = size;
  arena->last_release_alignment = alignment;
}

static hxc_allocator hxc_test_allocator(hxc_test_arena *arena) {
  hxc_allocator allocator;
  allocator.context = arena;
  allocator.allocate = hxc_test_arena_allocate;
  allocator.reallocate = NULL;
  allocator.release = hxc_test_arena_release;
  return allocator;
}

static int hxc_test_produce(
  const hxc_allocator *allocator,
  hxc_allocation *out_allocation
) {
  hxc_allocation local = HXC_ALLOCATION_INITIALIZER;
  uint8_t *bytes;
  size_t index;
  hxc_status status = hxc_allocation_allocate(
    allocator,
    4u,
    8u,
    64u,
    &local
  );
  if (status != HXC_STATUS_OK || !hxc_allocation_is_valid(&local)) {
    return 20;
  }
  bytes = (uint8_t *)local.memory;
  for (index = 0u; index < local.size; index++) {
    bytes[index] = (uint8_t)(index + 1u);
  }
  status = hxc_allocation_move(&local, out_allocation);
  if (status != HXC_STATUS_OK) {
    (void)hxc_allocation_dispose(&local);
    return 21;
  }
  if (local.memory != NULL
    || local.size != 0u
    || local.alignment != 0u
    || hxc_allocation_is_valid(&local)) {
    return 22;
  }
  if (hxc_allocation_dispose(&local) != HXC_STATUS_OK) {
    return 23;
  }
  return 0;
}

static int hxc_test_consume(
  hxc_allocation *allocation,
  const hxc_allocator *origin,
  hxc_test_arena *arena
) {
  const void *old_memory = allocation->memory;
  const size_t old_size = allocation->size;
  const size_t old_release_count = arena->release_count;
  uint8_t *bytes;
  size_t index;
  hxc_status status;

  if (!hxc_allocation_is_valid(allocation)
    || !hxc_allocator_same_identity(&allocation->allocator, origin)
    || allocation->size != 32u
    || ((uintptr_t)allocation->memory % (uintptr_t)64u) != (uintptr_t)0u) {
    return 30;
  }
  if (hxc_allocation_allocate(
      origin,
      1u,
      1u,
      1u,
      allocation
    ) != HXC_STATUS_INVALID_ARGUMENT
    || allocation->memory != old_memory
    || allocation->size != old_size) {
    return 37;
  }

  arena->force_failure = true;
  status = hxc_allocation_resize(allocation, 32u, 8u);
  arena->force_failure = false;
  if (status != HXC_STATUS_OUT_OF_MEMORY
    || allocation->memory != old_memory
    || allocation->size != old_size
    || arena->release_count != old_release_count) {
    return 31;
  }

  bytes = (uint8_t *)allocation->memory;
  for (index = 0u; index < allocation->size; index++) {
    if (bytes[index] != (uint8_t)(index + 1u)) {
      return 32;
    }
  }

  status = hxc_allocation_resize(allocation, 16u, 8u);
  if (status != HXC_STATUS_OK
    || allocation->size != 128u
    || !hxc_allocation_is_valid(allocation)
    || !hxc_allocator_same_identity(&allocation->allocator, origin)
    || arena->release_count != old_release_count + 1u
    || arena->last_released != old_memory
    || arena->last_release_size != old_size
    || arena->last_release_alignment != 64u) {
    return 33;
  }
  bytes = (uint8_t *)allocation->memory;
  for (index = 0u; index < old_size; index++) {
    if (bytes[index] != (uint8_t)(index + 1u)) {
      return 34;
    }
  }

  status = hxc_allocation_resize(allocation, 0u, 8u);
  if (status != HXC_STATUS_OK
    || allocation->memory != NULL
    || allocation->size != 0u
    || !hxc_allocation_is_valid(allocation)
    || arena->release_count != old_release_count + 2u) {
    return 35;
  }
  status = hxc_allocation_dispose(allocation);
  if (status != HXC_STATUS_OK
    || allocation->memory != NULL
    || allocation->alignment != 0u
    || hxc_allocation_is_valid(allocation)
    || arena->release_count != old_release_count + 2u) {
    return 36;
  }
  return 0;
}

int main(void) {
  hxc_test_arena arena = {0};
  hxc_test_arena other_arena = {0};
  hxc_allocator allocator = hxc_test_allocator(&arena);
  hxc_allocator alias = allocator;
  hxc_allocator other = hxc_test_allocator(&other_arena);
  hxc_allocator unavailable = hxc_default_allocator();
  hxc_allocation allocation = HXC_ALLOCATION_INITIALIZER;
  hxc_allocation overflow = HXC_ALLOCATION_INITIALIZER;
  size_t size_result = 17u;
  unsigned char marker = 0u;
  void *memory = &marker;
  size_t calls;
  int result;

  if (hxc_allocator_is_valid(&unavailable)
    || !hxc_allocator_is_valid(&allocator)
    || !hxc_allocator_same_identity(&allocator, &alias)
    || hxc_allocator_same_identity(&allocator, &other)) {
    return 1;
  }

  if (hxc_size_add(SIZE_MAX, 1u, &size_result) != HXC_STATUS_SIZE_OVERFLOW
    || size_result != 17u
    || hxc_size_mul(SIZE_MAX, 2u, &size_result) != HXC_STATUS_SIZE_OVERFLOW
    || size_result != 17u
    || hxc_size_mul(0u, SIZE_MAX, &size_result) != HXC_STATUS_OK
    || size_result != 0u) {
    return 2;
  }

  calls = arena.allocation_count;
  if (hxc_alloc(&allocator, 0u, 64u, &memory) != HXC_STATUS_OK
    || memory != NULL
    || arena.allocation_count != calls) {
    return 3;
  }
  memory = &marker;
  if (hxc_alloc(&allocator, 8u, 3u, &memory)
      != HXC_STATUS_UNSUPPORTED_ALIGNMENT
    || memory != &marker
    || arena.allocation_count != calls) {
    return 4;
  }
  arena.force_misalignment = true;
  memory = &marker;
  if (hxc_alloc(&allocator, 8u, 64u, &memory) != HXC_STATUS_INTERNAL_ERROR
    || memory != &marker
    || arena.release_count != 1u
    || arena.last_release_size != 8u
    || arena.last_release_alignment != 64u) {
    return 7;
  }
  arena.force_misalignment = false;
  if (hxc_allocation_allocate(
      &allocator,
      SIZE_MAX,
      2u,
      64u,
      &overflow
    ) != HXC_STATUS_SIZE_OVERFLOW
    || overflow.memory != NULL
    || overflow.alignment != 0u) {
    return 5;
  }

  result = hxc_test_produce(&allocator, &allocation);
  if (result != 0) {
    return result;
  }
  result = hxc_test_consume(&allocation, &allocator, &arena);
  if (result != 0) {
    return result;
  }
  if (other_arena.release_count != 0u) {
    return 6;
  }
  return 0;
}
