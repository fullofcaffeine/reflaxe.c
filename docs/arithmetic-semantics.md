# UB-safe primitive arithmetic

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
projects; checks HxcIR, helper, header, source, symbol, and build-fact
snapshots; compares the defined common subset with the pinned Eval oracle; and
builds production `auto`, `minimal`, and `none` configurations. Required CI
lanes compile and execute both GCC and Clang output at `-O0` and `-O2`, then run
eligible output under undefined-behavior and floating-divide-by-zero sanitizers.

This proves the currently admitted primitive operations and conversions. It
does not claim general statement control flow, arrays, objects, strings,
exceptions, the broader Haxe standard library, or any generated-Haxe `hxrt`
feature.
