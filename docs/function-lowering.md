# Static functions, direct calls, and executable entry

E2.T03 extends the primitive body pipeline into the first production-emitted
Haxe-to-C executable slice. The compiler collects the reachable typed static
function graph, lowers signatures and bodies through validated HxcIR, emits a
private prototype header plus deterministic structural C translation units,
and packages the result through Reflaxe output ownership. This is deliberately a narrow
primitive capability, not a claim of general Haxe or standard-library support.

E2.T09 adds every class/static initializer expression as another graph root.
Direct static functions used only during initialization are therefore prepared
and prototyped before initializer HxcIR is built. A separately typed plan—not
function discovery order—selects the bootstrap calls made before Haxe `main`;
see [deterministic static initialization](static-initialization.md).

## Typed function boundary

Every admitted function has a compiler-provided `TFun` contract. Parameters and
results must resolve to the already admitted direct primitive or bounded enum
representations. Non-generic primitive functions map `Void`, `Bool`, `Int`,
`UInt`, and `Float` through `CPrimitiveSemantics`; closed generic calls first
apply their normalized type arguments and then enter the same typed boundary.
Parameters become immutable HxcIR values and structural C parameters. The
compiler never reconstructs a signature from text and never uses `Reflect`,
`Dynamic`, `Any`, an unchecked cast, or raw C as a missing-type escape.

Only direct calls to typed static Haxe fields are admitted. The graph collector
walks real `TypedExpr` nodes with the typed compiler API, is cycle-safe, and
normalizes the reachable set before lowering. An unavailable body, indirect
call, unsupported type, or other unimplemented semantic form stops with exact
source-positioned `HXC1001`.

Default, optional, and rest parameters are not silently approximated. Until
their complete Haxe semantics are implemented, each has a distinct scoped
`HXC1001` detail naming the affected parameter, and the build owns no output.

## Module-level functions and fields

Haxe lets a module contain functions and values directly, without wrapping
them in a class. This is the clearest source shape when a file groups stateless
operations. A caller imports the field directly and may give it a descriptive
local alias, for example `import gameplay.Recovery.decide as decideRecovery;`
followed by `decideRecovery(...)`. Unlike a static class, a packed Haxe module
is not a runtime namespace object, so code should not pretend that
`Recovery.decide(...)` remains a class-style call.

The Haxe compiler represents these fields internally as a synthetic (hidden)
static class with `KModuleFields`. haxe.c keeps that full hidden path as the
semantic identity used for reachability, collision detection, reports, and
determinism. Generated C uses the source module as its separate readable name.
For example, an internal identity such as
`_Recovery.Recovery_Fields_.decide` emits as `hxc_Recovery_decide`, not as a
machine-shaped copy of the hidden class name. Static globals and their startup
functions follow the same rule. This separation is important: shortening the
display name must never make two distinct Haxe declarations become the same
compiler entity.

The focused `ModuleFunctions.hx` fixture covers a module constant, public and
private functions, an inline helper, startup initialization, the Haxe entry
function, split project ownership, Eval parity, and strict native C. Its typed
inventory, symbol table, project plan, header, module source, and entry source
are registered snapshots. A module function using a not-yet-supported optional
argument proves that unsupported module fields still stop at the source with
`HXC1001` and leave no output.

## Closed generic functions

E3.T03 specializes reachable direct static generic functions for closed
primitive and admitted enum arguments. The graph worklist keys each instance by
the base function plus a length-prefixed normalized argument sequence, merges
aliases and repeated call reasons, and registers the instance before scanning
its body so recursion terminates. Full keys remain authoritative when SHA-256
supplies compact instance and C-symbol suffixes.

Dynamic, open, reference, class, anonymous-record, function, nullable, and
native-pointer arguments remain exact source-positioned `HXC1001` boundaries.
The compiler also rejects a 65th generic function or enum instance and a project
whose conservative specialization estimate exceeds 524,288 C bytes. Successful
generic builds emit the schema-1 `hxc.specializations.json` sidecar and remain
runtime-free. See [deterministic generic
specialization](generic-specialization.md).

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
finalized or any body is emitted. In unity mode the private
`include/hxc/program.h` header contains every prototype before `src/program.c`
defines a function. In the default split mode the stable umbrella includes
deterministic module-private headers. Package mode instead groups those
declarations into one private header per normalized Haxe package. In both
multi-file modes, each generated header owns its group's globals and
prototypes, including initializer prototypes needed by the small entry unit.
Recursive and mutually recursive call graphs therefore compile without
discovery-order dependencies in all three layouts. Every file assignment
consumes the same finalized HxcIR, C names, representations, and function
bodies.

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
deterministically ordered safety translation unit and continues to call the
next member directly through the shared private headers. Unity names these
`src/nonreturn_NNNN.c`; split keeps the partition beside its source-shaped
module as `<Module>.nonreturn_NNNN.c`; package mode similarly uses
`<Package>/package.nonreturn_NNNN.c`. That narrow exception prevents one
optimizing C compilation from diagnosing the whole proven closed cycle while
preserving ordinary direct calls, strict C11, and zero runtime support. A
cycle-free unity program is exactly one implementation unit, as the layout
contract promises. Making mutually recursive tail cycles a true single-unit
structural dispatcher remains separate future tail-cycle lowering; the
ordinary intra-function region structuralizer does not rewrite calls across
functions. This layout change does not hide the problem with a pragma, opaque
call, printer rewrite, or disabled warning. The native gate keeps
`-Winfinite-recursion` enabled at both `-O0` and `-O2`.

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

When a closed generic instance is reachable, `hxc.runtime-plan.json` also names
the direct decision `closed-generic-specializations`, and
`hxc.specializations.json` records its full key, arguments, reasons, recursion,
and code-size attribution. That report is omitted for non-generic programs.

This is the hard ordering for future standard-library work as well: direct
idiomatic C first, then a compiler-generated program-local specialization, and
only the narrowest dependency-closed optimized `hxrt` feature when compile-time
lowering is genuinely infeasible. An empty primitive program must remain wholly
runtime-free.

## Evidence and limits

Run:

```sh
npm run test:function-lowering
npm run test:generic-specialization
npm run snapshots:check
```

The suite renders twice, reverses discovery order, compares portable and metal,
checks exact HxcIR/header/C-source-set/symbol snapshots, proves explicit
argument conversion order, verifies direct and mutual recursion planning, and
checks readable module-level fields under Eval, generated C, and a strict native
compiler. It also exercises the scoped
default/optional/rest plus non-hosted-entry diagnostics, then runs portable,
metal, and explicit `hxc_runtime=none` production builds, compares isolated
output roots byte for byte, validates the analyzed sidecars, and compiles/runs
both fixture and production C under strict GCC and Clang lanes at `-O0` and
`-O2`.

Objects, strings, general arrays, generic classes/references, descriptor-driven
generic bodies, instance/virtual calls, closures, exceptions, allocation,
public exports, user arguments, and general standard-library lowering remain
outside this slice and fail closed. Native
build orchestration is still future `hxc`/adapter work; direct Haxe invocation
emits the owned C project but does not replace the C compiler invocation.
