# Allocator ownership contract

This document is the E4.T02 contract for the checked-in `hxrt` allocator slice.
It hardens the internal native-seed boundary used by future runtime features; it
does not make `alloc` compiler-selectable, define a generated public C ABI, or
promise application compatibility for these structs. E4.T11 established the
internal same-major runtime/versioned-manifest contract, while E7 owns exported
application APIs.
The incompatible callback/owner revision advanced the provisional runtime ABI
marker from 0.1.0 to 0.2.0. E4.T03's private string-contract revision advanced
it to 0.3.0, E2.T07's hosted literal-output addition advanced it to 0.4.0, and
E4.T04's additive typed-array slice advances it to 0.5.0. The schema-2 ABI
contract now defines same-major compatibility, but none of these internal
markers is a stable application ABI promise.

## Allocator handle and callback protocol

`hxc_allocator` is a copyable handle whose identity is the complete
`context + allocate + reallocate + release` tuple. The context must remain alive
until every allocation carrying that identity has been disposed. Two handles
compare as the same identity only through `hxc_allocator_same_identity`; context
alone is not sufficient.

`allocate` and `release` are required. `reallocate` is optional: when absent,
`hxrt` allocates a replacement, copies `min(old_size, new_size)` bytes, and then
releases the old block. A conforming callback follows these rules:

- `allocate` is called only for a nonzero size and a validated alignment. It
  writes a non-null aligned block that aliases no live allocation only when
  returning `HXC_STATUS_OK`.
- `reallocate` is called only with non-null old storage and nonzero old/new
  sizes; `old_size` is exact. A non-OK return leaves the old allocation live
  and does not write its output. Success transfers ownership to the returned
  aligned block.
- `release` receives the exact pointer, size, and alignment attached to the
  owner. It cannot report failure and is never called for canonical null/zero
  storage.

Every operation that publishes storage or a computed size uses a status plus
out-parameter boundary. An out-parameter is written only on success. This makes
the common `hxc_realloc(..., &pointer)` form failure-atomic: on failure
`pointer` still names the old live block.

## Size, alignment, and zero-size rules

| Case | Result |
| --- | --- |
| `hxc_size_add` or `hxc_size_mul` overflow | `HXC_STATUS_SIZE_OVERFLOW`; output unchanged |
| Zero size with a valid allocator/alignment | Success with canonical null storage; no allocation callback |
| Alignment is zero or not a power of two | `HXC_STATUS_UNSUPPORTED_ALIGNMENT`; no callback |
| Hosted over-alignment | Supported by an internal base-pointer prefix when size arithmetic and system allocation succeed |
| Reallocate to zero | Old storage is released and null is published |
| Null with nonzero size, or non-null with zero size | `HXC_STATUS_INVALID_ARGUMENT` |
| Callback reports failure | Old allocation and caller output remain unchanged |
| Callback reports success with null/misaligned storage | `HXC_STATUS_INTERNAL_ERROR` |

The hosted adapter uses `malloc`/`free` only behind the allocator callbacks. It
does not rely on `realloc`, `aligned_alloc`, or implementation-specific aligned
allocation APIs. Prefix-size and alignment padding are checked before calling
the system allocator.

## Cross-boundary ownership

`hxc_allocation` is the canonical internal owner for a raw block. It stores the
pointer, exact byte size, alignment, and allocator handle by value. A live owner
is move-only by convention:

1. create it with `hxc_allocation_allocate`;
2. transfer it with `hxc_allocation_move`, which clears the source;
3. resize it with `hxc_allocation_resize`, which preserves the complete record
   on failure; and
4. dispose it with `hxc_allocation_dispose`, which calls the stored allocator
   rather than an ambient/default allocator.

Copying a live `hxc_allocation` creates two aliases to one owner and is invalid.
Initialize every allocate/move destination with `HXC_ALLOCATION_INITIALIZER`;
the operation rejects a nonempty destination instead of overwriting an owner.
Empty disposal is idempotent. A zero-size live allocation still keeps its
allocator identity and alignment until disposal.

## Environment and capability boundary

| Axis | E4.T02 result |
| --- | --- |
| Portable/metal | The allocator slice is profile-neutral; neither profile selects it merely from an import or type mention. |
| Runtime policy | The feature remains `native-seed-only`; compiler programs still reject an `alloc` request. |
| Hosted | The default handle supports checked arbitrary power-of-two alignment subject to address space and `malloc`. |
| Freestanding | The default handle is invalid; a caller-supplied allocator is required. The compiled allocator object has no undefined libc allocation symbol. |
| WASI/Emscripten | No environment-specific allocator evidence exists yet, so the feature remains unavailable there. |
| Generated C | Primitive-only output stays runtime-free. Literal output selects its separate allocation-free carrier and does not include or link `allocator.c` or allocator symbols. |
| Public ABI | C/C++ agree on the native-seed callback types and layouts, but the structs remain internal, versioned, and forbidden from application exports; E7 owns future public admission. |

The executable evidence is registered by
[`test/abi/allocator-contract/case.json`](../test/abi/allocator-contract/case.json).
`scripts/ci/runtime_smoke.py` runs hosted over-alignment and overflow cases,
repeats the hosted allocator path under AddressSanitizer and UndefinedBehaviorSanitizer,
links a freestanding static-arena allocator with failure injection, inspects the
freestanding allocator object for libc allocation symbols, moves an owner
across a fixture boundary, and compares C-produced `sizeof`/alignment/offset
facts with a C++17 consumer under the required GCC and Clang lanes.
