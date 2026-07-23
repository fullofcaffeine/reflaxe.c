# Primitive semantic contract

Primitive representation is a typed compiler decision, not a C-printer
shortcut. [ADR 0008](adr/0008-primitive-representations-and-conversions.md) is
the normative rationale. The exact reviewable table is
[`specs/primitive-semantics.json`](specs/primitive-semantics.json), with its
shape constrained by
[`specs/primitive-semantics.schema.json`](specs/primitive-semantics.schema.json).

## Compiler model

`CPrimitiveTypeMapper` classifies real `haxe.macro.Type` values. It preserves
`Null<T>`, exact/native integer identity, references, and native pointers; an
unsupported source type is rejected instead of being hidden behind an opaque
or dynamically typed placeholder. `CPrimitiveSemantics` then produces a closed
typed mapping, conversion, or operation decision for the shared portable/metal
pipeline.

The current HxcIR model keeps five primitive facts structural:

- ABI-sized integer identity (`size`, `ptrdiff`, `intptr`, or `uintptr`);
- tagged-scalar versus pointer nullability;
- exact, wrapping, checked, saturating, binary32-rounding, and
  binary64-widening numeric conversion intent;
- typed unary/binary operation identity plus direct or program-local
  implementation intent;
- an optional failure edge on conversion, required for checked numeric and
  nullable-unwrap operations.

Every primitive decision selects direct C or a named program-local helper.
Primitive mapping, conversion, and arithmetic add no runtime feature.

## Observable rules

The contract fixes ordinary Haxe representations to `bool`, `int32_t`,
`uint32_t`, binary64 `double`, and `void`. Exact `c.Int*`/`c.UInt*` surfaces use
the corresponding `<stdint.h>` typedef. ABI integers retain `size_t`,
`ptrdiff_t`, `intptr_t`, or `uintptr_t` identity and never use a guessed `long`.

`c.Float32` is a distinct exact C `float` carrier. It does not change Haxe
`Float`, which remains binary64 `double`, and it has no implicit conversion.
`c.Float32.fromFloat(value)` performs explicit narrowing; `value.toFloat()`
performs exact widening. Narrowing uses round-to-nearest, ties-to-even;
preserves infinity and zero signs; keeps NaN as NaN without a payload promise;
preserves representable subnormals; rounds smaller values to a subnormal or
same-signed zero; and maps finite overflow to same-signed infinity. Widening is
exact for finite binary32 values and preserves value class and zero sign.

Every reachable Float32 program emits target-compiled structural assertions for
32-bit binary32 `float` storage, precision, exponent range, radix, and
subnormals. The qualified native probe validates default-rounding conversion
behavior because those facts cannot be inferred from the compiler host.
Foreign unsafe code must restore any changed floating rounding environment.
Neither conversion selects a helper or `hxrt`.

Conversions are admitted by operation, not by a generic target cast. Widening
requires a range subset. Unsigned narrowing uses modulo arithmetic. Signed
wrapping reconstructs a two's-complement value without depending on an
out-of-range unsigned-to-signed cast. Checked narrowing carries a failure edge.
For `Std.int`, finite in-range values truncate toward zero; NaN maps to zero and
infinities or finite overflow saturate to the applicable 32-bit endpoint.

The bounded executable body path exposes the direct subset through the typed
`c.IntConvert.exact` and `c.IntConvert.modulo` operations. Cross-carrier `Int ->
c.UInt8` uses `modulo` for literal and runtime values, while `c.UInt8 -> Int`
uses `exact`. The inferred result type supplies the target carrier. `exact`
requires source-range containment; `modulo` requires an unsigned target and
uses modulo 2^N. A signed-target conversion that names a reconstruction helper,
a checked/failing conversion, or any target-ABI-width conversion remains
fail-closed rather than becoming an unchecked C cast.

`Int` addition, subtraction, multiplication, and negation use explicitly
unsigned 64-bit intermediates, retain the low 32 bits, and then reconstruct the
signed result. This remains correct even when C would promote `uint32_t` to a
wider signed `int`. Shifts mask their count with `31`; signed right shift uses
explicit sign extension. Bit operations work on unsigned bits and reconstruct
the signed value without an implementation-defined cast. Integer modulo by
zero returns `0`, as does `INT32_MIN % -1`. Haxe division produces binary64
`Float`, so integer operands convert before the operation and zero yields IEEE
NaN or signed infinity through a guarded helper.

Safe `UInt` arithmetic and masked shifts remain direct C, with explicit
`uint64_t` intermediates and `uint32_t` narrowing to defeat integer-promotion
ambiguity. Floating addition, subtraction, multiplication, negation, and
comparisons remain direct binary64 operations; division is zero-safe and
modulo uses `fmod` with an exact compiler-selected `m` link fact. See [UB-safe primitive
arithmetic](arithmetic-semantics.md) for the production operation boundary.

Nullable scalars use a direct tagged optional:

```c
struct hxc_optional_int {
  bool has_value;
  int32_t value;
};
```

The generated names vary, but the rule does not. `has_value = false` means
Haxe `null`; `has_value = true` permits every payload value, including `false`,
integer zero, and `0.0`. This uses no heap allocation, boxing, NaN trick, or
magic integer. Nullable references and native pointers already have a unique
null pointer, so they do not acquire this second wrapper. A nullable unwrap
either has a control-flow presence proof or retains an explicit failure edge.
The function-lowering fixture carries nullable Bool, Int, UInt, and Float
through parameters, returns, locals, records, enum payloads, and Arrays, then
compares native execution with Eval.

## Evidence and current limit

Run the focused contract with:

```sh
npm run test:primitive-semantics
npm run test:arithmetic-semantics
npm run test:function-lowering
```

The primitive suite maps real typed Haxe declarations in both profiles, renders
the schema-3 machine contract twice, checks the centrally owned snapshot, and
compiles the independent native semantic probe as strict C11 with GCC and
Clang at `-O0` and `-O2`. That probe demonstrates accepted algorithms and
platform facts; it is not generated C.

The arithmetic suite supplies the generated-Haxe proof: typed operations,
`Std.int`, and direct `c.IntConvert` exact/modulo operations flow through HxcIR,
selected static-inline helpers, strict C11, portable/metal/runtime-none
production projects, an Eval oracle, boundary execution, UBSan where supported,
and optimized-shape inspection. Invalid or helper-requiring signed-target
conversions and other unsupported signatures or body nodes report exact
`HXC1001` without output.

`test/c_import` supplies complementary generated-Haxe evidence for
`c.Float32`: exact foreign constants, struct fields, parameters, and returns,
explicit narrowing/widening, generated ABI assertions, boundary execution, and
complete runtime absence under strict C11. That bounded hand-authored import
proof is not broad bindgen, public ABI, or platform-support evidence.
E2.T11 owns broader generated-program differential and sanitizer proof. See
[primitive function-body lowering](body-lowering.md), [static function
lowering](function-lowering.md), and [explicit evaluation
order](evaluation-order.md).
