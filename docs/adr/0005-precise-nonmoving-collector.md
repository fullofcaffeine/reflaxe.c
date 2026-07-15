# ADR 0005: Precise non-moving portable collector

- Status: Accepted
- Date: 2026-07-14
- Decision owners: project owner and runtime maintainers
- Related requirements: HXC-RT-003, HXC-RT-005, HXC-RT-006, HXC-RT-007,
  HXC-RT-008, HXC-SEM-025

## Context

Portable Haxe permits cyclic, identity-bearing object graphs, so reference
counting alone is not a complete memory strategy. A conservative collector is
easy to bootstrap on some hosted systems but obscures roots, retains false
positives, and does not provide one deterministic contract across freestanding,
WASI, and foreign C stacks.

At the same time, selecting a collector for every allocation would contradict
the project's direct-C and runtime-free contracts. The memory backend must be a
selective implementation detail, not a field baked into every generated or
exported type.

## Decision

### The default portable backend is precise, non-moving mark-and-sweep

When reachable semantics require tracing collection, the selected `gc` runtime
feature uses a target-owned stop-the-world, non-moving, tracing mark-and-sweep
collector. It reclaims cycles and never relocates a live allocation. Allocation
uses the versioned allocator ABI, with overflow and failure behavior owned by
the allocator/runtime contracts.

The compiler emits exact trace descriptors or specialized trace functions for
managed fields. It also emits explicit root metadata:

- managed globals are registered in deterministic tables;
- compiler-owned functions link explicit stack/root frames while managed
  values are live across safepoints;
- each participating thread registers its root chain and cooperates with the
  stop-the-world protocol;
- exception payloads and cleanup state remain rooted while unwinding.

The default backend does not conservatively scan arbitrary C stack words,
registers, globals, or foreign allocations. Collection and allocation reports
include live bytes, reclaimed bytes, allocation count, collection count, and
pause measurements so the implementation can be evaluated rather than assumed
cheap.

### Managed pointer rules are explicit

A managed allocation's base address is stable until reclamation, but an
interior or foreign pointer is not automatically a GC root:

- a typed managed handle/root keeps the owning base allocation alive;
- a borrowed interior `c.Ptr` is valid only within the proven owner lifetime
  and may not be the sole reference across a safepoint;
- a raw pointer retained by foreign C must be paired with an explicit pinned
  root/handle and release operation;
- memory returned by foreign allocators is opaque unless a registered adapter
  supplies exact tracing and lifetime operations.

The compiler and typed `c.*` APIs diagnose provable escapes. Raw-pointer code
remains unsafe and cannot claim that the collector inferred ownership from an
address-shaped value.

### Collection remains selective and backend-pluggable

Escape, lifetime, and representation analysis runs before the collector is
requested. Static literals, values proven not to escape, bounded regions,
manual ownership, and program-local specialized storage remain direct C when
they preserve Haxe identity and lifetime. `hxc_runtime=none` rejects every
remaining `gc` requirement and emits no collector code or metadata.

Generated public types and exported C ABIs contain no backend-private header or
pointer. A versioned internal backend interface owns allocation, root/thread
registration, safepoints, collection, tracing, statistics, and shutdown.
Alternative precise backends and an optional conservative hosted adapter may
implement that interface, but they must pass the same reachability, cycle,
cleanup, thread-root, failure, and reporting conformance suite. A conservative
adapter is never the Tier 1 default and its restrictions are reported.

Portable Haxe has no general deterministic finalization guarantee. Native
resources use explicit `Owned`/cleanup contracts. Any runtime-internal
finalizer is at-most-once, non-prompt, non-resurrecting, and separately tested;
it must not become the primary resource-management API.

## Consequences

- Cycles are collectible without moving addresses observed by C or FFI code.
- Exact descriptors and shadow roots add compiler work but avoid accidental
  retention and platform-specific conservative scanning assumptions.
- Stop-the-world pauses and fragmentation are accepted baseline tradeoffs and
  must be measured before beta.
- Thread support and the collector share a registration/safepoint protocol;
  single-thread builds do not link thread coordination.
- Runtime-free and manual/region-managed programs remain collector-free.
- An implementation experiment may change backend internals without changing
  generated public types or the portable reachability contract.

## Rejected alternatives

- Reference counting as the only portable strategy: it cannot reclaim ordinary
  cyclic Haxe object graphs without an additional cycle collector.
- Conservative scanning as the default: false roots and foreign-stack behavior
  are difficult to make deterministic across the accepted platform matrix.
- A moving collector as the first backend: it complicates C pointer stability,
  pinning, callbacks, and exported handles before the compiler has mature root
  and barrier infrastructure.
- Selecting GC for every portable allocation: it would hide avoidable runtime
  cost and make the hard no-runtime proof impossible.
- Relying on GC finalizers for C resources: collection time is not deterministic
  cleanup time.
