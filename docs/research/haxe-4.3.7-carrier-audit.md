# Haxe 4.3.7 carrier audit

- Audit date: 2026-07-18
- Requested candidate: official Haxe 4.3.7
- Candidate source revision: `e0b355c6be312c1b17382603f018cf52522ec651`
- Current carrier: official Haxe 5.0.0-preview.1 at
  `2c1e544e0a2c7524ef4c8e103f1b0580362ea538`
- Governing decisions: [ADR 0004](../adr/0004-utf8-scalar-string-contract.md)
  and [ADR 0007](../adr/0007-strict-c11-target-and-platform-baseline.md)

## Question and outcome

Can reflaxe.c replace its exact Haxe 5 preview pin with the unmodified official
Haxe 4.3.7 release while retaining the accepted C target identity, scalar
Unicode platform facts, target-owned environment capabilities, Reflaxe
lifecycle, and current typed compiler source?

No. The official Haxe 4.3.7 compiler has no custom-target command and no macro
API that can install or replace a platform configuration. A Reflaxe build with
only an output define therefore types as Haxe `Cross`, which publishes static,
hosted, UTF-16 platform facts before initialization macros run. Those facts
cannot be removed through the public macro API. Using Eval or another built-in
target would substitute that target's typing and standard-library contract
rather than create a C target.

The current compiler source also uses Haxe 5 typed-AST surface that is absent in
4.3.7. A direct Haxe 4 all-source probe stops at `AccPrivateCall`, before any
production C carrier could be exercised. A stable-4 migration is consequently
not a version-string change: it requires both a maintained compiler fork with a
custom-target backport and an intentional compiler/stdlib compatibility port.

The accepted decision remains to keep the checksum-locked Haxe 5 preview until
either a stable upstream Haxe release supplies the required custom-target
contract or the project owner explicitly accepts ownership of a Haxe 4 fork.

## Source evidence

All Haxe paths below refer to the official `4.3.7` tag unless stated otherwise.

### There is no C-capable target hook in 4.3.7

- `src/compiler/args.ml` registers the built-in target switches. It has no
  `--custom-target` option.
- `src/compiler/compiler.ml` calls `set_platform com Cross ""` when no built-in
  target is selected.
- `src/context/common.ml` defines the Cross `default_config` with
  `pf_static = true`, `pf_sys = true`, `pf_uses_utf16 = true`, and
  `pf_supports_unicode = true`; `get_config` returns that record for `Cross`.
- `init_platform` publishes `target.static`, `target.sys`, `target.utf16`,
  `utf16`, `target.unicode`, and `target.name=cross` from that configuration.
- `std/haxe/macro/Compiler.hx` exposes `define`, `getOutput`, `setOutput`, and a
  read-only `getConfiguration` result. It does not expose
  `setPlatformConfiguration`.
- `std/haxe/macro/Context.hx` documents that modifying the map returned by
  `getDefines()` has no effect on the compiler. There is no public undefine
  operation.

Haxe 5 commit `4d9bd89703635716b663fdaaa8557dbe7159f98e`
(`Add "Custom" target (#11128)`) introduced `--custom-target`,
`CustomTarget(name)`, and `Compiler.setPlatformConfiguration`. The original
change touched 37 compiler, macro, standard-library, and test files. It is not
a target-library shim that can be reproduced by changing reflaxe.c macros.

At the current Haxe 5 pin:

- `src/compiler/args.ml` owns `--custom-target`;
- `src/context/common.ml` defers `CustomTarget` platform facts to the macro API;
- `std/haxe/macro/Compiler.hx` exposes `setPlatformConfiguration`; and
- `c.Init.init()` can install the static, scalar-Unicode, environment-derived C
  configuration before standard-library and application typing is finalized.

### Built-in carriers are not substitutes

Haxe 4 Eval has non-UTF-16 strings, but its configuration is dynamic, pads
nulls, enables thread support, wraps captured references, advertises
`target.name=eval`, and invokes the interpreter. Cross is static but publishes
UTF-16 and hosted `sys`. Every other selectable carrier similarly owns its own
target identity, platform configuration, `_std` branches, and generator
filters. Relabeling one define after initialization would leave those semantic
facts in place.

Target-specific `_std` replacements also cannot correct application code or
macros that branch on `target.utf16`, `target.sys`, `target.threaded`, or
`target.name`, nor can they replace the compiler's already-selected
`PlatformConfig`. This is why the current lifecycle rejects rather than
relabels legacy Cross.

## Executable evidence

With `HAXE_437` set to the official 4.3.7 executable:

```sh
"$HAXE_437" --help | rg 'custom-target'
"$HAXE_437" --custom-target c=out
```

The help contains no custom-target entry and the second command reports:

```text
Error: : unknown option '--custom-target'.
```

The checked-in lifecycle boundary can be exercised without Lix or a selected
native generator:

```sh
"$HAXE_437" \
  -cp test/bootstrap \
  -cp src \
  -cp std \
  -cp vendor/reflaxe/src \
  -D reflaxe=4.0.0-beta \
  -D reflaxe_c_lifecycle_probe \
  -D c_output=bootstrap-probe-output \
  --macro 'reflaxe.c.CompilerBootstrap.Start()' \
  --macro 'reflaxe.c.CompilerInit.Start()' \
  -main BootstrapProbe \
  --no-output
```

It reaches the intended source-positioned failure:

```text
HXC0003: Haxe carrier `cross` exposes `target.utf16`, but reflaxe.c requires Unicode-scalar String indexing.
```

A direct compatibility type-check using the same classpaths and the all-source
fixture currently fails because Haxe 4.3.7's `FieldAccess` enum has no
`AccPrivateCall` constructor. Haxe 5 defines that constructor in
`std/haxe/macro/Type.hx`, and current aggregate/class lowering handles it
explicitly. This is concrete compiler-preview API drift to port, not a reason
to weaken exhaustive matching.

The standard-library review is also material. Between the two exact source
tags, `std/` has 392 changed paths (387 Haxe source paths), with 9,101 inserted
and 19,353 removed lines in the tag-to-tag diff. `String`, `StringTools`,
`UnicodeString`, `haxe.macro.Compiler`, `PlatformConfig`, and `Type` all differ.
The schema-2 stdlib ledger, primitive contracts, divergence corpus, lifecycle
snapshot, and source hashes must therefore be regenerated and reviewed rather
than textually relabeled.

## Alternatives

| Alternative | Technical result | Decision |
| --- | --- | --- |
| Pin unmodified official Haxe 4.3.7 | No custom target; Reflaxe falls through to Cross and inherits unremovable UTF-16/hosted facts | Rejected |
| Relabel Cross and replace selected `_std` modules | Leaves compiler and application conditionals inconsistent | Rejected |
| Use Eval, Python, HL, or another built-in target as a carrier | Types the program under another target's semantics and generator policy | Rejected |
| Backport Haxe custom targets onto a 4.3.7 fork | Can be made technically viable, but creates a compiler distribution, patch, artifact, server, and security-maintenance obligation | Requires a new owner-approved ADR |
| Keep exact Haxe 5.0.0-preview.1 | Preserves the already-tested target identity and platform contract; retains bounded preview-API risk | Current accepted choice |
| Move to a future stable Haxe release with custom targets | Removes the preview-release concern if source/API and snapshot audits pass | Preferred future migration |

## Requirements for an authorized Haxe 4 fork

If the project deliberately chooses a stable-version fork, the migration must
at minimum:

1. publish an immutable fork revision and reviewed patch series derived from
   the custom-target change, including upstream attribution;
2. build and checksum reproducible Linux, macOS, and Windows compiler
   artifacts instead of continuing to claim official 4.3.7 artifacts;
3. audit compiler-server reset behavior, macro API encoding, target setup,
   package rules, standard-library precedence, generation suppression, and
   every changed compiler file;
4. port repository-owned Haxe source to the 4.3.7 typed-AST API without
   weakening exhaustive semantic handling;
5. regenerate the target/platform snapshot, stdlib ledger, primitive and
   divergence contracts, provenance, notices, package probes, and capability
   evidence; and
6. run the complete toolchain, compiler-server, generated-C, native,
   sanitizer, governance, and reproducibility gates on every supported host.

Until that scope is explicitly accepted, changing `.haxerc` to `4.3.7` would
produce a misleading pin and a non-conforming compiler.
