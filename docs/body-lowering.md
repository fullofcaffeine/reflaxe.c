# Primitive function-body lowering

E2.T02 is the first compile-backed path from real pinned-Haxe `TypedExpr`
bodies through validated HxcIR to structural strict C11. It deliberately admits
only the semantic slice needed to establish the pipeline; it does not yet emit
a production C project or claim that `reflaxe.c` can build an executable.

## Admitted subset

`CBodyLowering` accepts parameter-free typed functions containing:

- `Void`, `Bool`, `Int`, `UInt`, and `Float` return/local/value types;
- compiler-typed `TInt`, `TFloat`, and `TBool` constants (unary operators are
  still outside this slice);
- initialized automatic locals and local reads;
- nested primitive blocks and parentheses; and
- explicit value/void returns or an implicit final void return.

The lowering preserves each typed instruction in Haxe evaluation order. Nested
blocks can currently share one HxcIR block because this subset has no branch,
lifetime, cleanup, or failure edge; that flattening must stop when any such
semantic edge is admitted. Locals remain addressable HxcIR places, while
constants and loads produce immutable values.

Return validity is checked twice. Frontend lowering maps the typed function and
return expression independently, then `HxcIRValidator` rejects missing values,
values on `Void` returns, and value/type mismatches as internal invariants. The
C body emitter consumes only validated IR.

Arguments, calls, operators, assignment, control flow, objects, strings,
closures, allocation, exceptions, and cleanup are outside this slice. The first
unsupported typed node fails with `HXC1001` at its exact Haxe range; lowering
never substitutes `Dynamic`, `Any`, reflection, raw C, or an invented value.

## Names, source mapping, and C shape

Every function and local is registered in the per-compilation
`CSymbolRegistry` before names are finalized. A local request includes its
lexical source ordinal and function namespace, so two shadowed Haxe variables
with the same spelling receive distinct deterministic C identifiers. Input
order reversal and repeated renders must produce identical HxcIR, C, and symbol
records.

Haxe compiler positions become normalized, repository-relative, one-based
HxcIR spans. The C body emitter has two structural modes: ordinary output and
output with typed `#line` statement/declaration nodes. It never interpolates a
raw directive. Both forms are strict C11 and use only the required standard
headers (`<stdbool.h>` and/or `<stdint.h>` for the admitted types).

Unused local reads become explicit `(void)local` statements so source order is
retained without native warnings. The emitted body slice selects no runtime
feature, header, source, define, library, or symbol. Portable and metal use the
same representation for these already-ratified primitive semantics.

## Production boundary

`CCompiler` now locates the real typed static `main` body, including the Haxe
case where the module array omits the main class but retains the typed
entry-point call. That call is used only to locate the static field; call and
entry-point emission remain E2.T03 work.

Production behavior is fail-closed:

- an unsupported body node emits exact source-positioned `HXC1001` and no
  artifact;
- a fully supported body passes HxcIR validation and structural C lowering,
  then emits `HXC1000` and no artifact because static-function, call, and
  executable entry-point/project emission are not implemented by E2.T02.

Thus the reviewed C fixture is generated from real typed Haxe by the body
pipeline, but its test-only translation-unit envelope and native `main` harness
are not production output.

## Evidence and compatibility

Run:

```sh
npm run test:body-lowering
npm run snapshots:check
```

The focused suite renders twice, reverses function discovery order, compares
portable with metal, checks exact HxcIR/C/symbol snapshots, proves shadow-safe
names and exact diagnostic ranges, and compiles/runs mapped and unmapped C with
available GCC and Clang at `-O0` and `-O2` under
`-std=c11 -Wall -Wextra -Werror -Wconversion -Wsign-conversion` and
`-pedantic-errors`. Required CI lanes pin each compiler family and verify its
command identity before compiling the checked-in generated snapshots.

This slice has no public C ABI, environment-specific behavior, standard-library
support, allocation, or `hxrt` dependency. Standard-library parity remains the
E5 feature graph: direct idiomatic C first, program-local specialization next,
and only the narrowest justified optimized runtime slice last.
