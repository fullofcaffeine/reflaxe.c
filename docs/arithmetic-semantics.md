# UB-safe primitive arithmetic and typed integer conversions

Primitive arithmetic is a compiler semantic decision, not a request for a
general runtime. [ADR 0008](adr/0008-primitive-representations-and-conversions.md)
is normative, and the operation table in
[`specs/primitive-semantics.json`](specs/primitive-semantics.json) is the
machine-readable contract. This document explains the production E2.T05 slice.

## Operation contract

The frontend converts typed Haxe operators into explicit HxcIR operation IDs.
Each instruction says whether its implementation is direct C or a named
program-local specialization. No primitive operation may select `hxrt`.

| Haxe operation | Defined result | Generated strategy |
| --- | --- | --- |
| `Int` `+`, `-`, `*`, unary `-` | Wrap modulo 2^32 and interpret as signed two's-complement | Compute through `uint64_t`, retain the low 32 bits, and reconstruct `int32_t` in a static-inline helper |
| `Int` `/` | Binary64 division; zero produces NaN or signed infinity | Convert operands exactly to `double`; use a zero-safe helper |
| `Int` `%` | Signed remainder; zero and `INT32_MIN % -1` produce `0` | Guard exceptional divisors before C remainder |
| `Int` shifts | Count masked with `31`; signed right shift has explicit sign extension | Shift unsigned bits in a static-inline helper |
| `Int` bit operations | Operate on the 32-bit representation | Use unsigned bits and defined signed reconstruction |
| `UInt` arithmetic and shifts | ISO C modulo-2^32 behavior with count masked by `31` | Direct widened unsigned C with explicit `uint32_t` narrowing |
| `UInt` `%` | Unsigned remainder; zero produces `0` | Guard zero in a static-inline helper |
| `Float` arithmetic | Binary64 NaN, infinity, and signed-zero behavior | Direct `double`, except zero-safe division and `fmod` |
| `Std.int(Float)` | Truncate finite in-range input; NaN to `0`; overflow/infinity saturates | Compare first, then perform only an in-range C conversion |

The zero result for integer modulo by zero is an intentional deterministic
target refinement. Haxe leaves this exceptional edge target-specific; the C
target fixes it so generated programs never execute undefined native division.
`INT32_MIN / -1` is not signed integer division at all because Haxe `/` returns
`Float`; exact conversion happens before the binary64 operation.

Floating modulo calls `fmod` and records the exact `m` link fact in the compiler
manifest with the requesting module as provenance. A build-library fact is not
a runtime feature. Fast-math remains forbidden because it can erase the NaN,
infinity, and signed-zero behavior on which this contract depends.

## Bounded typed integer conversions

The `c.IntConvert` contract makes conversion intent explicit without using
Haxe's unchecked `cast` assertion:

```haxe
final channel:c.UInt8 = c.IntConvert.modulo(value);
final index:Int = c.IntConvert.exact(channel);
```

The result context infers the target carrier. `exact` requires the entire
source range to fit in that target. `modulo` requires an unsigned target and
retains the low N bits, including for negative inputs. Both operations are
admitted between Haxe `Int`/`UInt` and the exact-width
`c.Int8/16/32/64`/`c.UInt8/16/32/64` carriers only when the existing primitive
decision is non-failing direct C. The frontend records `numeric-exact` or
`numeric-wrapping` in HxcIR before the emitter constructs a typed structural C
cast. Neither operation selects a helper or `hxrt` feature.

The rule applies to runtime values as well as literals. For example,
`Int -> c.UInt8`, `c.Int64 -> c.UInt16`, and `c.UInt32 -> c.UInt8` retain the
low target-width bits, while `c.UInt8 -> Int`, `c.UInt8 -> c.Int16`, and
`c.UInt32 -> c.UInt64` are exact. The native matrix exercises negative and
oversized dynamic values rather than relying on constant-folded range proofs.

Conversions that require signed reconstruction are deliberately not admitted
through this API. `c.IntConvert.exact` also rejects any conversion that would
discard values, while `c.IntConvert.modulo` rejects a signed target. `Int ->
c.Int8` and `c.UInt32 -> Int`, for example, fail with source-positioned
`HXC1001` and leave no output because their primitive decisions name
program-local helpers. Target-ABI integers, checked conversions, and conversions
with failure edges remain outside this slice. The pre-existing Haxe `UInt ->
Int` reconstruction used by the pinned `UInt` abstract remains a separately
admitted compatibility path; it is not evidence that an exact-width
unsigned-to-signed conversion is direct.

## Compiler-owned helpers

`CPrimitiveHelperSelection` walks the validated whole-program HxcIR, computes a
deterministic dependency closure, and registers every helper, parameter, and C
standard-library identifier before symbol finalization. The helper emitter then
builds private structural CAST declarations; it never concatenates a user-
derived C fragment.

Helpers are emitted only when direct C would be undefined, implementation-
defined, or semantically wrong. They are `static inline`, request-local, and
runtime-free. Explicit `uint64_t` intermediates prevent C from promoting a
32-bit unsigned operand to a wider signed `int`; each semantic operation then
narrows to the required low 32 bits. At `-O2`, the arithmetic matrix verifies
that no specialization name remains as an out-of-line symbol. Safe unsigned
addition and masked shifts are inspected separately to ensure they remain
direct, idiomatic C and that the widening casts optimize away.

The pinned Haxe `UInt` abstract expands some source operators through signed
bit casts and, for modulo, a `toFloat`/`Std.int` sequence. The compiler matches
only the exact typed expansion shipped by the pinned Haxe revision and lowers
the original unsigned intent. This is a typed standard-library intrinsic, not
reflection, `Dynamic`, `Any`, raw C, or a general AST-shape escape. A toolchain
pin change requires an intentional audit of that expansion.

## Sequencing and updates

Arithmetic lowering reuses the E2.T04 sequencing rules. Binary operands are
lowered left to right. If the right operand creates control flow, the left value
is stored in a typed flow local before that control flow begins. Compound
assignment evaluates and stabilizes the destination, loads it once, evaluates
the right side, performs the typed operation, and stores once.

Prefix and postfix increment/decrement are explicit load, one, operation,
store sequences. `Int` updates use the same wrapping helpers as ordinary
arithmetic; `UInt` updates use defined unsigned arithmetic. The emitted C never
depends on C operand evaluation order.

## Evidence and boundary

Run:

```sh
npm run test:primitive-semantics
npm run test:arithmetic-semantics
npm run snapshots:check
```

The arithmetic suite renders repeated, reversed-input, portable, and metal
projects; checks HxcIR, direct-conversion, helper, header, source, symbol, and
build-fact snapshots; compares the defined common subset with the pinned Eval
oracle; and builds production `auto`, `minimal`, and `none` configurations. It
also checks exact HXC1001 ranges and no-artifact behavior for invalid or
helper-requiring conversions. Required CI lanes compile and execute both GCC
and Clang output at `-O0` and `-O2`, then run eligible output under
undefined-behavior and
floating-divide-by-zero sanitizers.

This proves the currently admitted primitive operations and conversions. It
does not claim general statement control flow, arrays, objects, strings,
exceptions, the broader Haxe standard library, or any generated-Haxe `hxrt`
feature.

The [seeded primitive differential suite](primitive-differential.md) complements
this fixed boundary matrix with a reproducible 128-case generated corpus,
mismatch reduction, an explicit target-refinement ledger, and combined
AddressSanitizer/UndefinedBehaviorSanitizer runs. It does not replace the
reviewable edge and optimized-shape assertions above.
