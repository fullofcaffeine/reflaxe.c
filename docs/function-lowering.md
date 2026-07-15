# Static functions, direct calls, and executable entry

E2.T03 extends the primitive body pipeline into the first production-emitted
Haxe-to-C executable slice. The compiler collects the reachable typed static
function graph, lowers signatures and bodies through validated HxcIR, emits a
private prototype header plus deterministic structural C translation units,
and packages the result through Reflaxe output ownership. This is deliberately a narrow
primitive capability, not a claim of general Haxe or standard-library support.

## Typed function boundary

Every admitted function has a compiler-provided `TFun` contract. Parameters and
results must currently map to `Void`, `Bool`, `Int`, `UInt`, or `Float` through
`CPrimitiveSemantics`; parameters become immutable HxcIR values and structural C
parameters. The compiler never reconstructs a signature from text and never
uses `Reflect`, `Dynamic`, `Any`, an unchecked cast, or raw C as a missing-type
escape.

Only direct calls to typed static Haxe fields are admitted. The graph collector
walks real `TypedExpr` nodes with the typed compiler API, is cycle-safe, and
normalizes the reachable set before lowering. An unavailable body, indirect
call, unsupported type, or other unimplemented semantic form stops with exact
source-positioned `HXC1001`.

Default, optional, and rest parameters are not silently approximated. Until
their complete Haxe semantics are implemented, each has a distinct scoped
`HXC1001` detail naming the affected parameter, and the build owns no output.

## Calls, conversions, and C evaluation order

Call arguments lower in Haxe's left-to-right order. Each implicit primitive
conversion is first selected by the typed primitive contract and then recorded
as an `IRIOConvert` before the `IRIOCall`. The current admitted conversions use
direct structural C casts and select no runtime feature; a conversion requiring
a program-local helper remains fail-closed until that helper can be emitted.

C does not guarantee Haxe's argument evaluation order. Consequently,
value-producing calls are materialized as typed C temporaries when their result
is consumed. Later call arguments therefore cannot be fused into a C expression
that weakens the ordered HxcIR instruction stream. A discarded non-`Void` call
remains an explicit expression statement.

## Prototypes, recursion, and entry point

All admitted signatures and symbol requests are prepared before the registry is
finalized or any body is emitted. The private `include/hxc/program.h` header
contains every prototype before `src/program.c` or a partitioned source defines
any function. Recursive and mutually recursive call graphs therefore compile
without discovery-order dependencies.

Only a one-block admitted body is eligible for the unconditional-call proof.
Multi-block evaluation-order bodies are excluded conservatively. When the
remaining direct-call graph proves a closed recursive cycle, the compiler also
proves that its HxcIR return is unreachable, marks every cycle member with the
standard C11 `_Noreturn` function specifier, and omits the unreachable C
return. A direct self-tail call becomes a structural `while (1)` loop. Its arguments first enter
registry-named typed temporaries in HxcIR order and only then replace the C
parameters, so swaps and other parameter dependencies cannot weaken Haxe's
evaluation semantics.

Every remaining member of a closed multi-function cycle receives its own
deterministically ordered `src/nonreturn_NNNN.c` translation unit and continues
to call the next member directly through the shared prototype header. That
partition prevents a single optimizing C compilation from diagnosing the
whole closed cycle while preserving ordinary direct calls, strict C11, and
zero runtime support. The native gate keeps `-Winfinite-recursion` enabled at
both `-O0` and `-O2`.

The Haxe entry function for this slice must be `static function main():Void`.
It keeps its compiler-owned internal C name. The executable wrapper is the exact
hosted C signature:

```c
int main(void)
```

The wrapper calls the internal Haxe entry and returns zero. The exact `main`
identifier and the declaration planner's collision-free path-derived header
guard are registered before symbol finalization; no emitted identifier is
constructed directly from source text. Non-hosted entry contracts remain
environment-specific future work and fail with source-positioned `HXC1000`
without output instead of reusing the hosted signature.

## Compiler-first, zero-runtime project

The admitted slice performs representation, reachability, specialization,
conversion choice, declaration ordering, and dead-function exclusion in the
compiler. Its generated C is direct, idiomatic, warning-clean strict C11. The
resolved project contains no `hxrt` include, source, define, library, feature,
or symbol.

Every successful production build records that proof:

- `hxc.runtime-plan.json` has `analyzed-runtime-free`, the resolved
  `auto|minimal|none` policy and provenance, direct-lowering decisions, an empty
  feature/artifact set, and a positive no-runtime proof;
- `hxc.abi.json` has `analyzed-no-public-exports` and names only the executable
  `main` entry;
- `hxc.stdlib-report.json` has `analyzed-no-stdlib-use`.

This is the hard ordering for future standard-library work as well: direct
idiomatic C first, then a compiler-generated program-local specialization, and
only the narrowest dependency-closed optimized `hxrt` feature when compile-time
lowering is genuinely infeasible. An empty primitive program must remain wholly
runtime-free.

## Evidence and limits

Run:

```sh
npm run test:function-lowering
npm run snapshots:check
```

The suite renders twice, reverses discovery order, compares portable and metal,
checks exact HxcIR/header/C-source-set/symbol snapshots, proves explicit
argument conversion order, verifies direct and mutual recursion planning, and
exercises the scoped
default/optional/rest plus non-hosted-entry diagnostics. It also runs portable,
metal, and explicit `hxc_runtime=none` production builds, compares isolated
output roots byte for byte, validates the analyzed sidecars, and compiles/runs
both fixture and production C under strict GCC and Clang lanes at `-O0` and
`-O2`.

General arithmetic, statement branches, loops, objects, strings, arrays,
instance/virtual calls, closures, exceptions, allocation, public exports, user arguments, and actual
standard-library lowering remain outside this slice and fail closed. Native
build orchestration is still future `hxc`/adapter work; direct Haxe invocation
emits the owned C project but does not replace the C compiler invocation.
