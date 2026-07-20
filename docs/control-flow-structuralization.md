# HxcIR-to-C control-flow structuralization

`haxe_c-xge.18.2` adds a validated boundary between semantic HxcIR and
syntax-only CAST. Its purpose is narrow: keep HxcIR's explicit control-flow
facts while making ordinary reducible Haxe programs render as maintainable C
sequences, conditionals, loops, switches, and returns instead of a literal
basic-block label/goto listing.

This is not a replacement for HxcIR and not a second durable compiler IR.
`CBodyControlFlowPlan` is a request-local derived plan. Its nodes refer back to
validated HxcIR block IDs rather than copying instructions, values, places,
failure policy, or cleanup state.

## Pipeline and ownership

```text
TypedExpr
  -> HxcIR builder
  -> HxcIRValidator
  -> CBodyControlFlowPlanner
  -> CBodyControlFlowPlanVerifier
  -> CBodyEmitter builds CAST
  -> CASTPrinter formats strict C11
```

| Boundary | Owns | Must not own |
| --- | --- | --- |
| HxcIR | Ordered semantic instructions, basic-block edges, values, places, failure, cleanup, lifetime, and runtime intent | C lexical nesting or punctuation |
| Control-flow analysis | Reachability, predecessors, dominance, post-dominance, natural loops, backedges, and multi-entry SCC detection | Emitted identifiers or C text |
| Region plan | The proven C structural choice and exact HxcIR block ownership | Copies of instructions or new Haxe semantics |
| Plan verifier | Coverage, edge/terminator agreement, condition availability, proof validity, and bounded escape authority | Tree repair or fallback selection |
| CAST emitter | Typed C statements for the already-selected plan | Rediscovery of joins, loop targets, or source constructs |
| Printer | Precedence, delimiters, whitespace, escaping, and line placement | Semantic lowering or illegal-node repair |

HxcIR remains CFG-shaped because explicit edges are the simplest reliable
semantic form for evaluation order, values crossing control flow, abrupt
completion, future failure paths, and cleanup. Readable C is a downstream
representation choice. Collapsing the CFG early would make validation depend
on target syntax; retaining blanket gotos forever would make a correct
compiler unnecessarily hostile to C readers. The derived plan preserves both
properties without duplicating the semantic program.

## Closed plan model

A structured plan contains a root region and any explicitly authorized label
targets. A region owns an ordered node array plus one completion:

- fall through to the declared continuation;
- closed because nested nodes complete every path;
- return, throw, or unreachable at one HxcIR owner block;
- break or continue to the verified active loop target; or
- one bounded reason-tagged goto described below.

The node family is closed and typed:

- `CFNBlock` emits one block's ordered instructions;
- `CFNIf` owns true/false regions plus its post-dominator or escape proof;
- `CFNWhile` owns a condition region, body region, one loop exit, and the
  verified condition polarity whose edge remains inside;
- `CFNDoWhile` owns the corresponding post-test shape and polarity;
- `CFNSwitch` and `CFNTagSwitch` own grouped labels, arm regions, and a join or
  closed-arm proof.

Each reachable HxcIR block must occur exactly once in a structured plan. A
missing block, duplicate block, unknown target, mismatched terminator, stale
condition value, invalid loop fact, unlabeled switch arm, or unowned
completion is an `HXC9000` compiler invariant failure. The verifier reports the
first broken boundary; neither the emitter nor printer tries to recover.

## Reducibility and joins

Analysis is deterministic in HxcIR block order. It builds predecessor sets,
iterative dominators and post-dominators, natural loops from dominated
backedges, and strongly connected components. Irreducibility analysis is
recursive rather than limited to maximal SCCs: when a cyclic SCC has one entry,
the analysis removes that unique outer header and scans the remaining induced
subgraph again. This detects a nested two-entry cycle hidden inside an otherwise
single-entry outer loop. An SCC with more than one entry is genuinely
irreducible and cannot be represented by ordinary nested C regions without node
duplication or a state-machine transformation.

For a reducible branch, the immediate post-dominator is the simplest normal
join. A formal post-dominator may not exist when one arm returns or otherwise
completes abruptly, so the planner can also prove a *normal join*: every path
either reaches the candidate, reaches an explicit active loop escape, or ends
abruptly. That proof is tied to exact one-time ownership:

- an abrupt terminal reachable forward from the candidate is continuation
  content, not an alternative arm completion;
- the pre-candidate block sets for distinct arm occurrences must be pairwise
  disjoint, including two branch edges that initially target the same block;
- an already-owned block cannot enter a new arm prefix;
- a direct break or continue target becomes an empty region with a typed escape
  completion rather than being traversed as an ordinary block; and
- when a nested construct is being built toward an enclosing stop and that stop
  satisfies the proof, it is preferred over a locally attractive candidate.

These rules prevent shared abrupt tails or same-target branches from being
emitted twice. The planner may separately prove that one arm returns, throws,
breaks, or continues while the other is the continuation, or that both arms
complete. Nested early returns remain inside their owning branch and loop
regions.

## Loop proof and emitted shape

A natural loop is admitted only when the planner proves one structural
condition and one ordinary exit. For pre-test loops, the selected condition is
the unique outermost exit decision: it dominates every nested exit decision
and all backedges. This distinguishes the real loop condition from a body-level
`if (...) break`. For post-test loops, the condition's inside edge returns to
the loop header. Acyclic header-dominated chains that lead to the proven exit
or an explicit return/throw/unreachable are absorbed as owned break or
terminal arms; an arbitrary second continuation is rejected. Escape-chain
discovery sorts sources and targets in validated block order. If two chains
converge, the second may stop at an already absorbed suffix, so the result does
not depend on map traversal or which prefix is discovered first.

The plan records whether the HxcIR `true` or `false` edge stays in the loop.
Emission breaks on the opposite polarity; it never assumes that `true` means
"continue." The verifier rechecks both exact branch targets against that typed
fact. Synthetic inverted pre-test and post-test graphs keep this independent
of the frontend's usual edge orientation.

The usual pre-test rendering is deliberately:

```c
while (1) {
  /* ordered HxcIR condition instructions */
  if (!condition) {
    break;
  }
  /* body */
}
```

Haxe loop conditions may contain multiple loads, calls, conversions, or
short-circuit regions that must run in statement order on every iteration.
`while (1)` keeps those instructions in the loop and preserves their lexical
scope. It is more truthful than hoisting them before a compact C condition.
`do`/`while (1)` applies the same rule to post-test loops. A future cleanup pass
may choose `while (condition)` only for a stable, total expression whose
sequencing proof remains intact.

Source `break` and `continue` are never guessed from Haxe block names. Frontend
lowering first resolves them to HxcIR edges. The region planner then checks
those edges against the active natural-loop header, exit, and continue target
before selecting structural C `break` or `continue`.

## Switches and bounded goto policy

Haxe switches do not acquire C fallthrough. The subject is evaluated once,
labels with the same HxcIR target are grouped, each target region is emitted
once, and any arm that rejoins ends with structural C `break`.

Blanket goto emission is prohibited for a reducible function. There are only
two current authorities:

1. `CCFLegacyIrreducible` retains the validated whole-function CFG only after
   SCC analysis proves multiple entries. Its ordered entry IDs are part of the
   proof and the verifier rejects the fallback for a reducible graph. Every
   legacy jump target is labeled; this includes the function entry when an
   irreducible backedge targets it.
2. `CFCGoto(..., CBGRLoopBreakThroughSwitch)` is a bounded structured escape.
   A Haxe `break` inside a switch nested in a loop exits the loop, while a C
   `break` exits only the switch. The direct jump targets the already-verified
   loop exit and avoids an invented mutable flag or duplicated continuation.

No current source-backed fixture requires the bounded switch escape, and all
checked-in reducible generated-C snapshots contain no compiler-emitted goto.
Normal edges with block arguments or cleanup, and throw-to-block edges, are not
silently assigned a legacy fallback. Instruction-level call, conversion, or
allocation failures targeting another HxcIR block are rejected at the same
planning boundary; only already-supported propagate/abort instruction failures
remain local to their owning instruction. This structuralization slice rejects
cross-block failure edges until a typed ownership policy exists. Cleanup on an
already-supported terminal return/propagate/abort remains explicit in HxcIR and
is emitted by the existing terminator path.

## Determinism and evidence

Planning uses the function's validated block order and explicitly sorted
derived label/entry sets; it does not depend on map iteration, source label
spelling, checkout paths, locale, or compiler-server history. The plan is
validated before any CAST node is constructed. HxcIR dumps therefore remain
byte-identical when only the selected C structure changes.

The focused evaluation-order probe covers:

- nested `if`/`else`, short-circuit values, pre/post-test and range loops;
- nested break/continue and early return;
- grouped non-fallthrough statement and value switches;
- a synthetic reducible diamond; branch and three-arm switch normal joins with
  abrupt peers; same-target nested branch arms; direct break/continue peers;
  loops with early break/return; and inverted pre-test/post-test polarity;
- deterministically reordered, converging break chains with a shared absorbed
  suffix;
- a loop-nested switch requiring exactly one bounded jump to its proven exit;
- a maximal two-entry SCC plus a nested two-entry SCC hidden below a unique
  outer entry, with exact fallback proofs;
- CAST/printer output for the bounded jump and nested irreducible fallback,
  compiled and executed as strict C11 under the native O0/O2 matrix (including
  a backedge to the labeled function entry); and
- negative overlapping-prefix normal joins, unreachable blocks,
  cleanup-bearing edges, instruction failure-to-block edges, malformed regions,
  swapped edges, and reordered sequence plans.

The native matrix executes strict C11 under available identity-checked GCC and
Clang at O0/O2, compares an Eval oracle, exercises sanitizer/UB gates, and
checks repeated/reversed/profile determinism. Caxecraft additionally exercises
deep nested domain control flow in split, package, and unity layouts with exact Eval/C
traces, O0/O2, ASan/UBSan, zero-runtime evidence, and a source-wide rejection
of compiler `goto` statements or labels in its reducible showcase output.

Run:

```sh
npm run test:evaluation-order
npm run test:caxecraft-domain:full
npm run snapshots:check
```

Human-oriented names and conservative temporary coalescing are covered by their
own lowering work, and the [maintainability rubric](generated-c-maintainability.md)
now guards the fixed body, evaluation-order, and Caxecraft corpora. Declaration
cleanup and broad arbitrary-program handoff evidence remain separate work.
Structural control flow is a prerequisite for those improvements, not proof
that every generated program already resembles handwritten C.
