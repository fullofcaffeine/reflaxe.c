# Haxe compilation-server audit

- Audit date: 2026-07-27
- Haxe carrier: exact `5.0.0-preview.1` pin from `.haxerc`
- Compiler source reviewed:
  `../haxe.compilerdev.reference/haxe` at the revision recorded in
  `docs/specs/toolchain-lock.json`
- Comparative repositories: `../haxe.elixir.codex` and `../haxe.ocaml`
- Implementation owners: Beads `haxe_c-5sd.8.2` and `haxe_c-5sd.8.3`

## Question and outcome

Should haxe.c use Haxe's long-lived compilation server in its normal
development loop, and what may safely survive between requests?

Yes, for changed-Haxe development requests, after haxe.c owns the server
lifecycle and proves cold/server parity. The server is a useful **frontend
cache**: it can retain parsed files, eligible typed modules, dependency data,
and macro setup. It is not an incremental haxe.c backend. Every request must
still create a fresh `CompilationContext` and perform the required HxcIR
construction, validation, C AST (CAST) projection, runtime/project planning,
printing, and output transaction.

The development order is therefore:

1. an exact unchanged-build hit launches the already-validated executable
   without starting Haxe;
2. a changed-Haxe request uses a compatible repository-owned Haxe server;
3. haxe.c runs a fresh target request and publishes a complete immutable
   generated-project generation; and
4. the native build reuses only objects whose compiler-produced dependency
   closure still matches.

Step 1 is implemented by `haxe_c-5sd.8.1`. Steps 2 and 3 are implemented by
`haxe_c-5sd.8.2`: the focused lifecycle tests own process/cookie safety, while
the exhaustive domain lane owns cold/server artifact parity and interleaved
layout contexts. Step 4 remains planned and owned by `haxe_c-5sd.8.3`.

Keep a visible `--server=off` cold path. Cold compilation remains the
independent correctness reference in continuous integration (CI), even after
the development path enables server reuse.

## What Haxe actually reuses

The pinned Haxe compiler creates one server context containing:

- a persistent `CompilationCache`;
- classpaths grouped by the compile-time-define signature;
- changed-directory observations;
- a request counter; and
- a flag recording whether the macro context has been set up.

This is visible in
`src/compiler/serverCompilationContext.ml:6-34` of the pinned Haxe checkout.
Before every request, Haxe clears changed-directory observations, parser
request state, warning positions, timers, and request statistics
(`serverCompilationContext.ml:44-58`). It deliberately retains the compilation
cache and performs macro setup only once (`serverCompilationContext.ml:60-70`).

For a source file, the parser cache first compares the current file time with
the cached time. A mismatch reparses the file
(`src/compiler/server.ml:46-88`). Cached modules receive a stronger check:
Haxe checks their dependencies and module shadowing, and retypes a module when
the check cannot prove it reusable (`server.ml:765-790`). Compile-time defines
form a signature, and a classpath change under that signature clears cached
directory state and marks the context uninitialized
(`server.ml:795-815`).

The server installs its parse/type hooks once, but `process` resets request
state and calls the high-level compiler entry for every connection
(`server.ml:944-973`). The long-lived loop then accepts the next complete
argument list (`server.ml:975-1032`).

That boundary explains both the benefit and the limit:

- Haxe can avoid some parsing, typing, module loading, and macro setup.
- Haxe does not know how to retain haxe.c's target-owned HxcIR, CAST, runtime
  plan, project plan, generated files, native objects, or executable.

Persisting any of those target-owned values would require a separate complete
dependency and invalidation design. This audit does not authorize it.

## Current haxe.c evidence

### Exact toolchain and arguments

`examples/caxecraft/run.py:298-421` resolves the exact Haxe binary, standard
library, Haxelib repository, and native-library environment selected by
`.haxerc`. It rejects an unsafe pin or a binary whose reported version differs
from the pin.

HaxeShim normally expands nested HXML and scoped library arguments before
starting Haxe. A native server client cannot send HaxeShim's private
multi-version routing argument to the Haxe server, so
`run.py:474-508` asks HaxeShim to resolve the request first, validates the
result, and then sends only native Haxe arguments. Server reuse therefore does
not bypass scoped dependency resolution.

### Ownership and shutdown

The existing test harness reserves an operating-system-selected loopback port,
starts the exact pinned compiler with `--server-listen 127.0.0.1:<port>`,
waits for readiness, and owns the resulting process
(`run.py:1500-1546`). On ordinary exit or interruption it sends a graceful
termination request, waits for five seconds, then kills a server that did not
stop (`run.py:1547-1554`).

`test/governance/test_caxecraft_timing.py:474-565` proves that pin resolution
cannot escape the selected installation, that the listener is loopback-only,
and that interruption still reaches the bounded terminate/kill path.

This harness is deliberately scoped to one exhaustive test run. It is not yet
the persistent per-worktree development owner required by
`haxe_c-5sd.8.2`.

### Request isolation and byte parity

`test/typed_ast/run.py:262-317` sends a rich request followed by two smaller
requests through the same server. It compares the smaller requests with a cold
compile and rejects leaked modules or declarations.

`test/aggregate_lowering/run.py:1116-1166` compares cold, first-server, and
warm-server HxcIR plus the complete generated artifact tree. Caxecraft's
exhaustive determinant check does the equivalent comparison for split,
package, and unity layouts (`examples/caxecraft/run.py:1557-1607`).

These tests prove useful current boundaries: a server request still receives a
fresh haxe.c compilation context, and admitted repeated requests produce the
same normal artifacts. The persistent development owner must extend that
evidence to mixed success/failure, profile, layout, define, and toolchain
sequences before it becomes the default.

## Measured effect

The measurements below stop after Haxe and haxe.c write generated C. They do
not include C compilation, native linking, or game launch.

An earlier three-cold/three-post-prime runtime-free Caxecraft sample measured:

| Transport | Samples | Median |
| --- | --- | ---: |
| fresh Haxe process | 18.326s, 18.408s, 19.887s | 18.408s |
| owned server after one unmeasured prime | 16.243s, 16.486s, 16.766s | 16.486s |

The server saved about 1.9 seconds, or 10%, by reducing the combined
host/frontend/setup remainder from about 2.7 seconds to 0.35 seconds. It did
not remove the dominant target-owned CAST body work. The exact phase evidence
and its contention qualification are recorded in
`docs/test-performance.md:708-727`.

The larger split/package/unity experiment was heavily contended and found no
useful end-to-end server improvement. It still proved byte-identical output
(`docs/test-performance.md:473-510`). That result does not contradict the
smaller measurement: keeping Haxe alive can save frontend work while unrelated
host contention or much larger target work overwhelms the saving.

The exact unchanged-build path now skips both sides of that comparison. It
performs zero Haxe requests, zero C compilations, and zero links when every
reviewed input and output still matches
(`docs/test-performance.md:512-552`).

A one-Haxe-module edit now has a truthful **generation** inventory, but not yet
a truthful incremental native timing. The opt-in
`reflaxe_c_incremental_input_report` asks Reflaxe which class declarations Haxe
rebuilt on the current server request; it does not claim that non-class module
fields, HxcIR, CAST, or generated files were reused. The Caxecraft edit profiler
then compares those later layers independently.

In the first `Vitals.hx` experiment, Haxe rebuilt 14 of 135 Caxecraft class
declarations. Only one of 531 HxcIR functions had a semantic body change, and
only `Vitals.c` plus the project manifest changed among 229 normal generated
artifacts. All generated headers were byte-identical, leaving one directly
changed source candidate among 84 C translation units. This is actionable
invalidation evidence; it is not an object-cache hit. The profiler labels the
native projection unmeasured because only compiler depfiles plus exact
compiler, flag, header, and library identities can prove which objects and link
product are reusable. `haxe_c-5sd.8.3` owns that proof and its
edit-to-launch budget.

The same experiment found that Haxe's warm graph may report a named
anonymous-record field at a typedef line in one request and at an object-literal
line in another. Normal generated C remained byte-identical, but the optional
HxcIR dump did not. `haxe_c-5sd.8.4.1` therefore blocks per-function backend
reuse until source provenance is exact, revision-bound, and invalidated by
source content. Coarsening the position or ignoring it would make cached
diagnostics and source maps less trustworthy, so it is not an accepted speed
optimization.

## Comparative designs

### Reflaxe.Elixir

`../haxe.elixir.codex/lib/haxe_server.ex` uses a supervised `GenServer` to own
one Haxe process. One-shot compilation does not start it by default; the
long-lived watch task owns it, while explicit environment policy can enable
autostart (`haxe_server.ex:1-92`).

Its useful patterns are:

- a project-root/tool-command cache identity
  (`haxe_server.ex:721-749`);
- opt-in rather than automatic attachment to an external process;
- relocation when the preferred port is occupied
  (`haxe_server.ex:511-664`);
- a bounded readiness probe and request timeout;
- process-tree cleanup for wrappers that spawn the real Haxe child
  (`haxe_server.ex:396-435`); and
- a watcher whose input roots come from the same HXML/resource/extra-input
  model used for freshness
  (`lib/mix/tasks/haxe.watch.ex:96-149` and
  `lib/haxe_build_inputs.ex:1-180`).

The cache-invalidation regression under
`test/snapshot/regression/server_cache_non_class_invalidation/` covers enum,
typedef, abstract, module-rename, configuration, cross-project, macro external
input, and hook-registry changes. This is the right kind of mutation evidence
for a persistent server.

Two details should not be copied:

- its cookie is written directly rather than atomically; and
- a server failure falls back to a direct compile while restarting in the
  background.

haxe.c should retry once in a fresh, explicitly reported mode and must never
launch a previous executable as though it contains the requested edit.

### hxhx / Reflaxe.OCaml

`../haxe.ocaml/scripts/hxhx/haxe-server.sh` owns a repository-scoped server
through explicit state files. It derives a stable per-repository port, records
the resolved Haxe executable, and reuses the process only when that identity
matches (`haxe-server.sh:42-96` and `278-316`).

The stronger pattern is process ownership. The helper records the launcher and
its child process tree together with process-start identities, so a recycled
process identifier is not enough to authorize a kill
(`haxe-server.sh:137-229`). Stop sends `TERM`, waits, escalates to `KILL`, and
removes only its own state (`haxe-server.sh:318-346`).

`scripts/ci/haxe-server-identity-fixture-test.sh` proves same-binary reuse,
binary-change replacement, wrapper-child ownership, launcher-crash cleanup,
and interrupted-start cleanup. The bootstrap lifecycle fixture proves that a
failed compilation preserves its report and either stops or deliberately keeps
the owned server according to policy.

The useful lesson is exact ownership, not the shell implementation. haxe.c
should use a typed schema-versioned state record and atomic publication rather
than several independently written state files.

## Accepted development contract

Beads `haxe_c-5sd.8.2` implements this contract for the Caxecraft runner's
shipped paths. Remaining soak and interruption evidence stays on that issue
until it closes. Later extraction into generic `hxc dev` tooling must preserve
the contract.

### Modes

- `auto`: reuse or start one compatible repository-owned loopback server.
- `off`: use a fresh Haxe process. This is the authoritative cold mode.
- `attach`: connect only to an explicitly supplied endpoint. Never discover
  and attach to an arbitrary listener merely because `--connect --version`
  succeeds.

The exact public spelling may change when the generic `hxc dev` command is
implemented. The semantic distinction must remain.

### Compatibility identity

An automatic server is reusable only when its state matches at least:

- canonical worktree root and output-root identity;
- exact Haxe executable bytes/version and standard-library identity;
- scoped package/Haxelib resolution identity;
- haxe.c and vendored Reflaxe identity;
- macro and target activation sources;
- base HXML, classpath, target, and server-state schema.

Request-specific defines, main class, profile, layout, diagnostics, and output
path still travel with every request. Haxe partitions or invalidates eligible
frontend cache state; haxe.c creates a fresh target context.

Changing the compatible toolchain identity stops only the exactly recorded
owned process and starts a new server. An occupied preferred port causes
relocation. It never authorizes killing an unknown listener.

### Process state and publication

The server record must be atomic and contain the endpoint, owner process
identifier, process-start identity, complete compatibility identity, and
schema. A readiness probe must verify the same compiler identity, not only
that a TCP port accepts connections.

A crash or stale record is a visible miss. The owner either proves the exact
recorded process still matches or quarantines the record and starts a new
server. It must not infer ownership from a process name or port alone.

Generated output is published separately as an immutable, complete generation.
Server success does not authorize reusing a partial output directory, and
server failure does not authorize launching the last successful executable as
the result of the failed request.

### Retry and shutdown

One failed automatic server request may stop the exact owned server and retry
once through a fresh process. The retry and original failure remain visible in
structured timing/diagnostics. A second failure ends the build.

Interrupting startup or a build must leave no newly created unowned process or
selected partial generation. Deliberately persistent project-owned servers
need explicit status and stop operations; test-created servers always stop at
test completion.

### Cache boundary

The persistent Haxe server may own only the reuse implemented and invalidated
by Haxe itself. haxe.c must not retain:

- `TypedExpr` or normalized typed program objects;
- `CompilationContext` or symbol/runtime request state;
- HxcIR or CAST;
- output-ownership decisions or generated-file objects; or
- native objects and links under the server cookie.

Native objects receive their own content-complete depfile-backed keys in
`haxe_c-5sd.8.3`.

## Verification owned by the implementation

The implementation owner must prove:

1. cold A, server A, and repeated server A produce identical normal artifacts
   and diagnostics;
2. A-B-A sequences across define, classpath, profile, runtime, and project
   layout changes restore exact cold-A output;
3. success-failure-success does not leak symbols, runtime features, output
   files, or diagnostics;
4. changing Haxe, haxe.c, Reflaxe, macro, or package identity rejects the old
   server;
5. stale state, an occupied port, an unknown listener, server crash, timeout,
   and interruption recover without killing an unknown process;
6. two worktrees and build variants do not share mutable server or output
   state; and
7. repeated requests do not leak file descriptors or grow memory without a
   bounded explanation.

These are implementation gates for `haxe_c-5sd.8.2`, not reasons to weaken the
existing cold CI proof. The next native task separately measures generated-file
diffs, translation units recompiled, link reuse, and edit-to-launch latency.

## Rejected alternatives

| Alternative | Decision | Why |
| --- | --- | --- |
| Treat the Haxe server as the complete incremental compiler | Rejected | It has no haxe.c HxcIR, CAST, project, output, object, or link cache |
| Persist target-owned mutable compiler state | Rejected | No complete cross-request dependency/invalidation contract exists |
| Enable a new one-request server and stop it immediately | Rejected | It pays lifecycle cost without a later request that can reuse frontend state |
| Attach automatically to any responsive Haxe port | Rejected | Version response alone does not prove worktree, package, target, or ownership compatibility |
| Launch the last good executable after a failed requested build | Rejected | It makes stale code look like the result of the new edit |
| Keep every development compile cold | Rejected as the optimized default | The server has a measured bounded frontend benefit and cold remains available for evidence |
| Make warm-server output the only correctness oracle | Rejected | A stale cache could otherwise become the only reason the build passes |

The practical rule is simple: reuse Haxe's own frontend cache, rebuild haxe.c's
request-owned semantics, publish only complete output, and keep the cold path
close enough to detect every false hit.
