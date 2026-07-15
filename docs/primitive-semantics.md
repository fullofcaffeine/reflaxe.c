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
typed mapping or conversion decision for the shared portable/metal pipeline.

The HxcIR schema-2 additions keep four facts structural:

- ABI-sized integer identity (`size`, `ptrdiff`, `intptr`, or `uintptr`);
- tagged-scalar versus pointer nullability;
- exact, wrapping, checked, and saturating numeric conversion intent;
- an optional failure edge on conversion, required for checked numeric and
  nullable-unwrap operations.

Every primitive decision selects direct C or a named program-local helper.
Primitive mapping and conversion add no runtime feature.

## Observable rules

The contract fixes ordinary Haxe representations to `bool`, `int32_t`,
`uint32_t`, binary64 `double`, and `void`. Exact `c.Int*`/`c.UInt*` surfaces use
the corresponding `<stdint.h>` typedef. ABI integers retain `size_t`,
`ptrdiff_t`, `intptr_t`, or `uintptr_t` identity and never use a guessed `long`.

Conversions are admitted by operation, not by a generic target cast. Widening
requires a range subset. Unsigned narrowing uses modulo arithmetic. Signed
wrapping reconstructs a two's-complement value without depending on an
out-of-range unsigned-to-signed cast. Checked narrowing carries a failure edge.
For `Std.int`, finite in-range values truncate toward zero; NaN maps to zero and
infinities or finite overflow saturate to the applicable 32-bit endpoint.

Nullable scalars use a tagged optional. Nullable references and native pointers
use a null pointer. A nullable unwrap either has a presence proof or retains an
explicit failure edge.

## Evidence and current limit

Run the focused contract with:

```sh
npm run test:primitive-semantics
```

The suite maps real typed Haxe declarations in both profiles, renders the
machine contract twice, checks the centrally owned snapshot, and compiles the
independent native semantic probe as strict C11 with GCC and Clang at `-O0` and
`-O2`. The native probe demonstrates the accepted algorithms and platform
facts; it is not generated C.

Production Haxe-to-HxcIR/C lowering remains deliberately unavailable at this
stage and still reports `HXC1000`. E2.T02 owns the first expression/body
lowering, E2.T05 owns the remaining arithmetic undefined-behavior rules, and
E2.T11 owns generated-program differential and sanitizer proof.
