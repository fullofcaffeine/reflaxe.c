# Fixed arrays and span-based iteration

The fixed-array slice admits compact local storage without admitting the general
Haxe `Array` runtime. A nonempty literal or a typed `c.CArray.zero<T, N>(length)`
construction lowers through validated HxcIR to an automatic C array. Calling
`span()` or `constSpan()` creates a borrowed pointer-and-length view, and a Haxe
`for` over that view becomes compiler-owned indexed control flow. No array,
view, or iterator object survives into C.

This remains a bounded compiler feature, not broad collection or standard-
library support. The compiler rejects general `Array<T>` values, empty fixed
arrays, unsupported element storage, unknown or excessive zero-initialized
lengths, escaping views, and every source form whose representation or lifetime
it cannot prove.

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

Borrowing remains local and lexical:

```haxe
var mutable:c.Span<c.UInt8> = voxels.span();
var readOnly:c.ConstSpan<c.UInt8> = voxels.constSpan();

mutable[index] = replacement;
for (value in readOnly) {
  observed = value;
}
```

A function cannot return either borrowed span type. The signature is rejected
as `TFunction(return-type:borrowed-span-escape)` at its exact source range.
Borrow and iteration recognition also uses the typed declaring abstract, so a
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

The same safe behavior applies to `portable` and `metal`, and to `debug`,
`release`, and `minsizerel`. Build mode is optimization intent only; it never
removes a required check or changes failure semantics. `hxc_build` defaults to
`debug`, and any noncanonical value fails closed with profile-aware `HXC0003`
before output.

Dynamic negative and upper-bound failures are executable evidence in every
profile/build combination. The current failure hook is the exact standard C
`abort` function, registered through `CSymbolRegistry`; it is not raw C and is
not an `hxrt` feature.

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
types such as `UInt8` remain exact through iteration. The generated structural
C currently prints labels, `if`, and `goto`, but semantically it is a guarded
indexed loop with no iterator allocation or `iterator`, `hasNext`, or `next`
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

Run:

```sh
npm run test:span-lowering
npm run snapshots:check
```

The suite renders twice and reverses function discovery across both profiles
and all three build modes. It snapshots HxcIR/C/header/symbol output; executes
literal and 16,384-byte zero-initialized `UInt8` storage, repeated mutation,
mutable/const borrowing, exact-width iteration, and ordinary-Haxe 3D indexing
at GCC/Clang `-O0` and `-O2`; and exercises dynamic negative/upper fail-stop
paths. Source-positioned negatives cover zero, negative, unknown, overflowing,
and over-budget lengths, unsupported element storage, static out-of-bounds
access, escaping borrows, general/empty arrays, and lookalike intrinsics.
Required native lanes inspect generated artifacts and linked images for the
absence of allocation and `hxrt` dependencies.
