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
| `caxecraft` | the target-neutral CAXEMAP model contract; complete split/package/unity, oracle, native, optimized, and sanitizer showcase lane; then the snapshot ownership catalog | its long end-to-end proof receives its own resource budget; the model and catalog checks stay separately visible and fast |

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

The pre-commit hook uses this exhaustive path once when staged changes touch
cross-cutting test infrastructure such as `package.json`, the snapshot
registry, shard runner, CI policy, or their governance tests. That avoids the
old behavior where the same global file matched nearly every focused condition
and serialized all owners one by one. It still runs governance and native
smoke around the exact 39-command partition. Ordinary compiler edits continue
to select only their narrower focused gates.

### Safe local resume

The hook opts into `--resume` so a retry after one failed or timed-out shard
does not discard the other three successes. A reusable record is not a general
test cache. It is a local, ignored receipt saying one exact shard passed one
exact set of inputs, and it expires after 24 hours.

The receipt key includes:

- the staged Git tree plus unstaged tracked changes and relevant untracked
  files under compiler, runtime, test, script, specification, and example roots;
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

Receipts live under ignored `.cache/toolchain-shards/`. Logs still appear in
canonical order, and timing summaries distinguish `executedShards` from
`reusedShards`; a historical duration is never presented as current work.
Beads export, formatting, local-path and whitespace checks, staged secret
scanning, governance, and the native smoke lane remain outside this mechanism
and run on every applicable hook invocation. GitHub runs `--run <shard>` in a
clean job without `--resume`, so local evidence can never replace hosted CI.

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
