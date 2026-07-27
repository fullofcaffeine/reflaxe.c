# Test feedback-loop and CI performance

Developer waiting time is an engineering constraint. This document turns the
repository directive in [`AGENTS.md`](../AGENTS.md) into an executable lane
design without weakening the evidence contract in [`testing.md`](testing.md).

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

The exhaustive pre-commit fallback also queues native smoke in this same worker
pool. It does not start an unbounded fifth worker: four toolchain shards plus
native smoke are five pieces of work sharing the selected one, two, or four
workers. Native smoke starts as soon as a worker becomes free, so it no longer
waits for every shard to finish. Its isolated log is replayed after the four
shard logs and failures remain attributed to `native`. The canonical serial
`test:toolchain` and standalone `test:native` commands are unchanged.

Local commits now have three explicit routes:

1. A narrow fixture, target-library surface, or focused document runs its
   existing path-owned checks.
2. A known compiler semantic layer or Caxecraft change runs governance plus a
   small, deterministic set of affected owners. The base sentinels type-check
   all target-owned Haxe, validate HxcIR, compile and run the small Hello
   product, and verify snapshot ownership. Staged paths then add only relevant
   owners such as body lowering, runtime planning, spans, or Caxecraft.
3. An unknown compiler subdirectory or cross-cutting test/CI input fails closed
   to the complete bounded shard and native matrix.

This is a cadence boundary, not weaker proof. Every pull request, nightly run,
and release still runs the complete cold matrix; developers can run the same
proof explicitly with
`npm run test:toolchain:parallel -- --with-native`. The local route answers the
smaller question “did this known semantic change preserve its direct owners?”
without pretending that a 61-command repository replay is useful after every
edit. [`select_pre_commit_route.py`](../scripts/ci/select_pre_commit_route.py)
keeps classifications and owner order explicit, deduplicated, fail-closed, and
unit-tested.

An established root compiler module must be named explicitly before it can use
the affected route. `CPhaseTiming.hx` is one such reviewed module; a new
unrecognized root file or subdirectory still takes the exhaustive fallback.
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

The exhaustive fallback still runs governance before the complete canonical
partition and native-smoke queue. It does not omit evidence or run commands
concurrently inside a shard. On a busy host it may choose one worker; safe
resume then avoids discarding completed shards when the same unchanged staged
tree is retried.

### Safe local resume

The hook opts into `--resume` so a retry after one failed or timed-out shard
does not discard the other three successes. A reusable record is not a general
test cache. It is a local, ignored receipt saying one exact shard passed one
exact set of inputs, and it expires after 24 hours.

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
and deterministic pilot input before the broader scenario, editor, and full
generated-C lanes. These commands
make a local gameplay regression cheap to reproduce; the later full lane remains
the integrated authority and is not replaced by them. Water and aquatics use
the shared focused Haxe-to-C runner: their Haxe probes own behavior while the
host only coordinates Eval, native C, exact trace parity, and sanitizers. These
focused lanes run their shared Haxe rules under Eval and generated C, then
perform one strict native build plus an AddressSanitizer/UndefinedBehaviorSanitizer
run when the selected compiler can provide them.

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
   build-script, asset, localization, content-pack, scenario, pinned Haxe, and
   standard-library input;
2. compare the requested configuration, selected native tools, flags, and
   environment with the request that produced the executable;
3. re-hash the exact Raylib/Raygui headers and libraries used by that native
   build; and
4. re-hash the complete generated project, staged assets/content, and linked
   executable.

Only an exact match launches the existing executable. A missing, malformed,
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
and zero links.

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
Game source is deliberately excluded from the server compatibility digest:
Haxe's own module cache observes and invalidates those edits, which is the work
the server is meant to reuse.

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

### Caxecraft target-phase profile and duplicate-body removal

Beads issue `haxe_c-fbq` added an opt-in compiler profiler rather than guessing
from whole-suite wall time. A *phase* here means one named portion of the build,
such as “turn validated HxcIR into structural C bodies.” The current schema-6
`HXC_PROFILE` stream records phases as a checked parent/child tree only when
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

Profile schema 6 records requests, real computations, hits, and CPU time for
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
