# Typed resizable-array runtime contract

This document records both the bounded E4.T04 native `hxrt` array storage and
the first E5.T03 ordinary-Haxe lowering that selects it. A program can now use
empty or nonempty `Array<T>` literals, aliases, `length`, checked indexing,
`push`, `copy`, in-place `sort`, and source-order iteration for the admitted element types
described below. An exact managed `Array<String>` also supports
`join(separator)` with one explicit String separator. Elements may now be
plain direct values,
`haxe.io.Bytes`, another
managed Array, a tagged enum with managed Array payloads, a closed record
that recursively contains those values, or a concrete mutable class reference.
Such a record may also contain
the direct, unmanaged `Null<Record>` representation documented in
[aggregate lowering](aggregate-lowering.md). This is deliberately not
general collection parity: other methods, escaping element-copy ownership,
other managed element families, and broad standard-library behavior still fail
before C is written. `Array<Class>` is deliberately different from the earlier
acyclic value families: it uses the precise collector and can reclaim cycles.

The original typed storage advanced the provisional same-major runtime
Application Binary Interface (ABI) from 0.4.0 to 0.5.0. Adding the
compiler-used shared-identity container advanced it to 0.6.0. The two public
copy entry points advanced the internal marker from 0.10.0 to 0.11.0. Adding
the compiler-used in-place sort entry point advances the current marker to
0.12.0; the intervening additions are recorded in their owning runtime
documents. These are internal compatibility markers, not a stable application
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

The starting rule is Haxe behavior, not a preference internal to this
repository. An ordinary Haxe Array has shared identity: after
`alias = values`, a `push` through either name must be visible through both.
`values.copy()`, by contrast, creates a different Array whose later `push`,
indexed assignment, or resize does not change `values`. The copy is *shallow*:
it copies the element values but does not recursively clone an object, nested
Array, or other reference stored in a slot.

For example:

```haxe
final original = [player];
final copied = original.copy();

copied.push(enemy);       // changes only `copied`
copied[0].health -= 10;   // both Arrays still refer to the same `player`
```

This distinction determines the C ownership plan. Assignment keeps one shared
Array container alive. `Array.copy()` must instead allocate a distinct
container and backing buffer, then shallow-copy each slot using that element
type's checked copy or retain rule. Resizing or destroying either container
must not invalidate the other. If a slot copy fails, the compiler/runtime must
destroy the successfully copied prefix and leave the source unchanged. The
compiler and runtime implement that contract for the exact element families
listed below. The focused ordinary-Haxe, direct-runtime, native-compiler, and
sanitizer evidence is described under
[Executable evidence](#executable-evidence). Other element families remain
unsupported rather than receiving a weaker copy.

The compiler therefore represents an admitted ordinary alias as an
`hxc_array_ref *`, not a by-value copy of `hxc_array`. `hxc_array_ref` owns the
one move-only buffer plus a local reference count. Assigning an admitted local
alias retains that container; leaving its scope releases it; the final release
destroys the elements and frees both storage layers. The current evidence
covers acyclic graphs, where this small ownership mechanism is sufficient.

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

That separation is intentional in both compiler profiles. Choosing `metal`
does not silently change the meaning of ordinary Haxe `Array<T>` assignment or
`Array.copy()`: the same source operation keeps its Haxe behavior. A developer
who wants fixed storage or a borrowed pointer-and-length view chooses the
visible typed `c.CArray<T>`, `c.Span<T>`, or `c.ConstSpan<T>` API instead.
This keeps low-level lifetime decisions explicit in the source while allowing
ordinary Haxe code to remain portable. The currently admitted forms and their
fail-closed limits are listed in
[the typed C authoring contract](typed-c-authoring.md#choosing-haxe-or-c-shaped-semantics).

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

An exact managed String is admitted as an Array element when the reachable
program has selected managed String representation. Its specialized element
callbacks retain, assign, and release the immutable shared owner just like a
normal Haxe String local.

Array construction and `push` use those callbacks for both ordinary source
syntax and comprehensions. Haxe represents a comprehension as a loop that
pushes each produced value, so it does not need a compiler-only collection
operation. A borrowed String can be passed directly because the callback
retains a new owner for the slot before the borrow ends. A fresh String, such
as the result of `String.fromCharCode`, first enters a hidden caller-owned
temporary. The callback copies and retains it into the slot; the caller then
releases its temporary. This short-lived owner is necessary because allocation
or element copying can fail: without it, either the new String could leak or
its bytes could be released before the Array owns them.

The narrower `Array<String>.join` method reads those owned elements, appends
every element and separator to one checked UTF-8 builder, then moves that
allocation into one fresh managed String owner. This makes runtime work linear
in the output bytes and preserves embedded NUL bytes without repeated
whole-result copying. Other unsupported managed values remain rejected. A
class element is admitted only through the exact traced representation: direct
nonescaping classes remain stack-shaped C values, while every class reachable
from the admitted `Array<Class>` graph receives stable collector storage and a
descriptor. Each additional managed family still needs a complete lifetime
rule before it can broaden the support claim.

## In-place sorting and typed comparators

`Array.sort(compare)` changes the existing Array rather than creating another
one. Haxe defines the comparator by the sign of its result: a negative result
places the first value before the second, zero treats them as equal for this
comparison, and a positive result places the first value after the second.
Like Haxe's `Array.sort` contract, this implementation is deliberately
unstable: values that compare equal are not promised to retain their earlier
relative order. Code that needs that stronger promise should use a stable-sort
API when haxe.c admits one.

The current compiler slice accepts an exact, non-capturing comparator such as
`(left:Int, right:Int) -> left - right`. “Non-capturing” means the function does
not read a local variable or `this` from the surrounding function. The compiler
lowers an inline function literal of that shape to a private, typed HxcIR
function. The Array lowering then emits a small typed adapter: the runtime hands
the adapter two addresses, and the adapter reads them as the exact element type
before calling the Haxe comparator. This keeps the erased byte-storage detail
inside the runtime boundary; ordinary application code and HxcIR retain
`(T, T) -> Int`.

The adapter receives its comparator through an explicit context pointer. That
context is the address of a local variable whose type is the exact C function
pointer. This avoids process-global comparator state, preserves nested or
concurrent sorts, and does not perform the undefined conversion between a C
function pointer and an object pointer. The receiver and comparator expressions
are each evaluated once, and every alias still observes the same reordered
Array.

`hxrt` uses an allocation-free heap sort. It swaps the bytes of already-live,
relocatable slots and does not call element copy, assignment, destruction,
retain, or release callbacks while ordering them. That distinction matters for
managed elements such as Strings and class references: sorting moves their
existing owners between slots without creating or dropping ownership. Native
tests check that allocation and lifetime counters remain unchanged.

The implementation does not use the C library's `qsort`. Portable C `qsort`
cannot carry a caller-owned context into its comparator, so using it would
require global mutable state or a non-portable callback cast. The local heap
sort is slightly more code, but it keeps the Haxe semantics deterministic and
reentrant across strict C11 platforms.

Capturing comparators and element types without an admitted Array storage plan
fail with source-positioned `HXC1001` before C is written. They are not silently
copied, boxed, or routed through raw C. General closure environments remain
owned by `haxe_c-ckk.3` under E3.T08; function-valued Array element storage is
tracked separately by `haxe_c-7d0.7`.

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

An ordinary Haxe `for (item in array)` is represented by the pinned Haxe
compiler as a checked indexed read inside a `while` body. That body is now an
explicit ownership boundary for managed record and managed enum elements.
Each iteration owns its copied element, nested loops and calls may borrow from
that owner, and the compiler releases it exactly once before a back edge,
`continue`, `break`, or early function return. The release keeps the source span
of the expression that created the owner; the loop decides when cleanup runs,
but it is not a new reason for selecting a runtime operation.

An enum-pattern binding such as `case Schedule(arguments)` is different: the
binding borrows the Array from the still-live enum owner for that switch arm.
The compiler therefore does not retain it merely because Haxe gives the payload
a local name. This keeps the generated C cleanup in the correct lexical scope.
An owned managed-element copy created only inside an `if` or switch arm remains
fail-closed. Those sibling paths do not yet have independent typed cleanup
scopes, so adding the arm-local owner to the function-wide cleanup list could
release an uninitialized value on the other path.

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
- distinct primitive-copy storage, independent mutation, and the in-place copy
  used when the collector owns the destination Array container;
- exact-slot aliasing across both relocation and suffix shifts;
- reference-element shallow-copy retain counts, retain-before-release
  assignment, and balanced destruction;
- rollback after copy, insertion, and partial resize lifecycle failures,
  including an injected failure after the first copied reference;
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
without leaving output. The fixture includes empty and populated copies of
`Array<Int>`, managed `Array<String>`, nested Arrays, and an unboxed
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
`ManagedNode` instances in `Array<ManagedNode>`. It grows and shallow-copies
the Array, proves that the two outer Arrays mutate independently while both
retain the same class instances, reads and replaces elements, retains a null
slot, mutates through an alias, and connects two nodes into a cycle. It crosses
the deterministic collector-pressure threshold and then
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
