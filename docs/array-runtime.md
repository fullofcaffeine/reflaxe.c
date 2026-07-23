# Typed resizable-array runtime contract

This document records both the bounded E4.T04 native `hxrt` array storage and
the first E5.T03 ordinary-Haxe lowering that selects it. A program can now use
empty or nonempty `Array<T>` literals, aliases, `length`, checked indexing,
`push`, and source-order iteration for the admitted element types described
below. Elements may now be plain direct values, `haxe.io.Bytes`, another
managed Array, a tagged enum with managed Array payloads, a closed record
that recursively contains those values, or a concrete mutable class reference.
Such a record may also contain
the direct, unmanaged `Null<Record>` representation documented in
[aggregate lowering](aggregate-lowering.md). This is deliberately not
general collection parity: other methods, escaping element-copy ownership,
other managed element families, and broad standard-library behavior still fail
before C is written. `Array<Class>` is deliberately different from the earlier
acyclic value families: it uses the precise collector and can reclaim cycles.

The original typed storage advanced the provisional same-major runtime ABI from
0.4.0 to 0.5.0. Adding the compiler-used shared-identity container advances it
to 0.6.0. These are internal compatibility markers, not a stable application
ABI or supported-release promise.

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

An ordinary Haxe Array has shared identity: after `alias = values`, a `push`
through either name must be visible through both. The compiler therefore emits
an `hxc_array_ref *`, not a by-value copy of `hxc_array`. `hxc_array_ref` owns
the one move-only buffer plus a local reference count. Assigning an admitted
local alias retains that container; leaving its scope releases it; the final
release destroys the elements and frees both storage layers. The current
evidence covers acyclic graphs, where this small ownership mechanism is
sufficient.

Haxe's explicit `Null<Array<T>>` spelling uses that same `hxc_array_ref *`
carrier. A present value points at the shared container and an absent value is
plain C `NULL`; the compiler does not wrap the pointer in another
`{ has_value, value }` record. Direct null checks therefore become pointer
comparisons. A returned Array transfers its fresh or local owner to the caller;
returning a borrowed Array first retains it. The matching runtime retain and
release operations treat `NULL` as a successful no-op, while still rejecting a
malformed non-null reference. This makes cleanup of a dynamically nullable
local safe without turning null into an empty Array.

`Array<Class>` cannot use that local reference count. A class can point to an
Array that points back to the same class, so neither side would ever reach a
zero count. For this one graph-shaped family, the compiler instead asks the
precise collector for stable outer Array storage. The resizable backing buffer
still uses the same checked `hxc_array` implementation. Two matching operations
make the ownership boundary explicit:

- `hxc_array_ref_init_in_place` initializes a zeroed Array payload that the
  collector already owns; and
- `hxc_array_ref_dispose_in_place` releases only the backing buffer when the
  collector sweeps that payload. It never frees the collector-owned outer
  address.

The Array descriptor walks the live pointer slots and reports each non-null
class base exactly. Pointer relocation during growth therefore moves pointer
values, not class objects; aliases keep observing the same mutable instances.

Every admitted element representation is byte-relocatable: moving the same live
value to another correctly aligned address preserves it without invoking a
logical copy or destructor. Logical copies, assignments, and destruction still
use callbacks when present. A future representation that is not byte-
relocatable must choose another typed plan; it cannot silently enter this
slice.

`hxc_array`, `hxc_array_ref`, and `hxc_array_element_ops` are private runtime
structs. The schema-3 ABI manifest explicitly forbids all three in generated
application exports. Fixed
`c.CArray<T>`, `c.Span<T>`, and `c.ConstSpan<T>` remain separate direct,
non-owning representations and select no array feature.

The compiler-used element plan currently admits non-string scalar values,
`haxe.io.Bytes`, managed Arrays, tag-aware managed enums, and closed records
whose fields recursively contain those values. A nested `Null<Record>` is also
admitted when that optional payload is itself recursively unmanaged; copying
the enclosing record copies its explicit
presence flag and inline payload like any other direct value. Scalars use the
null-callback byte-copy strategy. A Bytes element or a
record containing managed fields gets one program-local, element-specialized
copy/assign/destroy callback trio. In plain terms, the generated functions know
the exact record shape and do three jobs:

1. copying keeps each shared buffer alive and undoes an incomplete copy
   if a later retain fails;
2. assignment keeps the replacement alive before releasing the old value; and
3. destruction releases the fields in reverse order.

The record remains an unboxed, statically typed C struct. Another Array field
uses `hxc_array_ref_retain`/`release`; a tagged enum delegates to its own helper,
which switches on the discriminant and visits only the active union arm. This
is important: reading every union member would inspect inactive storage and
could release a pointer that was never constructed. The callbacks do not
introduce reflection, a generic box, or a tracing collector.

An owned String or another unsupported managed value is still rejected. A
class element is admitted only through the exact traced representation: direct
nonescaping classes remain stack-shaped C values, while every class reachable
from the admitted `Array<Class>` graph receives stable collector storage and a
descriptor. Each additional managed family still needs a complete lifetime
rule before it can broaden the support claim.

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
any successfully constructed prefix if a later copy fails. A future broader
Haxe lowering must supply the correct static-target default—zero, `0.0`,
`false`, or null—after representation selection. The current compiler slice
does not expose `resize`; the generic runtime never guesses a type or
manufactures a boxed default.

Borrowed pointers returned by `hxc_array_at` and `hxc_array_at_const` remain
valid only while the array stays alive and no mutation can relocate or shift
their slot. Out-of-range and invalid calls leave output pointers unchanged.
The compiler-used checked-copy operation converts an invalid index into its
explicit fail-stop edge. A future borrowed-element or nullable lookup API must
define its own source-positioned lifetime and result policy.

Generated checked indexing returns a logical element copy, not a pointer into
the resizable buffer. For a managed element, HxcIR first creates a hidden owned
temporary, then lends that value to the surrounding expression, and finally
runs the matching typed destroy callback on every normal exit. “Lends” means
the expression may read the temporary while the compiler keeps ownership of
it. For example, `history[index].after.get(0)` may read the copied record and
its Bytes field, after which the temporary record is cleaned up.

Returning that borrowed value or passing it somewhere that could keep it is
still rejected before C is emitted. Those operations need an explicit transfer
of ownership; accepting them without that rule would either leak the copy or
destroy it while another user still refers to it.

An enum-pattern binding such as `case Schedule(arguments)` is different: the
binding borrows the Array from the still-live enum owner for that switch arm.
The compiler therefore does not retain it merely because Haxe gives the payload
a local name. This keeps the generated C cleanup in the correct lexical scope.
Checked indexing that creates an owned managed-element copy inside nested
control flow remains fail-closed until scoped cleanup regions can represent
that shorter lifetime.

## Feature and capability boundary

The `array` feature is compiler-selectable and depends exactly on `alloc`, whose
closure adds `status` and `runtime-base`. A reachable admitted Haxe Array
selects:

```text
runtime-base + status + alloc + array
```

No string, object, collector, dynamic, reflection, exception, or thread source
is selected for those value-only Arrays. A mere unreachable type mention still selects nothing. Every
reachable create/read/mutate/retain/release operation has a source-rooted reason
in `hxc.runtime-plan.json`; `hxc_runtime=none` rejects those reasons before any
artifact is written. The fixed-array/span suite continues to prove a positive
runtime-none plan and zero `hxrt` artifacts or symbols.

An admitted `Array<Class>` graph selects the larger, still exact closure:

```text
runtime-base + status + alloc + array + object + gc
```

`object` supplies immutable size/alignment/trace/finalizer descriptors. `gc`
supplies stable allocation, exact roots, and cycle reclamation. The compiler
emits neither feature for the direct class fixture or for an ordinary
`Array<Int>`/record/enum program. `hxc_runtime=none` rejects the traced graph
before any plausible C project is written.

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

The same registered suite also compiles an ordinary-Haxe executable through
the production custom target. It checks managed Array HxcIR before C syntax is
chosen, compares normal, reversed-discovery, and two requests through one warm
Haxe compilation server byte-for-byte, checks the exact dependency-closed
runtime feature set and source reasons, compiles the emitted project as
warning-clean C11,
runs it under sanitizers, and rejects unsupported ownership or element shapes
without leaving output. The fixture includes both `Array<Int>` and an unboxed
Array-owning tagged enum whose payload is `Array<enum>`, a closed record
containing another Array, and a closed record containing two shared Bytes
references and a direct optional bounds record in a class-owned Array. It
copies, matches, projects, and mutates the managed enum and nested-record paths,
then mutates a Bytes value through the
original alias, observes that change through a copied Array element, and reads
the guarded optional payload. Structural checks require the typed callbacks
and the HxcIR owned-temporary, borrow, and cleanup order. Native reference
counters and sanitizers prove balanced success, replacement, rollback, and
reverse destruction.

A second ordinary-Haxe program in the same suite stores mutable
`ManagedNode` instances in `Array<ManagedNode>`. It grows the Array, reads and
replaces elements, retains a null slot, mutates through an alias, and connects
two nodes into a cycle. It crosses the deterministic collector-pressure threshold and then
reads the live graph, so a missing class or Array trace edge becomes an
observable native failure. Structural assertions require managed HxcIR
allocations, exact root slots, class and Array descriptors, node-to-Array and
Array-to-node trace edges, and the in-place Array finalizer. Each node also owns an ordinary
`Array<Int>`; its generated class finalizer releases that reference-counted
field while leaving collector-owned links to tracing. After the generated
Haxe function returns and its root frame is gone, an independent C test driver
forces collection and checks the public statistics: every remaining graph
payload must be reclaimed. Strict native execution and sanitizers exercise
both paths. The same source also runs under the pinned Haxe Eval oracle, while
split, package, and unity projects compare repeated/reversed discovery bytes
and the split project compares two requests to one warm Haxe compilation
server. Finally, the suite invokes Caxecraft's real compile-only path and
requires it to reach the later `FlowRuntimeDiagnostic` boundary, proving the
former `CaxeFlowState` Array failure has not returned.

Run the focused evidence with:

```sh
npm run test:array-runtime
npm run test:runtime-features
npm run test:span-lowering
npm run test:native
```

This evidence implements only the named generated-Haxe slices. Whole-reference
reassignment, arguments that transfer ownership, managed families beyond the
admitted Array/Bytes/class graph, most upstream methods, public export layout,
and performance claims remain deferred to their named owners. Array returns are
part of the implemented slice: the fixture exercises a nullable local Array
whose owner moves to the caller.
