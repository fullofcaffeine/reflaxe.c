# Explicit Haxe evaluation order and control flow

E2.T04 makes Haxe sequencing a semantic compiler decision rather than an
accident of C expression evaluation. The admitted primitive frontend records
order in validated HxcIR, and `CBodyEmitter` consumes that order as structural
strict-C11 statements and control flow. It never relies on C operand or
argument evaluation order. E2.T06 extends the same edge model to statement
conditionals, loops, switches, `break`, and `continue`.

## Sequencing contract

The current source-backed slice covers:

- direct-call arguments evaluated from left to right before the consuming
  call;
- initialized locals, local assignments, and primitive static-field
  load/store;
- `&&` and `||` as branch edges that evaluate the right side only when Haxe
  requires it;
- value-form `if`/ternary expressions as true/false blocks that store into a
  typed flow local and rejoin explicitly;
- statement `if`/`else` as explicit branch targets, with a join only when at
  least one source arm continues;
- pre-test and post-test loops as condition/body/exit blocks, with nested
  `break` and `continue` targeting the innermost active loop;
- pinned-Reflaxe range `for` graphs through the same primitive loop path;
- pinned-Reflaxe span iterator graphs recognized from typed `Span`/`ConstSpan`
  and replaced by compiler-owned condition/body/increment/exit blocks, with no
  iterator operation reaching HxcIR;
- `Int` statement and value switches whose subject is evaluated once and
  whose ordered constant cases target explicit blocks; and
- prefix/postfix primitive increment/decrement as load, constant, typed UB-safe
  operation, store, then selection of the new or old value; and
- compound arithmetic assignment as one stable destination load, left-to-right
  operand evaluation, one typed operation, and one store.

Static-field declarations and initializers come from the `TypedProgramInput`
captured inside `filterTypes`. Lowering never asks a mutable `ClassField` for a
later expression after Reflaxe preprocessing. Referenced primitive fields
become typed HxcIR globals with registry-finalized internal C names. The focused
evaluation-order unit retains direct constant initialization; the production
whole-program path uses the E2.T09 deferred initializer and explicit bootstrap
contract so class `__init__` ordering cannot be changed by C file-scope syntax.

HxcIR instruction arrays remain semantic order. Lazy, conditional, loop, and
switch expressions create explicit blocks and terminators; their C form uses
typed labels, `if`, `switch`, and `goto` nodes. Every emitted C case body ends
in a structural `goto`, including grouped Haxe cases and `default`, so there is
no user-observable C fallthrough. Source `break` and `continue` are already
resolved to HxcIR edges and are not reconstructed from C lexical nesting.
Loads and stores retain structural local/global
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

This remains a primitive control-flow slice, not general pattern matching or
collection iteration. The fixed-array slice admits only local literal-backed or
bounded compile-time-sized zero-initialized fixed arrays and
the exact typed span iterator shape. Enum/string/object patterns, Float
switches, arbitrary iterators, general arrays, escaping views, exception edges,
and cleanup-bearing exits remain fail-closed.
E2.T05 routes signed updates through wrapping program-local helpers and keeps
safe `UInt` updates as direct unsigned C. A
right operand that creates control flow forces the already evaluated left value
into a typed flow local before that control flow begins.

The existing representation-neutral HxcIR fixture for
`arr[nextIndex()] += produce()` continues to fix the general compound-index
order as index call, stable element address, load, right-side call, and store.
E2.T08 separately provides generated fixed-array/span evidence; it does not yet
generalize that compound operation to arbitrary arrays. See
[fixed arrays and span-based iteration](span-lowering.md).

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
npm run test:span-lowering
npm run snapshots:check
```

The suite renders repeated, reversed-input, portable, and metal reports;
checks exact HxcIR/header/C/symbol snapshots; checks nested `while`/`do-while`
and range-loop graphs, innermost jumps, statement/value switches, explicit
non-fallthrough case edges, exact-once subjects, both required and skipped lazy
operands, and the indexed compound-assignment IR; runs an Eval oracle; builds
real production projects including `hxc_runtime=none`; and compiles/runs checked-in
and production-generated C
with required GCC and Clang lanes at `-O0` and `-O2` under warning-clean strict
C11. The arithmetic suite extends that proof to signed updates, compound
assignment, boundary arithmetic, and eligible UBSan execution.
The span suite adds the six-way profile/build bounds-policy matrix, direct
element-scaled indexing, fail-stop out-of-bounds execution, and linked
zero-`hxrt` evidence.
