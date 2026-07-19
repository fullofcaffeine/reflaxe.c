# Fixed arrays, spans, and bounded span parameters

Use this feature when a program knows how many values it needs and wants compact
C storage without a general-purpose Haxe `Array` object.

A **fixed array** owns a known number of adjacent values. For example, four
integers become an ordinary C array with four elements. A **span** is a small,
temporary view of existing storage: it carries where the elements begin and how
many there are. The span does not own, copy, or free those elements. We call it
**borrowed** because it may be used only while the original array is still alive
and it cannot be saved somewhere that outlives that array.

**Lowering** is the compiler step that turns a typed Haxe operation into a more
concrete representation. Here, a nonempty literal or
`c.CArray.zero<T, N>(length)` lowers to an automatic C array (normally local
stack storage). `span()` and `constSpan()` lower to an element pointer plus an
element count. A Haxe `for` loop becomes a direct indexed C loop. An admitted
private helper function receives the same pointer and count. There is no
allocated array object, span wrapper, or iterator object in the generated C;
this is what this document means by **runtime-free**.

The compiler first records and validates these choices in HxcIR, its typed
semantic intermediate representation. That extra representation lets the
compiler prove evaluation order, bounds policy, and borrow lifetime before it
chooses C syntax. See [HxcIR](hxc-ir.md) for the broader design.

This remains a bounded compiler feature, not broad collection or standard-
library support. The compiler rejects general `Array<T>` values, empty fixed
arrays, unsupported element storage, unknown or excessive zero-initialized
lengths, escaping views, unsupported call kinds, and every source form whose
representation or lifetime it cannot prove.

## Typed source contract

Both of these local-storage forms are admitted:

```haxe
var small:c.CArray<Int, Length4> = [3, 5, 7, 11];

static inline final WIDTH = 32;
static inline final HEIGHT = 16;
static inline final DEPTH = 32;
var voxels:c.CArray<c.UInt8, GridVolume> =
  c.CArray.zero(WIDTH * HEIGHT * DEPTH);
```

`N` is a phantom Haxe type identity. HxcIR records that identity together with
the concrete element count. It does not use reflection, `Dynamic`, `Any`, or an
untyped value to recover the size. The explicit `length` argument makes the
storage requirement inspectable at the call site; it does not create a dynamic
array or become a runtime length.

The zero-initialized length must be a positive compiler-known integer
expression. The current constant evaluator accepts integer literals,
parentheses, metadata/casts around them, multiplication, and reached `static
inline` integer fields. Multiplication is folded with checked arithmetic. Zero,
negative operands, an overflowing product, a nonconstant expression, or a
length whose result exceeds the storage policy emits source-positioned
`HXC1001` and no project.

The initial automatic-storage policy admits at most 65,536 bytes per
zero-initialized fixed array. Element storage must have an exact compiler-owned
size: 8/16/32/64-bit integers and binary64 floats are currently accepted. For
example, `CArray<UInt8, GridVolume>` with `32 * 16 * 32` elements occupies
16,384 bytes. `Bool` is deliberately rejected here because the target contract
does not yet assign it an exact storage size. This is a per-array ceiling, not
whole-frame stack accounting; whole-function storage budgeting remains future
analysis.

The `Array<T>` conversion on `CArray` exists only so ordinary Haxe typing can
validate a literal. `CBodyLowering` accepts it only when the original typed
expression is the literal itself. A function result, ordinary array local, or
other general `Array<T>` value fails closed.

Borrowing remains compiler-proven and nonescaping. A view may be consumed in
the local function:

```haxe
var mutable:c.Span<c.UInt8> = voxels.span();
var readOnly:c.ConstSpan<c.UInt8> = voxels.constSpan();

mutable[index] = replacement;
for (value in readOnly) {
  observed = value;
}
```

It may also cross a bounded internal function boundary:

```haxe
static function readAt(values:c.ConstSpan<c.UInt8>, index:Int):c.UInt8 {
  return values[index];
}

static function replaceAt(
  values:c.Span<c.UInt8>,
  index:Int,
  replacement:c.UInt8
):c.UInt8 {
  values[index] = replacement;
  return values[index];
}
```

The current parameter admission is deliberately exact:

- the callee is a reached direct static, non-generic function;
- the argument is an admitted `Span<T>` or `ConstSpan<T>` value borrowed from
  fixed `CArray` storage, or the same typed parameter forwarded by another
  admitted direct static call;
- element type and mutability remain exact through the call;
- the callee may index the borrow but cannot store or return it; and
- the call is compiler-private implementation, not an exported/native/callback
  ABI.

Instance, virtual, interface, closure, constructor, native/imported, exported,
recursive, and generic-specialized span calls remain rejected. A parameter span
also has a runtime-forwarded length rather than a compile-time fixed-array
length, so the current exact `for (value in span)` proof does not admit it;
ordinary indexed access retains checked bounds against the forwarded count.

A function cannot return either borrowed span type. The signature is rejected
as `TFunction(return-type:borrowed-span-escape)` at its exact source range.
Span fields, global storage, enum/record payload storage, address taking, and
other escape paths also remain unsupported. A recursive direct call carrying a
span fails as
`TCall(recursive-borrowed-span-target-not-admitted:<function-id>)` rather than
silently extending the borrow lifetime.

Borrow and iteration recognition uses the typed declaring abstract, so a
lookalike method named `span`, `constSpan`, `zero`, or `iterator` cannot
impersonate a compiler intrinsic.

## HxcIR representation and storage policy

HxcIR distinguishes:

- `IRTFixedArray(element, length, witnessId)` for owned inline storage;
- `IRTSpan(element, mutable)` for a borrowed view;
- `IRIOInitializeFixedArray` for element-by-element literal initialization;
- `IRIOZeroInitializeFixedArray` for validated fixed-storage zeroing;
- `IRIOInitializeSpan` for a borrow; and
- `IRIOBoundsCheck` with one explicit proof or failure policy.

A source span parameter remains one semantic HxcIR parameter of
`IRTSpan(element, mutable)`. It does not become two HxcIR parameters merely
because C later represents the value as a pointer and a length. Function entry
uses the ordinary typed `IRIOInitialize` operation to create one synthetic
callee-local borrow from that parameter. Existing structural index places and
bounds instructions then operate on that local exactly as they do for a local
fixed-storage borrow.

A direct HxcIR call likewise carries one span argument in source evaluation
order. Only C emission expands the validated argument into pointer and length
expressions. A forwarded call therefore preserves the original count without
reconstructing it, accepting an application-authored byte count, or allocating
a wrapper.

This distinction is intentional:

- HxcIR records that pointer and count are one nonescaping semantic borrow;
- the C emitter owns the internal pair representation;
- mutability and element compatibility remain type-checkable before C exists;
- bounds policy consumes the forwarded element count rather than a guessed
  array extent; and
- another hypothetical emitter could choose a native slice representation
  without changing the Haxe/HxcIR call contract.

No new HxcIR schema form is needed for this extension. Schema 9 already has the
required span type, parameter, ordered call, typed initialization, place, and
bounds operations. Adding a separate “span parameter” type would duplicate a
source/call-site distinction that is not a distinct semantic value.

The zero-initialization instruction is an effect on one fixed-array place with
an explicit uninitialized-to-initialized transition. The shared typed storage
policy is checked by both HxcIR validation and C emission. It rejects invalid
lengths, unsupported element sizes, byte-count overflow, and storage beyond the
65,536-byte ceiling before a C declarator is emitted.

The bounds policy is semantic evidence, not an emitter guess:

| Index fact | HxcIR policy | Generated behavior |
| --- | --- | --- |
| Literal integer index | Compile-time range validation, then `IRBPStaticProof` | An out-of-range constant is `HXC1001`; an in-range constant emits direct access |
| Compiler-created loop index dominated by `index < length` | `IRBPLoopGuarded(guard, indexLocal, length)` | Direct indexed access after validator confirmation |
| Any other admitted `Int` index | `IRBPCheckedAbort(profile, buildMode)` | Reject a negative value, cast to `size_t`, compare with the element count, then call C `abort()` on failure |

The current frontend does not perform interprocedural constant-length
specialization for span parameters. Even when a caller's backing `CArray` has a
known extent, the callee validates an indexed access against its forwarded
`size_t` count. This keeps separately compiled function bodies sound and avoids
silently treating one call site's length as a function-wide fact.

The same safe behavior applies to `portable` and `metal`, and to `debug`,
`release`, and `minsizerel`. Build mode is optimization intent only; it never
removes a required check or changes failure semantics. `hxc_build` defaults to
`debug`, and any noncanonical value fails closed with profile-aware `HXC0003`
before output.

Dynamic negative and upper-bound failures are executable evidence in every
profile/build combination for both local views and forwarded span parameters.
The current failure hook is the exact standard C `abort` function, registered
through `CSymbolRegistry`; it is not raw C and is not an `hxrt` feature.

## Generated C shape and safety proof

Literal storage is emitted as a structural declarator and initializer such as
`int32_t values[4] = { 3, 5, 7, 11 }`. Zero-initialized storage uses the C11
aggregate rule directly, for example `uint8_t voxels[16384] = { 0 }`. It does
not allocate and does not call `memset` or a runtime helper.

`Span<T>` is a `T *` plus `size_t` length, while `ConstSpan<T>` is a `const T *`
plus `size_t` length. The length is derived from owned storage as
`sizeof(array) / sizeof(array[0])`; it is never an application-authored byte
count. All access uses element-scaled C subscripting,
`base[(size_t)index]`.

The fixture's three-dimensional mapping is ordinary Haxe:

```haxe
return x + WIDTH * (y + HEIGHT * z);
```

It lowers through the existing UB-safe primitive operations, and the resulting
linear `Int` still passes through the fixed-array bounds policy before access.
For the 32 × 16 × 32 fixture, `(31, 15, 31)` maps to the last valid element,
16,383. A compile-time access at 16,384 is rejected before C emission. Dynamic
indices test `index < 0` before conversion and compare the nonnegative result
with the `size_t` element count, avoiding signed-to-unsigned wrap and one-past
dereference.

The pinned Haxe compiler presents `for (value in span)` through its typed
iterator protocol. The frontend recognizes only that exact shape and replaces
it with HxcIR condition, body, increment, and exit blocks. Exact-width element
types such as `UInt8` remain exact through iteration. The validated region
planner turns those blocks into one structural C loop whose ordered condition,
body, and increment statements remain explicit. It contains no blanket block
labels/gotos and no iterator allocation or `iterator`, `hasNext`, or `next`
call.

## Runtime, ABI, and compatibility boundary

This lowering follows the compiler-first rule: direct idiomatic C is the
representation, and request-local primitive helpers are selected only for the
ordinary Haxe arithmetic that needs them. Fixed arrays, zero initialization,
spans, bounds checks, and iteration select no runtime feature. Under
`hxc_runtime=none`, the project contains no `hxrt` include, source, define,
library, artifact, or symbol, and `hxc.runtime-plan.json` records an analyzed
empty plan.

The views are local compiler representations, not stabilized public C ABI
types. Exported pointer/length ownership, nullability, lifetime, and ABI shape
remain owned by E7. General arrays, dynamic or heap storage, global/static large
buffers, slicing, view escape, user-authored pointer arithmetic, empty-array
representation, aggregate element zeroing, whole-frame stack analysis, and
arbitrary constant-expression evaluation remain fail-closed.

## Evidence

The command name `test:span-lowering` refers to this complete proof suite, not
to one compiler pass whose own computation takes several minutes. The suite
tests successful programs, expected failures, repeatability, build profiles,
bounds failures, native optimization, and sanitizers. Its total time therefore
describes exhaustive test orchestration; it is not the expected compile time
for one application that uses a span.

Run:

```sh
npm run test:span-lowering
npm run snapshots:catalog
```

The first command runs this feature's complete semantic and native proof. The
second quickly checks that its snapshots are registered and have exactly one
focused owner. To independently re-render every snapshot suite in the
repository, use the slower cold-audit command:

```sh
npm run snapshots:check
```

The runner deliberately mixes two kinds of compiler request. A **cold** request
starts a new Haxe compiler process and proves that the result does not depend on
remembered process state. A **warm** request sends another compilation to one
already-running compiler server. Warm requests are faster and, more
importantly, prove that one build cannot leak symbols, configuration, or output
into the next build.

The exact full-suite topology is guarded in code:

- three cold typed-report requests: normal, exact repeat, and reversed function
  discovery;
- each typed-report request types the fixture once, then lowers that same typed
  input through all six `portable`/`metal` and
  `debug`/`release`/`minsizerel` combinations with a fresh compiler context for
  each combination;
- 36 negative-diagnostic requests, with one cold representative for body
  lowering and one for the C ABI boundary while the remaining 34 exercise the
  warm server;
- two invalid-configuration requests, seven production/repeat requests, and
  six bounds-matrix requests, each retaining a cold representative; and
- 54 Haxe requests in total: 8 cold and 46 warm. Including the one compiler
  server, the compiler executable is loaded 9 times instead of the previous 87.

The typed-report requests stay cold for a specific reason. The pinned Haxe
compiler can represent a cached one-expression body with a narrower source
position than its first typed form—for example, the `return` line instead of
the surrounding block. Both forms mean the same thing, but an exact HxcIR dump
must not silently erase or normalize a source-location difference. The three
cold renders therefore remain the byte-determinism authority while the normal
custom-target requests provide the ordered warm-server/reentrancy lane.
Beads issue `haxe_c-xge.27` owns making those source anchors stable enough to
add an exact warm HxcIR comparison later.

Each bounds-matrix compilation keeps all four failure cases reachable. A tiny
typed native selector chooses local upper, local negative, parameter upper, or
parameter negative access. The native harness links and runs all four choices,
records which branch was entered, requires that branch to abort, and checks the
linked image for zero `hxrt` symbols. This performs six Haxe compilations—one
per profile/build pair—instead of compiling four almost identical Haxe programs
for every pair.

The suite snapshots HxcIR/C/header/symbol output; executes
literal and 16,384-byte zero-initialized `UInt8` storage, repeated mutation,
mutable/const borrowing, exact-width iteration, nested parameter forwarding,
and ordinary-Haxe 3D indexing at GCC/Clang `-O0`, `-O2`, and combined
ASan/UBSan; and exercises local and parameter dynamic negative/upper fail-stop
paths in every profile/build coordinate. Source-positioned negatives cover
zero, negative, unknown, overflowing, and over-budget lengths, unsupported
element storage, static out-of-bounds access, borrowed returns, field/global
storage, instance/virtual dispatch, callbacks, recursion, exported/native ABI
boundaries, general/empty arrays, and lookalike intrinsics. Required native
lanes inspect generated artifacts and linked images for the absence of
allocation and `hxrt` dependencies.

For a phase-by-phase, path-free timing record, run:

```sh
python3 test/span_lowering/run.py \
  --timing-report /tmp/hxc-span-lowering-timing.json
```

The JSON conforms to
[`span-lowering-timing.schema.json`](specs/span-lowering-timing.schema.json).
It records suite phases, every request's cold/warm transport and exit code, the
number of real compiler loads, and wall time. It deliberately omits checkout
paths, temporary paths, timestamps, ports, and compiler output.
