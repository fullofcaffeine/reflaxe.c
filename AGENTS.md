# Agent Instructions

## Reasoning-effort calibration

Before claiming or beginning substantial work on a new task, use
`$calibrate-reasoning-effort`, state the lowest effective level and why in one
sentence, and apply it for subsequent turns when the host exposes that control.
Recalibrate when evidence materially changes the task's ambiguity, risk, blast
radius, verification cost, or ability to split into independent work.

At task closure, check whether the completed work changed the shape or risk of
the next task. If another task will begin, calibrate that new task before doing
its substantial work. Do not invoke the skill a second time merely to assign a
retroactive label to finished work or for a tiny administrative close: the
finished task can no longer benefit from a new setting. A recommendation or
task label records intent; only a successful host/App Server update proves that
the setting changed for subsequent turns.

This project uses **bd** (beads) for issue tracking. Run `bd prime` for full workflow context.

> **Architecture in one line:** Issues live in a local Dolt database
> (`.beads/dolt/`); cross-machine sync uses the guarded
> `scripts/beads/push-safe.sh` wrapper and `bd dolt pull` (a git-compatible
> protocol), stored under `refs/dolt/data` on your git remote — separate from
> `refs/heads/*` where your code lives. The wrapper scans decoded current and
> historical Beads records before it delegates to `bd dolt push`.
> `.beads/issues.jsonl` is a passive export, not the wire protocol.
>
> See [SYNC_CONCEPTS.md](https://github.com/gastownhall/beads/blob/main/docs/SYNC_CONCEPTS.md)
> for the one-screen overview and anti-patterns (don't treat JSONL as the
> source of truth; don't `bd import` during normal operation; don't
> reach for third-party Dolt hosting before trying the default).

## Quick Reference

```bash
bd ready              # Find available work
bd show <id>          # View issue details
bd update <id> --claim  # Claim work atomically
bd close <id>         # Complete work
npm run beads:push     # Scan and push Beads data to remote
```

## Commit Messages

- Treat a verified task boundary as a Git boundary. After the owning Beads
  issue is updated or closed and its required gates pass, commit that task's
  complete scoped changes before beginning substantial work on the next task.
  Aim to leave the worktree clean between tasks so later failures and reviews
  have an honest starting point. Do not achieve cleanliness by sweeping
  unfinished, unrelated, generated-local, or user-owned work into the commit:
  give unfinished work an active issue, keep it clearly isolated, and report
  the remaining paths. If several already-completed tasks have accumulated,
  prefer one explicitly named catch-up integration commit over inventing
  misleading historical boundaries after the fact.
- Keep the conventional-commit subject concise, then add a useful commit body
  for every non-trivial change. Write the body in friendly, beginner-readable
  language so someone who does not already know the compiler internals can
  understand what problem was solved.
- Explain what changed, why it matters, and how it was verified. Call out
  important behavior or output changes and name any intentionally deferred
  scope so the commit does not imply broader closure than it provides.
- Prefer concrete descriptions of the old and new behavior over a list of
  filenames or internal type names. Technical details are welcome, but
  introduce them in plain language and make the practical outcome clear first.

## Readable, Educational Writing

Treat source code, comments, diagnostics, examples, and documentation as a
learning path for a curious contributor, not as notes for people who already
know the compiler. Reading this repository should be accurate, approachable,
and pleasant.

Treat working Haxe source as executable teaching material for the language,
software design, compiler construction, and--where applicable--game-engine
architecture. Model the practice being taught in the real implementation rather
than adding tutorial-only parallel code. Point out a useful pattern where it is
used, explain its tradeoff in plain language, and link to
[`docs/haxe-code-architecture.md`](docs/haxe-code-architecture.md) or another
focused guide for the deeper lesson. Keep every lesson evidence-bounded: clean
prose must never turn planned behavior or unsupported lowering into fact.

- Before writing or materially revising technical documentation or comments
  whose meaning depends on specialized domain knowledge, use
  `$explain-technical-work`. Apply its first-read check: state the practical
  outcome first, give the smallest useful mental model, define exact terms
  before they carry the explanation, and connect the mechanism to an observable
  consequence or failure. The skill improves clarity and completeness; it does
  not relax this repository's evidence, precision, or scope rules.
- Prefer common, concrete words whenever they express the same idea. Do not use
  specialist vocabulary merely because it sounds more formal.
- When an exact technical term is necessary, define it in plain language the
  first time it appears, then use the precise term consistently. Expand an
  acronym on first use unless it is already a language-level name shown in the
  surrounding code.
- Lead with purpose and observable behavior: what problem this solves, what the
  user writes, and what the compiler or program does. Introduce internal layers,
  type names, and algorithms only after that frame is clear.
- Use small examples to connect source input, compiler decision, generated C,
  runtime behavior, or failure. Explain why the example matters instead of
  assuming the reader will infer the contract from a snapshot or test name.
- In code comments, explain intent, ownership, invariants, tradeoffs, and
  surprising constraints. Do not narrate obvious syntax. A reader should learn
  why the code has this shape and when the rule applies.
- Start every repository-owned Haxe source file, after its package/import
  preamble, with module-level HxDoc that gives a newcomer the local mental
  model: what the module owns, why that work has a separate home, and briefly
  how its main data or control flow works.
  Give every class, abstract, enum, public function or field with meaningful
  behavior, and every complex private function clear HxDoc using the same
  compact **what / why / how** order. Document inputs, outputs, observable side
  effects, ownership or lifetime, failure behavior, and important invariants
  when they matter. A short one-line HxDoc is enough for a simple public value;
  a complex boundary needs a fuller explanation and a small example when that
  is the clearest teaching tool. Do not add boilerplate that restates the name,
  type signature, or obvious syntax, and do not let comments compensate for an
  oversized or poorly named API. Update the HxDoc in the same change whenever
  the documented behavior changes.
- When a declaration makes a meaningful design choice and a real, plausible
  neighboring design exists, keep the local reason beside it. Briefly name that
  alternative--for example a `typedef` record versus a class, a module function
  versus a static-only class, or composition versus inheritance--and explain
  why the chosen shape matches this value's identity, ownership, mutability,
  and lifetime. Say what would make the decision worth revisiting when that is
  useful. Never invent or ritualistically reject an alternative just to satisfy
  documentation. Link to a canonical guide for the reusable rule, but do not
  make a newcomer leave the source file to understand the local choice or its
  important generated-C consequence.
- Link to the canonical deeper document when a short explanation would become
  repetitive. The local text must still make sense on its own; a link is a path
  to more depth, not a substitute for a basic definition.
- Preserve precision. Plain language must not blur a normative distinction,
  weaken a safety rule, or overstate support. Give the simple explanation first,
  then state the exact bounded contract and unsupported cases.
- Do not describe an idea, intended architecture, roadmap item, or partial seed
  as behavior the repository already has. Before making a present-tense claim,
  check the current checkout and cite the nearest executable evidence: a test,
  generated artifact, validated schema, working command, or implemented source
  path. If the behavior is not implemented and checked, label it clearly as
  planned and name its durable Beads owner. If neither evidence nor an accepted
  plan exists, describe it only as a proposal or question; do not turn it into
  documentation fact. Keep these distinctions local and readable so a reader
  does not have to inspect the issue tracker to learn what works today.
- `AGENTS.md` is the only authored agent-instruction file. A compatibility
  `CLAUDE.md` must be a Git-tracked relative symbolic link to the `AGENTS.md` in
  the same directory; never copy, generate, or manually mirror the prose. Run
  `npm run test:agent-instructions` after adding, moving, or changing either
  path. The guard rejects regular-file copies, broken links, absolute or
  escaping targets, and links to an untracked file.
- Structure longer material from overview to example to implementation details
  to evidence. Use headings, short paragraphs, and terminology consistently so
  the document can be read as a tutorial as well as a reference.
- Update nearby explanations and comments when behavior changes. Stale,
  unexplained, or needlessly opaque documentation is a product and maintenance
  defect, even when the implementation is correct.

## Non-Interactive Shell Commands

**ALWAYS use non-interactive flags** with file operations to avoid hanging on confirmation prompts.

Shell commands like `cp`, `mv`, and `rm` may be aliased to include `-i` (interactive) mode on some systems, causing the agent to hang indefinitely waiting for y/n input.

**Use these forms instead:**
```bash
# Force overwrite without prompting
cp -f source dest           # NOT: cp source dest
mv -f source dest           # NOT: mv source dest
rm -f file                  # NOT: rm file

# For recursive operations
rm -rf directory            # NOT: rm -r directory
cp -rf source dest          # NOT: cp -r source dest
```

**Other commands that may prompt:**
- `scp` - use `-o BatchMode=yes` for non-interactive
- `ssh` - use `-o BatchMode=yes` to fail instead of prompting
- `apt-get` - use `-y` flag
- `brew` - use `HOMEBREW_NO_AUTO_UPDATE=1` env var

## Developer Feedback-Loop Performance

Fast, dependable feedback is repository infrastructure, not optional polish.
Design build, watch, test, snapshot, and CI tooling so developers spend as
little time waiting as correctness permits.

- Provide the narrowest useful focused command as well as the exhaustive
  reference lane. Do not force an unrelated full repository run to answer a
  local question.
- Measure before and after changing a slow lane. Record phase timings, identify
  the first real bottleneck, and keep enough evidence to distinguish useful
  work from setup, contention, duplicated work, or a hang.
- Treat resource contention as a timing variable, not semantic evidence.
  Isolated jobs from other repositories should not change generated bytes, but
  CPU, memory, and disk pressure can trigger a bounded timeout. Preserve the
  timeout and diagnostics, retry under representative load before classifying
  a timeout as a compiler regression, and never publish performance claims from
  a saturated host.
- Do not repeat an expensive compile, render, native build, or oracle run in one
  workflow unless the second execution proves a documented independent
  invariant. Share or reuse evidence only when its inputs and provenance are
  content-addressed and validated fail closed.
- Parallelize independent lanes when each worker has isolated process state,
  temporary/output roots, ports, caches, and artifact ownership. Bound local
  concurrency with a documented, resource-aware setting; make result and log
  collation deterministic.
- Never parallelize a lane whose purpose depends on warm compiler-server reuse,
  ordered mutation, shared-output stale deletion, contention, or a fixed port
  until the test has an explicit isolation design proving that it remains the
  same test.
- CI must use bounded timeouts, visible per-lane status, and a fail-closed
  aggregate required check. A timeout increase is containment, not a substitute
  for removing redundant work or fixing a performance regression.
- Caches must be derived from reviewed locks and complete input hashes. Keep a
  cold path so a warm cache cannot become the only reason a required gate
  passes.
- Faster must never mean silently dropping snapshot ownership, deterministic
  byte comparison, sanitizers, native compiler families, platform lanes,
  negative diagnostics, or another acceptance requirement.
- When a required lane exceeds its documented budget, create or update an
  owning Beads issue with timing evidence and a remediation plan instead of
  normalizing the delay.

The current lane topology, commands, safety classifications, baseline, and
optimization sequence are maintained in
[`docs/test-performance.md`](docs/test-performance.md).

## Flagship End-to-End Feedback Loop

Treat realistic applications as active compiler-development instruments, not
as demos that are polished only after the compiler is finished. Caxecraft is
the current flagship application and primary product-level end-to-end (E2E)
quality-assurance workload for haxe.c. It should continually exercise ordinary
Haxe, typed `c.*` interop, generated-C quality, runtime selection, native tools,
performance, packaging, and developer experience together.

- When a flagship workload breaks or exposes weak output, first reproduce and
  classify the problem: application design, binding, compiler semantics,
  generated-C structure/readability, runtime/ABI, build tooling, or test cost.
  Do not hide a compiler defect with example-only raw C, copied logic, a looser
  assertion, or a Caxecraft name/path special case.
- Reduce each compiler defect to the smallest reusable positive, negative,
  semantic, structural, or native fixture that explains it. Fix it in the
  narrowest owning compiler/runtime/tooling layer, then retain both that fast
  focused regression and the flagship E2E path that originally exposed it.
  The focused test diagnoses the rule; the flagship proves the layers work
  together.
- Design the flagship's public APIs in the natural, safe Haxe shape first. If
  that shape reveals a missing haxe.c capability, do not replace it with a
  compiler-shaped bandage: generated target-library calls, duplicated
  application logic, scattered target conditionals, raw C, or a weaker public
  abstraction must not become the design merely because the current compiler
  accepts them. A compiler limit changes the current work lane, not the product
  architecture: pause the application change at its smallest honest
  reproducer, switch to the owning compiler/runtime work, lift the general
  limit, and then resume the natural Haxe implementation. Give the compiler gap
  a focused fixture and an owning Beads issue, implement the reusable typed
  lowering, and keep the flagship path as its integrated proof. Do not document
  an avoidable compiler restriction as an application design rule, propagate it
  into new APIs or data formats, or declare the feature complete while its
  natural implementation is still blocked. A temporary bridge is acceptable
  only when an unrelated blocker prevents that compiler work. Before adding one, state the
  concrete question or learning it unlocks, the smallest evidence it must
  produce, the definitive path it is standing in for, and the observable
  removal condition plus owning Beads issue. Keep it private, explicitly
  transitional, and minimal. Link the temporary code and its nearest truthful
  documentation to that removal issue so a later contributor cannot mistake it
  for the intended design. Do not polish, broaden, duplicate, or build new
  features on top of it. Remove it as soon as it has produced the stated
  evidence or its blocker clears; learning from an experiment is not a reason
  to keep the experiment in the product path. Do not close or advertise the
  definitive capability while a required path still depends on the bridge,
  unless its acceptance criteria explicitly describe that bounded experimental
  state. Fail review if the bridge starts shaping source data, public APIs,
  tooling requirements, or durable architecture. Prefer a focused check that
  fails if the removal issue is closed while the temporary path still exists.
- Apply the same loop to improvements, not only failures. Measure readability,
  output shape, compile time, runtime cost, allocations, code size, diagnostics,
  and workflow friction where relevant. Turn a broadly useful improvement into
  a compiler capability rather than local game scaffolding.
- Treat Caxecraft's Haxe source as part of the showcase. It should be inviting
  to a TypeScript developer: use ordinary Haxe, descriptive domain names,
  nominal abstracts, immutable records, closed enums, exhaustive matching, and
  small cohesive modules where they make intent clearer. Do not transliterate
  C/Java machinery into Haxe, encode closed concepts as numeric/string tags, or
  let one parser/controller/game-loop class become a procedural blob. Isolate
  unavoidable target or ABI detail behind a narrow typed adapter and explain
  why it exists. Compiler limitations must not force unrelated shared gameplay
  code into a low-level style; fix or track the general compiler gap instead.
- Prefer a Haxe module-level function when a source file only groups stateless
  operations. “Module-level” means the function is declared directly in the
  `.hx` file and imported from that module, optionally under a clear local
  alias, without a wrapper class whose only members are static. A packed Haxe
  module is not a runtime namespace object; do not imply that `Module.call()`
  remains available after removing its class. Keep a real class when values
  need per-instance state, identity, ownership/lifetime, inheritance or
  interfaces, or when a framework, macro, generated/public API, entry-point
  contract, or deliberate
  nominal boundary requires a class. Do not apply this mechanically: haxe.c
  must first have focused production evidence for every admitted module-field
  shape, and a migration must preserve behavior, generated-C readability,
  allocation/runtime selection, and source-module ownership. Until that
  evidence exists, track the blocked cleanup rather than adding more static-only
  classes by habit or disguising unsupported module fields with raw C.
- Treat ordinary Haxe classes as a source-language contract, not as syntax that
  application authors must manually translate into C-shaped records. Preserve
  construction, initialization, identity, mutation, inheritance, interfaces,
  dispatch, nullability, ownership, cleanup, and failure behavior, then choose
  the least costly correct C representation from typed whole-program facts. A
  source class does not automatically require heap allocation, an object header,
  a virtual table, reflection data, or a generic runtime wrapper: use automatic
  storage, scalar replacement, plain structs plus typed functions, direct calls,
  and minimal reachable tables whenever proven; select managed allocation and
  exact roots only for values whose escape/lifetime semantics require it. Keep
  split C recognizable by source module, type, field, and method, and measure
  allocations, code size, C compile time, and runtime against reviewed
  hand-written C baselines. Current bounded class support is a capability stage,
  not the design ceiling; track missing general semantics instead of forcing
  Caxecraft or another flagship into an unnatural low-level source model.
- Preserve the accepted architecture and policy hierarchy while improving the
  workload: typed source and plans, HxcIR where C semantic gaps require it,
  structural CAST, explicit ownership/runtime decisions, formatting-only
  printing, fail-closed unsupported behavior, and ordinary Haxe plus narrow
  typed C adapters at the application edge.
- A bounded discovery that is required for the active issue's acceptance may
  be fixed with that issue. A material scope expansion gets a durable Beads
  owner and dependency instead of silently turning the active task into a
  rewrite. Prioritize discoveries that block correctness, safety, the playable
  path, or trustworthy QA.
- If a critical correctness or safety issue appears to require violating an
  accepted architectural decision, stop before implementing the deviation.
  Record the minimal reproducer, observed evidence, affected invariants,
  plausible options, migration cost, and regression gates; check the governing
  PRD/ADR; and ask the maintainer for an explicit decision. When the decision is
  unusually broad, irreversible, or uncertain, prepare that evidence as a
  focused deep-research brief for the designated Oracle model rather than
  improvising a new architecture.
- Keep claims evidence-bounded. Caxecraft is the primary integrated proof, not
  permission to infer general language support from one game. Conversely, a
  focused compiler fixture passing is not enough to claim the flagship is
  playable, attractive, fast, or portable.

The long-term intent is a constructive flywheel: Caxecraft finds real pressure,
haxe.c gains a reusable principled capability, focused tests make the fix fast
to maintain, and Caxecraft keeps the complete user journey honest.

## Project-Specific Contract

### Normative sources and read order

This is a brownfield M0 compiler scaffold. Do not treat it as a blank Reflaxe
target or as a functioning Haxe-to-C compiler.

Before changing compiler, runtime, target API, ABI, or build behavior, read:

1. `AGENTS.md` and `bd prime` for the current operating rules;
2. `docs/PRD.md` completely, especially Sections 0, 10-17, 24, 29, 32,
   36, and 40;
3. the relevant accepted records under `docs/adr/` for public decisions that
   refine the PRD;
4. `docs/architecture.md` for compiler-layer invariants;
5. `docs/research/repo-patterns.md` for sibling-target and hxcpp precedent;
6. `docs/BEADS_PLAN.md` and the active issue from
   `docs/specs/beads-plan.json`;
7. every existing source/runtime file in the area being changed.

An accepted ADR that explicitly supersedes the PRD has highest authority.
Otherwise use this order: PRD, architecture/configuration docs, active Beads
acceptance criteria, scaffold code, then examples/comments.

### Trust the checkout, not the aspirational inventory

The current checkout is a deliberately partial slice of the fuller scaffold
described by PRD Section 32. Verify files with `rg --files` before citing or
running them. In particular, this checkout currently lacks several documented
assets, including `CODEX_HANDOFF.md`, some schemas, broad runtime
implementation, and the CLI implementation. The
pinned Haxe/Reflaxe/Lix package metadata, M0 target API contracts, and minimal
compiler lifecycle are present and compile-verified; do not repeat older
missing-metadata or missing-adapter assumptions.

- A path named in the PRD is not proof that it exists here.
- A present seed is not proof that its capability works.
- Do not invent passing validation output for absent scripts or toolchains.
- Track missing or contradictory scaffold assets as Beads discoveries under
  the appropriate epic; do not silently expand the issue already claimed.
- Keep README and capability claims at M0 until executable evidence advances
  the release gate.

### Capability and limitation claims

- `docs/specs/bootstrap-inventory.json` is the machine-readable authority for
  what the current checkout implements and what remains unavailable. Keep its
  schema, evidence paths, stable Beads owners, hard claim gates, this policy,
  and the generated README capability block in one change.
- Use the four statuses literally. `implemented` means the exact bounded scope
  has executable repository evidence; `scaffold-only` means only a contract,
  seed, fixture, or plan exists; `experimental` means an explicit opt-in
  surface without a support promise; and `unsupported` means absent,
  deliberately fail-closed, or not validated. Never promote a neighboring
  capability from evidence for a narrower one.
- Regenerate the README block with
  `python3 scripts/ci/check_capability_manifest.py --write-readme`; do not edit
  that generated block by hand. Run `npm run test:capabilities` for every
  capability, evidence, product-claim, CLI, runtime, stdlib, platform, ABI, or
  release-status change.
- `hxc doctor` is currently unsupported and has no implementation to invoke.
  E8.T06 must derive its future human and JSON status from the same manifest
  capability IDs rather than introducing a second hand-maintained inventory.
- Full standard-library support, general Haxe-to-C support, production
  readiness, performance, supported platforms/releases, and public ABI
  stability remain false until their named owner beads add the required
  executable evidence and intentionally advance the manifest gates.

### Compiler and runtime invariants

- Extend the existing architecture; do not run a new-target generator over
  the repository or replace it with a generic transpiler template.
- Preserve the intended two-stage `CompilerBootstrap` / `CompilerInit`
  activation and exactly-once Reflaxe registration when those files are
  implemented or restored.
- `CReflaxeCompiler` is an adapter. Whole-program semantics belong behind
  `CompilationContext` and `CCompiler`, with fresh mutable state per build.
- Use Haxe's type system as a compiler correctness boundary. Represent compiler
  state, configuration, IR, plans, manifests, and diagnostics with explicit
  target-owned classes, enums/enum abstracts, and typedefs; use exhaustive
  pattern matching for closed variants. Do not introduce `Dynamic`, `Any`,
  `Reflect`, `untyped`, anonymous open-ended maps, or unchecked casts as a
  substitute for a missing semantic model. When an external API is inherently
  untyped (for example JSON decoding, a compiler-preview API, or a foreign
  interface), isolate it in the smallest adapter, validate every admitted
  shape/value immediately, convert it to a typed target-owned model, add
  malformed-input tests, and never let the untyped value escape that boundary.
  Existing upstream generic signatures are boundary facts, not precedent for
  untyped compiler internals.
- `docs/specs/typed-boundaries.json` is the exact searchable inventory and
  allowlist for repository-owned Haxe. Run `npm run test:typed-boundaries` for
  every owned `.hx`, boundary-policy, compiler-preview API, JSON ingress, raw
  authority, or allowlist change. Do not add an entry merely to silence the
  guard: each remaining occurrence must name its smallest adapter, closed typed
  model, immediate validation, and malformed-input evidence. A repeated
  allowance is a missing typed abstraction to implement.
- Preserve fail-closed behavior. Unsupported admitted-body paths report exact
  source-positioned `HXC1001`; a body that lowers successfully still reaches
  the deliberate `HXC1000` static-function/call/entry-point boundary until
  E2.T03. Neither result is a failure to hide, weaken, or report as successful
  compilation, and neither may leave a plausible production artifact.
- Lower Haxe semantics into an explicit target-owned HxcIR before choosing C
  syntax when evaluation order, lifetime, ownership, cleanup, dispatch,
  boxing, or failure edges matter.
- Extend `CAST` and `CASTPrinter` structurally. Normal lowering must not
  concatenate user-derived C strings. No raw expression or declaration node is
  currently admitted; any future compiler-owned escape valve needs a narrow
  typed authority, validation, executable negative tests, and an owning issue.
- Treat `CDeclarator` as the C grammar tree, not as a semantic type string:
  `DGroup` is the parenthesized-declarator production, `DName(null)` is an
  abstract declarator, and array/function variants retain their distinct C11
  forms. Never infer grouping in the printer or encode `*`, `[]`, or `()` in an
  identifier.
- Construct finalized output names through `CIdentifier`. It checks lexical
  C11 spelling after `CSymbolRegistry` has finalized namespace ownership,
  reserved-name escaping, and collisions. Never construct emitted identifiers
  directly from Haxe names or add a second sanitizer. Typed attributes,
  alignment specifiers, and atomic type names have
  dedicated nodes—do not restore `Array<String>` attributes.
- Preserve expression-tree association exactly; do not algebraically
  reassociate, commute, or flatten nodes even when a C operator looks
  associative. Numeric precedence alone is insufficient for C grammar:
  assignment left operands and prefix increment/decrement operands require a
  `unary-expression`, and adjacent unary tokens need separation when they
  could fuse into `++`, `--`, or `&&`.
- Construct integer and floating constants through validated literal nodes,
  `_Generic` through typed associations, and source anchors through structured
  `DLineDirective`/`SLineDirective` nodes. Never restore opaque literal or
  preprocessor-token strings. A `#line` mapping applies to the immediately
  following physical line, so the printer must not insert a blank after it.
- Treat generated C as adversarial source text: identifiers, strings, byte
  characters, comments, and line filenames must pass their dedicated
  validators/escapers. Preserve trigraph safety and reject malformed Unicode;
  never interpolate user text into comments, literals, directives, or tokens.
- `CASTPrinter` defaults to strict ISO C11. GNU/Clang attribute output requires
  an explicit dialect and must never leak into the normative strict lane.
  Update `test/c_ast/expected/` only from an intentional structural fixture
  change and only through the registered snapshot updater, then run both
  `npm run test:c-ast` and the native matrix. The expression fixture's literal
  6×6 family matrix is the reviewable contract; do not replace it with expected
  values derived from the printer's own precedence table.
- Extend the direct C AST corpus through `ASTFixtureCompiler` and typed AST
  builders, then update its centrally owned snapshots; never add separately
  authored C and describe it as AST-emission evidence. Native fixture projects
  use the argument-array `c_fixture_harness.py` boundary. Its report must retain
  compiler identity, normalized compile/link/run arguments, input hashes, exit,
  stdout, and stderr without serializing temporary or checkout paths.
- Write generated artifacts only through Reflaxe output ownership. Never
  hand-manage stale output outside its generated-file manifest.
- Construct every project artifact as a validated, typed `GeneratedFile` and
  package it through `CProjectEmitter`; do not pass output roots into the pure
  plan or add a parallel file writer. `ReflaxeOutputWriter` must validate the
  complete prior ownership set and complete new artifact set before the first
  save, reject traversal, descendant symlinks, duplicates, and unowned existing
  destinations, then delegate saves and stale deletion to `OutputManager`.
  `_GeneratedFiles.json` is Reflaxe activity/ownership metadata, not a normal
  compiler artifact. Runtime/ABI/stdlib placeholder sidecars must remain
  explicitly unproven, and `lowered-program` emission stays rejected until
  real semantic analyses can populate them.
- Preserve Haxe evaluation order explicitly and never depend on C undefined
  behavior, including signed overflow, invalid shifts, aliasing violations,
  misalignment, uninitialized reads, or lifetime bugs.
- Output must be deterministic across module order, checkout path, locale, and
  repeated/compiler-server builds. Do not embed unstable absolute paths,
  timestamps, random IDs, or map iteration order in normal artifacts.
- Determinism evidence compares normal compiler artifacts as raw bytes. Order
  paths by UTF-8 and report the first differing artifact plus exact byte offset;
  do not normalize away an unexplained difference. Cover isolated roots,
  discovery order, locale, CRLF inputs, renamed ownership, and warm compiler-
  server reuse. Reflaxe's `_GeneratedFiles.json` invocation/activity fields are
  engine metadata, not normal compiler artifacts, and must be tested separately.
- Examples, tests, templates, and standard-library surfaces must not use raw
  `__c__` injection to make an unsupported path appear complete. Add a typed
  `c.*` abstraction or a reusable compiler lowering instead.
- Runtime support is a feature graph. Every selected helper needs a stable
  feature ID and a source reason; unused features must remain absent.
- The checked-in `runtime/hxrt` implementation is a provisional native smoke
  seed for its public header, not an unconditional runtime core and not evidence
  that generated Haxe selects or links it. Keep fixture-only C/C++ code under
  `runtime/hxrt/test` or `test/native`; never describe those independent native
  inputs as generated output.
- Runtime planning happens only after direct-representation, escape/lifetime,
  and specialization decisions. Prefer direct idiomatic C, then a program-local
  specialized helper, then the narrowest dependency-closed `hxrt` slice. Never
  add an unconditional runtime `core`; a runtime-free build contains no `hxrt`
  include, source, define, library, or symbol.
- Apply that hierarchy as a hard compiler/stdlib optimization rule. Whenever a
  fact is statically knowable, `hxc` must perform the semantics-preserving,
  deterministic, and reasonably bounded work at compile time: representation
  selection, specialization/monomorphization, constant evaluation, static
  dispatch/devirtualization, escape/ownership/lifetime analysis, table/layout
  construction, and dead-feature/code elimination belong in the compiler rather
  than a generic runtime path. Do not move work into `hxrt` merely because a
  runtime implementation is easier. Never precompute observable runtime effects
  or accept pathological compiler cost/code-size growth; when such a tradeoff
  requires runtime work, record the concrete reason in the analysis/runtime plan.
- Lower Haxe standard-library operations through the same order: emit direct
  idiomatic C or a C-library/intrinsic operation when it preserves the Haxe
  contract, otherwise emit a program-local type/value-specialized helper, and
  select `hxrt` only for semantics that genuinely depend on runtime state or
  values, justified shared machinery, or an unavoidable platform/ABI service.
  A one-wrapper-per-stdlib-method runtime is not an acceptable default design.
- `docs/specs/stdlib-ledger.json` is the schema-2 pinned typed-surface authority;
  `stdlib-ledger.csv` is its generated review view and must not be hand-edited.
  Every applicable source module matches exactly one explicit ownership rule—no
  catch-all—and every planned/partial API links implementation/test Beads owners
  plus a source-positioned diagnostic policy. The real C carrier owns common
  typing; the test-only no-generator custom target may inspect only declarations
  that intentionally require an unadvertised thread/atomic capability, and that
  never counts as C support.
  Regenerate intentionally with `npm run snapshots:update -- --suite
  stdlib-ledger`; a conformant/partial row requires named executable tests, and
  a Haxe pin/surface drift must fail CI until its ownership is reviewed.
- Treat selected `hxrt` code as production performance-critical code. Keep each
  slice narrow, typed, warning-clean, allocation- and code-size-conscious, and
  free of general boxing/reflection when reachability or specialization can
  remove it. Hot helpers require owning benchmarks/profiles plus regression
  coverage for throughput/latency, allocations, and code size as applicable;
  never call an `hxrt` path optimized or fast without reproducible evidence.
- Runtime warnings identify deduplicated root semantic requirements at source
  spans. Transitive dependencies belong in `hxc.runtime-plan.json`, not as
  repetitive warnings. Every successful build, including an empty plan, records
  the resolved policy and its provenance.
- C-facing features use ordinary Haxe first, then typed `c.*` abstractions,
  validated metadata/macros, a narrow typed DSL only for a demonstrated language
  gap, and finally explicit raw C authority. Repeated raw snippets indicate a
  missing abstraction.
- Repository-owned program behavior is written in Haxe and compiled through
  haxe.c. This applies to examples, flagship applications, editor/game systems,
  test-only code that runs inside a generated executable, and reusable project
  libraries. An `extern` describes an ABI that already exists outside that Haxe
  program; it is not permission to implement a new repository feature in C and
  place a Haxe declaration in front of it. When ordinary Haxe exposes a missing
  lowering, standard-library adapter, runtime feature, diagnostic, or output-
  quality problem, reduce it to a general compiler fixture, create or update its
  Beads owner, fix the owning compiler/runtime layer, and keep the application
  path as end-to-end evidence. Do not bypass that flywheel with an example-
  specific C helper, application-owned shim, or raw call.
- Direct repository-owned C or C++ is an exceptional leaf boundary, not an
  alternative implementation language. Legitimate cases are limited to such
  things as ABI/layout probes, native consumer harnesses, generated-C fixtures,
  a narrowly selected `hxrt` or platform primitive that cannot soundly be
  expressed before the relevant Haxe capability exists, and adaptation of an
  external library whose existing ABI requires it. Every exception must be the
  smallest possible unit, have a named owner, state in a nearby comment why
  Haxe cannot currently express it, document ABI/ownership/failure and removal
  or permanence policy, and carry focused native plus generated-Haxe evidence.
  It must never duplicate game, application, compiler-policy, or scenario
  semantics. Host-side Python/shell orchestration may invoke compilers, inspect
  artifacts, and control processes, but it must not become a second
  implementation of behavior that belongs in the generated Haxe program.
- Do not require or generate redundant metadata when a safe canonical identity
  default expresses the exact same fact. Prefer the ordinary Haxe spelling or
  structure, resolve that default once at the typed boundary, validate it with
  the same rigor as an explicit value, and record the resolved fact in plans,
  locks, or reports. Emit metadata only for a real source/target difference,
  ambiguity, non-default policy, or unsafe authority. This rule removes
  boilerplate; it never permits guessing an ABI fact that needs Clang, a native
  probe, or an explicit ownership/lifetime decision.
- A new macro, metadata spelling, or DSL needs typed/inspectable inputs and
  outputs, source-positioned negative diagnostics, deterministic expansion
  evidence, explicit allocation/ownership/unsafe/runtime/portability effects,
  and a documented benefit that justifies its surface area. It must not bypass
  profile, runtime, environment, ABI, or raw-boundary policy.
- Use Haxe compile-time checks for declaration graphs, layouts, qualifiers,
  ownership, lifetimes, constants, and schemas where sound, but retain native
  authority: imported ABI facts come from Clang/probes and generated layout
  claims receive `_Static_assert` plus C/C++ consumer tests.
- Public C ABI types are separate from internal object/runtime layouts. Every
  exported pointer, string, buffer, handle, callback, error, and allocator
  boundary needs explicit layout, ownership, nullability, lifetime, calling
  convention, and failure policy. No exception may cross the C ABI.
- Preserve ADR 0004 string units: portable `String` is valid UTF-8 but every
  ordinary Haxe index is a Unicode-scalar index. Never expose byte offsets or
  UTF-16 surrogate halves through `String`, normalize implicitly, store malformed
  UTF-8, or conflate `String` with binary `Bytes` or NUL-terminated `c.CString`.
- The default tracing backend, when selected, is ADR 0005 precise non-moving
  mark-and-sweep with compiler-emitted exact roots. Do not treat arbitrary raw,
  interior, or foreign pointers as roots; require a typed live base/root or
  pinned handle, and keep collector details out of generated public types.
- Preserve ADR 0006 explicit exceptional and cleanup edges through HxcIR.
  Prefer proven result/status lowering; otherwise isolate exception frames and
  `setjmp`/`longjmp` in the selected runtime slice. Never transfer across an
  export, callback return, foreign frame, signal, or thread boundary, and never
  rely on indeterminate C locals after `longjmp`.

### Pinned toolchain and activation

- Treat `.haxerc`, `package.json`, `package-lock.json`,
  `docs/specs/toolchain-lock.json`, `haxe_libraries/*.hxml`, and
  `vendor/reflaxe` as one reviewed dependency contract. Never float Haxe,
  Reflaxe, or Lix versions and never refresh one pin without its checksums,
  provenance, notices, and lifecycle evidence.
- The production typing carrier is exactly Haxe `5.0.0-preview.1` at the
  revision and official artifact hashes in `toolchain-lock.json`. Its custom-
  target `PlatformConfig` API is preview surface. A pin change requires a source
  API audit, intentional platform/stdlib snapshot review, full package/server
  probes, provenance updates, and ADR review; do not broaden the version range.
- The reviewed Reflaxe bundle is compiler-only source. It must not appear in
  generated C, become an `hxrt` feature, or be described as a generated-program
  runtime dependency. Do not patch it silently; document target-required patches
  and include them in the bundle digest, or prefer a new upstream commit.
- `haxe_libraries/reflaxe.hxml` supplies only the vendored classpath and version.
  It must not start Reflaxe. `CompilerInit` owns the exactly-once
  `ReflectCompiler.Start()` and `AddCompiler(...)` sequence.
- Keep `extraParams.hxml` caller-CWD-independent: no relative `-cp`/`-p` entries,
  and `CompilerBootstrap.Start()` must precede `CompilerInit.Start()`. Scoped
  source-checkout paths use `${SCOPE_DIR}` and precedence-sensitive `_std` roots
  must exist on the initial classpath rather than being injected late.
- Production builds use `--custom-target c=<output>`. Haxe must invoke
  `c.Init.init()` and `TargetPlatform.configure()` before target facts are
  finalized. Validate a static, scalar-Unicode platform snapshot before
  registration; derive `target.sys` from the resolved environment and advertise
  no threading or atomics until the owning adapter has evidence.
- `c_output` is the internal Reflaxe output transport and legacy early detector.
  Derive it from `Compiler.getOutput()`; if explicitly supplied it must
  normalize to the same path. `c_output` alone, `-D c`, or a rewritten
  `target.name` is not a supported production carrier. On valid activation
  expose `c`, `reflaxe_c`, and `target.unicode`; reject `target.utf16` and do not
  invent other target capability defines.
- `reflaxe_c_lifecycle_probe` is an internal test seam that avoids entering the
  incomplete lowering boundary during E0 bootstrap tests. It must retain the
  real `CustomTarget(c)` platform and may skip only Reflaxe registration/
  lowering. Never document it as a user option or let it bypass a production
  compilation claim.
- Eval is only a target-neutral future `hxc` host, a differential oracle, and a
  non-C isolation test. It is never a user-program production carrier, and its
  host capabilities must never be copied into C manifests.
- Legacy Reflaxe `Cross` predefines `target.name=cross`, `target.utf16`, and
  `utf16`. Preserve its source-anchored `HXC0003` regression failure. Never
  relabel Cross, patch upstream conditionals ad hoc, maintain an implicit Haxe
  fork, or weaken the UTF-8 scalar contract to make bootstrap work.
- Keep `test/bootstrap/expected/target-contract.json` structural and
  path-stable. It must prove cold/package/server behavior, exactly-once counts,
  public versus internal defines, and the typed upstream
  `String`/`StringTools`/`UnicodeString` branch. A pin or carrier change requires
  an intentional snapshot and ADR review.
- Keep `test/all_sources/all_sources.hxml` and `test/all_sources/run.py`
  exhaustive. The runner automatically enumerates every repository-owned
  `src/**/*.hx` and `std/c/**/*.hx` module, checks explicit compiler-macro
  branches under Eval, and re-enters the real custom target to prove
  exact source-anchored `HXC1001` for its first unsupported call with no
  plausible artifact. The supported-body fixture separately proves the later
  `HXC1000` no-output boundary. New owned Haxe sources must stay visible to this
  gate; never replace its dynamic inventory with a frozen module count or treat
  its Eval pass as a production carrier.
- The package-layout probe may flatten the pinned framework into a temporary
  classpath to prove installed resolution. It is not release assembly or
  permission to publish while `haxe_c-od2.5` remains unresolved.
- The future `hxc` command is an optional orchestration layer. Bootstrap its
  target-neutral core on Eval, later dogfood the C target for the native binary,
  and retain direct Haxe/HXML plus the C toolchain as the recovery build path.
  Compiler registration must never depend on an existing `hxc` executable.

### Typed-AST input boundary

- `CReflaxeCompiler.filterTypes` is the complete typed-module capture boundary:
  it runs before Reflaxe callback filters. Preserve typedefs, externs,
  abstracts, secondary declarations, metadata, and module ownership there;
  never reconstruct whole-program input from class/enum callback order.
- Treat the pending/current module arrays as request-local handoff state.
  `onCompileStart` must create a fresh `CompilationContext`, consume and clear
  the pending capture, and `onCompileEnd` must clear current storage before
  invoking code that can fail. No capture or `TypedProgramInput` may survive a
  compiler-server request.
- Normalize unordered typed input by logical module/declaration coordinates
  while retaining raw Haxe compiler objects for semantic lowering. Preserve
  compiler/source order for field groups, enum constructors, metadata, and
  initializer roots with explicit ordinals; determinism must never erase enum
  indices or initialization/annotation order. Primary versus secondary
  ownership is explicit. Do not serialize raw
  positions, absolute roots, backslashes, timestamps, map iteration, or other
  host/process facts into an inventory.
- Reflaxe's exact process-local
  `@:build(reflaxe.ReflectCompiler.addToBuildCache())` hook is excluded from the
  normalized metadata view because it appears only after cache reuse. Do not
  generalize that exception to arbitrary `@:build` or source-authored metadata;
  the raw declaration remains untouched.
- `reflaxe_c_typed_ast_report` is an implementation-only unsupported-node
  inventory, and `reflaxe_c_test_reverse_typed_modules` is a test-only order
  seam. Neither is public application configuration. An intentional normalized
  input/report change updates `docs/typed-ast-input.md`, the fixture catalog,
  and `test/typed_ast/expected/`, then runs `npm run test:typed-ast`, the central
  snapshot check, and the exhaustive all-source gate.
- Typed-input collection is not itself Haxe-to-HxcIR lowering. The E2.T02
  parameter-free primitive body slice consumes retained raw `TypedExpr` values;
  the inventory report remains a pre-lowering view. Preserve exact `HXC1001`
  for the first unsupported body node and the later `HXC1000` no-output boundary
  until static-function, call, and executable entry-point emission is admitted.

### Diagnostic boundary

- `docs/specs/diagnostics.json` is the allocation and policy authority;
  `diagnostics.schema.json` and `diagnostic-event.schema.json` define its
  machine forms. `CDiagnosticId` must contain exactly the allocated IDs. Add
  the catalog entry, typed ID, tests, and affected docs together; never reuse a
  retired number or allocate from a reserved range.
- Production Haxe code emits through `CDiagnostic` with a typed ID. Do not add
  raw `"HXCdddd: ..."` prefixes outside `CDiagnostic.hx`. Exception/report
  models use `CDiagnostic.codeMessage` and retain the typed ID rather than
  inventing local constants.
- Every emitted diagnostic has an allowed severity, a primary Haxe source
  range, a profile, and remediation. Structured paths are normalized
  project/repository-relative UTF-8 with `/`; never serialize an absolute
  checkout path. `unresolved` is a profile only when configuration failed
  before profile selection.
- Tests normally assert the stable ID plus essential structured/semantic
  fields, not complete English prose. Use a full golden only when formatting is
  itself under test. `HXC1001` is unsupported source, `HXC9000` is an internal
  compiler invariant, and `HXC1000` is the deliberate compiler-capability
  boundary; never collapse those failure families.
- `CDiagnosticRecord` is the deterministic single-record JSON core. E8.T09
  owns public stream framing, native-tool attachments, command context, and
  CLI stdout/stderr behavior. A diagnostic contract change runs
  `npm run test:diagnostics` and the exhaustive all-source gate.

### HxcIR semantic boundary

- For the E2.T02 body slice, lower real parameter-free typed function bodies as
  `TypedExpr -> HxcIR -> HxcIRValidator -> structural CAST`; never emit C first
  and reconstruct semantics afterward. Admit only typed primitive constants,
  initialized locals/reads, cleanup-free nested blocks, and validated returns.
  Function/local names still go through the complete `CSymbolRegistry` batch,
  and optional source mapping uses structured line-directive nodes.
- Treat HxcIR instruction arrays as semantic evaluation order. Never reorder
  them or fuse side-effecting instructions into a C expression whose operand or
  argument order is weaker. Canonicalization may sort only collections that the
  model declares unordered.
- Immutable value IDs are function-wide unique but block-local in availability:
  an instruction may use function parameters, its block parameters, and earlier
  results in that block. Pass cross-block data through typed block arguments;
  never reach into another block's temporary namespace.
- Places are mutable addressable storage, not values. Preserve structural local,
  global, field, index, and dereference places through load/store/address and
  lifetime operations; do not encode an lvalue as a value string or C fragment.
- Every block terminates. Calls and allocations carry explicit failure edges;
  successful instruction continuation is ordered locally. Throw, result,
  allocation, and native-status failures must remain distinguishable until a
  validated exception/result strategy pass chooses C control flow.
- Cleanup actions are stored in registration order. Edge cleanup steps are the
  actual execution order: reverse within a region, then direct parent regions
  from inner to outer. Do not infer cleanup later from C lexical nesting, and do
  not put dereference/index temporaries in cleanup actions—materialize a stable
  local place first.
- Runtime intent is explicit: `IRIStatic`, then `IRIProgramLocal`, then a named
  `IRIRuntime` feature. A runtime call also names its feature and operation.
  Never turn an allocation, `Dynamic` type, failure edge, or cleanup region into
  an implicit `hxrt` request.
- Every semantic node carries a normalized repository-relative source span.
  Reject and redact absolute/backslash/dot-segment paths before dumping. Dumps
  retain ordered fields and use UTF-8 sorting only for unordered collections;
  do not add timestamps, locale formatting, absolute roots, or map iteration.
- `IRTDynamic` means the real Haxe `Dynamic` semantic type; it is not an opaque
  unsupported-node escape. Frontend lowering must emit source-positioned
  `HXC1001` and stop that path. Internal invalid IR uses `HXC9000`; never repair
  either by inventing a constant, null, raw C, or placeholder value.
- Preserve ADR 0008 primitive semantics structurally. Ordinary portable/metal
  Haxe primitives share exact mappings; target-ABI integers retain their
  `size_t`/`ptrdiff_t`/`intptr_t`/`uintptr_t` identities and must never be
  guessed from host `int` or `long`. Keep exact, wrapping, checked, saturating,
  nullable-inject, and nullable-unwrap conversions distinct; checked/unwrap
  operations retain failure edges. Avoid implementation-defined signed casts
  and undefined floating-to-integer casts. Primitive mapping/conversion uses
  direct C or a program-local helper and must not select `hxrt`.
- An intentional HxcIR model, validation, dump, or diagnostic change updates
  `docs/hxc-ir.md` and the reviewable files under `test/hxc_ir/expected/`, then
  runs `npm run test:hxc-ir` and the exhaustive all-source gate. HxcIR-only
  fixtures prove pre-emission semantics, not generated C or runtime selection.

### Typed C contract and declaration planning

- Files under `std/c/` are compiler contracts, not implemented wrapper objects.
  Importing or mentioning them selects no runtime feature. Do not add fallback
  allocations or ordinary runtime method bodies to make an unsupported operation
  appear usable; implement the owning typed lowering and runtime-plan evidence.
- Canonical declaration metadata is namespaced: `@:c.layout`, `@:c.header`,
  `@:c.name`, `@:c.include`, `@:c.link`, `@:c.define`, `@:c.pkgConfig`,
  `@:c.framework`, `@:c.pack`, `@:c.align`, `@:c.bitField`, `@:c.linkage`,
  `@:c.callingConvention`, `@:c.visibility`, `@:c.section`, `@:c.export`, and
  `@:c.constant`. Unknown `@:c.*` spellings fail with `HXC5002`; never add an
  alias without updating ADR 0002, the PRD, typed-C guide, validator, snapshot,
  and negative tests together.
- `TypedCContractMacro` rebuilds a structural snapshot from the current typed
  module set. It must remain deterministic and per-compilation, must not consult
  ambient process state, and must not write files. Later compiler stages route
  snapshot-derived artifacts through Reflaxe output ownership.
- Register every emitted name through the per-compilation `CSymbolRegistry` and
  finalize only after collecting the complete semantic request set. Stable keys
  contain qualified Haxe identity, normalized overload/specialization facts,
  real C namespace/scope, and source ordinals—never filesystem paths, discovery
  counters, object addresses, or map order. Preserve source display identities
  separately from semantic keys: collision-free generated C should use
  recognizable source words, while full structural digests stay in reports.
  Add compact hashes only for real namespace collisions, anonymous identity, or
  length limits; never expose encoded compiler roles merely because they are
  convenient internally. Keep `hxc-c-symbol-v2` changes
  explicit and update `docs/symbol-naming.md` plus the owned snapshot together.
- Exact `@:c.name` values are ABI/interop facts: preserve them byte-for-byte or
  reject them with `HXC5002`; never silently sanitize or hash them. Generated
  translation-unit internal/public defaults use the registry's `hxc_` and
  `hxc_api_` namespaces. Generated locals and aggregate members also keep the
  short `hxc_` ownership prefix while preserving the source word, for example
  `value` becomes `hxc_value`: C preprocessing happens before C scope lookup,
  so a bare local/member spelling could still be replaced by a macro from an
  included header. Do not trade that collision-proof boundary for bare names.
  A header-owned `extern` declaration without `@:c.name` uses its exact Haxe
  declaration/field spelling as a validated external identity default; add
  `@:c.name` only when the native spelling differs. `hxc_` and `hxrt_` remain
  unavailable to authored or inferred external names, and public C/C++ spellings
  may not begin with underscore or contain double underscore.
- Do not add metadata that merely repeats a safe compiler default. If a Haxe
  declaration and its C declaration are both named `position`, write that fact
  once and let the binding/compiler preserve it. Use `@:c.name` or another
  override only to express a real semantic or ABI difference, and improve the
  general inference/generator when repeated redundant annotations appear.
- Respect C namespaces. Ordinary identifiers, tags, per-aggregate members, and
  per-function labels do not share one flat collision map. A generated collision
  is resolved from canonical semantic identity and recorded with both source and
  C names; an exact collision is an error naming every source symbol.
- `TypedCNameFinalizer` is the structural schema-2 handoff to declaration
  planning. Preserve layout-significant field order and the snapshot's runtime,
  allocation, ownership, unsafe, and portability effects. Naming selects no
  runtime feature; a synthetic runtime-private role in the naming golden is not
  permission to register or link `hxrt` without runtime-plan evidence.
- Typed C contract schema 2 retains sorted `ownerModulePaths` on every merged
  build fact. Do not discard that provenance: declaration-owned includes are
  what let the planner keep generated headers minimal and explain why each
  include exists.
- `CDeclarationPlanner` is a pure structural analysis. Complete-type edges order
  declarations or select the owning header; pointer-only edges select typed
  forward declarations; authoritative includes on headerless opaque imports are
  propagated instead of guessed declarations. Never turn a forward edge into a
  convenience include or rebuild these decisions from C strings in an emitter.
- Public headers may forward-declare a type whose definition is private, but
  must not include a private header for a complete type. Header guards are the
  deterministic, collision-free path encoding from the plan; users never author
  or override them as declaration strings.
- `test/declaration_plan/` is a test-only AST adapter over the planner, not the
  production multi-file emitter. An intentional plan or header change requires
  `npm run test:declaration-plan` and the strict native matrix; preserve the
  independently compilable, repeated-include, pointer-cycle, and no-`hxrt`
  evidence.
- `reflaxe_c_contract_report` is an implementation-only inspection/test seam.
  Do not document it as an application option or confuse its empty M0 effect
  list with a whole-program `hxc.runtime-plan.json` proof.
- `c.StaticAssert.require` proves only literal Haxe compile-time facts. Layout,
  size, offset, alignment, toolchain, and imported-header claims still require
  generated `_Static_assert`, Clang/native probes, and independent consumers.
- `c.Syntax` and `c.Unsafe` intentionally expose no operation. Adding raw or
  unsafe authority requires the owning Beads issue, a searchable lexical/API
  boundary, inspection output, strict-mode enforcement, and positive/negative
  no-hidden-runtime evidence.
- The source vocabulary is ratified but its generated public ABI is not stable.
  E7 owns ABI-safe export/header implementation and E10.T09 owns stabilization;
  do not publish compatibility claims from the M0 snapshot.

### Fixture taxonomy and snapshot ownership

- Treat `docs/specs/fixture-taxonomy.json` as the machine-readable registry and
  `docs/testing.md` as its human contract. New cases belong in one of the eight
  canonical lanes under
  `test/{positive,negative,ast,snapshot,runtime,differential,abi,performance}/`
  and require a schema-valid `case.json`. Existing focused M0 suites remain in
  place only because the registry maps their runners and evidence explicitly.
- Every case declares its inputs, argument-array runner, working directory,
  timeout, network policy, exact exit code, and at least one semantic assertion.
  Do not infer success from a zero exit alone, silently skip required tools, or
  normalize output without naming the field and semantic reason.
- Checked-in expected artifacts are owned by the central snapshot registry.
  Check them with `npm run snapshots:check`; regenerate intentionally with
  `npm run snapshots:update -- --suite <suite-id>` (or the visibly broader
  `--all`). The updater must render twice, print semantic diffs before writing,
  stay inside catalog-owned roots, and run the owning suite afterward. CI never
  updates snapshots. Do not hand-edit expected files to bless a regression.
- A snapshot is evidence for the artifact it asserts, not for a neighboring
  layer. Direct HxcIR/C AST and planned-header fixtures do not prove
  Haxe-to-C lowering; independent native C/C++ fixtures do not prove generated
  runtime or ABI behavior. Preserve `seed-only` and `contract-only` labels until
  the corresponding generated-Haxe, sanitizer, differential, consumer, or
  benchmark evidence actually exists.
- Examples are explicit product proofs, never an implicit test glob. Every
  `examples/<name>/` requires a `case.json` with `role: "example"` and
  declared assertions before a runner may execute it. Put reusable compiler
  regressions in a focused fixture when that gives a smaller diagnostic proof.
- When adding a runner or expected tree, update the taxonomy, snapshot registry,
  package entry points, hook/CI guards, and testing guide together. The fixture
  policy guard must retain complete ownership of every checked-in `expected`
  file so stale or orphaned goldens cannot pass unnoticed.

### Product contracts

- One compiler pipeline serves both profiles. `portable` is the default Haxe
  semantic contract; `metal` exposes explicit C layout/ownership constraints.
- Runtime policy (`auto|minimal|none`) and environment
  (`hosted|freestanding|wasi|emscripten`) are orthogonal to profile.
- With no explicit runtime override, `portable` resolves to `auto + summary` and
  `metal` to `minimal + warn`; explicit valid combinations still win. Use
  `hxc_runtime=none` for the hard whole-program no-runtime proof.
- Strict ISO C11 with no extensions is the normative generated-source, runtime,
  fixture, and public-header floor. C17 preserves that contract; C23 internal
  syntax remains experimental and cannot silently alter semantics or ABI.
- `scripts/ci/runtime_smoke.py` owns the native warning baseline and must
  compile/run both structural C AST goldens, independently compile every
  declaration-plan header, run their combined consumer, compile the
  hosted/freestanding runtime paths, consume the runtime public header from
  C++17, and run the C-library and opaque-handle C++ shim fixtures. Auto mode
  may skip an unavailable optional compiler family only with an explicit `SKIP`
  reason and must run at least one complete C/C++ pair. CI passes `--toolchain
  gcc` and `--toolchain clang` in separate required lanes; either missing,
  mislabeled, unrun, warning-producing, or failing lane is a hard failure.
- Direct Haxe/Reflaxe builds activate through
  `--custom-target c=<directory>` and expose the public `c` target conditional.
  The compiler owns `target.name=c`; use `target.unicode` without
  `target.utf16`, and set `target.sys`,
  `target.threaded`, or `target.atomics` only from proven adapter capabilities.
  Do not use `reflaxe_c` as the recommended application portability condition.
- A platform support claim names the whole environment/runtime/architecture/
  compiler/capability tuple. Do not report a cross-compile as a native or
  emulated run, a capability-limited freestanding lane as hosted stdlib support,
  or a Tier 2/3 combination as part of the ADR 0007 Tier 1 release gate.
- Idiomatic, warning-clean C is required in every profile; it is not a third
  profile or a correctness-off mode.
- Clang-derived facts and compiled probes are authoritative for bindgen and
  ABI layout. AI suggestions may improve names/docs but never invent ABI facts.
- Arbitrary C++ APIs require an existing or generated `extern "C"` shim. Do
  not represent C++ classes, templates, overloads, exceptions, or ABI details
  as if they were ordinary C declarations.

### License and provenance invariants

- Repository-owned source is `GPL-3.0-only`. Treat
  `docs/specs/third-party-provenance.json` as the machine-readable inventory
  and `THIRD_PARTY_NOTICES.md` as its human-readable explanation; update both
  in the same change whenever distribution facts change.
- An external compiler, framework, or analysis tool is not redistributed merely
  because it is used during development. Conversely, never label copied source
  as “external only.” Distribution status must match the checkout and package.
- Do not add files under `vendor/`, `third_party/`, `runtime/vendor/`, or
  `runtime/third_party/` without an owning manifest entry, immutable source
  revision, concrete SPDX license expression, exact governed paths, and local
  license evidence. `runtimeDependencies: []` is an audited statement that no
  third-party runtime dependency is present.
- Every file under `std/c/_std/` must have a derived-source ledger entry with a
  content hash. Copied or adapted Haxe standard-library source records its exact
  upstream path/revision, MIT license, and retained notice; a genuinely
  repository-authored override is classified separately as `GPL-3.0-only`.
- Run `python3 scripts/ci/check_license_policy.py` after any dependency,
  vendoring, `_std`, runtime, license, notice, or package-layout change. Release
  assembly must additionally run it with `--package-root <assembled-tree>` so
  reviewed license and notice files are present byte-for-byte.
- Do not infer or promise a license for generated C, generated headers,
  templates, or selectively emitted/linked runtime slices from the repository
  license. That public treatment is owned by Beads decision `haxe_c-od2.5` and
  remains a release-packaging blocker until ratified.

### Contribution, security, and release governance

- `CONTRIBUTING.md` is the human workflow contract and this file is the agent
  contract. Keep their Beads commands, definition of done, generated-file
  ownership, vendoring/provenance rules, and close/push sequence aligned; run
  `python3 scripts/ci/check_governance_policy.py` after changing either.
- Security reports follow `SECURITY.md`. Never copy embargoed details into a
  GitHub issue, pull request, discussion, Beads record, CI output, snapshot, or
  generated artifact. Beads may track a neutral remediation task only after
  sensitive reproduction and reporter data have been removed.
- The repository is public and GitHub private vulnerability reporting is
  enabled and verified at
  `https://github.com/fullofcaffeine/reflaxe.c/security/advisories/new`;
  `boss@fullofcaffeine.com` remains the confidential fallback. GitHub secret
  scanning, push protection, Dependabot vulnerability alerts/security updates,
  and read-only default Actions permissions are also verified. Do not weaken or
  overstate those settings; optional validity/non-provider scanning is not part
  of the current claim.
- There is no supported release today. E10.T08 owns signed reproducible release
  automation and E10.T11 owns the beta gate. Development archives, native
  fixtures, package-layout probes, and generated snapshots are not releases.
- Release roles are explicit: the release manager owns version/tag/notes/gates
  and publication; artifact builders own reproducibility, checksums, and
  source/runtime/tool manifests; the provenance reviewer owns hashes, licenses,
  notices, and generated/runtime treatment; an owner-authorized signer protects
  private key material and records the public identity and verification command.
  A missing configured signature blocks publication, and an unconfigured
  signature must never be described as present.
- Do not publish packages, create release tags, rotate signing keys, or upload
  provenance without an owning release issue and explicit maintainer authority.
- Maintainers push the implementation and require remote Governance success,
  then close the Bead with evidence, run `npm run beads:push`,
  commit/push the passive export and interaction record, and verify a clean
  `0 0` Git state. A
  contributor without publication authority leaves final closure and remote
  synchronization to a maintainer.

### Beads plan execution

- The durable product source is `docs/PRD.md`; live ownership/readiness/status
  belongs only in Beads. Do not maintain a parallel Markdown task checklist.
- Seed issues retain `E0`...`E10` / `E<n>.T<nn>` in metadata and use
  `hxc-plan:<stable-key>` as the external reference. Search/reconcile these
  before creating plan work again.
- Parent-child hierarchy is structural and does not block siblings. A hard
  edge is always `child needs blocker`: `bd dep add <child> <blocker>`.
- With the installed Beads version, `bd ready` also lists unblocked epics.
  Use `bd list --ready --type task --json` to select actionable seed work.
  The intended first seed task is `E0.T01`.
- Claim one primary actionable task. Read its parent, blockers, requirement
  IDs, PRD references, acceptance criteria, compatibility impact, and
  documentation obligations before editing.
- Record discoveries with a specific parent and
  `--deps discovered-from:<source-id>`. Add a blocking edge only when the
  discovery genuinely prevents acceptance.
- Close an implementation issue only after positive/negative fixtures,
  IR/AST or snapshot evidence, strict native compilation, runtime/differential
  behavior, policy/runtime-plan assertions, sanitizer/ABI checks where
  relevant, and documentation/ledger updates required by PRD Section 40.
- Close reasons must name requirement IDs, exact commands, and produced
  artifacts. “Implemented” or “tests pass” alone is not sufficient evidence.
- Never use `bd init --force`, hand-edit `.beads`/Dolt internals, or treat
  `.beads/issues.jsonl` as the live source of truth.
- Do not use `bd import` for normal synchronization. Use `bd dolt pull/push`
  only when a Dolt remote is actually configured and policy authorizes it.

#### Reviewed Beads 1.1.0 client and schema

- The shared database is on schema v53 and requires Beads 1.1.0 from revision
  `8e4e59d39`. Resolve it through `scripts/beads/resolve-reviewed.py`; do not
  trust whichever `bd` happens to appear first on `PATH`.
- Hooks and guarded publication must verify the reviewed client before any
  Beads operation. A failure must leave `.beads/issues.jsonl`, the Git index,
  the Dolt working set, and reusable test receipts unchanged.
- The one authorized v32-to-v53 migration was completed and published on
  2026-07-21. Other clones must adopt that remote database with `bd bootstrap`
  after preserving any unpushed local issue work. Never set
  `BD_ALLOW_REMOTE_MIGRATE` or independently migrate another clone.
- A future schema change is a coordinated single-migrator operation: capture a
  complete backup and semantic export, migrate once, validate issue and graph
  parity, then publish immediately through `npm run beads:push`.
- `bd preflight` prints the Beads tool repository's checks, not this Haxe/C
  project's quality gates. Do not report it as project validation.

The plain-language workflow, rationale, and recovery steps are in
[`docs/beads-toolchain.md`](docs/beads-toolchain.md).

### Validation and evidence

Run the exact gates owned by the active issue. At the present partial-scaffold
stage, the locally available baseline is:

```bash
npm ci
npm test
npm run test:governance-policy
npm run test:typed-boundaries
npm run public:preflight
python3 scripts/beads/validate_plan.py --json
python3 scripts/beads/bootstrap.py --json
npm run test:fixture-policy
npm run snapshots:check
jq empty docs/specs/*.json .haxerc haxelib.json package.json package-lock.json
bd dep cycles --json
bd lint --json
bash scripts/lint/whitespace_guard.sh
```

`npm test` verifies the exact dependency lock, vendored Reflaxe checksum, the
complete current target-owned Haxe graph and explicit macro branches,
source/package lifecycle behavior, cold/compiler-server C/non-C isolation,
macro order, typed-AST normalization/order/server isolation, the structural
declarator and expression/statement goldens, the
declaration/include plan and header goldens, the canonical HxcIR semantic and
negative-diagnostic goldens, the available local strict native smoke lanes,
registered snapshot reproduction, fixture/example ownership, notices, and
governance policy.
GitHub CI additionally requires distinct GCC/G++ and Clang/Clang++ lanes. The
AST and declaration-header goldens are generated by Haxe code that directly
constructs target-owned structures; they prove C syntax and dependency
planning, not Haxe-language lowering or production generated-program success.

After cloning, run `scripts/hooks/install.sh`. The tracked pre-commit chain
keeps `.beads/hooks` as `core.hooksPath` so Beads checkout/merge/push hooks
remain active while repository checks run first. It exports/stages Beads JSONL,
formats repository-owned staged Haxe, preserves immutable vendor bytes, rejects
staged secrets and machine-local paths, checks whitespace, verifies dependency
checksums, enforces the license/provenance and fixture/snapshot inventories, and
runs relevant JSON, Haxe, or strict native gates. Gitleaks is required; the
exact formatter haxelib is required when repository-owned Haxe files are
staged. The tracked pre-push hook fetches the non-branch `refs/dolt/data` ref,
scans every reachable Git revision with the same narrow Gitleaks policy, and
revalidates security-tool/workflow pins. Because Dolt rows are opaque Git
chunks, `scripts/beads/push-safe.sh` separately scans decoded current records
and every historical issue version before synchronizing Beads. Never run `bd
dolt push` directly. Public CI checks all repository-owned Haxe formatting,
installs Gitleaks only after a reviewed SHA-256 match, fetches the Dolt ref,
scans full history from a depth-zero checkout, and pins every external GitHub
Action to a full reviewed commit. Run `npm run public:preflight` before any
visibility/publication change. Never broaden a
secret-scan allowlist to a whole generated file when a path-and-match rule can
identify deterministic non-secret bytes. Do not bypass a hook to publish a
failing change; record and fix the underlying gate instead.

Every tracked hook first selects the reviewed Beads 1.1.0 client. This happens
outside Beads' marker-managed hook section, so `bd hooks install --beads` may
upgrade its own integration without deleting the repository guard. The passive
export writes temporary files beside `.beads/issues.jsonl` and stages the final
file only after a complete successful export.

Claim Haxe/Reflaxe source-graph type-checking only with the dedicated all-source
gate. Do not claim generated-program runtime linking/execution, sanitizers,
cross-platform CI, bindgen, export, or stdlib parity until the responsible issue
adds a real command and that command passes.

### Git and remote completion

Preserve unrelated user changes and untracked scaffold files. Stage and commit
only the paths in the active issue unless the user explicitly broadens scope.

Before session close, inspect `git remote -v`:

- If a Git remote and the corresponding Beads remote are configured and the
  workflow authorizes publishing, rebase safely, push Git, push Beads with
  `npm run beads:push`, and verify both.
- If no remote is configured, do not invent one and do not claim a push. Commit
  the in-scope tracked changes locally, verify the worktree/Beads state, and
  state explicitly in the handoff that synchronization was unavailable.

<!-- BEGIN BEADS INTEGRATION v:1 profile:minimal hash:7510c1e2 -->
## Beads Issue Tracker

This project uses **bd (beads)** for issue tracking. Run `bd prime` to see full workflow context and commands.

### Quick Reference

```bash
bd ready              # Find available work
bd show <id>          # View issue details
bd update <id> --claim  # Claim work
bd close <id>         # Complete work
```

### Rules

- Use `bd` for ALL task tracking — do NOT use TodoWrite, TaskCreate, or markdown TODO lists
- Run `bd prime` for detailed command reference and session close protocol
- Use `bd remember` for persistent knowledge — do NOT use MEMORY.md files

**Architecture in one line:** issues live in a local Dolt DB; sync uses `refs/dolt/data` on your git remote; `.beads/issues.jsonl` is a passive export. See https://github.com/gastownhall/beads/blob/main/docs/SYNC_CONCEPTS.md for details and anti-patterns.

## Session Completion

**When ending a work session**, complete all applicable steps below. A push is
mandatory only when the relevant remote is configured and publishing is
authorized; otherwise follow the repository-specific remote rule above.

**MANDATORY WORKFLOW:**

1. **File issues for remaining work** - Create issues for anything that needs follow-up
2. **Run quality gates** (if code changed) - Tests, linters, builds
3. **Update issue status** - Close finished work, update in-progress items
4. **PUSH TO REMOTE WHEN CONFIGURED AND AUTHORIZED**:
   ```bash
   git pull --rebase
   git push
   git status  # MUST show "up to date with origin"
   ```
5. **Clean up** - Clear stashes, prune remote branches
6. **Verify** - All changes committed and, when a remote is configured, pushed
7. **Hand off** - Provide context for next session

**CRITICAL RULES:**
- When a configured workflow requires a push, work is not complete until it
  succeeds; resolve failures and retry.
- When no remote exists, do not create one or imply synchronization occurred.
- Never say "ready to push when you are" when the existing workflow already
  authorizes and requires the agent to push.
<!-- END BEADS INTEGRATION -->
