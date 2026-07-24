# reflaxe.c

> Haxe's missing C target: readable output, explicit ABI control, and honest
> runtime costs.

`reflaxe.c` is an experimental [Reflaxe](https://github.com/SomeRanDev/reflaxe)
target for compiling Haxe to C through the normal Haxe/HXML workflow. The PRD
separately proposes an optional future `hxc` orchestration tool for project builds,
diagnostics, bindings, exports, and other end-to-end workflows; it is not a
replacement Haxe compiler or a requirement imposed by Reflaxe.

**Project status: M0 architecture scaffold with primitive and literal-output
executable slices.**
The complete target-owned Haxe source graph type-checks. Typed primitive static
functions, arguments, conversions, calls, assignments, static fields, lazy and
value/statement control flow, UB-safe arithmetic, literal and bounded zero-
initialized local fixed arrays, checked span access, direct exact-width span
iteration, and dependency-ordered class/static
initialization now pass through
explicitly sequenced validated HxcIR and emit an owned runtime-free strict-C11
project with a private header and `int main(void)`. Compiler-known String
literals additionally support the exact hosted `Sys.println`/default `trace`
path through a selective runtime closure. This is not general Haxe or
standard-library support; every broader compiler path still fails closed
instead of producing plausible but incorrect C.

## Why another native target?

Haxe already has excellent native options. `reflaxe.c` is exploring a distinct
space:

- idiomatic, inspectable C as a first-class output;
- direct access to C libraries, embedded SDKs, and WebAssembly toolchains;
- stable C ABI exports that many languages can consume;
- explicit layout, ownership, allocator, and calling-convention control;
- a runtime-free subset where the source program genuinely permits one.

The goal is not to disguise C++ as C or compete with hxcpp on its home turf.
Arbitrary C++ APIs require an existing or generated `extern "C"` shim.

## Design direction

One compiler pipeline will serve two explicit contracts:

- **`portable`** preserves Haxe semantics and pulls in narrowly scoped runtime
  features only when direct C cannot preserve the required behavior.
- **`metal`** exposes C-native layout and ownership constraints, rejecting
  hidden allocation or broad runtime fallback unless explicitly enabled.

Runtime policy and target environment remain independent of that profile:

```text
-D reflaxe_c_profile=portable|metal
-D hxc_runtime=auto|minimal|none
-D hxc_runtime_diagnostics=off|summary|warn
-D hxc_environment=hosted|freestanding|wasi|emscripten
-D hxc_build=debug|release|minsizerel
```

Direct idiomatic C is the first lowering choice in both profiles. The compiler
may next generate a program-local specialization; `hxrt` is the last semantic
fallback and is selected feature by feature with source reasons. There is no
unconditional runtime core. A proven runtime-free build contains no `hxrt`
header, source, define, library, or symbol.

This hierarchy is also the hard standard-library rule. Statically knowable work
belongs in `hxc`: representation selection, specialization, constant/static
dispatch, escape/lifetime analysis, layout/table construction, and dead-feature
elimination. Standard-library operations lower to idiomatic C or a compatible C
library/intrinsic first, a program-local specialized helper second, and only a
narrow justified optimized `hxrt` feature last. The generated E5
[standard-library ledger](docs/stdlib-ledger.md) now checks 183 applicable
pinned-Haxe source modules and 2,311 public typed API rows, with explicit
profile/environment/runtime/test/diagnostic ownership and no catch-all. This is
planning evidence, not broad stdlib support: only the exact `Std.int` row is
currently conformant. Literal-only `Sys.println` and default `trace`, plus ten
bounded `haxe.io.Bytes` declarations, are partial. String/Unicode, collections,
unadmitted Bytes operations, BytesBuffer/Input/Output,
Std/Math/Type/Reflect, JSON, regex, time, sys, networking, and threading remain
owned roadmap slices.

With no explicit override, portable uses `auto` plus an aggregate runtime
summary; metal uses the narrow `minimal` allowlist and warns at each root runtime
requirement. `hxc_runtime=none` turns runtime freedom into a hard whole-program
proof. The schema-2 runtime plan records reachable-program coverage, exact
direct and program-local decisions, zero runtime intents, and empty runtime
effects; an ineligible program receives one sorted `HXC2000` with every source
root and dependency chain before output. Every successful build emits
`hxc.runtime-plan.json`, even when the selected feature set is empty.

The first compiler-selected slice is intentionally tiny: a compiler-known valid
UTF-8 String literal passed to hosted `Sys.println` or default `trace` packages
only `runtime-base`, `status`, `string-literal`, and `io`. Embedded NUL is written
by length, and write/flush failure aborts the generated program. Nonliteral
String output and general I/O still fail closed.

The bounded [`examples/hello`](examples/hello/) proof composes that reusable
path into the first repository-owned product executable. It emits exact
`Hello from hxc\n`, matches Haxe Eval, snapshots readable generated C and its
runtime reasons, and is rebuilt warning-clean under required GCC and Clang
lanes. It does not broaden the supported language subset.

The `c.*` source contract is designed to make C itself pleasant to author from Haxe:
typed pointers, spans, ownership, layouts, header groups, linkage, calling
conventions, compile-time constants/assertions, external build facts, and ABI
exports. Haxe types and macros should catch invalid declaration graphs, layout
contracts, ownership, qualifiers, and DSL schemas at the originating source
position; C compilers, Clang, static assertions, and ABI probes still verify the
native facts. Ordinary Haxe comes first, then typed APIs and validated metadata;
a DSL must justify a real language gap, and raw C stays explicit and unsafe.

The M0 intrinsic types and namespaced declaration metadata now type-check, and a
compile-backed validator produces a deterministic structural contract report
with source-positioned negative diagnostics. A target-owned planner now turns
that contract into stable header/source placement, complete-type ordering,
minimal includes, forward declarations, and portable guards. The deterministic
symbol registry finalizes exact or generated names across real C namespaces,
keeps overloads and specializations stable across discovery order, and exposes
the `hxc.symbols.json` table/collision-ledger shape. A typed project emitter now
packages structural headers and sources with content hashes, neutral build
facts, and symbol output. Direct structural fixtures retain honest
runtime/ABI/stdlib placeholders; the admitted primitive production path records
analyzed empty results and a positive no-runtime proof. Its guarded
Reflaxe adapter skips unchanged artifacts, safely removes only owned stale
paths, rejects unowned collisions and path/symlink escapes, and produces
byte-identical projects across absolute roots, discovery orders, locales, CRLF
inputs, and warm compiler-server reuse. Renamed-symbol tests remove only prior
owned paths, and failures identify the first differing artifact and byte. The
emitted structural corpus compiles and runs under strict GCC and Clang. The
separate typed-function suite proves production primitive output; unsupported
signatures and bodies stop at exact `HXC1001` without output.
See [project emission](docs/project-emission.md). The C authoring boundary is in
[typed C authoring](docs/typed-c-authoring.md); the ratified rationale lives in
[ADR 0001](docs/adr/0001-direct-c-and-selective-runtime.md) and
[ADR 0002](docs/adr/0002-haxe-first-typed-c-authoring.md).

The target-owned [HxcIR semantic core](docs/hxc-ir.md) now structurally records
values versus places, exact instruction order, initialization/lifetime changes,
all call dispatch forms, ABI integer and nullable identities, explicit
exact/wrapping/checked/saturating conversions, allocation intent, failure successors,
and reverse inner-to-outer cleanup paths before C syntax exists. Its validator,
canonical source-aware dumps, reordered-input goldens, matching Eval side-effect
oracle, and stable `HXC1001`/`HXC9000` negatives are compile-backed. The
[primitive function-body lowering](docs/body-lowering.md) carries real typed
constants, parameters, initialized locals/reads, nested blocks, calls,
conversions, assignments, primitive static storage, lazy/value control flow,
unsigned increments, and returns through this layer to structural C, with exact
source spans and deterministic names for globals, locals, stable temporaries,
and labels. [Static function
lowering](docs/function-lowering.md) collects the reachable typed graph, emits
all prototypes before definitions, preserves ordered arguments with explicit
temporaries, partitions compiler-proven closed recursion warning-cleanly, and
packages the production primitive executable project. It also keeps ordinary
Haxe module-level functions as readable source modules in split C instead of
leaking Haxe's hidden module-fields class. [Explicit evaluation
order](docs/evaluation-order.md) documents the stable-value proof and Eval/C
differential boundary. [Fixed arrays and span-based
iteration](docs/span-lowering.md) documents nonempty literals and compiler-known
zero-initialized `CArray` storage, pointer-and-length views, explicit size and
bounds policies, and zero-iterator lowering.

The [primitive semantic contract](docs/primitive-semantics.md) now maps real
typed Haxe declarations for both profiles to exact fixed-width and target-ABI
identities, records scalar/reference nullability, and defines widening,
wrapping, checked, NaN/infinity, and `Std.int` behavior without hidden runtime
selection. Its independently authored strict-C11 probe validates the ratified
algorithms at O0/O2. The E2.T02 body fixture separately proves the admitted
primitive mappings in generated strict C11.

The [seeded primitive differential suite](docs/primitive-differential.md)
regenerates 128 boundary-biased calls from a reviewed SplitMix64 seed, compares
the common result domain exactly with pinned Haxe Eval, reduces a mismatch to a
single replay case, and runs the checked-in generated C under GCC and Clang at
O0/O2 plus combined ASan/UBSan. Target-specific modulo-zero and positive
`Std.int` overflow behavior remains explicit in a schema-backed divergence
ledger; the suite does not broaden the supported language slice.

The [typed-AST input adapter](docs/typed-ast-input.md) now captures and
normalizes Haxe's complete module set and original field expressions before
Reflaxe callback filtering, records module and declaration ownership, retains
externs/typedefs/abstracts and raw expressions,
and records the entry point in a fresh per-request context. Its deterministic
inventory is identical across reordered input and cold/compiler-server builds.
The report remains a pre-lowering inventory; the body/function pipeline consumes
its retained raw typed expressions afterward. Unsupported nodes report exact
`HXC1001`; a wholly admitted primitive static graph emits the owned runtime-free
project.

### CLI bootstrap

Direct Haxe/HXML compilation remains the canonical path. During bootstrap, the
shared `hxc` CLI core will run on Haxe's built-in Eval target through
`haxe --run` or `haxelib run reflaxe.c`, so development requires no additional
host runtime.

The first target-neutral CLI component is now executable: the strict
[`hxc.json` configuration core](docs/configuration.md#implemented-schema-1-core)
validates the published schema, resolves defaults/presets/project/overlay/CLI/
direct-define precedence, roots project paths at the config file, and exposes
provenance for every effective value. This is a library-level bootstrap slice;
the user-facing `hxc build`, `run`, `inspect`, and dev/watch commands remain
owned by their later E8 tasks.

`hxc` exists for C's multi-tool workflows—native compiler selection, manifests,
headers, ABI inspection, bindgen, and diagnostics—not because Reflaxe or Haxe
requires a wrapper command. It is an optional ergonomics layer that could inform
other targets later; no application or direct compiler build may depend on it.

Once `reflaxe.c` supports the files, processes, configuration, strings, errors,
and cleanup behavior the CLI needs, that same core will be compiled through the
C target into a native `hxc` executable. Eval remains the recovery path and a
differential test oracle; an existing native `hxc` must never be required to
build its replacement.

The intended compiler shape is:

```text
Haxe typed AST
  -> target-owned semantic IR
  -> explicit evaluation, lifetime, and cleanup edges
  -> structured C AST
  -> deterministic C printer
  -> C sources, headers, runtime slices, manifests, and ABI reports
```

Strict hosted C11 is the working default. Generated code must preserve Haxe
evaluation order, avoid C undefined behavior, compile warning-clean, and remain
deterministic across repeated builds.

The remaining foundational contracts are now explicit design inputs, not
implementation claims: `--custom-target c=<output>` selects a `c` target that
will emit strict C11; portable strings are immutable UTF-8 with Unicode-scalar
indices; tracing, when actually required, uses a selective precise non-moving
collector; and general exceptions use contained C frames while every public C
boundary exposes ordinary status/error values. The 1.0 release matrix covers
hosted Linux, macOS, and Windows tuples plus a capability-limited ARM Cortex-M
freestanding-metal lane.
See [ADR 0007](docs/adr/0007-strict-c11-target-and-platform-baseline.md),
[ADR 0004](docs/adr/0004-utf8-scalar-string-contract.md),
[ADR 0005](docs/adr/0005-precise-nonmoving-collector.md), and
[ADR 0006](docs/adr/0006-explicit-failure-edges-and-contained-unwinding.md).

The production typing carrier is the exact official Haxe 5.0.0-preview.1
custom-target API. Its automatic `c.Init.init()` hook installs C-owned static,
scalar-Unicode, and environment capabilities before standard-library and user
typing. The preview pin, source revision, and official platform artifact hashes
are locked and compile-tested. Haxe 4's default Reflaxe `Cross` carrier remains
an `HXC0003` regression case because its unremovable UTF-16 facts contradict
the C string contract.

## What exists today

<!-- BEGIN GENERATED BOOTSTRAP CAPABILITIES -->
> Generated from [`docs/specs/bootstrap-inventory.json`](docs/specs/bootstrap-inventory.json) by `scripts/ci/check_capability_manifest.py`; edit the manifest, not this block.

**Project stage:** `experimental-scaffold-with-bounded-executable-slices` (`M0`).

This inventory combines product capability boundaries with repository infrastructure and evidence machinery. Governance, quality, and toolchain rows are not Haxe-to-C language or runtime features.

| Status | Count | Meaning |
| --- | ---: | --- |
| `implemented` | 39 | The exact bounded scope has executable repository evidence. This does not confer support on adjacent Haxe semantics or make a release promise. |
| `scaffold-only` | 3 | A typed contract, seed, fixture, or plan exists, but it is not evidence of an available user-program capability. |
| `experimental` | 1 | The surface is explicit and opt-in, remains unstable, and has not passed a supported-release capability gate. |
| `unsupported` | 11 | The surface is absent, deliberately fails closed, or lacks the evidence needed for a product claim. |

| Inventory item | Status | Current boundary |
| --- | --- | --- |
| `beads-bootstrap` | `implemented` | Repository governance tooling validates and previews the checked-in Beads plan deterministically; this is not compiler or runtime functionality. |
| `bindgen` | `unsupported` | No Clang-backed binding generator or user bindgen command exists. |
| `bootstrap-capability-manifest` | `implemented` | A schema-checked M0 product, infrastructure, and limitation inventory drives the README status table. |
| `build-adapter-seeds` | `implemented` | One typed neutral executable plan deterministically drives optional CMake and Meson files plus a direct argument-array consumer. |
| `c-ast-printer` | `implemented` | Target-owned AST corpora deterministically emit and native-run single-file and header/source C projects. |
| `c-export` | `unsupported` | No Haxe export analyzer, public-header generator, library packaging, or ABI diff workflow exists. |
| `c23-internal-dialect` | `experimental` | The c23 internal dialect spelling is explicit and opt-in but has no support-matrix claim. |
| `class-layout` | `implemented` | Concrete non-generic Haxe classes emit private base-prefix structs with checked nullable-reference operations and safe upcasts. |
| `closed-anonymous-record-lowering` | `implemented` | A bounded closed anonymous-record graph emits deduplicated private value structs and runs as runtime-free strict C11. |
| `configuration-policies` | `implemented` | A target-neutral typed core strictly parses schema-1 hxc.json, resolves six precedence layers, and reports every effective value with provenance. |
| `constructor-lowering` | `implemented` | Concrete non-generic Haxe classes support bounded stack or collector construction, final inline child ownership, by-value closed-record, literal-backed nominal String, and unmanaged-enum arguments, plus call-only or retained interface arguments with exact order and cleanup. |
| `declaration-planner` | `implemented` | Structural declaration planning handles complete types, forward declarations, includes, and private boundaries deterministically. |
| `diagnostics` | `implemented` | Thirteen typed diagnostic IDs and deterministic schema-1 records are registry- and drift-checked. |
| `direct-c-import` | `implemented` | Reached hand-authored extern declarations directly consume an authoritative C header and library through structural generated C. |
| `example-portfolio` | `implemented` | Repository-owned hello and Caxecraft examples compile ordinary Haxe through generated C; Caxecraft now includes a directly linked Raylib feasibility executable plus exact domain and presentation differentials. |
| `fixture-snapshot-policy` | `implemented` | Eight evidence lanes and centrally owned deterministic snapshots have fail-closed policy checks. |
| `general-haxe-lowering` | `unsupported` | General Haxe programs are not supported and stop at the first unsupported typed construct. |
| `generated-file-ownership` | `implemented` | Admitted projects use deterministic content-addressed artifacts and Reflaxe-owned stale-file handling. |
| `generic-specialization` | `implemented` | Reachable closed generic static functions over admitted primitives, enums, and selected direct records share deterministic program-local instances and emit a bounded code-size report. |
| `governance-and-provenance` | `implemented` | Contribution, disclosure, license, vendoring, provenance, and future release responsibilities are drift-checked. |
| `hxc-cli` | `unsupported` | No Run.hx, hxc command router, project schema, template, or packaged executable exists. |
| `hxc-doctor` | `unsupported` | The hxc doctor human and JSON command is not implemented. |
| `hxc-ir` | `implemented` | Schema-19 HxcIR structurally records values, tagged cases, concrete and header-owned nominal types, reachable class/interface layouts, slots and tables, distinct caller-owned class and interface borrows, exact native dispatch/constants, binary32 rounding/binary64 widening, strings, ordering, control flow, failures, cleanup, managed roots, unmanaged branch carriers, managed-enum ownership joins, and runtime intent. |
| `literal-backed-string-values` | `implemented` | Immutable Haxe String values backed by source literals can flow through direct typed values without allocation. |
| `literal-string-output` | `implemented` | Compiler-known String literals support hosted Sys.println and default trace with exact UTF-8/NUL bytes and explicit output failure handling. |
| `native-interop-fixtures` | `scaffold-only` | Independent C-library and C++ extern-C shim inputs validate interop boundary shapes. |
| `native-smoke` | `implemented` | Strict GCC/G++ and Clang/Clang++ CI lanes compile and run the declared structural, generated, runtime, import, aggregate/class/constructor/dispatch/enum-layout, generic-specialization, hello, and Caxecraft-domain corpus. |
| `performance-evidence` | `unsupported` | No compiler-time, C-compile-time, runtime, size, allocation, FFI, or agent benchmark claim is validated. |
| `platform-support-matrix` | `unsupported` | No operating-system, architecture, environment, runtime, and compiler tuple is currently a supported release lane. |
| `primitive-executable-lowering` | `implemented` | A bounded primitive static-function graph with deterministic initialization, typed direct exact/modulo integer conversions, and literal or bounded zero-initialized local or nonescaping class-owned fixed-array/span storage plus nonescaping internal span parameters emits and runs runtime-free strict C11. |
| `primitive-semantics` | `implemented` | A typed schema-3 primitive representation, nullability, conversion, and operation contract has independent and generated C evidence. |
| `public-c-abi` | `unsupported` | No generated public C header, stable export symbol set, ownership boundary, or ABI compatibility promise exists. |
| `raylib-raw-core` | `implemented` | A version-locked public raylib.raw core is generated from Clang declaration facts and consumed directly by generated Haxe/C. |
| `raylib-semantic-core` | `implemented` | A curated raylib facade adds typed domains, direct value constructors, named colors, and inline forwarding with zero runtime cost. |
| `reflaxe-adapter` | `implemented` | The Reflaxe adapter captures complete typed modules and routes admitted output through request-local compiler state. |
| `release-artifacts` | `unsupported` | No publishable compiler package, signed reproducible archive, supported version, or release automation exists. |
| `runtime-array-contract` | `implemented` | The bounded array slice provides checked native storage plus a first compiler-selected ordinary-Haxe Array path. |
| `runtime-bytes-contract` | `implemented` | A bounded ordinary-Haxe haxe.io.Bytes slice provides shared fixed-length binary storage and compiler-owned lifetimes. |
| `runtime-collector-contract` | `implemented` | A selective precise, non-moving collector backend reclaims cyclic managed graphs through exact roots and descriptors. |
| `runtime-feature-planning` | `implemented` | A typed deterministic feature graph resolves exact runtime closure under a versioned, provenance-locked internal ABI contract. |
| `runtime-hxrt-seed` | `scaffold-only` | Hardened allocator, UTF-8 scalar string, typed array, Bytes, status, hosted output, object descriptor, and collector slices coexist with other provisional runtime seeds. |
| `runtime-string-contract` | `implemented` | The bounded native string slice enforces valid UTF-8, scalar indexing, allocation-aware ownership, and explicit CString lifetimes. |
| `standard-library` | `unsupported` | General Haxe standard-library parity is not implemented. |
| `standard-library-ledger` | `implemented` | The exact pinned Haxe public standard-library surface has a deterministic ownership and parity ledger. |
| `symbol-registry` | `implemented` | Per-compilation C namespace ownership and deterministic exact/generated symbol finalization are implemented. |
| `tagged-enum-lowering` | `implemented` | A bounded Haxe enum graph emits private native enums or tagged unions and runs as runtime-free strict C11. |
| `target-activation` | `implemented` | The pinned Haxe 5 CustomTarget(c) carrier configures scalar Unicode facts and exactly-once target registration. |
| `target-c-api` | `scaffold-only` | Typed c.* pointer, span, ownership, integer, Float32, layout, linkage, and metadata contracts type-check. |
| `threading-and-atomics` | `unsupported` | No Haxe threading, synchronization, TLS, or atomic capability is advertised or implemented. |
| `toolchain-metadata` | `implemented` | Haxe 5.0.0-preview.1, Reflaxe, Lix, package metadata, checksums, and provenance are pinned and verified. |
| `typed-ast-input` | `implemented` | Complete typed-module capture and deterministic normalized ownership survive order changes and compiler-server reuse. |
| `typed-boundary-governance` | `implemented` | Repository-owned Haxe has a source-exact guard against unreviewed untyped compiler seams. |
| `typed-c-contract-validation` | `implemented` | Namespaced typed-C declaration and build metadata produce a deterministic validated structural snapshot. |
| `virtual-dispatch` | `implemented` | Reachable ordinary class and interface methods support deterministic minimal closed-world dispatch while statically known class calls remain direct. |

**Hard claim gates not passed:** `full-standard-library`, `general-haxe-to-c`, `performance-validated`, `production-ready`, `public-abi-stability`, `supported-platforms`, `supported-release`.

**`hxc doctor`:** `unsupported`; implementation is absent and owned by `E8.T06`. Use the documented repository validation commands today. E8.T06 must make future human and JSON doctor output consume these inventory IDs rather than maintaining a second status list.
<!-- END GENERATED BOOTSTRAP CAPABILITIES -->

## Explore the scaffold

Use Node.js 20 or newer; `npm ci` installs the pinned Lix shim and resolves the
exact Haxe 5.0.0-preview.1 release. Contributors also need at least one matched
GCC/G++ or Clang/Clang++ pair, plus `bd`, `jq`,
[Gitleaks](https://github.com/gitleaks/gitleaks), and the Haxe formatter. Then
activate the tracked pre-commit/pre-push chain:

```sh
npm ci
npm test

haxelib install formatter 1.18.0
scripts/hooks/install.sh

# Required before changing visibility or publishing a new reachable ref.
npm run public:preflight

# Required instead of calling `bd dolt push` directly.
npm run beads:push

bd prime
bd list --ready --type task

python3 scripts/beads/validate_plan.py --json
python3 scripts/beads/bootstrap.py --json

jq empty \
  docs/specs/beads-plan.json \
  docs/specs/beads-plan.schema.json \
  docs/specs/bootstrap-inventory.json \
  docs/specs/bootstrap-inventory.schema.json \
  docs/specs/cross-target-research-bundle.json \
  docs/specs/diagnostic-event.schema.json \
  docs/specs/diagnostics.json \
  docs/specs/diagnostics.schema.json \
  docs/specs/runtime-features.schema.json \
  docs/specs/stdlib-ledger.json \
  docs/specs/stdlib-ledger.schema.json \
  docs/specs/third-party-provenance.json \
  docs/specs/typed-boundaries.json \
  docs/specs/typed-boundaries.schema.json

python3 scripts/ci/check_toolchain.py --require-tools
python3 test/all_sources/run.py
python3 test/bootstrap/run.py
python3 test/typed_c/run.py
python3 test/typed_ast/run.py
python3 test/c_ast/run.py
python3 test/declaration_plan/run.py
python3 test/symbol_registry/run.py
python3 test/runtime/runtime-feature-graph/run.py
python3 test/hxc_ir/run.py
python3 test/stdlib_ledger/run.py
python3 test/body_lowering/run.py
python3 test/function_lowering/run.py
python3 test/evaluation_order/run.py
python3 examples/hello/run.py
python3 scripts/ci/runtime_smoke.py
python3 scripts/ci/check_fixture_policy.py
python3 scripts/ci/check_capability_manifest.py
python3 scripts/ci/check_typed_boundaries.py
python3 scripts/test/snapshots.py --catalog-check
python3 scripts/test/snapshots.py --check
python3 scripts/ci/check_license_policy.py
python3 scripts/ci/check_governance_policy.py
```

The catalog check is the fast integrated ownership proof after focused suites;
the full check independently re-renders all registered artifacts when snapshot
infrastructure changes and in the scheduled/manual cold workflow. For bounded
local concurrency with ordered logs, use `npm run test:toolchain:parallel`
(four workers on an idle 8+ CPU host, automatically fewer when the host is
busy). An explicit `--jobs 1..4` still wins. See
[test feedback-loop performance](docs/test-performance.md) for the exact
resource and safe-resume rules.

Expected artifacts are never refreshed implicitly. For an intentional change,
review the semantic diff produced by a targeted update, then inspect the Git
diff and rerun the owning native/oracle gates:

```sh
npm run snapshots:update -- --suite hxc-ir
```

See the [fixture and snapshot policy](docs/testing.md) for the eight evidence
lanes, case manifests, current mapped suites, and example assertion rules.

See the [pinned toolchain guide](docs/toolchain.md) for the exact dependency,
package-layout, bootstrap-order, and update contracts. The compile-backed probes
claim only the documented primitive subset; they do not claim broad
ordinary-Haxe or standard-library compatibility.

The eventual direct compiler invocation is:

```sh
haxe -lib reflaxe.c --custom-target c=build/c -main Main
```

At M0 that real production path emits an owned C project only for the admitted
primitive/static-array/span subset plus compiler-known literal output, and
reports exact `HXC1001` without output at the first unsupported signature or
expression. It is documented now so `c_output`, Eval, or the future `hxc`
wrapper are not mistaken for alternate user-program carriers.

The complete target-owned Haxe graph type-checks under the dedicated all-source
gate. E2.T03 supplies static functions, arguments, direct calls, recursive
prototypes, and executable entry emission. E2.T04 now serializes the admitted
Haxe evaluation-order forms explicitly while unsupported constructs remain
fail-closed.

## Project documents

- [Product requirements](docs/PRD.md)
- [Architecture](docs/architecture.md)
- [Haxe code and game-engine architecture learning guide](docs/haxe-code-architecture.md)
- [Caxecraft runtime architecture](docs/caxecraft-architecture.md)
- [Configuration contract](docs/configuration.md)
- [Runtime feature planning and selective packaging](docs/runtime-feature-planning.md)
- [hxrt architecture, inspection, and source guide](docs/hxrt.md)
- [Pinned toolchain and update procedure](docs/toolchain.md)
- [HxcIR semantic contract](docs/hxc-ir.md)
- [Cross-target semantics GPT-5.6 research bundle](docs/research/cross-target-semantics-bundle.md)
- [Primitive function-body lowering](docs/body-lowering.md)
- [Static function and direct-call lowering](docs/function-lowering.md)
- [Explicit Haxe evaluation order](docs/evaluation-order.md)
- [Deterministic static initialization](docs/static-initialization.md)
- [Fixed arrays and span-based iteration](docs/span-lowering.md)
- [Typed C authoring contract](docs/typed-c-authoring.md)
- [Typed Haxe boundary policy](docs/typed-boundaries.md)
- [Deterministic C symbol naming](docs/symbol-naming.md)
- [Fixture and snapshot policy](docs/testing.md)
- [Architecture decisions](docs/adr/README.md)
- [Third-party notices and provenance](THIRD_PARTY_NOTICES.md)
- [Beads plan](docs/BEADS_PLAN.md)
- [Contributing](CONTRIBUTING.md)
- [Security policy](SECURITY.md)
- [Agent rules](AGENTS.md)

## License

Repository-owned source is licensed under [GPL-3.0-only](LICENSE). Verified
upstream attribution, current distribution status, and fail-closed package
requirements live in [Third-party notices and provenance](THIRD_PARTY_NOTICES.md).

The licensing treatment of generated C/headers, templates, and emitted or
linked runtime slices is still an explicit pre-release decision; this README
does not assume that the repository license answers those separate cases.
