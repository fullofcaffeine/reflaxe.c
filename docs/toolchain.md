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
- full Reflaxe registration followed by source-anchored `HXC1000`, with no
  plausible generated artifact, in cold and compiler-server builds;
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
with warnings as errors, run the hosted runtime seed and native fixtures,
compile the freestanding runtime path, and link a C++17 consumer of the public C
header. These are native seed/interop checks, not generated-Haxe evidence or a
runtime feature-selection claim.

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
