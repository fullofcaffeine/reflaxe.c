# Fixture and snapshot policy

This document is the human-readable contract for HXC-COMP-014, HXC-QA-001,
and HXC-QA-007. The machine-readable join point is
[`fixture-taxonomy.json`](specs/fixture-taxonomy.json); individual future cases
use [`fixture-case.schema.json`](specs/fixture-case.schema.json).

The taxonomy describes evidence. It does not promote a seed into a supported
compiler capability. Direct HxcIR/C AST fixtures and independent native C/C++
inputs do not prove typed-Haxe lowering. The body-lowering suite is narrower
real TypedExpr-to-HxcIR/C evidence. The function-lowering suite extends it to a
production-emitted primitive private header/source/entry project. The
evaluation-order suite now adds E2.T06 primitive statement/control-flow
evidence; the arithmetic suite adds generated UB-safe operation, sanitizer,
and optimized-shape evidence. The span suite adds local fixed-array/view,
bounds-policy, and runtime-free iteration evidence. The runtime-feature suite
adds deterministic graph/policy and selective provisional native-seed packaging
evidence. None proves broader language, standard-library, public ABI, or
generated-Haxe `hxrt` support.

## Canonical lanes

| Lane | Canonical directory | Required evidence | Current state |
| --- | --- | --- | --- |
| Positive | `test/positive/` | Success exit plus exact semantic assertions and declared artifacts/effects | Active through mapped M0 suites, including primitive body, static-function, evaluation-order, arithmetic, and fixed-array/span lowering |
| Negative | `test/negative/` | Failure exit, stable diagnostic ID/essential fields/source span, and no plausible output | Active through exact `HXC1001` unsupported/unreachable body, signature, argument, general-array, empty-array, and lookalike-intrinsic boundaries plus invalid build configuration |
| AST/IR | `test/ast/` | Deterministic structural model, validator result, and native compile/run when C is produced | Active through `c_ast`, `declaration_plan`, `project_emitter`, `hxc_ir`, and lowering snapshots |
| Snapshot | `test/snapshot/` | Byte-exact text or semantic JSON, deterministic rerender, and reviewable diff | Active; existing expected trees remain mapped in place |
| Runtime | `test/runtime/` | Exit/stdout/stderr, runtime-plan effects, strict native build, and sanitizers where eligible | Runtime-free generated-body/span execution, bounds fail-stop behavior, arithmetic UBSan, deterministic feature closure, and selective provisional native-seed packages; no generated-Haxe `hxrt` proof yet |
| Differential | `test/differential/` | Named oracle, normalized oracle/target traces, deterministic seed, and allowed normalizations | Active for evaluation-order/control-flow and arithmetic Eval versus generated strict C, plus static initialization against the pinned Haxe JavaScript generator; HxcIR indexing oracle remains semantic-only |
| ABI | `test/abi/` | Headers, symbols/layouts, ownership/calling convention, and external consumers | Independent native seed only |
| Performance | `test/performance/` | Versioned measurements, units, inputs/toolchain/hardware/variance, baseline, and budget decision | Contract only |

The canonical directories are stable homes for new cases. Existing focused
suites stay in their current paths and are mapped by the catalog; moving them
would create noise without improving their evidence.

The mapped `diagnostics` suite is both positive and negative policy evidence.
It compares the typed Haxe definitions with the machine catalog, validates two
byte-identical structured renders, and proves that unregistered IDs, absolute
paths, and forbidden severity changes are rejected. It asserts stable IDs and
essential fields rather than treating complete prose as an API. See
[diagnostic contract](diagnostics.md).

## Case contract

A new case contains `case.json` conforming to the checked-in case schema. It
declares:

- a stable ID, role (`fixture` or `example`), lane, and current status;
- every Haxe/native/config source input;
- an argument-array runner command, case-relative working directory, timeout,
  and whether network access is required;
- the exact expected exit code;
- at least one semantic assertion: stdout, stderr, diagnostic IDs, artifacts,
  runtime plan, oracle output, or metrics.

Runners execute from the declared working directory, default to no network,
use bounded timeouts, preserve stdout/stderr/exit status separately, and never
silently skip a required toolchain. Normalization is permitted only when the
case names the field and the underlying Haxe contract permits it.

Expected outputs are owned by the test case, not by generated-output caches.
Generated scratch/output trees stay ignored. A snapshot asserts source shape;
runtime or differential behavior requires its own lane assertion as well.

## Snapshot workflow

Check every registered expected tree without writing:

```sh
npm run snapshots:check
```

Regenerate one suite intentionally:

```sh
npm run snapshots:update -- --suite c-ast
```

An intentional full refresh requires the visibly broader command:

```sh
npm run snapshots:update -- --all
```

The updater renders twice, rejects nondeterministic output, prints a unified
semantic diff before any write, and then updates only catalog-owned expected
roots with per-file atomic replacements. JSON comparisons ignore object-key
formatting but display a canonical structural diff. C, headers, and HxcIR remain
exact text. Stale files
inside a managed expected root are shown as deletions. Update mode refuses to
run when `CI` is set.

Never hand-edit an expected file merely to make a failure disappear. Review the
printed semantic change, update through the command, inspect `git diff`, and run
the owning suite plus every required native/oracle gate. CI only checks; it
never blesses output.

The registered snapshot selectors are:

- `bootstrap`
- `typed-c`
- `typed-ast`
- `c-ast`
- `declaration-plan`
- `symbol-registry`
- `project-emitter`
- `hxc-ir`
- `primitive-semantics`
- `stdlib-ledger`
- `body-lowering`
- `function-lowering`
- `evaluation-order`
- `arithmetic-semantics`
- `runtime-feature-graph`
- `span-lowering`

List them from the executable registry with:

```sh
python3 scripts/test/snapshots.py --list
```

The `typed-ast` suite is the compile-backed frontend boundary. It captures real
custom-target input, classifies modules/declarations/externs/typedefs/abstracts,
metadata, fields, expressions, and the entry point, and compares forward,
reversed, repeated, cold, and compiler-server reports. Its expected JSON is a
pre-body-lowering inventory; it does not itself prove HxcIR or C lowering. The
internal report and reverse-order defines are test/diagnostic seams, not
user-facing compiler configuration. See [typed-AST input boundary](typed-ast-input.md).

`test/c_ast` is the direct AST fixture-compiler and native round-trip boundary.
One typed Haxe macro constructs declarations, precedence expressions,
initializers, control flow, an explicit GNU-attribute unit, and a guarded
header/source/consumer project, then writes the complete tree through Reflaxe
ownership. Two unrelated absolute output roots must be byte-identical and match
the centrally owned snapshots. The canonical case is
`test/ast/c-ast-roundtrip/case.json`.

The shared `scripts/test/c_fixture_harness.py` consumes closed project records,
never shell command strings. It verifies the requested `gcc` or `clang`
identity, compiles each source independently with the strict C11 warning floor,
links, runs, and captures a schema-1 report containing the compiler version,
normalized argument arrays, input hashes, exit codes, stdout, and stderr. Save
that report explicitly when reproducing a native result:

```sh
python3 test/c_ast/run.py --toolchain clang --report /tmp/c-ast-report.json
python3 test/c_ast/run.py --native-only --toolchain gcc
```

`--native-only` consumes the checked-in AST outputs and is the entry used by
the required native matrix. Local `auto` mode runs every available
identity-matching family and requires at least one. Report normalization names
only `${CC}`, `${FIXTURE_ROOT}`, `${BUILD_ROOT}`, and `${REPOSITORY_ROOT}`;
native compiler versions remain deliberately factual. This harness is reusable
by later lowering suites, but direct AST construction still proves no
typed-Haxe lowering semantics.

`test/primitive_semantics` is the focused positive/snapshot contract for
ADR 0008. A typed initialization macro classifies real pinned-compiler fields
in portable and metal, while a second typed fixture renders the exact mapping,
conversion, floating, nullability, and zero-runtime table twice. Its independent
strict-C11 probe runs under available GCC and Clang at O0/O2. That probe proves
the accepted algorithms and target prerequisites only; it is not generated
Haxe output. E2.T02 separately wires the admitted primitives into real bodies.
See the
[primitive semantic contract](primitive-semantics.md).

`test/stdlib_ledger` is the focused positive/negative/snapshot contract for the
pinned standard-library inventory. It renders the real C common surface and the
separately labeled thread/atomic declaration-only surface twice, then requires
exactly one ownership rule for every applicable source module and a stable row
for every public typed API. In-memory negatives reject missing or overlapping
ownership, stale exact overrides, unknown runtime features, missing diagnostic
policy, and evidence-free partial/conformant status. The central snapshot owner
manages both the semantic JSON authority and exact generated CSV. This is
ownership and drift evidence, not generated-C stdlib behavior. See the
[standard-library ledger](stdlib-ledger.md).

`test/body_lowering` is the focused positive/negative/snapshot/runtime suite for
the first real pinned-Haxe `TypedExpr -> HxcIR -> structural C` path. It renders
twice, reverses discovery order, compares portable and metal, asserts exact
source spans and shadow-safe finalized names, and compiles/runs both optional
line-mapped and ordinary strict C11 at O0/O2. Unsupported bodies fail at the
first typed node with `HXC1001`; an admitted production body now emits the
runtime-free E2.T03 project. See
[primitive function-body lowering](body-lowering.md).

`test/function_lowering` is the focused positive/negative/snapshot/runtime suite
for typed primitive parameters, explicit HxcIR conversion/call order, direct
static calls, direct self-tail lowering, mutually recursive source partitioning,
recursive prototype planning, and executable entry emission. It
checks deterministic portable/metal renders, scoped default/optional/rest
`HXC1001`, byte-identical production roots, explicit `hxc_runtime=none`, analyzed
empty runtime/ABI/stdlib sidecars, and no `hxrt` artifact or symbol. Required CI
lanes compile and run its checked-in and production-generated strict C under GCC
and Clang. See [static function lowering](function-lowering.md).

`test/evaluation_order` is the focused positive/snapshot/runtime/differential
suite for E2.T04 and E2.T06. It proves source-backed call arguments,
assignments, primitive static fields, lazy Boolean operators, value ternaries,
statement `if`/`else`, nested pre/post-test loops, range-loop preprocessing,
innermost `break`/`continue`, `Int` statement/value switches, and unsigned
prefix/postfix increments through explicit HxcIR and structural C. It asserts
that every C switch arm jumps to a typed target, each subject executes once,
and the production value-switch carrier cannot expose uninitialized storage.
It checks the pre-existing general indexed compound-assignment HxcIR separately
from the narrow fixed-array/span source slice, verifies the stable-value
temporary proof, compares an Eval oracle with generated projects across both
required and skipped lazy operands, and runs strict GCC/Clang at O0/O2 with a
positive zero-runtime plan.
See [explicit evaluation
order](evaluation-order.md).

`test/static_initialization` is the focused
positive/negative/snapshot/runtime/differential suite for E2.T09. Its lexically
reversed three-class graph proves source-positioned dependency reasons,
dependency-first type order, class-before-field phases, field source order,
deferred HxcIR globals, and exactly one generated bootstrap call before Haxe
entry. Repeated and reversed typed-module inputs must produce identical reports
and output roots. A two-class cycle emits the same exact `HXC1002` path with no
artifact. The pinned Haxe JavaScript generator supplies the phase-order oracle,
and an independent C harness checks the initialized result under strict GCC and
Clang at O0/O2 with portable, metal, and runtime-none no-`hxrt` evidence. See
[deterministic static initialization](static-initialization.md).

`test/arithmetic_semantics` is the focused
positive/snapshot/runtime/differential suite for E2.T05. It lowers real typed
`Int`, `UInt`, and `Float` operators, compound/update forms, and `Std.int` into
explicit HxcIR implementation decisions. Its managed snapshots show the helper
dependency closure, structural private header, direct unsigned fast paths,
exact `m` build fact, and finalized symbols. It compares the common defined
subset with Eval, then checks target refinements for overflow, zero divisors,
`INT32_MIN / -1`, negative and oversized shifts, bit operations, NaN,
infinities, signed zero, and floating-to-integer bounds. Required GCC and Clang
lanes run `-O0`, `-O2`, and eligible UBSan builds and reject an out-of-line
helper in optimized assembly. See [UB-safe primitive
arithmetic](arithmetic-semantics.md).

`test/span_lowering` is the focused positive/negative/snapshot/runtime suite for
E2.T08. It admits typed nonempty `CArray` literals, mutable and const local span
borrows, direct indexing, and exact typed span `for` iteration. Repeated and
reversed renders cover both profiles and all three build modes; checked,
static-proof, and loop-guarded bounds policies remain visible in HxcIR. The
suite executes in-range behavior at O0/O2, executes both negative and upper
out-of-range fail-stop paths in the six-way configuration matrix, rejects
general/empty arrays and lookalike intrinsic names without output, validates the
profile-aware `hxc_build` diagnostic, and inspects runtime-none links for zero
`hxrt` symbols. See [fixed arrays and span-based iteration](span-lowering.md).

`test/runtime/runtime-feature-graph` is the focused
positive/negative/AST/snapshot/runtime suite for E4.T01. It renders the typed
catalog and plans twice, reverses definition/reservation/reason input, and
checks canonical cycle and unknown-dependency failures. Every selected root,
transitive feature, and dependency edge must retain a source-reason ID. Policy
fixtures cover `auto|minimal|none`, portable and metal presets, manual
confirmation/forbid rules, environments, reserved features, and rejection of
provisional seeds from generated-Haxe planning. The packager performs zero
reads for an empty plan and materializes exact alloc-only and string closures;
the full focused gate proves those files came from two byte-identical Haxe
renders. Strict GCC and Clang jobs separately rebuild both closures from the
validated checked-in catalog and plan, without needing Haxe in the native-only
environment, then reject string symbols in the alloc-only link. See
[runtime feature planning and selective packaging](runtime-feature-planning.md).

## Examples are product proofs, not implicit tests

The current checkout has no `examples/` directory; that inventory mismatch is
tracked separately. When examples are restored or added, every immediate
`examples/<name>/` directory must contain a schema-valid `case.json` with
`role: "example"` and explicit commands and expected assertions.

A test harness may run an example only through that declared case. It must not
glob `examples/**`, infer success from compilation alone, or treat README prose
as an assertion. Examples may have broader product narratives, but reusable
compiler regressions belong in a focused fixture when that produces a smaller,
more diagnostic proof.

## Policy validation

Run:

```sh
npm run test:fixture-policy
```

The guard validates the catalog/schema shape, all canonical directories,
registered runners and expected roots, complete ownership of existing expected
files, snapshot-registry parity, package/pre-commit/CI wiring, and the example
manifest rule. It is part of `npm test` and the pre-commit path for relevant
changes.

`test/symbol_registry` is the focused positive/negative/snapshot suite for the
schema-1 `hxc.symbols.json` shape. It renders twice, reverses request and typed
declaration discovery order inside the Haxe fixture, validates exact-name
failures and both collision origins, feeds finalized defaults into declaration
planning, rejects host paths, and asserts an empty typed-contract runtime feature
set. It proves naming analysis only; it does not claim production C emission.

`test/project_emitter` is the negative/AST/snapshot/runtime suite for typed
schema-1 project packaging, neutral build planning, optional adapters, and the
Reflaxe ownership boundary. It renders fresh projects
under unrelated absolute roots, reversed discovery, a fixed locale, CRLF inputs,
and real warm compiler-server reuse. Its raw-byte comparator reports the first
UTF-8-ordered artifact difference and byte offset; normal-artifact scans reject
host paths, carriage returns, timestamps, random IDs, and unstable JSON keys.
The suite validates every SHA-256, verifies unchanged artifact mtimes, and
proves a full-to-renamed-symbol transition removes every prior owned payload
while preserving an unlisted user file. It rejects traversal, descendant
symlinks, unowned destinations, duplicate paths, malformed ownership JSON,
non-canonical line endings, adapter-sensitive source bytes, malformed or
conflicting typed build facts, and premature lowered-program status before any
partial write. Reflaxe invocation/activity metadata is validated separately. The
required GCC/Clang adapter lanes consume the same manifest through a direct
argument-array build, CMake, and Meson, then run the structural project plus
both C AST/printer corpora from paths containing spaces and apostrophes. An
adversarial string definition proves semicolons, generator-expression spelling,
quotes, and backslashes cannot become build-language syntax. The checked-in
headers and C sources also compile and run in the native matrix. They are built
directly by a test macro, so the suite remains separate from the
production generated-Haxe primitive project and exact-`HXC1001` unsupported
boundaries. See [project-emission
boundary](project-emission.md).
