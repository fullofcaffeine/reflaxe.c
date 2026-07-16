# reflaxe.c

> Haxe's missing C target: readable output, explicit ABI control, and honest
> runtime costs.

`reflaxe.c` is an experimental [Reflaxe](https://github.com/SomeRanDev/reflaxe)
target for compiling Haxe to C through the normal Haxe/HXML workflow. The PRD
separately proposes an optional future `hxc` orchestration tool for project builds,
diagnostics, bindings, exports, and other end-to-end workflows; it is not a
replacement Haxe compiler or a requirement imposed by Reflaxe.

**Project status: M0 architecture scaffold with a primitive executable slice.**
The complete target-owned Haxe source graph type-checks. Typed primitive static
functions, arguments, conversions, calls, assignments, static fields, lazy and
value/statement control flow, UB-safe arithmetic, local fixed arrays, checked
span access, and direct span iteration now pass through
explicitly sequenced validated HxcIR and emit an owned runtime-free strict-C11
project with a private header and `int main(void)`. This is not general Haxe or
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
narrow justified optimized `hxrt` feature last. The planned E5 stdlib epic owns
the generated parity ledger and String/Unicode, collections, Bytes/I/O,
Std/Math/Type/Reflect, JSON, regex, time, sys, networking, and threading slices.

With no explicit override, portable uses `auto` plus an aggregate runtime
summary; metal uses the narrow `minimal` allowlist and warns at each root runtime
requirement. `hxc_runtime=none` turns runtime freedom into a hard whole-program
proof. Every successful build is planned to emit `hxc.runtime-plan.json`, even
when the selected feature set is empty.

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
packages the production primitive executable project. [Explicit evaluation
order](docs/evaluation-order.md) documents the stable-value proof and Eval/C
differential boundary. [Fixed arrays and span-based
iteration](docs/span-lowering.md) documents the nonempty literal-backed
`CArray`, pointer-and-length view, explicit bounds-policy, and zero-iterator
lowering slice.

The [primitive semantic contract](docs/primitive-semantics.md) now maps real
typed Haxe declarations for both profiles to exact fixed-width and target-ABI
identities, records scalar/reference nullability, and defines widening,
wrapping, checked, NaN/infinity, and `Std.int` behavior without hidden runtime
selection. Its independently authored strict-C11 probe validates the ratified
algorithms at O0/O2. The E2.T02 body fixture separately proves the admitted
primitive mappings in generated strict C11.

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

The current checkout contains:

- a detailed [product requirements document](docs/PRD.md);
- [architecture](docs/architecture.md) and
  [configuration](docs/configuration.md) contracts;
- a [typed diagnostic contract](docs/diagnostics.md) with 12 registered IDs,
  exhaustive reserved ranges, schema-validated records, and registry-drift
  enforcement;
- a minimal Reflaxe adapter and whole-program boundary that lowers an admitted
  reachable primitive static-function graph, reports exact `HXC1001` for
  unsupported typed nodes/signatures, and emits an owned runtime-free private
  header/source project with `int main(void)` when wholly admitted;
- a deterministic typed-AST adapter and reviewed inventory covering complete
  module ownership, declarations, fields, metadata, expressions, entry points,
  reordered input, and compiler-server isolation;
- a dedicated HXML gate that automatically covers every `src/**/*.hx` and
  `std/c/**/*.hx` module plus explicit compiler-macro branches;
- a Haxe 5.0.0-preview.1 / Reflaxe `73a9831` / Lix 17.0.2 checksum-locked
  toolchain;
- production `CustomTarget(c)` platform setup plus target-gated
  `CompilerBootstrap` and `CompilerInit` lifecycle, identity, upstream
  stdlib-branch, conflict, package, and compiler-server probes;
- zero-runtime `c.*` contract types plus deterministic typed declaration/build
  metadata validation and `HXC5002` negative fixtures;
- a deterministic declaration planner with declaration-owned include
  provenance, complete-type diagnostics, pointer-cycle forward declarations,
  public/private separation, and independently compiled header goldens;
- a per-compilation deterministic C symbol registry with exact-name validation,
  namespace-aware collision provenance, typed-contract default finalization,
  and an owned zero-runtime `hxc.symbols.json`-shape snapshot;
- a target-owned HxcIR model, validator, and deterministic source-aware dumper
  covering explicit side-effect/cleanup order, places and values, dispatch,
  conversions, failure edges, initialization, and named runtime intent;
- a typed primitive mapping/conversion contract with profile-invariant exact and
  ABI integer identities, explicit scalar/reference nullability, owned machine
  snapshot, and strict-C11 O0/O2 algorithm probes without `hxrt`;
- real primitive `TypedExpr -> HxcIR -> structural C` body
  lowering with deterministic global/local/temporary/label names, exact source
  diagnostics, optional structural `#line` mapping, and runtime-free GCC/Clang
  execution at O0/O2;
- typed primitive parameters, explicit argument conversion/call order, direct
  static calls, recursive prototype planning, scoped default/optional/rest
  diagnostics, and deterministic production project/native execution without
  `hxrt`;
- explicit calls, assignments, primitive static fields, short circuit,
  value-form ternaries, and `UInt` increments with a stable-temporary proof and
  Eval-versus-generated-C differential execution at O0/O2;
- typed nonempty fixed arrays, mutable/const local span borrows, explicit
  static/loop/dynamic bounds policies, and direct element-scaled iteration with
  six-way profile/build fail-stop evidence and no `hxrt` link symbols;
- a structured C11 AST with deterministic declarator and exhaustive
  expression/statement precedence and escaping goldens, compiled and executed
  without `hxrt` by both GCC and Clang;
- a provisional standalone `hxrt` allocator/string/status/Int32 ABI seed with
  strict hosted and freestanding compilation plus C++17 header consumption; it
  is not yet selected or linked by generated programs;
- independent native C-library and opaque-handle C++-shim fixtures that validate
  the future interop boundaries without pretending they were generated;
- a fail-closed third-party provenance and release-notice policy;
- a drift-checked contributor, private-disclosure, and release-responsibility
  policy with no premature supported-release claim;
- a live Beads execution graph covering the planned milestones.

It does **not** yet contain general Haxe-to-C semantic lowering, indirect or
instance calls, arbitrary collection/iterator lowering, general Haxe arrays,
escaping span views,
standard-library parity, a complete runtime, the `hxc` implementation,
executable generated examples, or release tooling.
The package metadata added at M0 is a reproducible development/package-layout
seed, not a publishable compiler. Those capabilities remain tracked work.

## Explore the scaffold

Use Node.js 20 or newer; `npm ci` installs the pinned Lix shim and resolves the
exact Haxe 5.0.0-preview.1 release. Contributors also need at least one matched
GCC/G++ or Clang/Clang++ pair, plus `bd`, `jq`,
[Gitleaks](https://github.com/gitleaks/gitleaks), and the Haxe formatter. Then
activate the tracked pre-commit chain:

```sh
npm ci
npm test

haxelib install formatter
scripts/hooks/install.sh

bd prime
bd list --ready --type task

python3 scripts/beads/validate_plan.py --json
python3 scripts/beads/bootstrap.py --json

jq empty \
  docs/specs/beads-plan.json \
  docs/specs/beads-plan.schema.json \
  docs/specs/diagnostic-event.schema.json \
  docs/specs/diagnostics.json \
  docs/specs/diagnostics.schema.json \
  docs/specs/stdlib-ledger.json \
  docs/specs/third-party-provenance.json

python3 scripts/ci/check_toolchain.py --require-tools
python3 test/all_sources/run.py
python3 test/bootstrap/run.py
python3 test/typed_c/run.py
python3 test/typed_ast/run.py
python3 test/c_ast/run.py
python3 test/declaration_plan/run.py
python3 test/symbol_registry/run.py
python3 test/hxc_ir/run.py
python3 test/body_lowering/run.py
python3 test/function_lowering/run.py
python3 test/evaluation_order/run.py
python3 scripts/ci/runtime_smoke.py
python3 scripts/ci/check_fixture_policy.py
python3 scripts/test/snapshots.py --check
python3 scripts/ci/check_license_policy.py
python3 scripts/ci/check_governance_policy.py
```

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
primitive static-function subset and reports exact `HXC1001` without output at
the first unsupported signature or expression. It is documented now so
`c_output`, Eval, or the future `hxc` wrapper are not mistaken for alternate
user-program carriers.

The complete target-owned Haxe graph type-checks under the dedicated all-source
gate. E2.T03 supplies static functions, arguments, direct calls, recursive
prototypes, and executable entry emission. E2.T04 now serializes the admitted
Haxe evaluation-order forms explicitly while unsupported constructs remain
fail-closed.

## Project documents

- [Product requirements](docs/PRD.md)
- [Architecture](docs/architecture.md)
- [Configuration contract](docs/configuration.md)
- [Pinned toolchain and update procedure](docs/toolchain.md)
- [HxcIR semantic contract](docs/hxc-ir.md)
- [Primitive function-body lowering](docs/body-lowering.md)
- [Static function and direct-call lowering](docs/function-lowering.md)
- [Explicit Haxe evaluation order](docs/evaluation-order.md)
- [Fixed arrays and span-based iteration](docs/span-lowering.md)
- [Typed C authoring contract](docs/typed-c-authoring.md)
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
