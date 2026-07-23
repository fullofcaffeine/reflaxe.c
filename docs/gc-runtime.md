# Precise non-moving collector

<!-- hxrt-feature:gc -->

The optional `gc` runtime slice implements the memory backend selected by
[ADR 0005](adr/0005-precise-nonmoving-collector.md): exact roots, stable object
addresses, and mark-and-sweep reclamation for identity-bearing Haxe graphs that
may contain cycles.

“Precise” means the compiler tells the runtime exactly which values are managed
object bases. The collector does not inspect arbitrary stack bytes and guess
that an integer-shaped bit pattern might be a pointer. “Non-moving” means a
live payload keeps the same address until it becomes unreachable and is
reclaimed. That makes typed C borrows and foreign-function integration easier
to reason about, though it does not make an unrooted interior pointer safe.

This feature is selective. A direct value, static literal, nonescaping class,
or compiler-proven bounded region still emits ordinary C and no collector.
Merely importing or declaring a class never selects `gc`.

## Allocation layout and exact lookup

Each managed allocation contains backend-private bookkeeping followed by an
aligned generated payload:

```text
private collector node
  allocation-list and mark-worklist links
  hxc_object_header -> immutable hxc_type_descriptor
  size/alignment/finalizer state
aligned generated payload
  ordinary typed C fields
```

The payload pointer is the stable Haxe object identity. Private links and mark
bits never become generated fields or public application binary interface
(ABI) facts. The current correctness baseline validates an exact payload base
by looking it up in the collector's allocation list. That deliberately rejects
interior and foreign pointers without dereferencing unknown memory. It is
linear in the number of live allocations per lookup, so the pause report must
guide a later indexed-lookup optimization before large-heap performance is
claimed.

## Roots in plain language

A root is a managed reference that is live for a reason outside another managed
payload. If an object can be reached by following exact descriptor fields from
any root, the mark phase keeps it. Everything else is swept.

The runtime exposes three explicit root forms:

- `hxc_gc_root_frame` contains compiler-maintained local values for one lexical
  function scope. Frames are pushed and popped in last-in, first-out order.
- `hxc_gc_root_table` contains mutable global values with a program lifetime.
- `hxc_gc_pin` keeps one exact base alive while foreign C retains it.

Slots use an array of `const void *` values. Generated code copies a typed
managed pointer into the corresponding slot whenever a rooted local changes.
It does not cast a `T **` to `void **`, which would violate C's aliasing rules.

Null is an ordinary empty root. A non-null root must equal the base returned by
`hxc_gc_allocate`. An interior address or unrelated foreign pointer makes
collection fail closed with `HXC_STATUS_INVALID_ARGUMENT`. If a generated trace
callback reports such an address, that is a compiler/runtime-plan defect and
collection reports `HXC_STATUS_INTERNAL_ERROR` without sweeping.

## Thread boundary

`hxc_gc_thread` is currently an execution-context root chain, not a promise of
operating-system thread support. The collector can mark several registered
chains, which proves the shape required by future threads, but every API call
currently requires managed execution to be stopped. The runtime advertises no
thread or atomic capability, contains no hidden lock, and must not be called
concurrently.

E5.T11 owns actual thread registration, synchronization, safepoint
coordination, and stop-the-world handshakes. A single-thread build links none of
that future machinery.

## Collection and pressure

The mark phase uses an intrusive worklist stored in the private allocation
nodes, so tracing does not need a second allocation that could fail while the
heap is already under pressure. Exact descriptor callbacks add newly reached
payload bases to that worklist. Cycles terminate naturally because a marked
node is queued only once.

The sweep phase invokes an optional finalizer at most once and then releases
the complete allocation through the configured allocator. Finalizers are
non-prompt and non-resurrecting; native resources should use explicit ownership
and cleanup instead of depending on collection time.

`hxc_gc_allocate` checks a deterministic live-payload threshold before a new
allocation. If the projected size crosses it, collection runs first. After a
successful collection, the next threshold is the larger of the configured
minimum and twice the remaining live payload size. `hxc_gc_safepoint` exposes
the same pressure decision without forcing a collection; `hxc_gc_collect`
forces one.

The compiler must publish a fresh allocation in a root slot before the next
allocation or safepoint. That is an HxcIR lifetime invariant, not a convention
for the C printer to repair.

## Reports and clocks

`hxc_gc_stats` reports allocations, completed collections, pressure-triggered
collections, live/peak/reclaimed objects and payload bytes, collector overhead,
and pause ticks. Counts saturate rather than overflow.

Freestanding C has no required monotonic clock, so configuration accepts an
optional `hxc_gc_clock_fn`. The embedding target owns the tick unit and
monotonicity. With no callback, pause sampling is explicitly unavailable. A
clock that moves backward increments `clock_regression_count` and records a
zero-duration sample rather than corrupting the totals.

These counters make cost observable; they are not yet a performance claim. The
collector cannot portably observe fragmentation inside an arbitrary external
allocator, so it reports its own live payload and internal overhead instead of
inventing a misleading heap-fragmentation percentage.

## Current compiler boundary

E4.T06 owns the backend, exact root/pin contracts, pressure behavior, reports,
and selective `runtime-base + status + alloc + object + gc` package. HxcIR
schema 17 now records exact managed parameters, instruction results, and typed
paths to managed references embedded in direct records, enums, or optionals; and
generated functions emit balanced frames for normal and early propagated
returns. The project emitter owns one request-local collector and main
execution-context chain in unity, module-split, and package-coalesced layouts.
It initializes them before the Haxe entry function and unregisters/disposes them
afterward.

`haxe_c-53k.2.1.2` connects this foundation to one bounded generated-Haxe
graph: a concrete class used through `Array<Class>`. Whole-program
representation settling marks the class and array as collector-managed,
generated `new` publishes its allocation in a root before construction, and
exact descriptors trace class fields and live array slots. The class finalizer
releases any independently reference-counted fields, while the array finalizer
disposes its backing buffer. The fixture proves alias-preserving mutation,
growth, indexed get/set, and live pressure tracing through a two-object cycle.
An independent native driver then drops the generated root frame, forces a
collection, and checks that every remaining graph payload was reclaimed; both
paths run under strict C11 and sanitizers. A runtime-none request rejects this
graph, while the existing direct-class and primitive-array fixtures remain
collector-free.

This is still not general escaping-class support. Managed virtual hierarchies,
interfaces, generic classes, inline owned-class fields, and other unproved
escape shapes remain fail-closed. That boundary matters: one exact graph must
not silently become a claim that every Haxe object shape is collectable.

This separation is intentional: the runtime fixture independently proves the
C memory backend, while a generated-Haxe fixture must separately prove that the
compiler emits correct roots and descriptors. Neither layer validates itself
by comparing against output it generated.

Run the focused evidence with:

```sh
npm run test:gc-runtime
npm run test:array-runtime
npm run test:runtime-features
```

The native contract covers reachable and unreachable cycles, exact stack,
global, and multiple execution-context roots, foreign pins, rejected interior
pointers, malformed trace output, allocation pressure, deterministic fake-clock
reports, a custom freestanding allocator, strict C11 at `-O0`/`-O2`, C++17
header use, and AddressSanitizer/UndefinedBehaviorSanitizer execution. A second
fixture starts from typed HxcIR, emits structural C twice, and compares the
bytes. It collects while generated normal and failing-call frames are active,
then proves reclamation after each return. It also compiles, runs, and
C++-consumes the generated collector context in unity, split, and package
layouts. Required CI supplies real GCC and Clang authority when the local
machine has only one identified family.
