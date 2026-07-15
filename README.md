# reflaxe.c

> Haxe's missing C target: readable output, explicit ABI control, and honest
> runtime costs.

`reflaxe.c` is an experimental [Reflaxe](https://github.com/SomeRanDev/reflaxe)
target for compiling Haxe to C through the normal Haxe/HXML workflow. The PRD
separately proposes an optional future `hxc` orchestration tool for project builds,
diagnostics, bindings, exports, and other end-to-end workflows; it is not a
replacement Haxe compiler or a requirement imposed by Reflaxe.

**Project status: M0 architecture scaffold.** This repository does not compile
Haxe programs yet. Unsupported compiler paths intentionally fail closed instead
of emitting plausible but incorrect C.

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
```

Direct idiomatic C is the first lowering choice in both profiles. The compiler
may next generate a program-local specialization; `hxrt` is the last semantic
fallback and is selected feature by feature with source reasons. There is no
unconditional runtime core. A proven runtime-free build contains no `hxrt`
header, source, define, library, or symbol.

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
with source-positioned negative diagnostics. They do not lower to C or prove
native layouts yet. The exact boundary and examples are in
[typed C authoring](docs/typed-c-authoring.md); the ratified rationale lives in
[ADR 0001](docs/adr/0001-direct-c-and-selective-runtime.md) and
[ADR 0002](docs/adr/0002-haxe-first-typed-c-authoring.md).

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
implementation claims: `c_output` activates a `c` target that emits strict C11;
portable strings are immutable UTF-8 with Unicode-scalar indices; tracing, when
actually required, uses a selective precise non-moving collector; and general
exceptions use contained C frames while every public C boundary exposes ordinary
status/error values. The 1.0 release matrix covers hosted Linux, macOS, and
Windows tuples plus a capability-limited ARM Cortex-M freestanding-metal lane.
See [ADR 0007](docs/adr/0007-strict-c11-target-and-platform-baseline.md),
[ADR 0004](docs/adr/0004-utf8-scalar-string-contract.md),
[ADR 0005](docs/adr/0005-precise-nonmoving-collector.md), and
[ADR 0006](docs/adr/0006-explicit-failure-edges-and-contained-unwinding.md).

One bootstrap constraint is deliberately visible: Haxe 4.3.7's default
Reflaxe `Cross` carrier preselects UTF-16 conditionals that contradict the C
string contract, and public initialization macros cannot remove them. The M0
test carrier proves target identity and the correct upstream scalar branches;
an actual Cross request fails with `HXC0003` until the production carrier
decision is implemented. The project will not call a target working by merely
renaming contradictory compiler state.

## What exists today

The current checkout contains:

- a detailed [product requirements document](docs/PRD.md);
- [architecture](docs/architecture.md) and
  [configuration](docs/configuration.md) contracts;
- a partial Reflaxe compiler adapter whose missing whole-program dependencies
  are explicitly tracked for the next bootstrap task;
- a Haxe 4.3.7 / Reflaxe `73a9831` / Lix 17.0.2 checksum-locked toolchain;
- target-gated `CompilerBootstrap` and `CompilerInit` lifecycle, identity,
  upstream stdlib-branch, conflict, package, and compiler-server probes;
- zero-runtime `c.*` contract types plus deterministic typed declaration/build
  metadata validation and `HXC5002` negative fixtures;
- a structured C AST and printer seed;
- a fail-closed third-party provenance and release-notice policy;
- a live Beads execution graph covering the planned milestones.

It does **not** yet contain a type-checking whole compiler graph, a Haxe lowering
pipeline, runtime implementation, CLI, executable examples, or release tooling.
The package metadata added at M0 is a reproducible development/package-layout
seed, not a publishable compiler. Those capabilities remain tracked work.

## Explore the scaffold

Use Node.js 20 or newer; `npm ci` installs the pinned Lix shim and resolves Haxe
4.3.7. Contributors also need `bd`, `jq`,
[Gitleaks](https://github.com/gitleaks/gitleaks), and the Haxe formatter. Then
activate the tracked pre-commit chain:

```sh
npm ci
npm test

haxelib install formatter
scripts/hooks/install.sh

bd prime
bd list --ready --type task

jq empty \
  docs/specs/beads-plan.json \
  docs/specs/diagnostics.json \
  docs/specs/stdlib-ledger.json \
  docs/specs/third-party-provenance.json

python3 scripts/ci/check_toolchain.py --require-tools
python3 test/bootstrap/run.py
python3 test/typed_c/run.py
python3 scripts/ci/check_license_policy.py
```

See the [pinned toolchain guide](docs/toolchain.md) for the exact dependency,
package-layout, bootstrap-order, and update contracts. The compile-backed probes
do not claim that ordinary Haxe programs can be emitted as C yet.

The remaining bootstrap work will make the complete target-owned Haxe graph
type-check, preserve fail-closed unsupported lowering, and then build a small
end-to-end C emission slice before expanding language coverage.

## Project documents

- [Product requirements](docs/PRD.md)
- [Architecture](docs/architecture.md)
- [Configuration contract](docs/configuration.md)
- [Pinned toolchain and update procedure](docs/toolchain.md)
- [Typed C authoring contract](docs/typed-c-authoring.md)
- [Architecture decisions](docs/adr/README.md)
- [Third-party notices and provenance](THIRD_PARTY_NOTICES.md)
- [Beads plan](docs/BEADS_PLAN.md)
- [Contributor and agent rules](AGENTS.md)

## License

Repository-owned source is licensed under [GPL-3.0-only](LICENSE). Verified
upstream attribution, current distribution status, and fail-closed package
requirements live in [Third-party notices and provenance](THIRD_PARTY_NOTICES.md).

The licensing treatment of generated C/headers, templates, and emitted or
linked runtime slices is still an explicit pre-release decision; this README
does not assume that the repository license answers those separate cases.
