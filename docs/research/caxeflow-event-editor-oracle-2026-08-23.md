# CaxeFlow event/editor Oracle disposition

This record reconciles Oracle request
`orq_20260823T201032Z_d08d04c5` with the current checkout. Oracle reviewed the
first implementation of `haxe_c-xge.19.10` and correctly said not to close the
issue at that point. The issue closed after the remaining adapters, diagnostics,
canonical authoring round trip, and native screen proof passed.

## Local baseline

The reviewed design keeps one typed `Scenario` and one canonical CAXEMAP writer.
Closed Haxe enums own events, context, predicates, actions, repeat policy, and
runtime diagnostics. Data files own localized prose. The editor sends typed
commands through `EditorSession`; it does not keep another mutable rule document.

The reconciliation retained Oracle's central finding: each input boundary must
use the same semantic facts that validation used. It did not adopt a dynamic
event bus, string payload map, alternate editor document, or per-feature trigger
class.

## Claim disposition

| Oracle claim | Disposition | Integrated result or remaining owner |
| --- | --- | --- |
| Tick input mutates before complete validation. | Retained and fixed. | Positions, total events, context, source references, and actor roles validate before the clock or state changes. Position updates are atomic. |
| Restored history can come from a future tick. | Retained and fixed. | Planner restoration rejects history after the executor snapshot tick. |
| Deferred snapshots admit event kinds the executor cannot produce. | Retained and fixed. | Restore admits only signal, objective, state, and campaign-exit follow-ups plus validated scheduled sequences. |
| Pending `GameSession` events bypass full executor validation. | Retained and fixed. | The session now calls the executor's public scenario-specific occurrence check before restoring pending events. |
| Object state can be live but impossible to restore. | Retained and fixed. | Static validation, live mutation, and snapshot restore require a stateful target and a state owned by that target's content type. |
| Snapshot inventory and positions are under-validated. | Retained and fixed. | Restore rejects unknown items, values above the content stack limit, duplicate stacks, and positions outside the validated world. Distance math converts to `Float` before subtraction. |
| Restored collision is stale for the first movement step. | Retained and fixed. | `GameSession` rebuilds stateful collision immediately after a successful atomic Flow restore. |
| `NearObject` sees initial positions. | Retained and fixed. | Each successful game tick supplies active authoritative actor positions in stable order. |
| Spatial queue pressure can join separate movement segments. | Retained; fixed with bounded reservation. | Validation reserves the worst-case enter-and-leave pair for every zone/actor pair and requires at least one external slot. External adapters cannot consume the spatial reserve, so a committed movement cannot fail later for spatial queue capacity. |
| Inactive actors can still emit spatial events. | Retained and fixed. | Inactive and defeated actors are excluded from spatial observation and position publication. |
| Connected overlaps need a creator warning. | Retained and fixed. | The editor reports every connected positive-volume overlap as a non-blocking localized warning. Face contact is not overlap. Priority and rule-ID order are stated. |
| Native cards are read-only. | Retained and fixed for the minimum issue slice. | The screen can connect a zone, change enter/leave, replace `IF`, insert/replace/reorder/remove `DO`, and pick typed world references. Each pick is revision-bound and each edit commits through canonical `PutRule`. The polished card library remains `haxe_c-xge.19.6`. |
| Trigger copy semantics are ambiguous. | Retained and resolved. | Normal trigger duplication also copies directly connected enter/leave rules, allocates fresh rule IDs, and rewrites only the copied event source. |
| Runtime failure continues after a completed prefix. | Retained and fixed with Oracle's smallest policy. | The first runtime diagnostic latches a terminal Flow fault. Later calls return the same fault without advancing or consuming input. A validated restore is the explicit recovery boundary, and event queueing rejects work while faulted. |
| Projection and picker traversal can drift. | Retained and fixed at the active duplication seam. | `EditorFlowReferences` is the one typed traversal used for both card projection and picker replacement, including nested predicates, sequence arguments, and seeded choices. Further registry-field unification remains part of this open issue's diagnostic work. |
| English/es-MX text is duplicated or code-owned. | Retained and fixed for the implemented surface. | Diagnostics, card summaries, overlap warnings, and controls use stable keys plus typed arguments from the validated UI catalog. `AGENTS.md` now applies the data-catalog rule to every subsystem. |
| Block, item-use, defeat-cause, timer, and campaign-exit adapters are incomplete. | Retained and fixed. | Authoritative mine, remove, place, recovery, and combat transactions reserve their complete event batch before mutation. Damage carries a closed local-item, authored-actor, or environmental cause. Timer and campaign-exit events must name an action-owned identity. |
| Diagnostics need field-level kind mismatch, cycle, attempted-count, prefix, and disposition detail. | Retained and fixed for the accepted closed model. | Wrong-kind references identify the exact field, value, and expected role through catalog data. Validation rejects unconditional deferred-event feedback cycles. Terminal failures report attempted work, retained-prefix counts, and the restore-required disposition. |
| Advanced text/cards and native graphical proof are missing. | Retained and completed for this issue's boundary. | Visual edits write the canonical model. The native pilot passed place, resize, connect, edit, world-pick, copy cleanup, validation, Test Play, save, and reopen. Lossless advanced text and the complete child card library remain owned by `haxe_c-xge.19.6`. |

## Owner decisions

The reconciliation selected these bounded policies:

- The existing stable-ID rename stays atomic and rewrites typed references. A
  separate creator label is not added in this slice.
- Normal trigger copy includes connected behavior.
- Defeated actors stop producing spatial events.
- A runtime fault retains its visible completed prefix and becomes terminal.
- Every connected interior overlap warns; touching faces do not.
- Localized prose has one data owner. Code can carry only message keys, typed
  arguments, and locale-independent technical tokens, except for one documented
  bootstrap fallback.

## Verification reproduced locally

The following focused checks passed after integration:

- `haxe caxeflow.hxml`: all 13 events, 14 predicates, 19 actions, snapshots,
  terminal faults, content bounds, repeat policy, and budgets.
- `npm run test:caxecraft-caxeflow --silent`: pinned Haxe execution and
  C/es-MX locale determinism.
- `haxe editor.hxml`: canonical card edits, typed picks, trigger copy, overlap,
  history, Test Play, and presentation projection.
- `haxe runtime-level-loader.hxml`: moved positions, spatial semantics,
  save/reload, immediate collision refresh, and malformed pending-event refusal.
- `haxe scenario-model.hxml`, `haxe scenario-codec.hxml`, and
  `haxe scenario-native-codec.hxml`: schema and validator behavior.
- `haxe runtime-schemas.hxml`: complete English/es-MX catalog and compatible
  placeholders.
- `haxe runtime-content-generation.hxml`: refreshed content receipts and
  generation hash.
- The isolated `editor-shell` native pilot generated split C, compiled all 167
  C objects with strict C11 checks, and linked with Apple Clang 17. Two
  independent 1280x720 software-rendered runs produced identical semantic
  evidence with one accepted terrain edit and zero rejected edits. Each run
  completed trigger-card authoring, trigger copy/delete cleanup, validation,
  two fresh Test Play sessions, canonical save, and package reopen.
- `npm run test:agent-instructions --silent` and `git diff --check`.

## Closure boundary

The semantic, diagnostic, and native graphical evidence for this issue is
complete. The complete child card library and lossless advanced text surface
remain correctly owned by `haxe_c-xge.19.6` rather than this issue.

The final isolated native run used a diagnostic 3600-second Haxe bound because
the shared host remained saturated. The Haxe phase took about 30 minutes, then
the fresh 167-object C build and two native runs passed. This duration is
contention evidence, not a new compiler baseline or a reason to increase the
normal launcher timeout.

Integrated conclusion: Oracle changed the implementation materially. The
corrected architecture and focused cross-target evidence are sound. The native
graphical pilot passed, so `haxe_c-xge.19.10` can close without absorbing the
separate asset-browser and advanced-text work.
