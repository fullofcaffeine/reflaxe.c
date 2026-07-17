# Typed resizable-array runtime contract

This document records the bounded E4.T04 native `hxrt` array slice. It provides
the shared storage mechanics needed when compile-time representation and
program-local specialization cannot keep a resizable array runtime-free. It
does not make general Haxe `Array<T>` lowering or the standard-library surface
available. E3.T03 still owns generic specialization, E5.T03 owns collection
parity, and E4.T05/E4.T06 own object descriptors and tracing collection.

The additive internal types and functions advance the provisional same-major
runtime ABI from 0.4.0 to 0.5.0. This remains an internal compatibility marker,
not a stable application ABI or supported-release promise.

## Representation and specialization boundary

`hxc_array` owns one contiguous `hxc_allocation`, a logical length, a capacity,
and one `hxc_array_element_ops` strategy. Elements are unboxed fixed-width
values. The strategy supplies exact size and alignment plus either:

- three null callbacks for trivial byte-relocatable primitives; or
- a complete copy/assign/destroy callback trio for values with logical
  ownership, including references.

The callbacks do not turn every value into a generic box. A compiler may emit a
concrete typed wrapper or direct program-local operations around this storage,
and should omit `hxrt` entirely for bounded/static arrays. The shared slice is
only the last step in the required direct-C, specialized-helper, selective-
runtime order.

Every admitted element representation is byte-relocatable: moving the same live
value to another correctly aligned address preserves it without invoking a
logical copy or destructor. Logical copies, assignments, and destruction still
use callbacks when present. A future representation that is not byte-
relocatable must choose another typed plan; it cannot silently enter this
slice.

`hxc_array` and `hxc_array_element_ops` are private runtime structs. The schema-2
ABI manifest explicitly forbids both in generated application exports. Fixed
`c.CArray<T>`, `c.Span<T>`, and `c.ConstSpan<T>` remain separate direct,
non-owning representations and select no array feature.

## Growth and ownership invariants

Initialization retains the supplied allocator identity without allocating.
Capacity begins at zero, then grows deterministically from four slots by
doubling until the requested minimum fits. Both the capacity bound and
`capacity * element_size` use checked arithmetic. A request beyond the
representable byte size returns `HXC_STATUS_SIZE_OVERFLOW` before an allocator
callback.

Reserve and allocator-backed growth are failure-atomic:

- a failed allocation leaves the original pointer, byte size, length, capacity,
  and live elements unchanged;
- successful relocation preserves element bytes and invalidates prior borrowed
  slot pointers;
- capacity may remain larger after a later element-copy callback fails, but the
  logical length and sequence remain unchanged; and
- move transfers the unique storage owner and resets the source to
  `HXC_ARRAY_INITIALIZER`.

Disposal destroys live elements from the end toward the beginning and then
releases storage through the allocator stored in `hxc_allocation`. A canonical
initializer can be disposed repeatedly. Copying a live `hxc_array` struct would
duplicate one owner and is invalid; use `hxc_array_move`.

## Mutation, aliasing, and lifecycle rules

`push_copy`, `insert_copy`, `set_copy`, `remove_at`, and `resize` preserve the
logical source order. Exact existing-slot sources are supported:

- push records a source index before reserve and resolves it again after a
  possible relocation;
- insert shifts the suffix as byte-relocated values, adjusts a shifted source
  index, and restores the old sequence if copy construction fails;
- assignment of a slot to itself is a no-op; reference-like assignment must
  retain the replacement before releasing the prior destination; and
- remove destroys exactly the removed element and relocates the suffix left.

A lifecycle copy constructs into uninitialized storage. On failure it must
leave no live destination. Assignment operates on one live destination and is
failure-atomic: a non-OK result leaves source and destination unchanged.
Destruction cannot fail. Callback context outlives the array, and callbacks may
not re-enter or mutate that array. These rules make rollback reviewable without
requiring object, collector, reflection, or dynamic-value machinery.

Shrinking `resize` destroys removed elements in reverse order. Growing reserve
copy-constructs each new slot from a caller-provided typed default and destroys
any successfully constructed prefix if a later copy fails. The future typed
Haxe lowering supplies the correct static-target default—zero, `0.0`, `false`,
or null—after representation selection. The generic runtime never guesses a
type or manufactures a boxed default.

Borrowed pointers returned by `hxc_array_at` and `hxc_array_at_const` remain
valid only while the array stays alive and no mutation can relocate or shift
their slot. Out-of-range and invalid calls leave output pointers unchanged.
The later Haxe collection lowering must apply its source-positioned bounds and
nullable-return policies around these internal primitives.

## Feature and capability boundary

The `array` feature is `native-seed-only` and depends exactly on `alloc`, whose
closure adds `status` and `runtime-base`. Selecting it packages:

```text
runtime-base + status + alloc + array
```

No string, object, collector, dynamic, reflection, exception, or thread source
is selected. Compiler-program planning still rejects `array`, so seeing a Haxe
array type or importing a typed C span cannot turn this native fixture into a
generated-Haxe support claim. The fixed-array/span suite continues to prove a
positive runtime-none plan and zero `hxrt` artifacts or symbols.

## Executable evidence

[`test/differential/array-runtime`](../test/differential/array-runtime) runs the
slice as strict C11 under GCC and Clang at `-O0`, `-O2`, and combined Address/
UndefinedBehaviorSanitizer settings. Its static custom allocator always moves
on growth and injects allocation failure without libc allocation dependencies.
The fixture proves:

- primitive `int32_t` growth, indexing, push, insert, assignment, removal,
  resize, owner move, and overflow rejection;
- exact-slot aliasing across both relocation and suffix shifts;
- reference-element retain-before-release assignment and balanced destruction;
- rollback after insertion and partial resize lifecycle failures;
- unchanged logical contents after allocation failure; and
- absence of string, object, GC, reflection, and dynamic symbol families.

A pinned Haxe Eval trace covers the common observable mutation sequence. Eval
is a dynamic target, so the oracle pushes explicit zero values instead of using
its null-filling growth behavior; the native fixture separately supplies and
checks the static `Int` default during `resize`.

The typed runtime-feature fixture renders the dependency closure twice,
packages the exact reviewed source hashes, and compiles an independent array
consumer. The all-seed smoke lane also consumes the header from C++17 and
compiles every runtime source in hosted and freestanding modes.

Run the focused evidence with:

```sh
npm run test:array-runtime
npm run test:runtime-features
npm run test:span-lowering
npm run test:native
```

This evidence closes the E4.T04 native runtime slice only. General `Array<T>`
typed-AST/HxcIR lowering, all upstream methods and edge cases, object/GC
integration, public export layout, and performance claims remain deferred to
their named owners.
