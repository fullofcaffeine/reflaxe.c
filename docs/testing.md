# Fixture and snapshot policy

This document is the human-readable contract for HXC-COMP-014, HXC-QA-001,
and HXC-QA-007. The machine-readable join point is
[`fixture-taxonomy.json`](specs/fixture-taxonomy.json); individual cases
use [`fixture-case.schema.json`](specs/fixture-case.schema.json).

The taxonomy describes evidence. It does not promote a seed into a supported
compiler capability. Direct HxcIR/C AST fixtures and independent native C/C++
inputs do not prove typed-Haxe lowering. The body-lowering suite is narrower
real TypedExpr-to-HxcIR/C evidence. The function-lowering suite extends it to a
production-emitted primitive private header/source/entry project. The
evaluation-order suite now adds E2.T06 primitive statement/control-flow
evidence; the arithmetic suite adds generated UB-safe operation, sanitizer,
and optimized-shape evidence. The primitive-differential suite adds a
reproducible generated corpus, exact Eval comparison, mismatch reduction,
ledgered target refinements, and combined ASan/UBSan evidence. The span suite
adds local fixed-array/view, bounds-policy, and runtime-free iteration evidence.
The aggregate suite adds real closed-record structural deduplication, ordered
construction, explicit copy/address semantics, private layout assertions, and
independent C/C++17 layout agreement.
The runtime-feature suite
adds deterministic graph/policy, selective provisional native-seed packaging,
and the exact compiler-selected literal-output closure. The string-runtime suite
adds a bounded native UTF-8/scalar/CString contract plus an Eval differential
trace. The array-runtime suite adds bounded native primitive/reference growth,
aliasing, lifecycle, and failure evidence plus a common Eval mutation trace.
The string-output suite adds the narrow generated-Haxe `Sys.println` and
default `trace` proof. The declared `examples/hello` product proof composes that
same reusable compiler slice into the first ordinary Haxe-to-C executable. None
proves broader language, standard-library, public ABI, general String, or
general `hxrt` support.

## Canonical lanes

| Lane | Canonical directory | Required evidence | Current state |
| --- | --- | --- | --- |
| Positive | `test/positive/` | Success exit plus exact semantic assertions and declared artifacts/effects | Active through mapped M0/E3 suites and the declared hello example, including primitive body, static-function, aggregate, evaluation-order, arithmetic/differential, fixed-array/span, and literal-output lowering |
| Negative | `test/negative/` | Failure exit, stable diagnostic ID/essential fields/source span, and no plausible output | Active through exact `HXC1001` unsupported/unreachable body, signature, argument, aggregate identity/mutation/`Void`/`Dynamic`, general-array, empty-array, nonliteral output, and lookalike-intrinsic boundaries plus invalid build configuration |
| AST/IR | `test/ast/` | Deterministic structural model, validator result, and native compile/run when C is produced | Active through `c_ast`, `declaration_plan`, `project_emitter`, `hxc_ir`, and lowering snapshots |
| Snapshot | `test/snapshot/` | Byte-exact text or semantic JSON, deterministic rerender, and reviewable diff | Active; existing focused trees and the hello generated baseline are mapped explicitly |
| Runtime | `test/runtime/` | Exit/stdout/stderr, runtime-plan effects, strict native build, and sanitizers where eligible | Runtime-free generated-body/span execution, fixed arithmetic UBSan, seeded primitive ASan/UBSan, selective native-seed packages, allocator/string/array native contracts, generated literal output, and the hello executable |
| Differential | `test/differential/` | Named oracle, normalized oracle/target traces, deterministic seed, and allowed normalizations | Active for evaluation order, fixed and seeded arithmetic, static initialization, native string scalars and array mutation, generated literal output, and exact hello stdout against pinned Haxe oracles |
| ABI | `test/abi/` | Headers, symbols/layouts, ownership/calling convention, and external consumers | Hardened internal allocator, array lifecycle, and borrowed/owned CString seeds plus independent interop seeds; no generated public ABI |
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
- `aggregate-lowering`
- `evaluation-order`
- `arithmetic-semantics`
- `primitive-differential`
- `runtime-feature-graph`
- `string-output`
- `hello`
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

`test/aggregate_lowering` is the focused E3.T01
positive/negative/snapshot/runtime suite. Two differently ordered aliases must
deduplicate to one C struct, nested records must emit dependency-first, and
object-literal effects must remain in source order before canonical named
construction. HxcIR snapshots distinguish by-value local copies, parameter
projections, and local field address/dereference. Generated private layout
assertions compile under strict C11, while an independent C provider and C++17
consumer compare exact size, alignment, offsets, and copies at O0/O2 under
GCC/G++ and Clang/Clang++. Production roots are byte-identical and runtime-free;
identity/equality, mutation, `Void`, `Dynamic`, and metal packed-layout cases fail
closed without artifacts. See [closed anonymous-record
lowering](aggregate-lowering.md).

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

`test/primitive_differential` is the focused
positive/snapshot/runtime/differential suite for E2.T11. A versioned SplitMix64
seed deterministically builds 128 boundary-biased calls across the admitted
`Int`, `UInt`, finite `Float`, conversion, shift, bit, arithmetic, and comparison
surface. It snapshots both the corpus and ordinary generated Haxe fixture,
compares exact common-domain `Int`/`UInt`/`Bool` traces with pinned Eval, and
checks production artifacts across unrelated roots with an empty runtime plan.
Target-specific zero-modulo and positive `Std.int` overflow results are allowed
only through the schema-backed divergence ledger and remain executable on both
sides. Only the non-finite positive-infinity Eval conversion uses the ledger's
closed host-dependent-Int32 normalization; the finite overflow observation and
every generated-C value remain exact. A stored reducer regression proves that a
mismatch becomes one stable typed replay case.
Required GCC and Clang lanes consume the checked-in C without Haxe at O0/O2 and
under combined AddressSanitizer/UndefinedBehaviorSanitizer. See the
[seeded primitive differential suite](primitive-differential.md).

`test/span_lowering` is the focused positive/negative/snapshot/runtime suite for
E2.T08. It admits typed nonempty `CArray` literals, mutable and const local span
borrows, direct indexing, and exact typed span `for` iteration. Repeated and
reversed renders cover both profiles and all three build modes; checked,
static-proof, and loop-guarded bounds policies remain visible in HxcIR. The
suite executes in-range behavior at O0/O2, executes both negative and upper
out-of-range fail-stop paths in the six-way configuration matrix, rejects
general/empty arrays and lookalike intrinsic names without output, validates the
profile-aware `hxc_build` diagnostic, and inspects runtime-none links for zero
`hxrt` symbols. Its source also contains an unreachable String-typed function;
the schema-2 plan proves that merely seeing that declaration adds no runtime
intent, helper, artifact, or symbol. See
[fixed arrays and span-based iteration](span-lowering.md).

`test/runtime/runtime-feature-graph` is the focused
positive/negative/AST/snapshot/runtime suite for E4.T01/E4.T10. It renders the typed
catalog and plans twice, reverses definition/reservation/reason input, and
checks canonical cycle and unknown-dependency failures. Every selected root,
transitive feature, and dependency edge must retain a source-reason ID. Policy
fixtures cover `auto|minimal|none`, portable and metal presets, manual
confirmation/forbid rules, environments, reserved features, and rejection of
native-only features from generated-Haxe planning. Runtime-none fixtures retain
all sorted blockers and root-to-leaf dependency chains, while missing,
runtime-intent-bearing, or tampered positive proofs fail with `HXC9000` before
an artifact read. The packager performs zero reads for an empty plan and materializes exact alloc-only, array, full-string, and
compiler-selected literal-I/O closures; the full focused gate proves those files
came from two byte-identical Haxe renders. Strict GCC and Clang jobs separately rebuild all closures from the
validated checked-in catalog and plan, without needing Haxe in the native-only
environment, then reject string symbols in the alloc-only link. See
[runtime feature planning and selective packaging](runtime-feature-planning.md).

`test/abi/allocator-contract` registers the E4.T02 internal allocator boundary.
The native matrix proves checked overflow and zero-size behavior, hosted
over-alignment, failure-atomic resize/out-parameters, optional-reallocate
fallback, identity-preserving owner moves, and disposal through the stored
allocator, then repeats the hosted path under address/undefined sanitizers. Its
freestanding object is inspected for undefined libc allocation symbols before
a static-arena/failure-injection fixture is linked and run. A C layout producer
and C++17 consumer compare callback types plus every allocator and owner size,
alignment, and field offset. This is an internal native-seed contract, not
generated-Haxe runtime selection or public ABI stability. See
[allocator ownership](allocator-abi.md).

`test/differential/string-runtime` registers the E4.T03 internal string
boundary across differential, runtime, and ABI evidence lanes. The pinned Haxe
Eval oracle supplies the scalar trace for BMP/non-BMP length, scalar access,
embedded NUL, composed/decomposed text, and slicing. An independent C fixture
then covers checked and maximal-subpart lossy UTF-8, scalar bounds, comparison,
stable hashing, allocation-free literals/slices/compare, exact owned allocation,
mutable-builder aliasing and failure atomicity, allocator identity, and distinct
borrowed/owned CString lifetime rules. GCC and Clang run it at O0/O2 and under
address/undefined sanitizers, and link inspection rejects object, collector,
reflection, and dynamic symbol families. This is native runtime evidence, not
generated-Haxe `String` lowering or public layout stability. See the
[string runtime contract](string-runtime.md).

`test/differential/array-runtime` registers the E4.T04 internal array boundary
across differential, runtime, and ABI evidence lanes. A static custom allocator
forces every capacity growth to relocate and injects allocation failure. The C
fixture covers primitive and reference elements, exact-slot aliasing across
growth and insertion shifts, checked capacity overflow, failure-atomic reserve,
retain-before-release assignment, balanced reverse destruction, and rollback
after failed insert/partial resize construction. GCC and Clang run it at O0/O2
and under address/undefined sanitizers, while link inspection rejects string,
object, collector, reflection, and dynamic symbol families. A pinned Eval trace
checks the common mutation sequence; static-target default initialization is
asserted directly because Eval is a dynamic target. This proves the native
runtime slice, not generated Haxe `Array<T>` lowering, full collection parity,
or a public layout. See the [array runtime contract](array-runtime.md).

`test/string_output` is the focused E2.T07
positive/negative/AST/snapshot/runtime/differential suite. It lowers real
compiler-known ASCII, non-ASCII, embedded-NUL, and default-trace literals through
schema-3 HxcIR; checks exact byte lengths, runtime root reasons, stdlib reachability,
and the `runtime-base + status + string-literal + io` closure; and compares the
generated executable's raw stdout with Eval. Portable `auto` and metal `minimal`
both pass, `runtime=none` and freestanding fail before output, and diagnostic
`off|summary|warn` changes presentation only. Strict GCC/Clang O0/O2 runs prove
the bytes, while a closed stdout descriptor proves the generated caller aborts
on write/flush failure. Nonliteral String values, `Dynamic`, `Sys.print`, and
custom trace information remain exact `HXC1001` boundaries.

`examples/hello` is the focused E2.T10 product-level composition of that
literal-output support. Its ordinary `Main.hx` and `build.hxml` enter the real
custom target; no example-specific compiler branch or raw C is involved. The
runner compares exact `Hello from hxc\n` bytes with Haxe Eval, verifies that the
single `Main.hx` runtime reason reaches all four selected feature records,
compares normal artifacts across output roots, caller directories, and locales,
and checks the centrally owned readable HxcIR/header/C/runtime-plan/stdlib
baseline. Full mode builds generated output under available GCC/Clang at O0/O2;
the required native matrix uses `--native-only` to rebuild the checked-in
generated baseline under separately required GCC and Clang jobs without needing
Haxe in those jobs.

## Examples are product proofs, not implicit tests

The examples policy is active with one immediate child: `examples/hello`. Its
schema-valid `case.json` has `role: "example"`, an argument-array runner, an
exact expected exit, generated artifacts, runtime-plan evidence, and a named
Haxe oracle. Every additional immediate `examples/<name>/` directory must meet
the same declaration rule before any harness may run it.

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
registered runners and expected roots, complete ownership of existing test and
example expected files, snapshot-registry parity, package/pre-commit/CI wiring,
and the example manifest rule. It is part of `npm test` and the pre-commit path
for relevant changes.

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
