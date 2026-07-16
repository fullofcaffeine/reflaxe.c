# Explicit Haxe evaluation order

E2.T04 makes Haxe sequencing a semantic compiler decision rather than an
accident of C expression evaluation. The admitted primitive frontend records
order in validated HxcIR, and `CBodyEmitter` consumes that order as structural
strict-C11 statements and control flow. It never relies on C operand or
argument evaluation order.

## Sequencing contract

The current source-backed slice covers:

- direct-call arguments evaluated from left to right before the consuming
  call;
- initialized locals, local assignments, and primitive static-field
  load/store;
- `&&` and `||` as branch edges that evaluate the right side only when Haxe
  requires it;
- value-form `if`/ternary expressions as true/false blocks that store into a
  typed flow local and rejoin explicitly; and
- prefix/postfix primitive increment/decrement as load, constant, typed UB-safe
  operation, store, then selection of the new or old value; and
- compound arithmetic assignment as one stable destination load, left-to-right
  operand evaluation, one typed operation, and one store.

Static-field declarations and initializers come from the `TypedProgramInput`
captured inside `filterTypes`. Lowering never asks a mutable `ClassField` for a
later expression after Reflaxe preprocessing. Referenced primitive fields
become typed HxcIR globals with registry-finalized internal C names and direct
constant initialization.

HxcIR instruction arrays remain semantic order. Lazy and conditional
expressions create explicit blocks and terminators; their C form uses typed
labels, `if`, and `goto` nodes. Loads and stores retain structural local/global
places. No source-derived C fragment, C ternary, raw injection, reflection,
`Dynamic`, `Any`, or untyped escape is used.

## Stable-value and temporary proof

There is no general temporary-removal optimization pass in this slice. The C
emitter may keep only these values inline:

- constants and function parameters; and
- proven-total pure static conversions or operators whose inputs are already
  stable values.

Every load is copied immediately into a typed, registry-named C temporary.
Every consumed non-`Void` call result is likewise materialized before a later
instruction can mutate state. This prevents a local/global read from being
silently delayed and prevents two Haxe calls from becoming operands or
arguments whose order C leaves unspecified. The evaluation-order runner
compares every load and consumed call result in HxcIR with the recorded
temporary-value set, while also proving that the admitted pure values remain
eligible for inlining.

Any future pass that removes another temporary must carry a semantic proof that
the value is pure, total, non-volatile, non-failing, lifetime-safe, and
independent of intervening writes. It also needs a focused negative/adversarial
test and differential execution at both `-O0` and `-O2`; source-shape
convenience alone is not authority to weaken sequencing.

## Boundaries

This is expression-valued control flow, not the general statement-control-flow
implementation. Statement `if`, loops, switches, breaks, and continues remain
fail-closed for E2.T06. E2.T05 now routes signed updates through wrapping
program-local helpers and keeps safe `UInt` updates as direct unsigned C. A
right operand that creates control flow forces the already evaluated left value
into a typed flow local before that control flow begins.

Array representation and source `TArray` lowering remain E2.T08. The indexing
acceptance evidence is therefore the existing representation-neutral HxcIR
fixture for `arr[nextIndex()] += produce()`: it fixes the order as index call,
stable element address, load, right-side call, and store. That fixture proves
the semantic IR contract, not generated array support.

The entire admitted slice is compiler-lowered, identical in portable and metal,
and runtime-free under `auto`, `minimal`, and `none`. It selects no `hxrt`
feature or artifact. Future standard-library work follows the same hard order:
direct idiomatic C, then a program-local specialization, then only the
narrowest dependency-closed optimized `hxrt` feature when compiler lowering is
not feasible.

## Evidence

Run:

```sh
npm run test:evaluation-order
npm run test:arithmetic-semantics
npm run snapshots:check
```

The suite renders repeated, reversed-input, portable, and metal reports;
checks exact HxcIR/header/C/symbol snapshots; checks the indexed compound
assignment IR; runs an Eval oracle; builds real production projects including
`hxc_runtime=none`; and compiles/runs checked-in and production-generated C
with required GCC and Clang lanes at `-O0` and `-O2` under warning-clean strict
C11. The arithmetic suite extends that proof to signed updates, compound
assignment, boundary arithmetic, and eligible UBSan execution.
