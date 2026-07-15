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

The planned `c.*` surface also makes C itself pleasant to author from Haxe:
typed pointers, spans, ownership, layouts, header groups, linkage, calling
conventions, compile-time constants/assertions, external build facts, and ABI
exports. Haxe types and macros should catch invalid declaration graphs, layout
contracts, ownership, qualifiers, and DSL schemas at the originating source
position; C compilers, Clang, static assertions, and ABI probes still verify the
native facts. Ordinary Haxe comes first, then typed APIs and validated metadata;
a DSL must justify a real language gap, and raw C stays explicit and unsafe.

These are M0 design contracts, not currently implemented capabilities. The
ratified rationale lives in [ADR 0001](docs/adr/0001-direct-c-and-selective-runtime.md)
and [ADR 0002](docs/adr/0002-haxe-first-typed-c-authoring.md).

### CLI bootstrap

Direct Haxe/HXML compilation remains the canonical path. During bootstrap, the
shared `hxc` CLI core will run on Haxe's built-in Eval target through
`haxe --run` or `haxelib run reflaxe.c`, so development requires no additional
host runtime.

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

## What exists today

The current checkout contains:

- a detailed [product requirements document](docs/PRD.md);
- [architecture](docs/architecture.md) and
  [configuration](docs/configuration.md) contracts;
- a minimal Reflaxe compiler adapter that fails with `HXC1000`;
- a structured C AST and printer seed;
- a public runtime-header seed that parses as C11 and C++17;
- a live Beads execution graph covering the planned milestones.

It does **not** yet contain the complete bootstrap, Haxe lowering pipeline,
runtime implementation, CLI, package metadata, examples, or release tooling.
Those capabilities are tracked as work, not presented as finished features.

## Explore the scaffold

Requirements for the currently available checks are `bd`, `jq`, a C compiler,
and a C++ compiler. Contributors should also install
[Gitleaks](https://github.com/gitleaks/gitleaks) and the Haxe formatter, then
activate the tracked pre-commit chain:

```sh
haxelib install formatter
scripts/hooks/install.sh

bd prime
bd list --ready --type task

jq empty \
  docs/specs/beads-plan.json \
  docs/specs/diagnostics.json \
  docs/specs/stdlib-ledger.json

cc -std=c11 -Wall -Wextra -Werror -pedantic \
  -fsyntax-only -x c-header runtime/hxrt/include/hxc_runtime.h

c++ -std=c++17 -Wall -Wextra -Werror -pedantic \
  -fsyntax-only -x c++-header runtime/hxrt/include/hxc_runtime.h
```

The first implementation milestone will ratify the public contracts, pin the
Haxe/Reflaxe toolchain, make the target-owned Haxe sources type-check, and build
a small end-to-end C emission slice before expanding language coverage.

## Project documents

- [Product requirements](docs/PRD.md)
- [Architecture](docs/architecture.md)
- [Configuration contract](docs/configuration.md)
- [Architecture decisions](docs/adr/README.md)
- [Beads plan](docs/BEADS_PLAN.md)
- [Contributor and agent rules](AGENTS.md)

## License

Licensed under [GPL-3.0-only](LICENSE). Third-party attribution and provenance
remain tracked M0 governance work and must be complete before a public release.
