# Pinned development toolchain

The M0 compiler contract is Haxe 4.3.7 plus one reviewed Reflaxe
4.0.0-beta-compatible revision. Lix supplies the Haxe executable and scoped
library resolution; Reflaxe itself is vendored so source checkouts and staged
packages do not drift with a branch or registry release.

## Immutable inputs

| Input | Pin | Verification |
| --- | --- | --- |
| Haxe | `4.3.7`, tag revision `e0b355c6be312c1b17382603f018cf52522ec651` | `.haxerc`, `toolchain-lock.json`, active `haxe -version` |
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
vendored content checksum before running compile-backed lifecycle probes.

The probes cover:

- source-checkout resolution from a nested working directory;
- C activation through `c_output=<directory>` and a compiler-owned
  `target.name=c` stand-in;
- complete non-C isolation;
- public `c` versus implementation-only `reflaxe_c` visibility;
- bootstrap/init count `1` under duplicate calls;
- `target.unicode` without `target.utf16`/`utf16` and the selected typed fields
  in upstream Haxe 4.3.7 `String`, `StringTools`, and `UnicodeString`;
- source-anchored `HXC0003` rejection of identity, UTF-16, unproven atomic, and
  freestanding capability conflicts;
- explicit rejection of the real Haxe 4.3.7 `Cross` carrier, whose UTF-16
  facts cannot be removed by a public initialization macro;
- bootstrap-before-init failure behavior and duplicate-call idempotence;
- back-to-back C/non-C/C builds through one Haxe compiler server;
- a temporary package layout with Reflaxe flattened into the package classpath;
- required license and provenance files in that staged package.

The temporary package is a validation fixture, not authorization to publish a
release. Generated-output, template, and runtime-slice licensing remains owned
by Beads decision `haxe_c-od2.5`.

## Activation sequence

Direct Haxe/HXML use remains the compiler foundation:

```text
-lib reflaxe.c
-D c_output=<directory>
  -> CompilerBootstrap.Start()
  -> CompilerInit.Start()
  -> exactly one Reflaxe compiler registration
```

`CompilerBootstrap` resolves the reviewed framework before initialization.
`CompilerInit` does nothing unless `c_output` is non-empty or custom-target
plumbing reports `target.name=c`. A caller-supplied `-D c` does not activate the
compiler; `c` is a public target fact exposed only after valid activation.
Initialization also exposes the implementation marker `reflaxe_c` and
`target.unicode`, never `target.utf16`.

That paragraph is the semantic target contract, not a claim that Haxe 4's
default `Cross` carrier already satisfies it. Haxe 4.3.7 reserves command-line
`target.*` flags and its `Cross` platform predefines `target.utf16`/`utf16`
before macros. The checked `reflaxe_c_lifecycle_probe` uses Eval only to test
macro lifecycle and upstream scalar branch selection; it is forbidden in
production. A non-probe Cross build fails with `HXC0003` pending decision
`haxe_c-od2.6`. Do not work around that failure by defining `c`, overwriting
`target.name` alone, or treating the probe's inherited `target.sys` and
`target.threaded` as target capabilities.

The checked-in `extraParams.hxml` deliberately contains no classpaths because
Haxelib evaluates it relative to the consumer project. Source-checkout paths in
`haxe_libraries/*.hxml` use `${SCOPE_DIR}`. Keep the Reflaxe library HXML free of
startup macros: `CompilerInit` owns the single `ReflectCompiler.Start()` call.

## Updating a pin

Treat the following as one atomic review:

1. Select a concrete upstream Reflaxe commit; never use a floating branch.
2. Replace `vendor/reflaxe/src`, `vendor/reflaxe/haxelib.json`, and
   `vendor/reflaxe/LICENSE` with files from that exact commit.
3. Update the version, commit, Git-tree ID, file count, and bundle SHA-256 in
   `docs/specs/toolchain-lock.json`. The checker can print the candidate bundle
   digest with `python3 scripts/ci/check_toolchain.py --print-bundle-digest`.
4. Update `docs/specs/third-party-provenance.json` and
   `THIRD_PARTY_NOTICES.md` in the same change. Preserve the upstream MIT text.
5. If Haxe or Lix changes, update `.haxerc`, `package.json`, and
   `package-lock.json` together. Record the immutable Haxe source revision or
   npm integrity, not only a display version.
6. Run `npm ci`, `npm test`, `bash scripts/lint/whitespace_guard.sh`, and the
   repository pre-commit chain. A Reflaxe update is not accepted until the
   package and lifecycle probes pass. The whitespace guard intentionally skips
   immutable third-party paths; their exact bytes are checked separately.

Do not patch vendored Reflaxe silently. A target-required framework change must
be documented as a minimal patch with an upstream reference and included in the
locked bundle digest; prefer an upstream fix and a new immutable commit.
