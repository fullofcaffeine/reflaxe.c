# Pinned development toolchain

The M0 compiler contract is the exact official Haxe 5.0.0-preview.1 release plus
one reviewed Reflaxe 4.0.0-beta-compatible revision. Lix supplies the Haxe
executable and scoped library resolution; Reflaxe itself is vendored so source
checkouts and staged packages do not drift with a branch or registry release.

Haxe 5 is deliberately pinned because its custom-target initialization hook can
install C's platform facts before user code and the standard library are typed.
The release is preview surface, not a floating compatibility promise.

## Immutable inputs

| Input | Pin | Verification |
| --- | --- | --- |
| Haxe | `5.0.0-preview.1`, tag revision `2c1e544e0a2c7524ef4c8e103f1b0580362ea538`; official Linux/macOS/Windows release artifacts checksum-locked | `.haxerc`, `toolchain-lock.json`, active `haxe -version` |
| Reflaxe | `4.0.0-beta`, commit `73a983112e039daad46b37912ab238df6bf0cf53`, Git tree `05ab4abc670b6e3ed5424e78c622b92979b5affb` | deterministic SHA-256 of `vendor/reflaxe/{LICENSE,haxelib.json,src}` |
| Lix | npm package `lix@17.0.2` | exact `package-lock.json` version and npm integrity |

The machine-readable authority is
[`docs/specs/toolchain-lock.json`](specs/toolchain-lock.json). Reflaxe is a
compiler-only MIT dependency. Its presence does not select, emit, or link any
`hxrt` runtime feature; generated-program runtime planning remains strictly
feature-driven.

## Why this compiler uses Haxe 5 while sibling compilers use Haxe 4

The nearby Reflaxe compilers are intentionally not on one universal Haxe
version. On 2026-07-20, their checked-in `.haxerc` files had these exact pins:

| Repository | Haxe pin | Library resolution |
| --- | --- | --- |
| `haxe.elixir.codex` | `4.3.7` | scoped |
| `haxe.ruby` | `4.3.7` | scoped |
| `haxe.rust` | `4.3.7` | scoped |
| `haxe.go` | `4.3.7` | scoped |
| `haxe.ocaml` | `4.3.7` | scoped |
| this `haxe.c` repository | `5.0.0-preview.1` | scoped |

This table records inspected sibling checkouts; it is not a promise about their
future releases. It also does not mean Haxe 5 is automatically better for every
Reflaxe compiler. Those projects have working Haxe 4 contracts and target-
specific migration costs. Reflaxe.C has a narrower reason to differ: it needs
Haxe to recognize a real C target and install C's platform facts *before* Haxe
types the standard library and the program.

The important Haxe 5 improvement for this compiler is `--custom-target` plus
the early `PlatformConfig` hook. In plain terms, `c.Init.init()` can tell Haxe
“this is the C target, strings use Unicode-scalar indexing, and only these
environment capabilities exist” before a `#if`, a standard-library choice, or
a typed expression is decided. Official Haxe 4.3.7 instead gives a Reflaxe
backend the built-in `Cross` platform. `Cross` has the wrong `cross` identity
and preselects UTF-16 and hosted capabilities that a macro cannot fully remove.
Relabeling it after the fact would type one program and then pretend to emit a
different one. [ADR 0007](adr/0007-strict-c11-target-and-platform-baseline.md)
and the [Haxe 4.3.7 carrier audit](research/haxe-4.3.7-carrier-audit.md) contain
the source-level and executable evidence.

The pin changes more than target startup. Contributors should keep these
differences in mind:

- Run this checkout's `node_modules/.bin/haxe`, not an unqualified global
  `haxe`; the latter may be 4.3.7 for work on a sibling repository.
- Start production C builds with `--custom-target c=<output>`. Haxe 4-style
  `Cross`, a manual `-D c`, or `c_output` alone is not an equivalent carrier.
- Treat Haxe's typed AST (the compiler's already-resolved, typed program tree)
  as versioned input. For example, this compiler exhaustively handles Haxe 5's
  `AccPrivateCall`, which does not exist in 4.3.7. Do not copy a sibling
  compiler's pattern match without auditing the pinned Haxe 5 definitions.
- Treat the standard library as versioned semantic input too. Its declarations,
  conditionals, and implementation shapes differ between 4.3.7 and this
  preview, so source hashes, ledgers, snapshots, and differential oracles must
  match this repository's pin.
- `#if c` is reliable here because Haxe 5 establishes `c` during real target
  initialization. Keep such branches at genuine representation, ABI, or
  platform boundaries; they are not permission to duplicate portable logic.
- A Haxe compilation server and every client connected to it must use this
  exact Haxe 5 binary. Never reuse a sibling repository's Haxe 4 server. Server
  caching saves startup and repeated typing work; it does not make compiler
  versions or define sets interchangeable.
- Preview APIs can change. Upgrade only through the atomic pin, API, stdlib,
  snapshot, server-isolation, and native validation process below. A future
  stable Haxe release with the same proven custom-target contract is preferable
  to floating across previews.

## Install and verify

From a fresh checkout:

```sh
npm ci
npm run test:toolchain
```

`npm ci` installs the exact Lix artifact and `lix download` resolves the Haxe
version in `.haxerc`. `npm run test:toolchain` then verifies every pin and the
vendored content checksum before running compile-backed all-source and lifecycle
probes.
Governance CI currently runs this tooling on Node.js 24 with the current
documented checkout/setup actions. That CI selection does not silently change
the independently declared local package floor in `package.json`.

The probes cover:

- a dedicated HXML that dynamically covers every current `src/**/*.hx` and
  `std/c/**/*.hx` module, including explicit macro and non-macro compiler
  branches;
- source-checkout resolution from a nested working directory;
- production C activation through `--custom-target c=<directory>` and automatic
  `c.Init.init()` platform setup;
- derivation of the internal `c_output` transport from the custom-target output,
  plus missing-output and conflicting-output rejection;
- complete non-C isolation;
- public `c` versus implementation-only `reflaxe_c` visibility;
- bootstrap/init count `1` under duplicate calls;
- the exact static, scalar-Unicode, hosted/freestanding, thread, atomic, and
  `sys` platform snapshot;
- `target.unicode` without `target.utf16`/`utf16` and the selected typed fields
  in upstream Haxe 5.0.0-preview.1 `String`, `StringTools`, and `UnicodeString`;
- source-anchored `HXC0003` rejection of identity, UTF-16, unproven atomic, and
  freestanding capability conflicts;
- regression rejection of the legacy `Cross` carrier, whose UTF-16 facts
  contradict the C platform contract;
- bootstrap-before-init failure behavior and duplicate-call idempotence;
- back-to-back C/non-C/C builds through one Haxe compiler server;
- full Reflaxe registration followed by exact source-anchored `HXC1001` at the
  bootstrap fixture's first unsupported typed call, with no plausible generated
  artifact, in cold and compiler-server builds;
- two byte-identical renders apiece of the structural C11 declarator and
  expression/statement corpora, matched against
  `test/c_ast/expected/declarators.c` and `expressions.c`, with no runtime
  selection;
- two byte-identical declaration plans and planned-header renders, matched
  against `test/declaration_plan/expected/**`, including actionable complete
  cycle/source-placement diagnostics and no runtime selection;
- two byte-identical schema-18 HxcIR reports, including reversed unordered
  inputs, source-aware semantic/coverage goldens, exact UTF-8 String constants,
  explicit side-effect and cleanup order, every call dispatch form, named
  runtime intent, and stable negative diagnostics, plus Eval oracles;
- the generated literal-output project with exact ASCII/non-ASCII/NUL bytes,
  hosted error handling, and only the selected literal/I/O runtime closure;
- a temporary package layout with Reflaxe flattened into the package classpath;
- required license and provenance files in that staged package.

The temporary package is a validation fixture, not authorization to publish a
release. Generated-output, template, and runtime-slice licensing remains owned
by Beads decision `haxe_c-od2.5`.

## Native smoke matrix

`npm test` also runs `python3 scripts/ci/runtime_smoke.py`. Local auto mode
examines compiler identity rather than command names, runs every complete
GCC/G++ or Clang/Clang++ pair it finds, reports unavailable or aliased families
as explicit optional `SKIP` lines, and fails if no complete pair actually runs.

GitHub CI does not use that optional discovery contract. Its independent matrix
invokes `--toolchain gcc` and `--toolchain clang`; each named C11/C++17 pair is
required and a missing or mismatched executable is an error. Both lanes compile
with warnings as errors, compile and execute both structural C AST goldens, run
the hosted runtime seed and native fixtures, independently compile each planned
declaration header, run their combined C consumer, compile the freestanding
runtime path, and link a C++17 consumer of the runtime public header. The
runtime feature lane also rebuilds the exact compiler-selected literal/I/O
package, and each required C lane rebuilds the centrally owned generated hello
baseline at O0/O2 and checks exact stdout without needing Haxe in that job. The
`.cpp` consumer verifies C++17 compatibility and C linkage for the C header; it
is deliberately not generated C++ output. The
declarator, expression/statement, and declaration-header goldens are rendered
from target-owned structures by Haxe test fixtures. They are runtime-free
printer/planner proofs; they are not evidence that Haxe application lowering or
production multi-file emission exists. The remaining native lanes retain their
bounded seed/interop claims.

## Activation sequence

Direct Haxe/HXML use remains the compiler foundation:

```text
-lib reflaxe.c
--custom-target c=<directory>
  -> c.Init.init()
  -> TargetPlatform.configure()
  -> CompilerBootstrap.Start()
  -> CompilerInit.Start()
  -> c_output=<directory> (derived internal Reflaxe transport)
  -> exactly one Reflaxe compiler registration
```

`CompilerBootstrap` resolves the reviewed framework before initialization.
Before that sequence, Haxe resolves `c.Init` by convention and accepts its
`PlatformConfig`: static typing, scalar Unicode, block scoping, environment-
derived `sys`, and no threads/atomics until an adapter proves them.
`CompilerInit` validates the resulting compiler snapshot and output path before
registration. A caller-supplied `-D c` does not activate the compiler;
`c_output` alone is a legacy bootstrap signal rather than a conforming carrier;
and `target.*` facts are compiler-owned. The public `c` fact is exposed only
after valid activation, alongside implementation marker `reflaxe_c`.

`reflaxe_c_lifecycle_probe` is an internal seam that skips the deliberately
unimplemented lowering boundary while retaining the real `CustomTarget(c)`
platform. Eval is used only to prove non-C isolation and as a differential
oracle. A legacy Cross request still fails with `HXC0003`; do not relabel it,
copy host capabilities, or weaken scalar-Unicode semantics.

The checked-in `extraParams.hxml` deliberately contains no classpaths because
Haxelib evaluates it relative to the consumer project. Source-checkout paths in
`haxe_libraries/*.hxml` use `${SCOPE_DIR}`. Keep the Reflaxe library HXML free of
startup macros: `CompilerInit` owns the single `ReflectCompiler.Start()` call.

## Updating a pin

Treat the following as one atomic review:

1. Select concrete Haxe and Reflaxe releases/revisions; never use a floating
   branch or version range. A Haxe preview change requires auditing the
   custom-target and `PlatformConfig` APIs at the locked source revision.
2. Replace `vendor/reflaxe/src`, `vendor/reflaxe/haxelib.json`, and
   `vendor/reflaxe/LICENSE` with files from that exact commit.
3. Update the version, commit, Git-tree ID, file count, and bundle SHA-256 in
   `docs/specs/toolchain-lock.json`. The checker can print the candidate bundle
   digest with `python3 scripts/ci/check_toolchain.py --print-bundle-digest`.
4. Update `docs/specs/third-party-provenance.json` and
   `THIRD_PARTY_NOTICES.md` in the same change. Preserve the upstream MIT text.
5. If Haxe or Lix changes, update `.haxerc`, `package.json`, and
   `package-lock.json` together. Record the immutable Haxe source revision and
   official release-artifact hashes or npm integrity, not only a display
   version. Update the platform snapshot and ADR when carrier semantics move.
6. Run `npm ci`, `npm test`, `bash scripts/lint/whitespace_guard.sh`, and the
   repository pre-commit chain. A Reflaxe update is not accepted until the
   package, all-source, and lifecycle probes pass. The whitespace guard
   intentionally skips immutable third-party paths; their exact bytes are
   checked separately.

Do not patch vendored Reflaxe silently. A target-required framework change must
be documented as a minimal patch with an upstream reference and included in the
locked bundle digest; prefer an upstream fix and a new immutable commit.
