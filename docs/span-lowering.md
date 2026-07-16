# Fixed arrays and span-based iteration

E2.T08 admits the first typed collection representation without admitting the
general Haxe `Array` runtime. A nonempty array literal assigned directly to
`c.CArray<T, N>` lowers through validated HxcIR to an automatic C array. Calling
`span()` or `constSpan()` creates a borrowed pointer-and-length view, and a Haxe
`for` over that view becomes compiler-owned indexed control flow. No array,
view, or iterator object survives into C.

This is a narrow compiler feature, not broad collection or standard-library
support. The compiler rejects a general `Array<T>` source, an empty fixed array,
unsupported element types, escaping views, and any source form whose storage or
lifetime it cannot prove.

## Typed source contract

The admitted source forms are:

```haxe
var values:c.CArray<Int, Length4> = [3, 5, 7, 11];
var mutable:c.Span<Int> = values.span();
var readOnly:c.ConstSpan<Int> = values.constSpan();

mutable[1] = 13;
for (value in readOnly) {
  total += value;
}
```

`N` is a phantom Haxe type identity. The compiler records both that identity and
the literal element count in `IRTFixedArray`; it never uses reflection,
`Dynamic`, `Any`, or an untyped value to recover the length. The current slice
requires a nonempty literal because strict C11 has no zero-length array type.
It also requires direct local storage so a span's base remains live throughout
the borrow.

The `Array<T>` conversion on the extern abstract exists only so normal Haxe
typing can validate the literal. `CBodyLowering` accepts that conversion only
when the original typed expression is the literal itself. A function result,
ordinary array local, or other general `Array<T>` value fails with
source-positioned `HXC1001` and produces no files.

Borrow and iteration recognition uses the typed declaring abstract as well as
the method name. A user or library function merely named `span`, `constSpan`, or
`iterator` cannot impersonate a compiler intrinsic; lookalike calls fail closed
instead of being rewritten by spelling.

## HxcIR representation and bounds policy

HxcIR distinguishes:

- `IRTFixedArray(element, length, witnessId)` for owned inline storage;
- `IRTSpan(element, mutable)` for a borrowed view;
- `IRIOInitializeFixedArray` and `IRIOInitializeSpan` for ordered
  initialization; and
- `IRIOBoundsCheck` with one explicit proof or failure policy.

The bounds policy is semantic evidence, not an emitter guess:

| Index fact | HxcIR policy | Generated behavior |
| --- | --- | --- |
| Literal index proven in range | `IRBPStaticProof` | Direct indexed access; no redundant check |
| Compiler-created loop index dominated by `index < length` | `IRBPLoopGuarded(guard, indexLocal, length)` | Direct indexed access inside the guarded loop after validator confirmation |
| Any other admitted `Int` index | `IRBPCheckedAbort(profile, buildMode)` | Reject negative values, cast to `size_t`, compare with length, then call C `abort()` on failure |

The same safe behavior applies to `portable` and `metal`, and to `debug`,
`release`, and `minsizerel`. Build mode is optimization intent only: it may
later influence the shape of a proven check, but it never removes a required
check or changes failure semantics. `hxc_build` defaults to `debug`; any value
other than those three canonical spellings fails closed with profile-aware
`HXC0003` before output.

Upper-bound and negative failures are executable evidence in every
profile/build combination. The current failure hook is the exact standard C
`abort` function, registered through `CSymbolRegistry`; it is not raw C and is
not an `hxrt` feature. A future configurable bounds hook must preserve the same
typed HxcIR policy and failure guarantee.

## Generated C shape and safety proof

Owned storage is a structural C declarator such as `int32_t values[4]`.
`Span<T>` is a `T *` plus `size_t` length, while `ConstSpan<T>` is a
`const T *` plus `size_t` length. The length is derived from the owned array as
`sizeof(array) / sizeof(array[0])`; it is not a byte count authored by the
application.

All access uses C subscripting, `base[(size_t)index]`, so pointer movement is
scaled by the element type. The checked path tests `index < 0` before the cast
and then compares the nonnegative value with the `size_t` element count. No byte
arithmetic, unchecked signed-to-unsigned wrap, one-past dereference, null-pointer
arithmetic, or unaligned load is admitted.

The pinned Haxe compiler presents `for (value in span)` through its typed
iterator protocol. The frontend recognizes only that exact compiler-typed span
shape and replaces it with HxcIR condition, body, increment, and exit blocks.
The generated structural C currently prints that graph with labels, `if`, and
`goto`; semantically it is an ordinary direct indexed loop. It contains no
iterator allocation, `iterator`, `hasNext`, or `next` call. The compiler-created
index is `size_t`, starts at zero, enters the body only while it is below the
fixed length, and increments only from a value at most `length - 1`. The index
therefore cannot be negative, and its `+ 1` is both element-count based and
overflow-safe.

## Runtime, ABI, and compatibility boundary

This lowering follows the project-wide compiler-first rule: direct idiomatic C
is the representation, and the existing wrapping-add operation used by the
fixture is a request-local `static inline` specialization. Fixed arrays, spans,
bounds checks, and iteration select no runtime feature. Under
`hxc_runtime=none`, the generated project contains no `hxrt` include, source,
define, library, artifact, or symbol, and `hxc.runtime-plan.json` records an
analyzed empty plan.

The current views are local compiler representations, not stabilized public C
ABI types. Exported pointer/length ownership, nullability, lifetime, and ABI
shape remain owned by E7. General arrays, dynamic-length containers, view
escape, slicing, user-authored pointer arithmetic, empty-array representation,
non-primitive elements, and full `length` expression lowering remain
fail-closed until their typed semantic and lifetime work is implemented.

## Evidence

Run:

```sh
npm run test:span-lowering
npm run snapshots:check
```

The suite renders twice and reverses function discovery for every combination
of two profiles and three build modes. It checks exact HxcIR/C/header/symbol
snapshots, executes in-range mutable and const views at `-O0` and `-O2`, runs
negative and upper-bound fail-stop programs, rejects general and empty arrays,
and validates invalid build configuration. Required CI lanes independently
compile the checked-in generated project with GCC and Clang under warning-clean
strict C11 and inspect the linked image for absence of `hxrt` symbols.
