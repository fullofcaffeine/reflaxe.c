# Test feedback-loop and CI performance

Developer waiting time is an engineering constraint. This document turns the
repository directive in [`AGENTS.md`](../AGENTS.md) into an executable lane
design without weakening the evidence contract in [`testing.md`](testing.md).

## AI-agent test loop

The practical outcome is simple: an agent should get a useful failure quickly,
then increase evidence once at a review boundary. It should not run the full
repository matrix after every edit.

Use the changed-path planner before choosing commands:

```sh
# Plan from the files staged for the next commit. This runs no tests.
git diff --cached --name-only | npm run --silent test:plan

# Machine-readable form for an agent or editor integration.
git diff --cached --name-only |
  npm run --silent test:plan -- --json
```

The planner is explanatory, not a cache and not a test result. It reuses the
conservative route and central-owner rules used by the pre-commit hook, derives
direct test-directory owners from their package scripts, attaches each owner to
the product surfaces it may advance, and reports six rings:

| Ring | When an agent uses it | Evidence |
| --- | --- | --- |
| R0 — focused owner | Repeatedly while changing one behavior | The owning issue's smallest package script, plus a regression that fails for the observed defect. When the planner cannot infer an owner, the agent must choose the nearest existing package script; an empty mapping is not permission to skip tests. |
| R1 — local commit smoke | Once after the focused owner passes and before committing or handing off a central change | Governance plus `test:all-sources`, `test:hxc-ir`, `test:hello`, and `snapshots:catalog`. The installed pre-commit hook runs this bounded set for the affected route. |
| R2 — hosted pull-request gate | After the commit is ready for independent verification | The required `Governance` workflow: all four complete toolchain shards plus independent native, build-adapter, platform, provenance, and security jobs. |
| R3 — affected extended | For a high-risk or product-surface change after its primary owner passes | Secondary compilers, sanitizers, platform profiles, and downstream owners from the affected scorecards. Selection is explanatory today; no evidence has been removed from R2 while selector recall is established. |
| R4 — full/main/nightly backstop | On main/nightly, explicit investigation, or any unknown/ambiguous semantic owner | The complete active toolchain partition plus native smoke through `npm run test:toolchain:parallel -- --with-native`, hosted `Governance`, selector-miss review, and independent cold `snapshots:check` on its path/schedule. |
| R5 — release qualification | For a release candidate after support and release owners land | Clean package/install, every claimed ADR 0007 tuple, security, reproducibility, compatibility, and archived provenance. This command is intentionally marked unimplemented; E10.T07, E10.T08, E10.T11, and E10.T12 prevent a false release pass. |

R3 selects by semantic owner and product surface, not by changed path alone.
Paths are only the transparent input used to find those owners. Every plan
prints the inferred surfaces; policy-only owners print an empty surface set and
therefore advance no product claim. The planner loads both the surface map and
the concrete R3 package commands directly from the checked scorecards; script
names are not a claim heuristic. Unknown or ambiguous ownership marks R4
`required` and prints the reason. The complete R2 matrix remains required while
affected selection is in observation, so this update moves no evidence out of
the pull-request critical path. The full R4 run remains the backstop that can
expose a selector miss.

An agent should not wait twice for the same expensive test against the same
checkout. For example, when it runs `test:hxc-ir` while finishing a change, the
pre-commit hook may accept that exact passing run instead of immediately
running `test:hxc-ir` again. The planner therefore routes an R0 owner through
`test:local-gate` when it is also one of the expensive R1 sentinels. The initial
reviewed set is `test:all-sources`, `test:hxc-ir`, and the Haxe-only
`test:project-emitter`; native, repository-wide, and nonduplicated owners
remain ordinary direct commands.
`test:governance` remains cold because its broad scanners and transitive tool
inputs do not yet have a cheap complete evidence key. `test:hello` remains
cold. Project-emitter semantics and native build-adapter execution are separate
owners: the former can be keyed exactly from Haxe inputs, while
`test:build-adapters:local` always executes the available native consumers
because its selected compiler, system development kit, linker, and build-system
dependency closure is not yet cheap to hash completely. Hosted CI uses the
stricter `test:build-adapters` owner, which requires every declared tool.

The wrapper runs the named owner normally. After a stable pass it writes a
short-lived local *receipt* under `.cache/local-gates/`: a small record saying
which test passed against which exact inputs. At R1 the hook may reuse that
receipt only for the same owner and the same staged tree, unstaged and relevant
untracked inputs, every file in that owner's Haxe classpath trees (including
ignored files), package command, dependency locks, tool binaries, environment,
host, hook, and evidence-runner implementation. The receipt records work
already done; it is not a claim that the test or a broader CI lane is
unnecessary.

Git and the reviewed Beads hook add transport-only environment values while
starting pre-commit. The wrapper removes those values from both the direct and
hook test processes: the reviewed `bd` path entry (even when npm places its
own `node_modules/.bin` entry before it), Git's normal index,
commit-author fields, and the noninteractive `GIT_EDITOR=:` value. A partial
commit's temporary index is not normalized, and every remaining environment
difference still changes the receipt key. Reusable owners also run with
`PYTHONDONTWRITEBYTECODE=1`, so importing a repository test helper cannot
create an ignored `__pycache__` input halfway through its own evidence run.

To make the direct run reusable, stage the intended task boundary first, then
run the planner's `test:local-gate` command, and finally commit without changing
those inputs. A test run from before `git add` remains valid task evidence, but
the hook deliberately runs it again because moving bytes between the unstaged
worktree and the staged tree changes the exact key.

A missing, expired, corrupt, incomplete, or mismatched receipt is a visible
cache miss and runs the owner. A failing command or a command whose inputs
change while it runs publishes nothing. One operating-system lock serializes
each owner's read, run, and publish transaction, so simultaneous agents cannot
race a pass against a failure or publish out of order. Performance/timing owners are never reused.
`CI` and the explicit cold form always execute:

```sh
npm run --silent test:local-gate -- --cold test:hxc-ir
```

The hook reuses only an exact duplicate R0/R1 owner. It does not let one focused
test stand in for another owner, a full shard, native/platform evidence, or the
hosted required check.

One July 30, 2026 diagnostic sample on the declared local worktree measured the
split project owners as follows. This is a before/after observation, not a
percentile budget:

| Operation | Wall time |
| --- | ---: |
| Cold `test:project-emitter` semantic owner | 434.73s |
| Exact hook receipt validation for that owner | 1.29s |
| Always-cold local native build-adapter owner | 58.28s |

The receipt removed only the immediate 434-second duplicate. The independent
native owner still compiled and executed three generated projects through the
raw manifest and CMake; Meson was unavailable locally and remains mandatory in
the hosted `test:build-adapters` job.

For an AI-agent loop, “focused owner passes” is the normal signal to continue
implementation. “R1 passes” is the normal signal to create the task commit.
R2 through R5 are independent hosted or qualification evidence: an agent
reports them as pending unless it has an actual completed workflow result. It
must not report an unrun remote lane as green, and it must not block its next
local edit merely to replay the same full matrix on the developer machine.

The complete local commands remain available for diagnosis:
`npm run test:toolchain:shard -- <name>` runs one hosted-equivalent shard,
`npm run test:toolchain:parallel` runs all four with bounded workers, and
`npm run test:toolchain` is the canonical serial reference. These are explicit
tools, not automatic per-commit work.

This ring model separates two different questions:

- **portable Haxe semantics:** did ordinary Haxe source compile through haxe.c,
  build as C, and execute with the expected behavior?
- **native/metal correctness:** did the low-level target-facing path (the
  generated C and its direct native boundaries), runtime slices, strict C11
  compilers, optimizations, and sanitizers satisfy their independent contracts?

One axis cannot borrow a pass from the other. Eval is a reference result, not a
haxe.c runtime pass; a warning-clean C snapshot is not proof that the program
executed; and a native C adapter test does not prove broad ordinary-Haxe
compatibility.

## Current evidence and qualification gaps

The exact production baseline is Haxe `5.0.0-preview.1` at official source
revision `2c1e544e0a2c7524ef4c8e103f1b0580362ea538`. The older Haxe 4.3.7
taxonomy in the cross-repository review is useful precedent only; it is not
this target's qualification baseline.

The statuses below describe the current checkout, not the intended 1.0
destination:

| Area | Evidence status | Current result |
| --- | --- | --- |
| Compiler and target baseline | **Observed** | `.haxerc`, `toolchain-lock.json`, bootstrap tests, and ADR 0007 pin the exact Haxe preview, Reflaxe revision, target identity, and release artifact hashes. |
| Official Haxe source inventory | **Observed locally; not repository-owned** | The exact commit object available in the neighboring Haxe checkout contains 69 `unitstd` specifications, 1,167 general issue files, and 8 `hxcpp_issues` files. A neighboring checkout is convenient review evidence, not a reproducible project input. |
| Active official test inventory | **Unknown / unimplemented** | No haxe.c-owned adapter records which Haxe 5 tests and assertions remain active under the C target's defines. File counts do not count as passes. |
| Official-suite adapter self-test | **Unknown / unimplemented** | There is no deliberate failing official-harness case proving that compile, C-build, assertion, crash, timeout, and empty-test failures all propagate. |
| Official Haxe smoke | **Unknown / unimplemented** | No published top-level + `unitstd` + issue subset currently travels through custom-target compilation, strict C build, and target-runtime execution as one official-source lane. |
| Repository-owned portable semantics | **Observed, bounded** | Focused generated-Haxe differential suites and `test:hello` compile and execute the currently admitted language/runtime slices. They do not imply the complete applicable official suite. |
| Native/metal evidence | **Observed, bounded** | GCC/Clang strict C11, O0/O2, sanitizer, C/C++ consumer, runtime-policy, and platform lanes exist for their named fixtures and profiles. |
| Package-installed consumer path | **Observed for a staged development package; release path incomplete** | Bootstrap tests copy the reviewed package surface into a temporary consumer and compile from another working directory. A published release artifact and release-install qualification remain future evidence. |
| Local feedback-loop budgets | **Inferred from limited samples** | Focused and bounded-smoke responsibilities are implemented, but the stated percentile budgets are not yet backed by enough comparable samples to become hard gates. |
| Compatibility claim | **Observed as intentionally bounded** | The repository remains M0/developer-preview evidence. It does not claim “Haxe compatible,” the complete official `tests/unit` contract, full standard-library parity, or production readiness. |

Beads issue `haxe_c-6k7` owns the official Haxe test integration and parity
dashboard. Its first honest increment is baseline/source provenance, complete
active inventory, harness self-test, and a tiny real official-source
compile → strict-C-build → runtime smoke. Until those artifacts exist, the
planner reports `readiness-only-not-a-pass`; expected failures remain
nonpassing and visible rather than being converted into exclusions.

## Baseline and trigger

Governance run
[29696206056](https://github.com/fullofcaffeine/reflaxe.c/actions/runs/29696206056)
was cancelled by the old 30-minute `pinned-toolchain` timeout while it was still
making progress. The log established this sequence:

- the serial toolchain suites before the Caxecraft tail took about 19 minutes
  45 seconds;
- full Caxecraft QA took about 6 minutes 35 seconds;
- `snapshots:check` then regenerated the centrally registered suites and had
  run for another 3 minutes 42 seconds when GitHub stopped the job;
- every completed toolchain suite and all 11 independent platform, native,
  formatting, provenance, and security jobs had passed.

This was neither a compiler hang nor a reason to remove a gate. It exposed two
separate problems: unrelated suites were serialized on one runner, and the
central snapshot check repeated render work already performed by owning suites.

### First hosted result

The first post-partition Governance run,
[29698408759](https://github.com/fullofcaffeine/reflaxe.c/actions/runs/29698408759),
passed all 17 jobs in 8 minutes 7 seconds from creation through the aggregate
result. It is one cold hosted sample, not yet a `p95` baseline:

| Job | Duration |
| --- | ---: |
| `lowering-objects` | 4m48s |
| `caxecraft` | 6m14s |
| `contracts` | 6m35s |
| `snapshots` | 7m17s |
| `lowering-semantics` | 7m59s |
| `pinned-toolchain` aggregate | 3s |

The required result arrived more than 22 minutes earlier than the old
cancellation point while retaining every gate. The slowest current shard is
`lowering-semantics`, with the span suite as its previously measured long pole;
at this stage, snapshot de-duplication remained a compute-efficiency task even
though snapshot rendering was no longer serialized behind Caxecraft.

### Snapshot de-duplication hosted result

Governance run
[29702208826](https://github.com/fullofcaffeine/reflaxe.c/actions/runs/29702208826)
passed all 16 jobs in 8 minutes 25 seconds after the integrated cold snapshot
shard was replaced by the focused-owner catalog proof. Its uploaded timing
records provide the comparable after-sample:

| Shard | Command wall time | Longest command |
| --- | ---: | --- |
| `contracts` | 6m32s | `project-emitter`, 2m32s |
| `lowering-objects` | 6m11s | `constructor-lowering`, 1m04s |
| `lowering-semantics` | 7m42s | `span-lowering`, 4m50s |
| `caxecraft` | 6m41s | full Caxecraft QA, 6m40s |

The catalog proof took 0.614s. The independent
[cold snapshot audit](https://github.com/fullofcaffeine/reflaxe.c/actions/runs/29702208854)
also passed, re-rendering all 30 suites in 7m14s. The change therefore removed
roughly seven minutes of known duplicate rendering from the required workflow
without pretending that the independent cold work became free. Aggregate wall
time remains controlled by `lowering-semantics`; ordinary hosted variation made
this run 18 seconds longer than the first post-partition sample despite doing
substantially less total work. Beads issue `haxe_c-xge.26` owns the measured
span-runner bottleneck.

## Current lane topology

`npm run test:toolchain` remains the canonical, serial, cold reference command.
Its exact sequence is still guarded because it is useful for release-style
local reproduction and for detecting order/state leakage.

GitHub Actions partitions that same sequence into four isolated shards:

| Shard | Responsibility | Why it may run concurrently |
| --- | --- | --- |
| `contracts` | pins, Beads plan, diagnostics, activation, typed boundaries, C import, AST/plans, runtime contracts, hello, HxcIR, primitive and stdlib contracts | separate hosted runner, checkout, process tree, and temporary roots |
| `lowering-objects` | bodies, functions, aggregates, classes, constructors, dispatch, enums, and generics | no filesystem or process state is shared with another shard |
| `lowering-semantics` | evaluation order, initialization, arithmetic, differential corpus, spans, and project layouts | native/compiler-server work remains serial inside this shard but isolated from the others |
| `caxecraft` | the target-neutral CAXEMAP model, fixed-tick CaxeFlow, and bounded editor command/history/test-play contracts; separate locale/cold/warm-server codec determinism; complete split/package/unity, oracle, native, optimized, and sanitizer showcase lane; then the snapshot ownership catalog | its long end-to-end proof receives its own resource budget; the fast model/rule/editor checks, repeated codec, and catalog checks stay separately visible |

Project-emission semantics and native build consumption have different
authorities. `test:project-emitter` proves deterministic artifacts, ownership,
negative diagnostics, compiler-server parity, and the production failure
boundary without invoking a native compiler. The independent
`test:build-adapters:local` command generates only the three consumer projects
and executes each native consumer available on the workstation. The stricter
hosted `test:build-adapters` owner requires raw-manifest, CMake, and Meson
execution and runs once per required compiler family. Keeping these owners
separate avoids repeating the several-minute semantic matrix inside each
native build job without dropping either proof.

The matrix uses `fail-fast: false`, so one failure does not erase evidence from
the other shards. A small `pinned-toolchain` aggregate job runs with `always()`
and fails unless the entire matrix succeeds. This retains one stable,
fail-closed status for branch protection while exposing the slow or failing
shard directly.

[`scripts/ci/run_toolchain_shard.py`](../scripts/ci/run_toolchain_shard.py)
owns the partition. Before any shard runs, it proves that concatenating the
four shard lists exactly reproduces the canonical `test:toolchain` sequence,
with no missing, duplicated, unknown, or reordered command. Adding a canonical
gate therefore fails CI until it is assigned exactly once.

Useful commands are:

```sh
# Validate the partition without running the suites.
npm run test:toolchain-shards

# Run one focused shard locally; each command inside it remains serial.
npm run test:toolchain:shard -- contracts

# Run every isolation-approved shard with a resource-aware 1-4 workers.
npm run test:toolchain:parallel

# Override automatic selection and retain timing JSON.
npm run test:toolchain:parallel -- --jobs 4 --timing-dir /tmp/hxc-timings

# Reuse only exact, unexpired passing shards from an interrupted local run.
npm run test:toolchain:parallel -- --resume

# Also schedule native smoke inside the same global worker budget.
npm run test:toolchain:parallel -- --resume --with-native

# Run the canonical exhaustive reference lane.
npm run test:toolchain
```

## Snapshot de-duplication contract

The normal exhaustive lane does not run a second compiler render merely to
rediscover bytes that its focused suites just checked. Its final
`snapshots:catalog` step proves all of the following without invoking a
snapshot generator:

- the catalog and executable generator registry contain the same 30 suite IDs;
- every managed suite is a declared snapshot suite with existing, disjoint,
  normalized expected roots and declared formats;
- its catalog runner argument array exactly matches one, and only one,
  `package.json` focused script;
- that focused owner occurs exactly once in the canonical `test:toolchain`
  sequence; and
- the integrated sequence ends with the catalog check and contains no cold
  `snapshots:check` replay.

The focused runner remains responsible for generating and exactly comparing
its bytes while also executing its semantic, negative, native, differential,
or server-reuse evidence. A missing owner, duplicate owner, unregistered
generator, overlapping ownership root, absent expected root, or canonical
coverage drift fails before the integrated gate can pass.

`npm run snapshots:check` remains the independent cold authority. It renders
all 30 generators from scratch, compares every managed artifact, reports
semantic diffs, and never updates implicitly. The
[`Cold snapshot audit`](../.github/workflows/snapshot-audit.yml) runs this full
path when the registry or a focused runner changes, weekly, and on explicit
dispatch. Ordinary compiler changes continue to use the exact focused owners
without paying for a second render. Snapshot updates remain a targeted,
reviewed local transaction followed by the focused owner suite.

An unusually expensive suite may declare a `snapshotValidationRunner` for the
post-update step. This does not replace its ordinary suite runner or full CI
lane. It is allowed only because the central updater has already rendered the
generator twice, compared those renders, and written the transaction. The
focused post-update command must still regenerate and compare the new snapshot
and run the suite's edit-time semantic/native checks. Caxecraft uses this split
so a snapshot update does not immediately repeat its full cold/warm,
locale/order, O0/O2, and sanitizer matrix; that exhaustive matrix remains the
cataloged runner and hosted authority.

## Parallelization safety rule

Parallelize at an isolation boundary, not merely because commands look
independent.

Safe by default:

- separate GitHub jobs or separately provisioned workspaces;
- pure read-only policy checks;
- native compiler/platform matrix entries with distinct build roots;
- content-addressed work whose complete inputs and outputs are validated.

Serial until explicitly redesigned:

- warm Haxe compiler-server lifecycle and request-leakage tests;
- stale generated-file ownership and mode-switch tests sharing an output root;
- tests using a fixed port, singleton process service, or shared mutable cache;
- measurements intended to observe contention or sequential state;
- snapshot updates, which are an intentional reviewed write transaction.

The four current shards now have an executable local isolation declaration.
Their mutable outputs use suite-owned temporary roots, compiler servers bind
ephemeral loopback ports, and repository inputs are read-only. Warm-server,
stale-output, and mode-switch sequences remain serial inside their shard.

### Resource contention and timeout interpretation

Process isolation protects correctness state; it does not reserve the machine.
A Haxe job in another checkout cannot legitimately change this repository's
generated bytes, source mappings, or test assertions when both suites honor
their isolated output roots, caches, ports, and compiler-server rules. It can
still compete for CPU time, memory, and disk I/O. The practical symptom is a
longer wall-clock time or a bounded timeout, not a different expected result.

When a local timeout occurs under unusual load:

1. retain the command, elapsed time, timeout, stderr, and available load/process
   evidence;
2. check for a semantic mismatch, crash, deadlock evidence, shared-path/port
   violation, or still-running CPU work;
3. retry the same command under representative load before calling it a
   compiler regression; and
4. do not raise the timeout merely to make the symptom disappear. Repeated
   isolated slowness needs phase timing, an owning Beads issue, and an
   optimization or justified containment change.

Correctness evidence that completes under contention remains valid. A timeout
alone is inconclusive, and wall times collected on a saturated host are not
performance evidence. Hosted and local performance comparisons require the
same declared toolchain, inputs, concurrency, cache state, and a comparable
resource environment.

`test:toolchain:parallel` therefore runs shards concurrently but never runs the
commands inside one shard concurrently. Without an override, it compares the
one-minute system load with the logical CPU count:

- an idle host with at least eight logical CPUs, load at or below 0.50 per CPU,
  and four CPUs of measured headroom runs all four isolated shards;
- a moderately occupied host with at least four logical CPUs, load at or below
  0.85 per CPU, and two CPUs of headroom runs two; and
- a saturated host runs one.

When the operating system cannot report load, the portable fallback uses two
workers on a host with at least four logical CPUs and one otherwise. Invalid or
non-finite load data takes the same conservative fallback. `--jobs` has priority
over `HXC_TEST_JOBS`; both accept only 1 through 4. The runner prints the choice
and reason before starting, so "automatic" never means hidden.

Each shard writes to an isolated temporary log. Completion progress is printed
at 30-second intervals, then full logs are replayed in canonical shard order so
interleaving cannot hide the first useful failure. All scheduled shards finish
to retain independent evidence even when one fails.

The explicit exhaustive local command can also queue native smoke in this same
worker pool. It does not start an unbounded fifth worker: four toolchain shards
plus native smoke are five pieces of work sharing the selected one, two, or
four workers. Native smoke starts as soon as a worker becomes free. This mode
is useful when reproducing hosted CI, but the commit hook never starts it
automatically.

Local commits now have three explicit routes:

1. A narrow fixture, target-library surface, or focused document runs its
   existing path-owned checks.
2. A known compiler semantic layer or Caxecraft change runs governance plus a
   fixed four-owner smoke set. The sentinels type-check all target-owned Haxe,
   validate HxcIR, compile and run the small Hello product, and verify snapshot
   ownership. During implementation, contributors still run the relevant
   focused owner—such as body lowering, runtime planning, spans, or
   Caxecraft—before the change reaches the hook.
3. An unknown compiler subdirectory or cross-cutting test/CI input runs
   governance plus those same conservative base sentinels. “Fail closed” means
   the route cannot silently select no evidence; it does not mean making every
   local commit replay the whole repository.

This separates quick local defect detection from complete merge evidence.
Every pull request, nightly run, and release still runs the complete cold
matrix on hosted machines; developers can run the same proof explicitly with
`npm run test:toolchain:parallel -- --with-native`. The local route answers the
smaller question “does the repository still type-check, validate HxcIR, build a
small native product, and retain valid snapshot ownership?” Focused task tests
answer whether the changed subsystem preserved its direct contract. CI answers
the larger cross-repository question. A local commit never launches the
complete matrix automatically.
[`select_pre_commit_route.py`](../scripts/ci/select_pre_commit_route.py) keeps
classifications and owner order explicit, deduplicated, fail-closed, and
unit-tested. Its `--owners` output lists the relevant focused owners for task
work; pre-commit deliberately uses `--smoke-owners`.

An established root compiler module must be named explicitly before it can use
the affected route. `CPhaseTiming.hx` is one such reviewed module; a new
unrecognized root file or subdirectory takes the same conservative four-owner
smoke route.
Changes to specialization provenance select the generic-specialization owner.
The established direct-C interop layer and `c.Ref` select the C-import owner;
the reviewed Raygui generator, lock, raw binding, and typed surface select the
Raygui binding owner. This keeps a foreign-function change from falling into
the exhaustive route merely because its compiler code lives under `interop/`,
while still proving the exact ABI and generated-binding contracts it can
affect.
Changing only Caxecraft's compiler-profile consumer does not select the full
game-domain suite: the governance tests own its timing schema and parser,
whereas actual game source and content still select the Caxecraft owner.
The consumer's exact parser test travels on that same affected route. All files
under `scripts/ci/`, `scripts/test/`, `scripts/hooks/`, and `test/governance/`
take the conservative route, including newly added files. This prevents new
test infrastructure from silently receiving only the focused route before it
has a more specific owner.

The change was prompted by an interrupted local commit that had spent more
than 55 minutes in the one-worker exhaustive route under heavy host contention
and had not completed. That saturated duration is diagnostic evidence, not an
authoritative performance baseline. On the same exact staged tree, the new
affected route passed governance and eight selected owners in 464.47 seconds
(7 minutes 44 seconds; 339.03 seconds user CPU and 22.40 seconds system CPU).
The starting one-minute load was 5.73 on 12 logical CPUs. This single observed
run is below the initial eight-minute target, but one run cannot establish a
p95; publish repeated uncontended measurements before claiming that budget has
been achieved.

The complete canonical partition and native-smoke queue remain explicit local
commands and required hosted-CI jobs. They do not run from pre-commit. This
keeps a one-line compiler fix from blocking the next edit for hours while the
CI machine independently verifies all compiler families, layouts, sanitizers,
platforms, and integrated products.

### Safe explicit local resume

Developers may opt into `npm run test:toolchain:parallel -- --resume
--with-native` when they deliberately run the complete local matrix. A retry
after one failed or timed-out shard then does not discard the other successes.
Pre-commit never starts this command. A reusable record is not a general test
cache. It is a local, ignored receipt saying one exact shard passed one exact
set of inputs, and it expires after 24 hours.

The receipt key includes:

- the staged Git execution state plus unstaged tracked changes and relevant
  untracked files under compiler, runtime, test, script, specification, and
  example roots;
- catalog-owned `expected` output bytes only for the shard whose focused suite
  compares those bytes. Other shards still hash the expected-file path and mode,
  so adding, deleting, renaming, or replacing an output with a symlink
  invalidates the shared state, while a reviewed content-only snapshot
  correction does not discard unrelated Caxecraft or compiler-shard evidence;
- the complete canonical command sequence, four-shard registry, command bodies,
  and isolation declarations;
- the reviewed Haxe/Reflaxe, npm, and Raylib lock files;
- the shard runner and pre-commit-hook bytes;
- hashes of the resolved Haxe, Node, Python, native compiler, build-tool, shell,
  JSON, and Git identities; and
- one-way hashes of every inherited environment value, covering native
  compilers, SDKs, include/library paths, Python/Node options, locales,
  timezones, and package resolution without writing their values to a receipt.

Only the scheduling override and Caxecraft's internally replaced timing-output
path are excluded because neither changes what a shard proves. Missing,
malformed, failed, future-dated, expired, symlinked, or mismatched receipts
cause execution. The runner recomputes the input identity after the run and
refuses to write reusable evidence if the checkout or environment changed while
tests were active.

There is one narrower, fail-closed retry. If a prior shard receipt validates
against its own key and lifetime, every ordinary input is unchanged, the
snapshot-owner inventory is unchanged, and the only differing fields are the
content digests of existing catalog-owned `expected` outputs, the runner invokes
only those outputs' focused package-script owners. A successful focused rerun
then seals a new full-shard receipt: the unchanged commands are justified by the
old receipt, and the changed owners by the new run. The new receipt records that
`snapshot-refresh` basis, the prior evidence key, and the executed script IDs so
the combined proof is inspectable instead of looking like a fresh full run. A
fixture edit, compiler
edit, expected-file addition/deletion/rename/mode change, catalog edit, tool
change, environment change, corrupt receipt, or unknown owner always falls back
to the complete shard.

Receipts live under ignored `.cache/toolchain-shards/`. Logs still appear in
canonical order, and timing summaries distinguish `executedShards` from
`reusedShards`. `snapshotOnlyReruns` names each focused owner backed partly by
prior evidence; a historical duration is never presented as current work.
Beads export, formatting, local-path and whitespace checks, staged secret
scanning, governance, and the native smoke lane remain outside this mechanism
and run on every applicable hook invocation. GitHub runs `--run <shard>` in a
clean job without `--resume`, so local evidence can never replace hosted CI.

The staged tree is part of every receipt identity, so the passive Beads export
must also be deterministic. Repository hooks resolve the exact reviewed Beads
client before export and replace `.beads/issues.jsonl` only after a successful
complete write. An incompatible client therefore cannot make valid compiler
receipts appear stale by rewriting the export. The version and clone-recovery
rules are explained in [Beads toolchain and shared database](beads-toolchain.md).

## Budgets and observability

The first objective is a `p95` under 20 minutes for the aggregate required
GitHub check and under 2 minutes for an ordinary focused local gate. These are
initial objectives, not evidence-backed hard budgets yet. Record at least three
cold hosted runs before turning them into regression thresholds.

Every toolchain shard emits a machine-readable timing record conforming to
[`toolchain-timing.schema.json`](specs/toolchain-timing.schema.json), with schema
version, shard, ordered package-script IDs, outcome, exit code, command wall
time, and total wall time. GitHub uploads one record per shard even when a
suite fails. Local parallel runs can preserve the same records plus an ordered
aggregate with `--timing-dir`. Reports deliberately contain no timestamp,
checkout path, or log text. Do not compare developer laptop wall time directly
with hosted CI, and do not label a change a performance improvement without a
comparable before and after sample.

The Caxecraft shard starts with focused, isolated contracts for localization,
content, bounded voxel water, aquatic movement and breath, inventory, actors,
private level resolution, atomic content generations, and deterministic pilot
input before the broader scenario, editor, and full generated-C lanes. These commands
make a local gameplay regression cheap to reproduce; the later full lane remains
the integrated authority and is not replaced by them. Water and aquatics use
the shared focused Haxe-to-C runner: their Haxe probes own behavior while the
host only coordinates Eval, native C, exact trace parity, and sanitizers. These
focused lanes run their shared Haxe rules under Eval and generated C, then
perform one strict native build plus an AddressSanitizer/UndefinedBehaviorSanitizer
run when the selected compiler can provide them.

`npm run test:caxecraft-content-generation` is the focused ownership proof. It
builds fresh sessions, injects rejection before every named construction stage,
performs repeated successful owner swaps, rejects stale publication, compares
Eval/native semantic traces, and runs the native result under sanitizers. It
does not launch Raylib or repeat the full playable compile.

Complete runtime-package publication keeps diagnosis and vertical evidence
separate without compiling the large graph twice. The focused
`test:caxecraft-runtime-content-publication` Eval owner first observed 2.27
seconds; the final cold-process and immediate repeated measurements were 1.08
and 0.80 seconds for malformed-load invariance, exact candidate replacement,
retired-alias retention, and old/duplicate rejection. The existing
`test:caxecraft-runtime-content-generation` command is
the sole Haxe-to-C/native/sanitizer tracer. Eval loads two real package
candidates. Native loads the real package once, rebuilds a distinct
generation-2 level/session from the verified immutable facts, and publishes it;
repeated filesystem reads, hashes, and JSON decoding are not publication
evidence. A proposed second native case timed out at 60 and 90 seconds while
unrelated work held roughly 700–900 percent host CPU, so the duplicate case and
its proposed timeout exception were removed. This is an evidence-efficiency
improvement, not a semantic compiler shortcut.

Issue `haxe_c-xge.20.4.3.7.5` then profiled this exact HXML instead of inferring
its cost from the smaller runtime-free or larger playable workloads. The first
two samples below are contention diagnostics, not representative p50 or p95
claims; the first concise-report sample began under higher one-minute load. The
final sample is the required cold representative-host confirmation. The
complete compiler plan still contained 7,520 runtime requirements, 506 HxcIR
functions, and 25,914 validated names in every run.

| Cold runtime-content generation | Full symbol audit, contended | Concise report, contended | Concise report, representative |
| --- | ---: | ---: | ---: |
| Host load, start → end | 13.990 → 10.726 | 16.821 → 22.083 | 5.729 → 5.879 |
| Haxe-to-generated-C wall | 59.036s | 48.067s | 41.512s |
| Profiled target-request CPU | 47.457s | 34.917s | 30.029s |
| Symbol-report serialization CPU | 18.122s | 0.207s | 0.191s |
| Typed-body lowering CPU | 4.640s | 6.012s | 4.936s |
| Generated project bytes | 49,495,895 | 13,186,260 | 13,186,260 |

The retained change uses `hxc_symbol_report=summary` only in this publication
tracer. The compiler still finalizes and validates the full symbol table; the
concise report keeps exact symbol and collision counts plus every collision
record. The focused project-emitter owner independently renders both forms and
requires every generated C artifact and every unrelated sidecar to remain
byte-identical; only `hxc.symbols.json` and the manifest containing its digest
may differ. Full per-symbol evidence remains in the required symbol-registry and
project-emitter gates. The runtime report stays full because this tracer checks
individual feature-root reasons.

The exact `npm run test:caxecraft-runtime-content-generation` command then
passed without changing its 60-second frontend or 20-second native/sanitizer
bounds and retained the exact Eval/native envelope
`0,2,132089,3528,-1715484850,18389`. The earlier reusable strict native and
AddressSanitizer/UndefinedBehaviorSanitizer observers completed in 7.26 and
2.98 seconds. The representative cold frontend completed in 41.512 seconds,
leaving 18.488 seconds, or 30.8 percent, of the unchanged frontend budget. This
single diagnostic confirmation establishes margin for the task; it is not a
latency distribution. A formatted Linux playable snapshot render took 84.62
seconds and validated all 54 registered artifacts; that is product-call-graph
evidence, not a new latency baseline.

The successful Caxecraft command embeds its own phase report, also described by
[`caxecraft-timing.schema.json`](specs/caxecraft-timing.schema.json). It keeps
asset checks, the Eval reference run, the first three backend renders, cold
determinism, warm compiler-server reuse, snapshot validation, and native
compile/run work separate. It also records the reviewed 12 Haxe requests. This
distinguishes time spent starting or asking the Haxe compiler from time spent
compiling and running generated C. The shard runner rejects a successful
Caxecraft result if a phase is absent, reordered, failed, or reports the wrong
request count, so the measurements cannot silently lose work.

### Local contention diagnostic

A clean four-worker run on 2026-07-19 completed the exact 38-command partition
in 7m25s of wall time while recording 24m28s of aggregate command work. This is
a developer-machine contention diagnostic, not a hosted baseline:

| Shard | Wall time | Longest command |
| --- | ---: | --- |
| `contracts` | 6m14s | `project-emitter`, 2m39s |
| `lowering-objects` | 5m49s | `constructor-lowering`, 1m04s |
| `lowering-semantics` | 7m25s | `span-lowering`, 4m45s |
| `caxecraft` | 5m01s | full Caxecraft QA, 5m00s |

The non-rendering snapshot catalog took 0.55s. The critical path is therefore
`span-lowering`, not snapshot ownership validation. A span is a temporary,
non-owning view of adjacent values, represented in C as an element pointer plus
an element count. The compiler feature turns typed fixed arrays, borrowed spans,
checked indexing, and direct iteration into allocation-free C. The readable
feature and safety contract is in [Fixed arrays and spans](span-lowering.md).

The first phase-instrumented run on 2026-07-20 deliberately forced four workers
while the same pre-run probe saw load 11.16 on 12 logical CPUs. It passed all
38 commands in 546,791ms, but it is the **contended** sample, not the clean
after-number: automatic selection correctly chose one worker for those host
conditions. Caxecraft was the 546,780ms critical path, split as follows:

| Caxecraft phase | Time | Haxe requests |
| --- | ---: | ---: |
| Eval reference | 745ms | 1 |
| first split/package/unity backend renders | 150,698ms | 3 |
| cold repeated/order/locale determinism | 206,013ms | 4 |
| warm compiler-server determinism | 173,932ms | 4 |
| snapshot validation | 7,817ms | 0 |
| native O0/O2/sanitizer compile and run | 6,178ms | 0 |

This tells us why the forced run was slow: Haxe render requests competing with
other local compiler jobs dominated; compiling and executing generated C did
not. The earlier runner did not record these inner phases, so this sample
establishes the local phase baseline rather than inventing a before split.

### Resource-adaptive runner hosted result

Clean Governance run
[29757235207](https://github.com/fullofcaffeine/reflaxe.c/actions/runs/29757235207)
passed every required job and its aggregate gate. Hosted jobs never read local
resume receipts. Their uploaded reports recorded the exact work below:

| Shard | Command wall time | Longest command |
| --- | ---: | --- |
| `contracts` | 7m05s | `project-emitter`, 2m34s |
| `lowering-objects` | 5m56s | `constructor-lowering`, 1m01s |
| `lowering-semantics` | 4m03s | `span-lowering`, 1m16s |
| `caxecraft` | 13m03s | full Caxecraft QA, 13m02s |

The Caxecraft phase report accounts for all 12 Haxe requests:

| Caxecraft phase | Time | Haxe requests |
| --- | ---: | ---: |
| asset contracts | 101ms | 0 |
| Eval reference | 985ms | 1 |
| first split/package/unity backend renders | 202,155ms | 3 |
| cold repeated/order/locale determinism | 269,783ms | 4 |
| warm compiler-server determinism | 266,312ms | 4 |
| snapshot validation | 11,808ms | 0 |
| native O0/O2/sanitizer compile and run | 30,652ms | 0 |

This sample is slower than the earlier 6m41s hosted Caxecraft result, so it is
not evidence that an individual phase became faster. It is evidence that the
new instrumentation retains the complete QA workload and identifies its cost:
the repeated Haxe backend and determinism requests consumed about 12m18s,
whereas snapshot validation and native C work together consumed about 42s.
The overall required workflow still finished in about 13m26s, below the initial
20-minute objective. More cold hosted samples are required before treating
normal runner variation as a regression or setting a percentile budget.

### Caxecraft compilation-server experiment

The Caxecraft runner now has one owned way to start the exact pinned native
Haxe server on an ephemeral loopback address. HaxeShim resolves HXML and scoped
libraries first; the native Haxe 5 client and server then communicate without
HaxeShim's multi-version proxy argument. Focused tests prove that interruption
still terminates the process, a stalled termination is killed, and neither a
global Haxe 4 executable nor a non-loopback listener is used.

The server was not promoted into the normal edit gate because measurement did
not show a benefit. One local sequential sample began at load 9.66 on 12 logical
CPUs and became more contended while other repository compilers were active:

| Render | Cold process | One shared server |
| --- | ---: | ---: |
| split | 47.265s | 57.004s |
| package | 50.615s | 65.183s |
| unity | 49.147s | 64.226s |
| three-layout total | 147.026s | 186.413s |

Every normal artifact and HxcIR dump matched byte-for-byte. The shared-server
side was 26.8% slower in this **contended single sample**, which is diagnostic
evidence rather than a percentile or stable regression number. A second probe
kept the split layout and output directory identical for three consecutive
requests. At starting load 18.32, they took 52.759s, 52.933s, and 56.853s. That
flat result agrees with the hosted full-lane sample above, where four cold and
four warm requests took 269.783s and 266.312s respectively.

The result has a useful architectural meaning. Haxe's server can retain parsed
and typed modules, but it does not skip Reflaxe.C's per-request whole-program
lowering, validation, project planning, or output transaction. Different
`hxc_project_layout` defines also create distinct compiler cache contexts. The
current end-to-end cost is therefore not fixed by keeping the Haxe executable
alive. The normal Caxecraft gate stays cold, while the exhaustive lane retains
warm requests to prove deterministic output and absence of state leakage. The
next optimization must add phase timing around typing and target compilation,
then reduce demonstrated repeated target work without caching mutable
`CompilationContext` state or weakening stale-file ownership checks.

The focused [Haxe compilation-server audit](research/haxe-server-audit.md)
checks that conclusion against the pinned Haxe 5 implementation and the
long-lived server owners in Reflaxe.Elixir and hxhx/Reflaxe.OCaml. It defines
the accepted automatic/off/explicit-attach modes, compatibility identity,
process ownership, retry behavior, cache boundary, and parity matrix for
`haxe_c-5sd.8.2`. Immutable generated-project publication is implemented as
the first half of that task. The interactive launcher now also implements the
owned automatic/off/explicit-attach modes. The exhaustive Caxecraft lane
remains the independent cold/server byte-parity owner.

### Caxecraft unchanged-build launch path

Beads issue `haxe_c-5sd.8.1` implements the first user-visible incremental
milestone without caching typed Haxe or compiler state. After one successful
interactive build, `play.py` writes an ignored, schema-versioned state record
beside that build variant. On the next request it performs these checks in
order:

1. hash every reviewed Haxe, nested HXML, compiler, Reflaxe, runtime, binding,
   build-script, asset, compiled localization/content-pack, pinned Haxe, and
   standard-library input;
2. compare the requested configuration, selected native tools, flags, and
   environment with the request that produced the executable;
3. re-hash the exact Raylib/Raygui headers and libraries used by that native
   build; and
4. re-hash the complete generated project, staged assets, and linked
   executable; and
5. republish the current runtime content files before launch.

The staged map is intentionally not a compiler/build input: the executable
parses it after startup, so changing it must preserve the code hit. Restaging
still copies the exact repository bytes through the existing fail-closed owned
directory check. The pack and UI catalog remain compiler inputs until their
runtime-loading slice lands.

Only an exact code/configuration match launches the existing executable. A missing, malformed,
partial, or corrupt record is a visible miss. The ordinary build then runs and
publishes replacement state only after Haxe-to-C generation, native compilation
and linking, content staging, and a second input snapshot all succeed. If an
input changes during the build, publication fails; an old executable is never
reported as the result of the new request.

The default interactive fast path is deliberately separate from correctness
evidence. `--no-build-cache` rebuilds while retaining the developer's normal
Haxe-server policy. `--cold` (or `HAXE_NO_SERVER=1`) bypasses reuse and starts a
fresh Haxe process. Pilots, snapshots, sanitizers, and compile/build-only modes
do not turn an old interactive executable into passing test evidence.

After the input boundary was finalized, the end-to-end hit hashed 3,067 input
files, 239 reusable outputs, and nine external native inputs in 635.0ms; the
complete `npm run caxecraft:play` process reached launch preparation in 1.09s.
Twenty additional direct-run hits had p50 349.0ms, observed p95 858.6ms, and
maximum 901.7ms. During that sample the Mac's load average rose to 27.32 on 12
logical CPUs because unrelated compiler and operating-system work was active,
so these are conservative contention diagnostics rather than the parent's
authoritative unsaturated baseline. They do demonstrate the intended structural
result: an unchanged launch performs zero Haxe requests, zero C compilations,
and zero links. After runtime map authority landed, one observed hit including
current-content restaging reached launch preparation in 378.4ms and a later
final-state check reached it in 646.8ms on the same development checkout.
The generated-level removal gate then changed the authored map text, observed
a different runtime content hash, and still reached launch preparation after
662.2ms of validation and 1.35s for the complete command. That request started
no Haxe process, C compiler, or linker. The local shell had no usable desktop
display, so Raylib rejected window creation after the content had loaded; this
sample proves the build/content boundary and startup path, not graphical play.
These are diagnostic samples, not a percentile claim.

### Caxecraft immutable generated projects

Changed Haxe builds no longer write into the previously selected `generated/`
directory. The launcher holds one operating-system lock for the selected build
variant, gives haxe.c a unique directory under `transactions/`, and runs the
normal complete generated-project validation there. Only a valid, non-empty
project can move into `generations/<content-sha256>/`.

The hexadecimal directory name is the digest of every generated path and byte,
not a timestamp or random build ID. Repeating exactly the same output therefore
reuses one immutable generation. A small
`hxc-play-current-generation.json` record selects the active generation. It is
written to a temporary file and renamed atomically; it is deliberately not a
symlink because generated-output safety rejects symlink traversal.

This boundary matters for both correctness and speed:

- a compiler crash can leave only an unselected transaction, never a
  half-old/half-new selected C project;
- a crash after the generation rename but before the pointer rename leaves an
  complete but unselected generation;
- the previous pointer remains usable until the new complete pointer is
  published;
- native object caching can safely key an object to immutable source and header
  bytes instead of copying or trusting a mutable tree.

The variant lock currently covers the full one-shot build, so two launchers
cannot overwrite the generated project, executable, or publication state out
of order. Focused fault tests own transaction, pointer, corruption, duplicate
generation, and sequence behavior. Beads `haxe_c-5sd.8.3` owns the next
performance step: compiler depfiles, content-addressed objects, and a link
cache. Until that lands, a changed Haxe build still recompiles every generated
C translation unit.

The depfile-backed native layer is now implemented by Beads
`haxe_c-5sd.8.3`. Every generated source compiles with a GCC/Clang dependency
file. A cached object is accepted only when all of these still match:

- direct generated-C bytes;
- C compiler executable, version, target, flags, and relevant include
  environment;
- the ordered include-root path inventory, which detects a newly added
  shadowing header;
- every generated, Raylib, Raygui, and system header named by the compiler's
  dependency closure; and
- the cached object bytes and schema-versioned metadata themselves.

The fast base-key index is only a hint. Final object entries are immutable and
content-addressed by the complete dependency record. A missing/corrupt index,
dependency record, object, or link becomes a miss. Corrupt entries move to a
quarantine directory before replacement; a failed compiler or linker process
publishes no reusable result.

Independent source files compile through a bounded pool: half the detected
logical CPUs, capped at four by default. `--native-jobs` changes that bound;
logs and object order remain tied to the manifest's canonical source order.
The link cache separately keys the ordered objects, compiler/link arguments,
and exact Raylib/Raygui library bytes. A link-only change can therefore reuse
all objects. Use `--native-cache off` for the uncached authority; it follows
the same depfile-aware plan in private temporary directories.

On the current Mac, the first real 82-translation-unit split build took 6.28
seconds with four workers. The second build reported 82 object hits and one
link hit in 2.62 seconds. An uncached rebuild took 6.94 seconds and produced
the same executable SHA-256,
`b8afb9bf0f4b62bd34bfbca2e84ae8f66635e96aeadf2a9e671e33e61057921d`.
These are observed integration samples, not an unsaturated benchmark. The
important structural result is exact: a normal cache hit launches zero C
compiler or linker processes. The remaining changed-Haxe path is still
dominated by haxe.c's roughly 26-second request-local backend, which is the
next optimization owner rather than a reason to weaken dependency checks.

### Caxecraft owned Haxe server

On a changed build, normal interactive development uses one loopback-only Haxe
server owned by this worktree. A cookie records the exact Haxe process ID,
operating-system process-start identity, live executable identity, endpoint,
and compatibility digest.
That digest includes the canonical worktree, pinned Haxe compiler and standard
library, package/HXML inputs, haxe.c sources, and vendored Reflaxe sources.
Game source bytes are deliberately excluded from the server compatibility digest:
Haxe's own module cache observes and invalidates byte edits inside an existing
module, which is the work the server is meant to reuse. The relative `.hx`
module-path inventory is included without file contents. Adding, removing, or
renaming a module changes that inventory and replaces the exact owned server;
the pinned preview otherwise retained removed or newly shadowed modules in the
focused cold/warm catalog. Base HXML and classpath order remain compatibility
inputs for the same reason.

The launcher may stop or replace a process only when both its PID and
process-start and executable identities still match the cookie. A
malformed/stale cookie cannot authorize killing a process, and an explicitly
attached endpoint is never owned. Automatic mode retries one request only when
the failure is a server
transport failure; an ordinary source or compiler diagnostic is returned
without retry. Every target request still constructs a fresh
`CompilationContext` and reruns HxcIR, validation, CAST, and output planning.

Use the modes directly when diagnosing the loop:

```sh
# Default: reuse or start this worktree's exact pinned loopback server.
npm run caxecraft:play -- --haxe-server auto

# Independent fresh-process authority; --cold implies this mode too.
npm run caxecraft:play -- --haxe-server off --no-build-cache

# Borrow an operator-owned server; Caxecraft never stops it.
npm run caxecraft:play -- \
  --haxe-server attach \
  --haxe-server-endpoint 127.0.0.1:6123

# Stop only the exact auto-owned process recorded by this worktree.
npm run caxecraft:play -- --stop-haxe-server

# Rebuild every C object and link without publishing or accepting native cache entries.
npm run caxecraft:play -- --build-only --native-cache off
```

One real split playable sample on the current Mac took 35.28 seconds for the
first owned-server request, 26.05 seconds for the second request in the same
server, and 34.21 seconds through `--cold`. All three selected the same complete
generation digest
`e754f1333bfadd0052f6dd26e137177996e18190a83f9ed437901f1ec0123976`.
These are single observed integration samples, not an unsaturated benchmark.
They show that frontend reuse is real, but also that roughly 26 seconds remain
because haxe.c's request-local backend still runs. Native object reuse and
request-local backend deduplication therefore remain necessary.

### One-module incremental-edit inventory

`npm run profile:caxecraft-incremental-edit` answers a narrower question than
the repeated-build profiler: after one ordinary Haxe implementation edit, what
actually changed at each layer? It runs five independent sequences by default.
Each sequence uses one temporary source copy, one stable output root, and one
owned Haxe server. The sequence is cold prime, unchanged warm baseline, edit
`Vitals.hx`, and changed warm request. Every comparison is content based;
Haxe's rebuilt-class report is frontend evidence rather than permission to
reuse a later artifact. The outer report rejects structurally different runs
instead of averaging them and records median, median absolute deviation, and
p95 for each request kind.

The first full-playable run produced this structural result:

| Layer | Changed | Reusable candidate evidence |
| --- | ---: | ---: |
| Caxecraft class declarations | 14 of 135 rebuilt | 121 frontend class representations reused |
| HxcIR functions | 1 of 531 changed | 530 function sections byte-identical |
| Replay-owned semantic functions | 1 of 509 rebuilt | 508 exact function payloads replayed |
| Normal generated artifacts | 2 of 229 changed | 227 exact artifact digests reused |
| Generated C translation units | 1 of 84 changed | 83 C source digests reused |
| Generated headers | 0 changed | complete header set byte-identical |

The changed normal artifacts were
`src/modules/caxecraft/domain/Vitals.c` and `hxc.manifest.json`. The ABI,
dispatch, initialization, runtime, specializations, standard-library, and
symbol reports all retained exact bytes. These counts describe this checked
edit, not a permanent project-size contract.

The run began under one-minute host load above 21, so its 36.34-second cold,
21.04-second unchanged-warm, and 19.52-second edited-warm durations are
contention diagnostics, not a budget result. More importantly, this profiler
stops after generated-project ownership. It lists one directly changed native
source candidate but deliberately claims no object or link hit:
`haxe_c-5sd.8.3` must prove that result using compiler depfiles and the complete
native toolchain key.

The inventory also exposed a cache-safety issue. Across cold and warm
requests, named anonymous-record fields can retain the same layout while their
reported source position switches between a typedef declaration and an
object-literal use. Four modules showed that position-only drift during the
edited request, and 14 showed it during the unchanged cold-to-warm comparison.
Normal generated artifacts stayed stable, but an optional HxcIR report changed.

The typed frontend now captures exact declaration offsets from a cold or
rebuilt typedef and retains only bounded, content-keyed plain data across Haxe
server requests. It reconstructs request-local positions for warm requests and
fails before HxcIR when exact source bytes, defines, type range, or field names
do not match. The `test:typed-ast` server matrix covers unchanged, implementation
and public-type edits, failure diagnostics, request order, restart, server-off,
and a second worktree. HxcIR and normal generated files must match a
fresh-process oracle byte for byte. This closes the position-drift prerequisite
without treating source-map emission itself as shipped; E8.T08 still owns that
later feature.

The post-fix full Caxecraft profile found no cold-to-warm HxcIR difference:
zero functions, zero modules, and neither non-function nor schema/dispatch
skeleton changed. The same implementation edit then changed exactly
`Vitals.applyAttack`, its one `caxecraft.domain.Vitals` module section,
`Vitals.c`, and `hxc.manifest.json`. The cold/warm/edit requests took
42.71s/22.20s/22.29s with one-minute host load starting above the Mac's 12
logical CPUs, so those wall times are explicitly contended diagnostics rather
than a replacement for the earlier benchmark. Later semantic-function replay
keeps this same invalidation shape: the unchanged request reuses 509 of 509
eligible functions, while the edit reuses 508 and rebuilds only
`Vitals.applyAttack`. The other 22 HxcIR functions are small compiler-created
adapters that remain on their ordinary deterministic construction path. The
exact changed sets, rather than the contended wall values, are the reusable
invalidation evidence.

### Exact warm control-flow plan reuse

The first backend reuse boundary now acts on that exact per-function evidence.
Control-flow planning decides how one validated HxcIR function becomes
structured C branches and loops. An unchanged warm request no longer repeats
that work: it compares the HxcIR schema, logical function ID, and complete
canonical function text with the previous successful generation. It then
reuses only the already validated target-owned plan. The current request still
builds and validates HxcIR and still emits CAST and C using its current names,
representations, runtime plan, and project layout.

This is deliberately narrower than caching HxcIR or generated C. The retained
generation contains no Haxe `TypedExpr`, `Type`, `Position`, compiler context,
output manager, host path, or partial failed result. Full-text comparison makes
hash collisions irrelevant. Publication happens only after Reflaxe completes
generated-output ownership, and the next success replaces rather than appends
the generation.

One full-playable schema-8 profile on a **contended** 12-logical-CPU Mac
produced the same 229 normal artifacts with SHA-256 tree digest
`8b9f8c206d1bcd719d86f011a76b2e0dfe2f5a9ea9c104efd0988392ae016015`.
The exact counters and owned planning phase show the attributable result:

| Control-flow plan evidence | Cold request | Exact warm request |
| --- | ---: | ---: |
| Cache hits / misses | 0 / 531 | 531 / 0 |
| Retained generation | 531 functions | 531 functions |
| Retained exact-key size | 11,423,746 Haxe string code units | 11,423,746 |
| All-function planning wall | 2.481s | 6.6ms |
| Haxe-to-generated-C wall | 30.355s | 15.158s |

Only the planning reduction belongs directly to this cache. The full wall
difference also contains Haxe frontend reuse and other warm-process effects,
and host load ranged from 6.95 to 8.17, so these single samples are diagnostic
rather than a new p50 or p95 claim.

The fixed Caxecraft `Vitals.hx` edit supplies the changed-program proof. Its
cold prime recorded 531 misses, its unchanged request recorded 531 hits, and
the edit recorded 530 hits plus exactly one miss for
`Vitals.applyAttack`. The same request changed exactly one HxcIR function, one
C translation unit, and the manifest; all generated headers and the other 83 C
translation units remained byte-identical. The focused `test:typed-ast` server
matrix additionally owns public-type edits, cache-disabled behavior,
success-failure-success ordering, restart, a second worktree, and cold
generated-C/HxcIR parity.

### Caxecraft target-phase profile and duplicate-body removal

Beads issue `haxe_c-fbq` added an opt-in compiler profiler rather than guessing
from whole-suite wall time. A *phase* here means one named portion of the build,
such as “turn validated HxcIR into structural C bodies.” The original schema-6
`HXC_PROFILE` stream introduced the checked parent/child tree retained by the
current schema 8. It records phases only when
`reflaxe_c_phase_timing` is enabled. Inclusive time contains nested work;
exclusive time subtracts it. Bottleneck ranking uses exclusive wall and CPU
time, so a broad parent cannot make the same work look expensive twice.
Allocation changes, resident-memory samples, and bounded entity/output counts
help distinguish “one operation is intrinsically slow” from “the compiler
repeated or materialized far more work than expected.” Function-scoped work
records use a closed tagged envelope: control-flow and typed-body counts have
separate payloads, and the consumer rejects an unknown tag, a mixed payload, or
a payload on the wrong phase. Haxe's `--times` report still supplies the
surrounding parse/type/macro context. The command is:

```sh
npm run profile:caxecraft-compiler
```

It uses the exact Haxe pin, one runtime-free split Caxecraft workload, five
fresh compiler processes, one unmeasured server-prime request, and five
same-context warm requests. All eleven requests must produce the same complete
set of normal artifacts byte-for-byte; the report records the measured set
rather than treating today's fixture count as a permanent compiler contract.
`_GeneratedFiles.json` remains separately owned Reflaxe activity metadata. The
consumer independently validates span identity, nesting, containment,
exclusive-time arithmetic, counter order, and the final request totals before
it writes the path-free report. The profiler records no command, checkout path,
temporary path, timestamp, or environment secret.

The measured boundary is Haxe source through generated C files. It includes
Reflaxe.C lowering, printing, and output ownership, but excludes Clang/GCC
compilation, native linking, Raylib, and game execution. Native compile/run
timing belongs to the separate Caxecraft differential lane, so a slow target
compiler pass is not confused with native-toolchain work.

The first full-playable structured profile found one avoidable control-flow
cost. A *dominator* is a block that every route from the function entry must
pass through before reaching another block. Null-check coalescing and HxcIR
validation both need that proof. The old algorithm gave every reachable block a
hash-map set of possible dominators, then repeatedly copied and intersected the
sets until no answer changed. The replacement assigns blocks integer indices,
computes one immediate-dominator tree in reverse postorder, and derives the same
stable answers from that tree.

The focused regression uses an independent definition: it temporarily removes
each proposed dominator and asks whether graph search can still reach the
destination. Linear, branch/join, loop, unreachable, reversed-declaration, and
instruction-failure graphs must agree for every block pair. HxcIR, class-layout,
body-lowering, and evaluation-order owners then preserve the semantic,
diagnostic, generated-C, and native boundaries.

The before/after full-playable runs below are one-sample **contended diagnostic
evidence**, not medians or release budgets. Both emitted the same 225 normal
artifacts with SHA-256 tree digest
`de51d720add1179168963978c515dca3f5499586510db338088f88f2e473de8a`.

| Measured value | Before | After |
| --- | ---: | ---: |
| HxcIR null-check coalescing, exclusive | 4.760s | 0.083s |
| HxcIR validation, exclusive | 5.867s | 1.133s |
| Target request CPU | 64.730s | 53.547s |
| Haxe-to-generated-C wall | 75.551s | 62.156s |
| Cumulative allocation reported by Eval | 267.9GB | 238.0GB |

The tree algorithm removes the demonstrated hot path; it is not a cross-request
cache and retains no compiler state. The next full-playable costs are HxcIR
function construction, semantic analyses/naming, artifact planning, and CAST
control-flow planning. Those require their own bounded evidence before another
implementation change.

The function-construction detail clocks then separated four existing jobs.
Translating typed Haxe expressions into HxcIR took 12.166s of CPU in the first
sample. Finishing the function, deciding which values could remain inline, and
applying the temporary-name plan together took less than 0.2s. That result
rules out the final value plan as the immediate problem.

One attempted shortcut cached the complete Haxe-type-to-HxcIR-type answer.
Although 87.8% of 22,970 requests found a cached answer, the answer is not the
whole operation: encountering an enum also records the source locations that
explain why its specialization exists. Reusing only the type silently removed
those reasons from `hxc.specializations.json`; replaying the reasons preserved
the report but cost substantially more than doing the original work. The
experiment was removed rather than turning incomplete provenance into a faster
contract.

The retained smaller optimization indexes already-recorded specialization
reasons by their stable key. It still keeps the complete sorted reason arrays,
but a repeated encounter no longer rebuilds and compares every earlier key.
The before/after runs were again contended one-sample diagnostics. Both emitted
the same 225 normal artifacts and the same tree digest shown above.

| Measured value | Before | After |
| --- | ---: | ---: |
| HxcIR typed-body lowering, CPU | 12.166s | 11.576s |
| HxcIR typed-body lowering, cumulative allocation | 41.0GB | 39.2GB |
| Target request CPU | 56.093s | 55.466s |
| Total cumulative allocation reported by Eval | 238.4GB | 236.4GB |

This is a bounded improvement, not the feedback-loop solution. It removes
about 590ms of CPU and 1.83GB of allocation from the measured hot phase while
preserving the provenance that makes specialization decisions auditable. The
remaining typed-body work still needs structural attribution; semantic
analyses/naming and artifact planning each remain roughly ten-second targets.

The next detail split found that project emission spent 9.455s of CPU and
allocated 64.2GB cumulatively. `GeneratedFile` had already validated and hashed
each immutable generated C payload in its constructor, but project
canonicalization called `verifyIntegrity()` and hashed all 9.5MB again before
building sidecars. Because Haxe strings are immutable and the file's stored
fields are `final`, no compiler operation can change those in-memory bytes
between the two points. Removing that duplicate same-request proof does not
weaken output ownership, which still validates filesystem paths, prior
ownership, symlinks, and the complete artifact set at the external boundary.

One contended before/after diagnostic retained the same 225-file tree digest:

| Measured value | Before | After |
| --- | ---: | ---: |
| Artifact project emission, CPU | 9.455s | 5.081s |
| Artifact project emission, cumulative allocation | 64.2GB | 33.6GB |
| Target request CPU | 55.141s | 50.175s |
| Total cumulative allocation reported by Eval | 236.4GB | 205.8GB |

The remaining project-emission work is still about five seconds and needs its
own attribution. This result proves only that the second payload hash was
redundant; it does not justify skipping manifest hashing, filesystem checks, or
another independently owned integrity proof.

There is an important pinned-toolchain detail on macOS. Haxe
`5.0.0-preview.1` revision `2c1e544` computes the Mach timer's nanosecond value
but returns the unconverted counter in `libs/extc/extc_stubs.c`. On this host,
the Mach numerator/denominator is `125/3`: a deliberate one-second macro sleep
is reported by `--times` as about `0.025s`. The profiler preserves those raw
Haxe values and records the exact `125/3` adjustment, but bottleneck decisions
use the compiler's independent `Sys.time()` phase records. This is why an
uncorrected `--times` table must not be read as seconds on this pin/host.

The code audit found that an ordinary function body was structurally emitted
three times:

1. once for normal C;
2. once for the focused fixture's optional `#line` form; and
3. once again during project layout in case a closed call cycle required
   `_Noreturn`/tail-call changes.

Production now constructs only the normal body. The body fixture explicitly
requests its mapped copy, and project planning re-emits only when the call-graph
proof actually finds a non-returning cycle. This is not a cache: no mutable
Haxe tree or compiler context survives a request, and recursive-cycle output
keeps its existing specialized path.

The first three-plus-three post-change sample below predates structured
exclusive spans. Its “remainder” rows were derived by subtracting known child
wall timers from their parents; they remain historical evidence, not the
current accounting authority. The sample began at one-minute load `9.88`
on 12 logical CPUs and ended at `9.649`, so it is deliberately labeled
**contended diagnostic evidence**, not a p50/p95 budget:

| Transport | Wall samples | Median | Dominant non-overlapping phase |
| --- | --- | ---: | --- |
| fresh process | 18.326s, 18.408s, 19.887s | 18.408s | CAST body construction, 11.441s |
| owned server, after one prime | 16.243s, 16.486s, 16.766s | 16.486s | CAST body construction, 11.652s |

The cold median's other useful values were 2.715s for the combined
host/frontend/setup remainder, 1.603s for artifact planning, 1.497s for output
ownership, 675ms for semantic analyses/naming, 225ms for HxcIR construction,
58ms for HxcIR validation, 140ms for printing, and 2ms for project planning.
Warm reuse reduced the host/frontend/setup remainder to 350ms, but it did not
remove the 11.652s body traversal. The server therefore now has a measurable
same-context benefit of about 1.9s in this sample, useful for future `hxc dev`,
while algorithmic target work remains the larger opportunity.

An earlier instrumented source count and corrected timer sample attributed
roughly 26s to the redundant project-planning body traversal and roughly 50s
to the two eager body copies under heavier contention. Those numbers are not
compared as a stable ratio; the durable result is the executable traversal
policy plus the post-change repeated sample. The next optimization should
profile the remaining single `CBodyEmitter` walk before adding memoization.
Only immutable facts with complete semantic keys and explicit invalidation may
be cached. Haxe's official
[compilation-server guidance](https://haxe.org/manual/cr-completion-server.html)
supports frontend reuse, while Rust's
[incremental-compilation model](https://rustc-dev-guide.rust-lang.org/queries/incremental-compilation-in-detail.html)
and [Salsa's tracked dependency model](https://salsa-rs.github.io/salsa/reference/algorithm.html)
illustrate why a reusable result also needs stable fingerprints and dependency
tracking. None of those sources justifies persisting raw mutable `TypedExpr` or
skipping output-ownership validation.

### Ranked normal-join proof

Beads issue `haxe_c-xge.30` split the remaining CAST-body work into three
opt-in details. A *normal join* is the block where ordinary paths from separate
branches meet again. The first profile showed that control-flow planning, not
value setup or C syntax construction, consumed effectively the whole body
phase. The planner was proving every eligible block and only then selecting
the best valid join. Deep early-return code made those full-graph proofs grow
far faster than the source program.

The replacement keeps the exact selection rule but changes the work order:
compute graph distances once, rank candidates using the existing score, then
run the expensive validity proof until the first valid candidate is found.
Successor lists are also derived once per request instead of rebuilt during
every graph walk. A deterministic 30-branch work-counter fixture prevents a
return to exhaustive candidate proof.

Both three-cold/three-post-prime samples were contended diagnostics, not
performance budgets. The baseline began at one-minute load `142.506`; the
final optimized sample began at `15.518` on 12 logical CPUs. Corrected Haxe
detail timers provide the baseline split, while the final profiler's
independent `Sys.time()` detail records isolate the optimized target work:

| Transport | Before wall median | After wall median | Before control-flow median | After control-flow median |
| --- | ---: | ---: | ---: | ---: |
| fresh process | 46.336s | 7.884s | 28.875s | 0.522s |
| owned server, after one prime | 17.045s | 7.143s | 12.333s | 0.676s |

In the final cold sample, setup/value planning took 8ms and CAST emission took
22ms, versus 522ms for control-flow planning. That confirms the named
subphase rather than merely showing that the enclosing body phase improved.

The before/after reports produced the same 30 normal artifacts with SHA-256
tree digest `ae7c03d7c152fbea97591a565aa6eab074e012f10cff6a6b65e60d2aa5bcefd2`.
The internal control-flow reduction is the durable result; the wall numbers
show practical direction under load but must not be published as a stable
median, percentile, or machine-independent speedup claim.

### Function-attributed control-flow work

The full playable later made the remaining control-flow cost visible in one
real function: `caxecraft.app.CaxecraftApp.run`, whose HxcIR graph has 339
basic blocks. A *basic block* is a straight-line group of instructions with one
entry and one final jump, branch, return, or throw. The profiler schema now
attaches the function ID and deterministic graph-work counts to each
`body control-flow planning` span. This distinguishes “one large function did
expensive work” from “the compiler planned the same function twice”; the
measured playable had 529 spans for 529 different functions.

Two repeated operations owned most of the cost:

- dominator and post-dominator convergence copied and intersected
  string-keyed sets on every iteration; and
- completion proofs repeatedly rescanned all admitted blocks until no new
  answer changed.

The replacement uses dense bit sets while dominator answers converge, then
materializes the existing string-keyed sets once. Completion proofs now seed a
reverse worklist once and revisit only predecessors whose unresolved successor
count changed. The independent plan validator still consumes the finalized
plan, and neither optimization is cached across compiler requests.

These before/after values are one-sample **contended diagnostic evidence** from
the same full playable, not a stable wall-time benchmark. Both runs emitted the
same 225 normal artifacts with SHA-256 tree digest
`65d62650fe99b2fcac78ffe9e4a66293e2ab3b11f352be89c44ecff06db4749d`.

| Measured control-flow value | Before | After |
| --- | ---: | ---: |
| All 529 functions, planning CPU | 6.382s | 2.457s |
| All 529 functions, cumulative allocation | 17.06GB | 5.93GB |
| Analysis inside `CaxecraftApp.run` | 3.154s / 9.10GB | 0.109s / 0.29GB |
| Full planning for `CaxecraftApp.run` | 4.249s / 11.26GB | 1.110s / 2.45GB |
| Full Haxe-to-generated-C wall | 52.338s | 47.724s |

The work counters are the durable regression boundary. Every completion search
must perform exactly one initial block scan, and its worklist may dequeue each
block at most once. Host contention can stretch elapsed time but cannot make
those counts flaky. The overall compile remains too slow; this result closes
one measured hotspot and supplies better attribution for selecting the next
one.

### Request-local immediate post-dominator reuse

Plan construction and plan validation can ask the same immutable function graph
for an *immediate post-dominator*: the first later block that every path from a
given block must reach. The previous implementation answered each question by
scanning the settled post-dominator sets again. This repeated source-graph work
did not make validation more independent; validation was still checking a plan
against the same HxcIR function.

Each `CBodyControlFlowAnalysis` now remembers the answer by source block,
including the valid answer “there is no such block.” The cache is deliberately
limited to one function in one compile. The validator still interprets and
checks every plan node itself; it reuses only an immutable graph fact, never the
builder's decision that a plan is valid. A reducible-diamond fixture proves one
computation plus a later cache hit and accounts for every query with
machine-independent work counters.

One-sample full-playable profiles were both **contended diagnostics**, with
different host load, so their 25.795s and 22.318s wall values are not an
attributable speedup. The narrower same-phase CPU direction was 1.208s to
1.156s for construction and 0.809s to 0.774s for validation across 529
functions. Both profiles emitted the same 225 normal artifacts with SHA-256
tree digest
`65d62650fe99b2fcac78ffe9e4a66293e2ab3b11f352be89c44ecff06db4749d`.
This is a small, bounded reuse win; generated-file hashing and other larger
allocation owners remain higher priorities.

### Function-attributed typed-body work

The next full-playable profile gave every `HxcIR typed-body lowering` span its
stable function ID and counted both source work and produced HxcIR. *Type
classification* means deciding which exact C-facing value contract a typed
Haxe value needs: a primitive, record, collection, enum or class, callable, or
another supported carrier. This decision is semantic work, not merely spelling
a C type, because some families must also register layout, lifetime, import,
specialization, or source-provenance facts.

The playable asked for 21,698 body value classifications. Three distinct
repeated costs were safe to remove:

- 8,539 requests were exact non-nullable primitives such as Haxe `Int`, `Bool`,
  or a `c.UInt8` abstract. They now reuse one request-local immutable value plan.
  The shortcut deliberately does not unwrap `Null<T>`, imported typedefs, or
  user abstracts because those neighboring shapes can carry different
  representation or diagnostic meaning.
- Non-generic named record typedefs produced 80 distinct classifications and
  5,591 repeated requests. A named, non-generic typedef has the same transparent
  field definition throughout one compiler request, so those repetitions now
  reuse the first immutable record value plan. Generic records and anonymous
  object literals retain full structural classification because their shape
  can depend on type arguments or local typing context.
- Each distinct enum use records a source location explaining why that enum
  shape exists. The previous code re-sorted the enum's entire growing
  provenance list after every new location, rebuilding display keys during
  comparisons. Discovery now deduplicates by the same stable display key and
  sorts once when the final specialization report asks for canonical order.
  The report remains complete and byte-identical.

These changes do not persist Haxe `Type` or `TypedExpr` values between compiler
requests, do not skip HxcIR validation, and do not turn a side-effecting general
type lookup into a cache hit. The primitive and named-record plans are shared
only inside the current `CBodyAggregateRegistry`; enum provenance still records
every distinct source location before its deterministic final sort.

The table compares two single cold runs of the same full playable on a
**contended host**. It is diagnostic evidence, not a stable median or a release
budget. Both runs produced the same 225 normal artifacts with SHA-256 tree
digest
`65d62650fe99b2fcac78ffe9e4a66293e2ab3b11f352be89c44ecff06db4749d`.

| Measured typed-body value | Before | After |
| --- | ---: | ---: |
| Typed-body lowering, CPU | 11.338s | 4.443s |
| Typed-body cumulative allocation | 39.16GB | 15.55GB |
| Nominal-family classification, CPU | 3.242s | 0.899s |
| Haxe-to-generated-C wall | 47.007s | 38.338s |

The compile is still too slow. The durable result is that the largest named
semantic phase lost about 6.9 seconds of CPU and 23.6GB of repeated allocation
without changing compiler artifacts. The next optimization should move to the
remaining measured owners—especially symbol collision work and C printing—
rather than broadening these deliberately narrow caches.

### Exact source-position reuse

Every HxcIR instruction that came from Haxe source carries a *source span*: the
source file plus the start and end line and column. Diagnostics use that range
to point at the right expression, and generated `#line` directives use it to
map C compiler messages back to Haxe. One Haxe expression can produce several
HxcIR instructions, so the old builder repeatedly asked the Haxe compiler to
convert the same internal byte offsets into the same line and column range.

Each function builder now remembers conversions by the exact compiler filename,
start byte, and end byte. An identical request reuses the immutable
`HxcSourceSpan`; a different file or either different offset still performs a
fresh conversion. The cache ends with that function body, so it cannot retain a
compiler-owned `Position` or leak a result into another request. It does not
remove source maps, coarsen ranges, or trust a nearby position as equivalent.

Profile schema 7 records requests, real computations, hits, and CPU time for
this operation. Its parser fails unless every request is accounted for by
exactly one computation or hit. A focused macro probe requests one position
twice and another position once, proving both reuse and separation.

The following single cold samples used the same full playable on a
**contended host**. Their wall times are not comparable performance evidence
because host load differed. The narrower operation and allocation counters are
the attributable result. Both runs produced the same 225 normal artifacts with
SHA-256 tree digest
`65d62650fe99b2fcac78ffe9e4a66293e2ab3b11f352be89c44ecff06db4749d`.

| Measured source-position value | Before | After |
| --- | ---: | ---: |
| Requests | 35,875 | 35,875 |
| Haxe coordinate computations | 35,875 | 20,190 |
| Exact cache hits | 0 | 15,685 |
| Source-position CPU | 623ms | 408ms |
| Typed-body cumulative allocation | 15.55GB | 14.88GB |
| Whole-request cumulative allocation | 77.92GB | 77.23GB |

This removes about 674MB of typed-body allocation and 215ms from the measured
source-position operation. It is a useful local improvement, not a claim that
the feedback loop is fixed; the next iteration must continue with the largest
remaining independently attributed owner.

### C formatting and generated-file hashing

The next profile split corrected the meaning of the broad “C printing” phase.
It now measures two separate jobs for every generated source or header:

1. the structural `CASTPrinter` turns an already validated C tree into text;
2. `GeneratedFile` converts that text to UTF-8 and computes the SHA-256 digest
   used by manifests and build caches.

Only 0.648s of the original 4.992s CPU sample belonged to formatting 189 files.
Constructing the immutable generated files took 4.304s. The cause was not the
SHA-256 algorithm itself: Haxe's standard Eval implementation first expands
the complete input into large boxed integer arrays. Hashing 4.49MB of generated
C therefore allocated about 29.3GB cumulatively in the macro host.

`CContentDigest` preserves the exact SHA-256 contract but uses a fixed-memory
streaming implementation on Eval. It reads each complete 64-byte block
directly, keeps one 64-word schedule, and allocates only the final padded block.
Other Haxe targets retain the standard library. The registered
`test:content-digest` gate checks public known vectors and compares binary
payloads with Haxe's implementation around every one-block/two-block padding
boundary and across many blocks. This is an execution optimization, not a
weaker or different content identity.

The formatter itself now reuses one indentation prefix per nesting depth,
returns immediately for empty qualifier/specifier lists, and joins already
validated non-empty tokens without intermediate filter and join arrays. The
profile work counters prove the intended structural reduction: 69,143
indentation requests now construct 29 prefix steps instead of 260,397 repeated
indent units, and empty uniqueness checks fall from 56,395 calls to the 2,608
checks that have actual inputs.

These are single-run **contended diagnostic samples**, not stable medians or
release budgets. Both runs formatted the same 4,949 declarations, 45,083
statements, and 164,220 expressions into 4,493,190 bytes. Both emitted the same
225 normal artifacts with SHA-256 tree digest
`65d62650fe99b2fcac78ffe9e4a66293e2ab3b11f352be89c44ecff06db4749d`.

| Measured C-output value | Before | After |
| --- | ---: | ---: |
| Broad C printing, CPU | 4.992s | 3.144s |
| Broad C printing, cumulative allocation | 31.31GB | 18.68GB |
| Structural translation-unit printing, CPU | 0.648s | 0.488s |
| Structural printing, cumulative allocation | 1.95GB | 1.25GB |
| Generated-file construction, CPU | 4.304s | 2.617s |
| Generated-file construction, cumulative allocation | 29.26GB | 17.33GB |
| Full Haxe-to-generated-C wall | 41.483s | 39.959s |

The broad phase lost about 1.85s of CPU and 12.63GB of allocation while keeping
every integrity digest. The compiler is still too slow: even the streaming
digest costs about 2.6s in this workload, and other exclusive phases now rank
above the actual formatter. Further work must follow those measurements rather
than weakening hashing, collapsing output ownership, or adding speculative
cross-request state.

The follow-up profile found that the fixed-memory implementation still stored
its 64-word SHA-256 schedule and round constants in `haxe.io.Bytes`. Eval then
called byte-buffer helpers for values that were already 32-bit host integers.
Ordinary fixed-size integer arrays preserve the algorithm and memory bound while
removing those repeated conversions. The focused gate now also checks the
published SHA-256 vector for one million ASCII `a` bytes, in addition to the
standard-library padding-boundary differential.

These follow-up values are again one-sample **contended diagnostics**. The
before and after profiles emitted the same 225 normal artifacts with the same
tree digest shown above.

| Measured digest value | Byte schedule | Integer schedule |
| --- | ---: | ---: |
| Generated-file construction, CPU | 2.561s | 1.798s |
| Generated-file construction, cumulative allocation | 17.33GB | 12.87GB |
| Complete target request, CPU | 21.852s | 21.151s |
| Complete request cumulative allocation | 84.59GB | 78.82GB |

The isolated owner lost about 0.76s of CPU and 4.46GB of cumulative allocation.
An attempted `haxe.ds.Vector` schedule did not reduce allocation and was not
faster in the same workload, so the compiler keeps the simpler array rather
than retaining an abstraction that the evidence did not justify.

The next step addresses the common warm-server case without weakening content
identity. `GeneratedFileDigestCache` retains one completed request's immutable
path, artifact role, full Haxe string, UTF-8 byte length, and SHA-256 digest.
The following request first compares path, role, and the complete text. Only an
exact match reuses the byte length and digest; changed text still performs the
normal UTF-8 conversion and fixed-memory SHA-256 calculation. This is output
evidence, not a semantic backend cache: it retains no typed compiler objects,
HxcIR, CAST, request context, output manager, or partial generation.

The cache is published from Reflaxe's `onOutputComplete` hook, after planned
files, the stale-file policy, and ownership metadata have completed. Failed
and aborted requests discard their request-local lookup and cannot replace the
last successful generation. The retained set is replaced, not accumulated,
so its size stays bounded to one generated project. The
`reflaxe_c_test_disable_generated_digest_cache` define provides the
authoritative uncached comparison path.

The `test:content-digest` gate now starts an isolated Haxe compiler server and
mutates each exact key dimension independently. It proves same-input hits,
content/path/role misses, independent SHA-256 and byte-length equality, failed
request isolation, one-generation replacement, and cache-disabled behavior.
The existing project-layout gate compares cold and warm split/package/unity
trees through the real Reflaxe output lifecycle.

One schema-7 full-playable diagnostic produced the same 229 normal artifacts,
5,820,648 generated bytes, and tree digest
`8b9f8c206d1bcd719d86f011a76b2e0dfe2f5a9ea9c104efd0988392ae016015`
on cold and warm requests. The host was **contended** (one-minute load
6.08–7.79), and each transport had only one measured sample, so the whole-run
wall values are diagnostic rather than a new baseline. The isolated
generated-file span and exact cache counters are the attributable evidence.

| Measured generated-file value | Cold request | Warm exact-hit request |
| --- | ---: | ---: |
| Generated-file construction, CPU | 1.778s | 0.022s |
| Digest cache hits | 0 | 229 |
| Digest cache misses | 229 | 0 |
| Retained generation | 229 files / 5,820,648 bytes | 229 files / 5,820,648 bytes |
| Complete request CPU | 19.296s | 15.505s |
| Haxe-to-generated-C wall | 28.689s | 16.028s |

Only the roughly 1.76-second generated-file CPU reduction belongs directly to
this cache. The complete-request difference also includes Haxe frontend reuse
and other warm-process effects. Typed-body lowering remains the largest warm
target phase at about 4.27 seconds, so subsequent optimization work continues
there and in the next measured owners rather than broadening this output cache
into cross-request semantic state.

### Deterministic candidate-name ordering

The next profile attributed the remaining deterministic symbol work before
changing it. Caxecraft registered 22,876 C symbols. Resolving their generated
names sorted 22,221 first-round candidates and 850 names changed by a real
collision. The established comparator converted both strings to `Bytes` on
every comparison. That meant 408,804 comparisons performed 817,608 wrapper
conversions and accepted about 202 million Haxe string code units as repeated
conversion input.

The compiler itself runs on Haxe's Eval host. Eval stores a Haxe string as
UTF-8 bytes and its native string comparison delegates to OCaml's
lexicographic byte comparison. The registry can therefore compare those
strings directly without changing its canonical UTF-8 byte order. This is a
host execution optimization, not a naming-policy change: a non-Eval tooling
host retains the explicit byte comparator, and `CSymbolRegistry` remains the
only naming and collision authority.

The first attempted optimization cached one UTF-8 wrapper per candidate. It
removed the repeated conversions but made the profiled CPU sample worse
because the extra wrapper records had their own Eval cost. That version was
rejected rather than accepted on reduced work counts alone. Direct host
comparison removed the conversions without adding a parallel data structure.

These are single-run **contended diagnostic samples**, not stable medians or
release budgets. The host load was already above its 12 logical CPUs and
changed between runs, so process CPU and allocation are the primary attribution
evidence. Both runs performed the same 408,804 candidate comparisons, retained
the same collision counts, and emitted the same 225 normal artifacts with
SHA-256 tree digest
`65d62650fe99b2fcac78ffe9e4a66293e2ab3b11f352be89c44ecff06db4749d`.

| Measured naming value | Before | After |
| --- | ---: | ---: |
| Candidate collision resolution, CPU | 3.147s | 0.589s |
| Candidate collision resolution, cumulative allocation | 15.35GB | 3.59GB |
| Explicit candidate UTF-8 conversions | 817,608 | 0 |
| Symbol finalization, CPU | 6.636s | 4.122s |
| Symbol finalization, cumulative allocation | 29.86GB | 17.94GB |
| Full Haxe-to-generated-C wall | 36.889s | 35.012s |

Request ordering and final table construction now account for most of the
remaining symbol time. They use the request's already cached stable-order
bytes, so the next improvement must profile their specific work rather than
generalizing this candidate-name shortcut or weakening deterministic reports.

### Canonical symbol-request order reuse

The follow-up profile found two copies of the same ordering work. The registry
first sorted all 22,876 requests into its canonical semantic order. After
collision resolution changed some C spellings, it sorted those same requests
again even though a spelling change cannot change semantic order. Both sorts
also compared cached byte arrays one byte at a time in interpreted Haxe.

The registry now keeps the canonical tuple as an immutable String on Eval and
uses the same native UTF-8 comparison already proven for candidate names.
Non-Eval hosts retain the explicit byte comparator. Finalized drafts are
looked up by semantic key and projected through the first sorted request list,
so the second sort is gone rather than cached. Map iteration order remains
unobservable, and exact-name validation, collision grouping, suffix hashing,
the collision ledger, and the generated symbol table keep the same owners.

These are one-sample **contended diagnostic values**, not stable medians or a
release budget. The before and after runs used the same compiler workload and
emitted the same 225 normal artifacts with SHA-256 tree digest
`65d62650fe99b2fcac78ffe9e4a66293e2ab3b11f352be89c44ecff06db4749d`.

| Measured naming value | Before | After |
| --- | ---: | ---: |
| Request ordering, CPU | 1.515s | 0.180s |
| Final table materialization, CPU | 1.700s | 0.281s |
| Second-sort comparisons | 403,022 | 0 |
| Complete target request, CPU | 25.001s | 22.913s |
| Complete request cumulative allocation | 95.91GB | 84.60GB |
| Haxe-to-generated-C wall | 27.525s | 25.795s |

The stable evidence is structural: the second sort has zero comparisons, the
first sort retains the same 420,830 comparisons, every collision counter is
unchanged, and every compiler artifact is byte-identical. The timing direction
is useful, but the saturated host means it must not be published as a p50,
p95, or machine-independent speedup.

### Warm finalized symbol-table reuse

The next changed-Haxe profile showed that deterministic naming still repeated
three settled operations after the first canonical request sort: constructing
generated-name candidates, resolving their collisions, and materializing the
same final records. Caxecraft registered 22,923 requests. Its fixed
`Vitals.hx` implementation-only edit changed one HxcIR function and two normal
artifacts, but did not change any naming input.

`CSymbolRegistry` now retains exactly one preceding successful table. A new
request still registers, validates, and canonically sorts the complete set.
Reuse requires exact equality of every stable semantic key and every naming
fingerprint; the latter covers readable spelling, visibility, and explicit
names. A hit restores the immutable C spellings and report, while an
added/removed/renamed request runs normal finalization. Publication occurs only
after output ownership completes, so a failed request cannot replace the prior
success. The cache-off path uses
`reflaxe_c_test_disable_symbol_table_cache`.

The focused naming corpus proves an exact hit despite reversed discovery order;
added, removed, and fingerprint-changed misses; conflicting fact rejection;
failed/aborted request rollback; bounded replacement; and disabled behavior.
The integrated incremental probe additionally proves that the real Vitals edit
keeps the table reusable while its changed semantic function and C source still
rebuild.

These are single-run **contended diagnostics** from one owned Haxe server, not
an uncontended p50 or p95. They provide attributable phase and work evidence:
the cold request performed all naming algorithms; both warm requests reported
one exact table hit, zero misses, and zero candidate/collision/materialization
work. All requests retained 22,923 entries, and the generated artifacts kept
their expected byte differences.

| Measured naming value | Cold prime | Warm unchanged | Warm Vitals edit |
| --- | ---: | ---: | ---: |
| Symbol finalization, wall | 1.562s | 0.316s | 0.467s |
| Request ordering, wall | 0.152s | 0.161s | 0.242s |
| Table cache hits / misses | 0 / 1 | 1 / 0 | 1 / 0 |
| Candidate/collision/materialization work | performed | skipped | skipped |

A separate schema-9 warm full-playable diagnostic emitted the same 229-artifact
tree digest
`8b9f8c206d1bcd719d86f011a76b2e0dfe2f5a9ea9c104efd0988392ae016015`
and completed Haxe-to-generated-C in 13.977s on the contended host. That wall
number is not a formal budget; the stable claim is the exact 22,923-request hit
and removal of the three owned naming computations.

### Exact warm semantic-function replay

Typed-body lowering was the next dominant warm target cost, but skipping it
was initially unsafe. Constructing one function could be the first operation
to discover a shared anonymous layout, collection representation, C import,
generated adapter, runtime requirement, enum-report reason, or C name request.
Reusing only the old HxcIR function would then produce plausible output while
silently omitting one of those other products.

The compiler now separates those owners before lookup. A deterministic prepass
settles shared layouts, globals, imports, callable/constructor signatures,
dispatch, and adapters. It freezes a complete function-free HxcIR program
revision and rejects any later shared contribution. Each function entry then
owns exactly its HxcIR, C name requests, runtime requirements, and generic-enum
provenance ranges. A hit restores fresh arrays and the current request's
diagnostic positions before the ordinary managed-root, coalescing, HxcIR
validation, naming, CAST, artifact, and output-ownership stages run.

The key is deliberately exact rather than digest-only. Its program half holds
the Haxe version and complete settled shared revision. Its function half holds
the complete structural typed tree plus every stable source range. Haxe's
request-local variable allocator can assign different numbers to the same
bindings after a module edit, so the key renumbers only explicit
argument/local binding IDs by first appearance. A focused macro test proves
this keeps binding identity but still detects a changed expression. No Haxe
`TypedExpr`, `Type`, `Position`, compilation context, output manager, generated
C, or worktree path persists.

The fixed `Vitals.hx` probe supplies the integrated invalidation proof:

| Semantic-function replay evidence | Cold prime | Warm unchanged | Warm Vitals edit |
| --- | ---: | ---: | ---: |
| Shared program revision matched | no prior generation | yes | yes |
| Cache hits / misses | 0 / 509 | 509 / 0 | 508 / 1 |
| Canonical HxcIR functions changed | baseline | 0 | 1 |
| Normal generated artifacts changed | baseline | 0 | 2 |

The edited request rebuilds exactly `Vitals.applyAttack`; its other 508
eligible functions replay, and the resulting output changes only `Vitals.c`
plus the manifest that records its new digest. All headers and the other 83 C
translation units remain byte-identical. The same probe rejects a replay miss
count larger than the canonical HxcIR changed-function count, so a future
request-local identifier leak cannot quietly turn precise invalidation back
into a module-closure rebuild.

One full-playable warm diagnostic on the contended Mac reported all 509 hits,
zero misses, and zero time in typed-body lowering, function finalization,
value coalescing, or value-plan application. The complete compile took about
12.02 seconds in that sample, down from the preceding roughly 15-second warm
diagnostic, but neither value is an uncontended p50 or p95. The attributable
result is structural: 509 body constructions disappeared while all normal
artifacts stayed byte-identical. Representation discovery, exact key
construction, HxcIR validation, CAST projection, and artifact/output work still
run and are the next measured owners rather than hidden behind this cache.
The one retained generation held 5,931,151 Haxe string code units of exact
per-function inputs and 931,570 code units for the shared program revision;
these are bounded storage counts, not UTF-8 byte or total-memory claims.

The cache retains one successful generation and replaces it only after
Reflaxe completes output ownership. The focused macro probe covers exact hit,
typed-input miss, whole-program miss, mutation isolation, aborted publication,
deleted-function replacement, disabled behavior, and fail-closed recovery when
a required current source position is absent. The ordered Haxe-server matrix
also checks cold prime, unchanged reuse, a one-function implementation edit, a
public typedef edit that invalidates the shared revision, failure followed by
successful restoration, cache-off, a second worktree, and server restart.
Machine-readable evidence names the program-wide reason and splits
program-matched misses into a missing function versus a changed exact input,
so tooling need not infer invalidation from elapsed time. Use
`reflaxe_c_test_disable_body_function_replay_cache` for the authoritative
ordinary construction path; use
`reflaxe_c_body_function_replay_cache_report` when a server test needs the
machine-readable lifecycle result.

### Exact invalidation catalog and bounded server state

`npm run test:incremental-backend` answers a different question from the
full-playable timing profile: after a meaningful program fact changes, what may
the warm backend still reuse? The fixture applies one ordered mutation at a
time, compiles through one owned Haxe server where that is safe, and compares
the complete HxcIR and generated-file tree with a fresh-process request. The
tree comparison includes runtime, ABI, initialization, dispatch, symbol, and
specialization reports; a matching C file alone is not enough.

The current catalog proves these ownership boundaries:

| Edit | Required replay behavior |
| --- | --- |
| Static initializer value | Shared program still matches; rebuild the changed initializer function |
| Class hierarchy or dispatch | Shared program changes; rebuild every semantic function |
| Generic specialization set | Shared program changes; rebuild every semantic function |
| Reachable `Array` runtime use | Representation/runtime program changes; rebuild every semantic function |
| `@:c.name` on an existing C import | Replay unchanged semantic functions; rebuild the request-local import/name plan, whose generated C must match cold output |
| Compile-time define | Use Haxe's separate define context; no function from an incompatible context may hit |
| Classpath order or shadowing | Restart the exact owned Haxe server, then compare with cold output |
| Removed or renamed module | Restart the exact owned Haxe server, then compare with cold output |
| Declared external macro input | Haxe rebuilds the caller; the matching shared program permits only changed exact function inputs to miss |

The restart cases are a frontend safety boundary, not a lost backend
optimization. Once Haxe returns a typed program containing a stale module,
haxe.c cannot reconstruct which source the user intended. Caxecraft's
server-compatibility schema therefore hashes compiler/HXML/classpath
infrastructure and the path-only `.hx` module inventory, while deliberately
excluding ordinary game-module bytes. Method-body edits stay warm; path-set
changes replace the server before compilation.

The same change also repaired a regression exposed by this matrix. Static-field
initializer functions could mention a global before the replay prepass had
added that global to the function-free program. The prepass now registers both
the initializer's destination and every referenced ordinary Haxe static field
before it freezes the replay revision. The dedicated static-initialization
suite verifies exact-once behavior, dependency order, cycle diagnostics,
JavaScript differential behavior, and strict native C.

Persistent storage has a separate fast structural soak. It publishes 24
different two-function generations and follows each with an unchanged replay,
for 48 requests total. Every replacement retains exactly two functions and the
same complete input-text size; old generations do not accumulate. The ordered
real-server test continues to own failed-request rollback, cache-off, process
restart, and two-worktree isolation.

One contended run of the two-worker warm/cold catalog took 99.83 seconds while
an unrelated Haxe 4.3.7 process continuously used one CPU and one-minute load
was near the repository's 6.0-on-12-CPU cutoff. This is responsiveness
evidence, not the required p50/p95 budget. The catalog is a separate named CI
owner so ordinary `test:typed-ast` work does not pay for all nine cold oracles.

### Repeated changed-source budget

On 2026-07-28, the five-run incremental profiler completed with every sample
classified as representative on the 12-logical-CPU Apple arm64 development
host. Each sample started a new owned Haxe server, so its first request had
empty frontend and haxe.c caches. It then measured one unchanged request and
one real `Vitals.hx` implementation edit in that same server:

| Request | Median | Median absolute deviation | p95 |
| --- | ---: | ---: | ---: |
| Cold-prime | 33.822s | 0.402s | 34.209s |
| Warm unchanged | 11.993s | 0.007s | 12.317s |
| One-module edit | 12.166s | 0.026s | 12.285s |

The cold-prime p95 is below the 40-second cold budget and the edited p95 is
below the initial 20-second budget. The edit still misses the 10-second
destination. The report attributes the largest remaining edited-request
exclusive owner to HxcIR representation planning at a 1.257-second median;
function preparation and replay-key construction each cost about another
second. Beads issue `haxe_c-5sd.8.4.7` owns the narrow next experiment:
reconstruct the shared representation/import/adapter/global plan from bounded
immutable contributions for frontend-reused functions, while rediscovering
every changed or uncertain function and retaining the complete program-revision
comparison and independent HxcIR validator.

A separate five-run fresh-process cold report ended just above the load cutoff,
so its 34.593-second median and 35.335-second p95 remain contended diagnostic
evidence rather than a second budget claim. It agrees with, but does not replace,
the representative cold-prime distribution.

### Function-build contribution inventory

The first prerequisite for a safe function-body cache is knowing what a
function changes outside its own returned HxcIR. Returning only an old
`HxcIRFunction` can be wrong when the original build also registered a C import,
introduced a collection or optional representation, created a late adapter, or
added names that later phases expect to find.

Profile schema 11 therefore wraps every ordinary function build with two
shallow inventories. It first counts the shared facts, builds the function, and
then counts the same closed set again. The difference is attributed to that
function. The record separately counts function-owned HxcIR, runtime
requirements, and name requests. These are observations only: the profiler
does not retain compiler objects and does not authorize reuse.

One warm full-playable diagnostic produced the following inventory:

| Function-build observation | Count |
| --- | ---: |
| Ordinary function builds | 509 |
| Builds adding no representation/import/adapter semantics | 490 |
| Builds adding representation/import/adapter semantics | 19 |
| Builds adding only shared symbol requests | 452 |
| Shared symbol requests added during builds | 17,446 |
| Built HxcIR blocks / instructions | 5,734 / 35,050 |
| Built cleanup regions / runtime requirements | 139 / 3,645 |

The 19 semantic contributors introduced 50 imported functions, seven reached
import owners, four imported types, nine optional representations, one
`IntMap` representation, and one `StringMap` representation. No build added an
aggregate, enum, class, interface, Array, Bytes, late enum-constructor adapter,
function literal, or static callback adapter in this workload. This is useful
evidence that representation discovery is already mostly settled before body
construction, but it also identifies the exact remaining exceptions.

The result rejects a direct “reuse the old HxcIR function” cache. Most builds
still register names, and 19 builds enlarge other shared plans. A safe follow-up
must either move those discoveries into a complete pre-build plan or retain and
replay a closed, immutable contribution package alongside the function. It
must also compare exact typed input and settled program-plan identity and run
normal validation. Caching `TypedExpr`, a mutable registry, or an unexplained
subset of these effects remains out of scope.

The observed request was contended, so its 13.862s Haxe-to-generated-C wall
time is diagnostic rather than a baseline. Within that request, the 509
function-build spans contained 4.315s of typed-body lowering and 0.230s of
exclusive work not already owned by a nested body span. The profiler reported
byte-identical normal artifacts against its warm prime. Stable acceptance
evidence is the closed 509-function accounting, the zero/nonzero contribution
split, and the unchanged artifact bytes--not the wall-time number.

### Runtime feature-closure reuse

Runtime planning answers a small but important question: after direct C and
program-local helpers have been considered, which exact `hxrt` features does
this program still need, and why? The full playable supplied 4,662 independently
validated semantic reasons, but those reasons selected only a small set of root
features. Every root reaches a fixed *dependency closure*: the root feature
plus all of the features it depends on, directly or indirectly.

The earlier planner walked that same closure once per reason. It also kept a
hash map beside every feature and dependency edge to detect duplicate reason
IDs. After propagation, it sorted each large reason-ID array again. Those
defenses were unnecessary at this phase: canonicalization has already proved
that every reason ID is globally unique and put the reasons in deterministic
UTF-8 order.

The planner now validates and traverses each distinct root closure once per
compiler request. It then appends each canonical reason ID once to the
de-duplicated features and edges in that settled closure. The resulting arrays
are already in their final order, so projection copies them without another
large sort. This is request-local immutable planning work, not a cross-build
cache. Policy, environment, availability, dependency, reason, diagnostic, and
runtime-free checks remain in place.

Stable nested profiler spans separate catalog construction, direct-evidence
projection, HxcIR requirement reconciliation, reason canonicalization,
dependency propagation, and final plan projection. A focused diamond-graph
fixture proves that a dependency reached by two paths still receives each
reason exactly once and that reversing input discovery cannot change the plan.

The before and after samples below used the same full playable on a
**contended host**, so they are diagnostic attribution rather than a release
benchmark. Both emitted the same 225 normal artifacts with SHA-256 tree digest
`65d62650fe99b2fcac78ffe9e4a66293e2ab3b11f352be89c44ecff06db4749d`.

| Measured runtime-plan value | Before | After |
| --- | ---: | ---: |
| Dependency propagation, CPU | 0.590s | 0.011s |
| Dependency propagation, cumulative allocation | 2.45GB | 0.034GB |
| Final plan projection, CPU | 0.525s | 0.006s |
| Final plan projection, cumulative allocation | 2.27GB | 0.014GB |
| Complete feature closure, cumulative allocation | 5.36GB | 0.69GB |
| Complete target request, CPU | 20.810s | 20.189s |
| Complete request cumulative allocation | 78.82GB | 74.15GB |

Requirement reconciliation now owns most of the remaining broad runtime-plan
cost at about 0.48s and 1.91GB in this sample. It independently proves that
source-side runtime requests and validated HxcIR observations agree, so it must
not be skipped merely because closure projection is now cheap. Any further
change needs narrower attribution inside that proof.

### Allocation-free UTF-8 ordering on Eval

Compiler output must use one deterministic order even when source discovery,
map iteration, or the host machine changes. Many phases therefore sort module
paths, semantic IDs, generated paths, runtime facts, and diagnostic evidence by
their UTF-8 bytes. The earlier implementation repeated a byte comparator in
many owners. Each comparison converted both strings to new `Bytes` buffers;
an `O(n log n)` sort then performed that allocation thousands of times.

[`CUtf8Order.hx`](../src/reflaxe/c/CUtf8Order.hx) now owns this one ordering
rule. Haxe Eval stores compiler strings as UTF-8 and its native String
comparison produces the same lexicographic order, so the compiler can compare
those existing strings without allocating temporary buffers. Other Haxe hosts
retain an explicit byte walk. A focused differential check uses ASCII and
two-, three-, and four-byte Unicode values to compare the Eval shortcut with an
independent byte comparator. Existing reversed-discovery and raw-byte artifact
tests still own the complete output-order contract.

The first measured migration covered the largest lowering, validation,
project, runtime, naming, and output owners; the completed migration then moved
the remaining compiler comparators to the same owner. These are one-sample
diagnostics, not a stable benchmark: the first migrated sample met the
profiler's representative-host threshold, while the before and completed
samples were contended. Every sample produced the same 227 normal artifacts
with SHA-256 tree digest
`64c1a39e1e8814fc8f0146bbef0acdccfd9a266ba71b2dde5cb91da296515bb1`.

| Full-playable warm-profile value | Before | First migration | Complete migration |
| --- | ---: | ---: | ---: |
| Target CPU | 19.660s | 18.006s | 18.126s |
| Cumulative allocation reported by Eval | 73.728GB | 69.587GB | 68.339GB |
| Maximum observed resident memory | 1.571GB | not recorded here | 1.437GB |
| Haxe-to-generated-C wall | 20.186s | 18.717s | 18.759s |

The completed migration removed about 5.39GB of cumulative temporary
allocation from this request. That decrease is the important structural signal:
the migrated sort callbacks no longer create byte buffers. Wall-time and CPU
direction remain useful diagnostic evidence, but comparable uncontended
repetitions are still required before publishing a percentage speedup or
regression budget.

### Reusing exact nominal body-value plans

Typed-body lowering repeatedly asks what C representation one Haxe value needs.
For an exact, non-generic class or interface such as `GameSession`, that answer
is the same at every field, parameter, local, and expression that mentions the
type. The older path still replayed alias, callable, collection, primitive, and
nominal classification before reaching the already-canonical class or
interface registry.

`CBodyAggregateRegistry` now keeps the resulting immutable value plan for the
rest of the compilation request. This is deliberately a narrow cache, not a
shortcut around type checking:

- exact `TInst` class and interface types may reuse the plan;
- core `Array`, generic instances, imported or extern types, enums, typedefs,
  abstracts, `Null<T>`, and unresolved types continue through the complete
  classifier; and
- the cache stores only the wrapper around the registry-owned representation.
  It does not suppress a use-site diagnostic or create a second class layout.

The full-playable profile records cache hits and first-time plans so a future
change cannot mistake an unused cache for an optimization. The before and after
samples below were both collected on a contended host, so wall time is not used
as proof. Both produced the same 227 normal artifacts with SHA-256 tree digest
`64c1a39e1e8814fc8f0146bbef0acdccfd9a266ba71b2dde5cb91da296515bb1`.

| Full-playable warm-profile value | Before | Exact nominal reuse |
| --- | ---: | ---: |
| Nominal classifications | 5,470 | 5,470 |
| Nominal-classification CPU | 0.935s | 0.747s |
| Exact nominal cache hits / first plans | not recorded | 3,463 / 32 |
| Complete request cumulative allocation | 68.339GB | 67.595GB |
| Complete request CPU | 18.126s | 18.214s |
| Haxe-to-generated-C wall | 18.759s | 18.920s |

The owned nominal work fell by about 188ms, or 20%, and Eval reported about
745MB less cumulative allocation. The broad request CPU and wall values moved
in the opposite direction by less than the variation expected from the
recorded host contention; they remain diagnostics rather than a publishable
end-to-end speed claim.

### Ruling out specialization and coercion comparison

After the safe type-plan caches above, typed-body lowering still accounted for
about 4.4s of CPU. Two high request counts looked suspicious: applying a
function's generic specialization 28,907 times and comparing HxcIR types during
25,150 coercion checks. The profiler now measures the CPU inside those exact
operations, but only when structured profiling is enabled. Normal compilation
does not read the clock for either operation.

One full-playable warm profile on a contended host measured:

| Typed-body operation | Requests | CPU |
| --- | ---: | ---: |
| Apply the current generic specialization | 28,907 | 12ms |
| Serialize and compare coercion HxcIR types | 25,150 | 23ms |
| All recorded type-family classification | 21,722 | 1.291s |
| Source-position conversion | 35,914 | 403ms |
| Complete typed-body lowering | 45,454 source nodes | 4.382s |

The build retained the exact 227-artifact SHA-256 tree digest
`64c1a39e1e8814fc8f0146bbef0acdccfd9a266ba71b2dde5cb91da296515bb1`.
An attempted object-identity shortcut covered 8,727 coercions but reduced the
phase's 14.3GB cumulative allocation by only about 11MB, so it was removed.
The remaining time is distributed through real expression-to-HxcIR
construction and its ownership, lifetime, instruction, and provenance
bookkeeping. A future optimization must attribute one of those owners rather
than treating the two large request counts as cost by themselves.

### Span-lowering compiler-process reuse

The measured time belongs to that feature's exhaustive **test suite**, not to a
single lowering pass or a typical user build. Before `haxe_c-xge.26`, its
runner started 87 independent Haxe processes: 18 report/determinism renders, 36
negative fixtures, two invalid-configuration fixtures, seven
production/determinism builds, and 24 bounds-abort builds. It then ran its
GCC/Clang optimization and sanitizer matrix.

The optimized runner keeps every coordinate and assertion but now performs 54
Haxe requests: 8 retained cold requests and 46 ordered requests through one
isolated compiler server. Together, those paths load the compiler executable 9
times rather than 87. The request topology is executable and fails if a future
edit accidentally changes those counts.

Two changes remove duplicated semantic work:

- one typed fixture now produces all six profile/build HxcIR reports using a
  fresh compiler context for each coordinate, so normal, repeated, and reversed
  discovery require three cold compilations instead of 18; and
- one reachable bounds program per profile/build coordinate contains all four
  local/parameter and upper/negative failure paths. A typed native selector
  links and runs each path separately, proving the correct branch marker,
  `abort()` failure, strict C, and zero `hxrt` symbols with six Haxe builds
  instead of 24.

The exact report renders remain cold because the pinned Haxe compiler's cached
typed tree can retain a narrower—but semantically equivalent—source position
for a one-expression body. Exact HxcIR source locations are evidence, so the
runner does not normalize that difference away. Negative, configuration,
production, repeat, and bounds requests use the serial warm server while
retaining representative cold requests. That ordered sequence remains a
request-leakage test and is intentionally not internally parallelized.
Beads issue `haxe_c-xge.27` owns the source-anchor follow-up.

The first full intermediate topology (72 requests, before bounds batching)
completed locally in 145,336ms. The final 54-request topology completed on the
same checkout and machine in 91,226ms and passed all native checks. The older
285,042ms number above was captured under four-worker contention, so it is not
an apples-to-apples wall-time ratio; the process/load counts are the stable
structural comparison. The final isolated sample is below the initial two-minute
focused-lane objective, while hosted timings still require their own sample.

`test/span-lowering` accepts `--timing-report` through its Python runner. Its
path-free report conforms to
[`span-lowering-timing.schema.json`](specs/span-lowering-timing.schema.json) and
accounts for phase time, cold/warm transport, exit codes, compiler loads, and
total wall time even on a handled failure.

Four simultaneous shards also made the span command about 39 seconds and the
Caxecraft command about 50 seconds slower than an earlier two-worker sample.
That observation is why local execution is now resource-aware: an idle machine
gets the faster four-shard wall time, while a busy machine stops adding pressure.
GitHub shards run on separately provisioned runners and require their own
comparable timing series before budgets are enforced.

Timeouts are containment:

- each isolated toolchain shard has a 30-minute hard ceiling while the new
  baseline is collected;
- the aggregate has a 5-minute ceiling because it performs no compiler work;
- a shard approaching its ceiling requires an owning issue and decomposition
  or optimization, not another silent timeout increase.

### Consolidating Caxecraft host entry points

Caxecraft behavior tests used to have one Python launcher per mechanic even
when each launcher performed the same work: start pinned Haxe, repeat under a
second locale, compare a small success line, and scan reusable Haxe for target
dependencies. The behavior assertions were already Haxe-owned. Six launchers
for inventory, gameplay, the editor, CaxeFlow, the CaxeMap model, and CaxeMap
determinism now use cases in the existing `run_haxe_c_test.py` runner. Their npm
command names and Haxe programs remain unchanged.

The change removes six command files. In the reviewed commit tree, Caxecraft
falls from 22 to 16 tracked Python files and from 16 to 10 distinct Python
files behind its npm command surface. Total tracked Caxecraft Python source
falls from 17,606 to 17,347 lines even after adding the shared case
descriptions.

The same six focused npm commands took 11.30 seconds before the change. An
initial after sample took 14.29 seconds while host load was 15.82 on 12 logical
CPUs and several unrelated Haxe targets were compiling. Review also found that
the first shared implementation needlessly repeated compiler-pin and HXML
argument resolution for scenario-determinism samples. That duplication was
removed: the final command topology keeps the previous cold, locale, negative,
and warm-server request counts. The model/codec command also retains its
ordinary checkout Haxe-shim path; it measured 1.44 seconds under heavy load
against the 1.42-second baseline. The contended pre-fix sample proves only that
the commands completed. The corrected topology took 14.76 seconds while load
remained about 14 on 12 logical CPUs, so that sample is also diagnostic only.

The host did not reach the predeclared representative threshold in two bounded
waiting periods, even after unrelated compiler/test process trees were paused.
Rather than weaken that threshold, the final review executed each deleted
launcher directly from `HEAD` and its replacement back to back against the same
checkout and contention. All six pairs passed. Their old paths totaled 14.90
seconds wall time and 11.38 seconds combined user/system CPU time; the shared
paths totaled 13.71 and 10.95 seconds respectively. This paired evidence shows
that consolidation did not materially slow the focused commands. It is not an
absolute speedup claim, because the one-minute host load ranged from 19.35 to
23.20 on 12 logical CPUs. The specialized asset, content-pack, localization,
graphical Pilot, native C, and sanitizer owners remain separate because they
observe boundaries that Eval cannot.

## Optimization sequence

1. **Partition without weakening.** Keep the serial reference lane, run four
   isolated CI shards, and verify exact partition coverage. This is implemented.
2. **Measure each suite.** Stable per-command/shard timing reports are
   implemented. Collect at least three cold hosted samples before setting
   `p50`/`p95` thresholds.
3. **Remove duplicate snapshot computation.** Beads issue `haxe_c-xge.25` owns
   the implemented focused-owner/catalog split and independent scheduled cold
   audit described above. Parallel execution alone did not count as this step.
4. **Admit local concurrency by declared isolation class.** Four shard-level
   declarations and conservative bounded local execution are implemented;
   individual suite commands remain serial within each shard.
5. **Optimize setup and caches.** Cache only content-addressed artifacts keyed
   by the reviewed Node/Haxe/Reflaxe/native locks. Retain scheduled or release
   cold-cache evidence.
6. **Enforce regression budgets.** Once baselines are stable, fail or require an
   explicit reviewed override for meaningful wall-time, memory, generated-size,
   or runtime regressions.

## Non-goals

- skipping a slow semantic, snapshot, sanitizer, ABI, platform, or security
  gate;
- trusting stale generated output or an unverified cache;
- making every helper a CI job regardless of setup cost;
- hiding failures behind a successful aggregate;
- changing compiler behavior merely to make the harness faster.
