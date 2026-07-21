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

### Compiler-generated flow carriers

Haxe sometimes simplifies a lazy Boolean before haxe.c receives it. Source such
as `alive(state) && mode(state) == Windup && helper(state)` can arrive as this
equivalent typed shape:

```haxe
var condition;
if (alive(state))
	condition = mode(state) == Windup;
else
	condition = false;
if (condition && helper(state)) {
	// ...
}
```

Here `condition` is a **flow carrier**: a compiler-created local that carries
one value out of several control-flow paths. It is safe only because every path
of the immediately following `if` assigns it. The lowerer now recognizes that
closed shape for exhaustive `if` and `switch` expressions, creates the typed
local, and then lowers the assignments and later lazy operation normally. A
missing `else`, missing switch default, or arm that does not assign the exact
local remains the source-positioned `HXC1001` uninitialized-local error.

The local receives a defensive type-correct default before its exhaustive
branches. That value cannot be observed on an admitted path because each branch
overwrites it, but it also means emitted C never contains uninitialized storage
if a later compiler pass changes the structure incorrectly. Validation and the
ordinary short-circuit blocks still decide whether the right-hand helper runs;
the printer does not repair or rediscover this rule.

Static-field declarations and initializers come from the `TypedProgramInput`
captured inside `filterTypes`. Lowering never asks a mutable `ClassField` for a
later expression after Reflaxe preprocessing. Referenced primitive fields
become typed HxcIR globals with registry-finalized internal C names. The focused
evaluation-order unit retains direct constant initialization; the production
whole-program path uses the E2.T09 deferred initializer and explicit bootstrap
contract so class `__init__` ordering cannot be changed by C file-scope syntax.

HxcIR instruction arrays remain semantic order. Lazy, conditional, loop, and
switch expressions create explicit blocks and terminators. After HxcIR
validation, `CBodyControlFlowPlanner` derives a closed typed region plan and
independently verifies exact block coverage, value availability, edge shape,
dominance/post-dominance, loop ownership, and completion. Ordinary reducible
graphs then become structural C `if`/`else`, `while`, `do`/`while`, `switch`,
`break`, `continue`, and `return` nodes instead of a literal block-label dump.

Each switch subject is still evaluated exactly once. Cases that share one
HxcIR target share one emitted arm body, and each continuing C arm ends in a
structural `break`, so there is no user-observable C fallthrough. Source
`break` and `continue` are resolved to HxcIR edges first; the region plan may
select lexical C statements only after verifying the target belongs to the
active loop. The one bounded exception is a Haxe loop break nested inside an
emitted C switch: C's nearest `break` would leave only the switch, so the plan
may own one typed, reason-tagged jump to the already-proven loop exit. A
genuinely irreducible multi-entry SCC may retain whole-function CFG emission;
ordinary reducible functions may not. Loads and stores retain structural
local/global places. No source-derived C fragment, C ternary, raw injection,
reflection, `Dynamic`, `Any`, or untyped escape is used.

Pre-test loops are commonly rendered as `while (1)` with the condition's
ordered HxcIR instructions at the top and a structural `break` on false. That
shape is intentional: a Haxe condition can require several sequenced loads or
calls on every iteration, and moving them into a single C condition would
weaken their explicit statement order or scope. Post-test loops use the same
principle with a structural `do` body. A later canonicalizer may choose a
compact `while (condition)` only when it can prove the condition is already a
safe stable C expression.

## Stable-value and temporary proof

“Materialize” means copying a computed value into a named C variable at the
point where Haxe computed it. “Coalesce” means omitting that variable and
placing the expression directly at its one later use. Coalescing makes C easier
to read, but it can be wrong if it silently changes *when* a read or call
happens. C does not promise left-to-right evaluation for every operator or
function argument, while Haxe does.

`CBodyValueCoalescingPlanner` therefore performs a small proof per HxcIR
function. It is intentionally not a general optimizer. These values may remain
structural CAST expressions without a temporary:

- constants, parameters, and proven-total pure conversions/operators;
- a single-use compiler-owned record or enum value in the same block; and
- one single-use direct local or local-field read when its final use is in the
  same block and nothing observable occurs between the read and that use.

The last rule is the narrow one. The planner follows a chain of pure arithmetic
or record operations to one final consumer. It refuses to move the read across
another read, call, store, address operation, check/failure edge, allocation, or
lifetime change. A terminator carrying cleanup steps is also a hard barrier:
Haxe computes the returned/thrown value before cleanup runs, so that value must
already be materialized. The planner also refuses when the expression fans out
to several uses.

These values remain named C storage:

- global, pointer/dereference, indexed, and span reads, because they may observe
  foreign, aliased, or volatile-like storage;
- values used more than once or directly outside their producing block;
- call results and other effectful or unsupported producers; and
- any otherwise-local read separated from its consumer by an observable,
  failing, aliasing, or lifetime-changing instruction.

This keeps a Haxe read before an intervening call and keeps consumed Haxe calls
in their original order, while removing mechanical temporaries such as a
single-use `record.x` projection. The planner returns a closed typed reason for
every value that stays materialized; the C emitter consumes that plan and does
not rediscover the policy. Coalesced values are also removed before symbol-name
finalization, so `hxc.symbols.json` does not advertise C variables that were
never emitted.

The evaluation-order probe exercises safe local and field reads, pure records,
intervening reads and native calls, global/dereference/index places, multiple
uses, expression fanout, cross-block misuse, checked failure, lifetime change,
return-edge cleanup, call results, and planner reuse. It also requires the real source-backed fixture
to coalesce at least one eligible local read. Protected loads and consumed calls
must still appear in the generated temporary set. Native differential execution
at `-O0` and `-O2` remains the final behavior check.

Any future expansion must add the same kind of explicit proof and adversarial
case. Readability alone is not authority to weaken sequencing.

## Boundaries

This remains a primitive control-flow slice, not general pattern matching or
collection iteration. The fixed-array slice admits only local literal-backed or
bounded compile-time-sized zero-initialized fixed arrays and
the exact typed span iterator shape. Enum/string/object patterns, Float
switches, arbitrary iterators, general arrays, escaping views, exception edges,
instruction failures targeting another HxcIR block, and cleanup-bearing exits
remain fail-closed.
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
and range-loop graphs, lexical innermost jumps, statement/value switches,
structural non-fallthrough case arms, exact-once subjects, both required and
skipped lazy operands, and the indexed compound-assignment IR; directly probes
reducible diamonds, abrupt-arm branch/switch normal joins, same-target arms,
direct loop escapes, converging break chains, loops with early break/return,
inverted pre/post-test condition edges, maximal and nested irreducible graphs,
the bounded loop-break-through-switch escape, and malformed
region/edge/sequence/reachability rejection. The bounded and irreducible plans
are also passed through `CBodyEmitter`, CAST, and the printer; their checked-in
strict-C fixture proves the bounded exit label and an irreducible backedge to
the function-entry label natively. The suite runs an Eval oracle; builds real
production projects including `hxc_runtime=none`; and compiles/runs checked-in
and production-generated C
with required GCC and Clang lanes at `-O0` and `-O2` under warning-clean strict
C11. The arithmetic suite extends that proof to signed updates, compound
assignment, boundary arithmetic, and eligible UBSan execution.
The focused `FlowCarrierFixture` retains both `||` and `&&` versions of Haxe's
optimized if-assigned carrier. Its native result proves that skipped helpers do
not run and evaluated helpers run in order. Isolated split, package, and unity
builds must be byte-identical on repetition; the direct generated C must remain
free of `goto`, allocation calls, and `hxrt` before strict `-O0`/`-O2` native
execution.
The span suite adds the six-way profile/build bounds-policy matrix, direct
element-scaled indexing, fail-stop out-of-bounds execution, and linked
zero-`hxrt` evidence.
