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
snapshot de-duplication remains a compute-efficiency task even though snapshot
rendering is no longer serialized behind Caxecraft.

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
| `caxecraft` | the complete split/unity, oracle, native, optimized, and sanitizer showcase lane, followed by the snapshot ownership catalog | its long end-to-end proof receives its own resource budget; the catalog check is non-rendering and takes negligible time |

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

# Run every isolation-approved shard locally with two workers by default.
npm run test:toolchain:parallel

# Override the conservative worker count and retain timing JSON.
npm run test:toolchain:parallel -- --jobs 4 --timing-dir /tmp/hxc-timings

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

`test:toolchain:parallel` therefore runs shards concurrently but never runs the
commands inside one shard concurrently. It defaults to two workers regardless
of host core count; `--jobs` or `HXC_TEST_JOBS` may select one through four.
Each shard writes to an isolated temporary log. Completion progress is printed
at 30-second intervals, then full logs are replayed in canonical shard order so
interleaving cannot hide the first useful failure. All admitted shards finish
to retain independent evidence even when one fails.

The pre-commit hook uses this exhaustive path once when staged changes touch
cross-cutting test infrastructure such as `package.json`, the snapshot
registry, shard runner, CI policy, or their governance tests. That avoids the
old behavior where the same global file matched nearly every focused condition
and serialized all owners one by one. It still runs governance and native
smoke around the exact 38-command partition. Ordinary compiler edits continue
to select only their narrower focused gates.

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
`span-lowering`, not snapshot ownership validation. Its runner currently starts
87 separate Haxe processes with `HAXE_NO_SERVER=1`: 18 report/determinism
renders, 36 negative fixtures, two invalid-configuration fixtures, seven
production/determinism builds, and 24 bounds-abort builds. It then runs its
GCC/Clang optimization and sanitizer matrix. The next optimization should batch
compatible typed fixtures or share validated compiler results, retain a
separate warm-server lifecycle lane, and keep the complete negative/native
contract. Merely deleting matrix coordinates is not an acceptable speedup.
Beads issue `haxe_c-xge.26` owns that follow-up.

Four simultaneous shards also made the span command about 39 seconds and the
Caxecraft command about 50 seconds slower than an earlier two-worker sample.
That contention is why local execution defaults to two workers. GitHub shards
run on separately provisioned runners and require their own comparable timing
series before budgets are enforced.

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
