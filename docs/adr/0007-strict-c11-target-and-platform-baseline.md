# ADR 0007: Strict C11 target and platform baseline

- Status: Accepted
- Date: 2026-07-14
- Amended: 2026-07-15 (production Haxe carrier selected)
- Decision owners: project owner and compiler maintainers
- Related requirements: HXC-RT-011, HXC-RT-012, HXC-QA-002,
  HXC-QA-003, HXC-QA-007, HXC-QA-010

## Context

The selected C dialect affects every generated declaration, runtime source,
public header, native fixture, and downstream consumer. Target identity also
affects which Haxe standard-library branches are typed. Those choices cannot be
left to whichever compiler happens to build the first vertical slice.

A useful platform promise is more specific than an operating-system name. It is
a tested tuple of environment, operating system or C runtime, architecture,
compiler family, artifact kind, profile, and runtime capabilities. Cross
compilation alone is not proof that the resulting program runs correctly.

## Decision

### Strict ISO C11 is the normative source floor

Compiler-owned C, selected `hxrt` slices, generated public headers, and test
fixtures use strict ISO C11 with no language extensions by default. They must
compile warning-clean under the supported strict flags and must not depend on a
GNU, Clang, MSVC, POSIX, or platform extension unless an explicit capability or
adapter owns it.

The configured modes have these meanings:

- `c11` is the normative language and public-header baseline;
- `c17` compiles the same C11-compatible contract in C17 mode and does not
  change representation, behavior, or ABI;
- `c23` is opt-in and experimental until its syntax/features have compiler
  matrix evidence. It may improve internal spelling, but public headers remain
  C11-compatible and the selection may not silently change behavior or ABI.

`hxc_c_extensions=none` remains the default. `gnu` and `msvc` are explicit,
manifested compatibility modes for an owning platform adapter or typed unsafe
surface; they are not permission for normal lowering to emit arbitrary
extensions. Optional C11 facilities such as threads are capability-gated rather
than inferred merely from the language mode.

### The Haxe target identity is `c`

The production Haxe/Reflaxe bootstrap contract is:

- direct builds select `--custom-target c=<directory>`;
- Haxe invokes `c.Init.init()` before platform facts are finalized;
- `c.Init` installs the target-owned platform configuration, after which
  `CompilerBootstrap` and `CompilerInit` perform exactly-once Reflaxe
  registration;
- `CompilerInit` derives the internal `c_output` Reflaxe transport from the
  custom-target output. An explicitly supplied `c_output` is accepted only
  when it normalizes to the same path;
- compiler initialization exposes `c` as the public application conditional;
- `reflaxe_c` is an implementation-owned compatibility marker, not the
  recommended conditional for portable application code.

`c_output` remains the output define consumed by the reviewed Reflaxe API and
an early bootstrap detector. It is not a second production carrier: a lone
`-D c_output=...`, caller-supplied `-D c`, or rewritten `target.name` cannot
substitute for `--custom-target c=...`.

The target declares Unicode scalar string semantics by enabling
`target.unicode` and leaving `target.utf16` disabled. Environment capability
defines such as `target.sys`, `target.threaded`, and `target.atomics` are set
only when the resolved adapter actually provides them. Target initialization
must reject conflicting pre-existing identity or platform defines instead of
silently typing the wrong standard-library branch.

### Version 1.0 has explicit release-blocking lanes

The following combinations are the version 1.0 Tier 1 support promise. Exact
compiler and operating-system version floors live in the machine-readable
support matrix so they can be maintained without revising this architecture
record.

| Support lane | Environment and runtime | Architectures | Required compiler families | Required evidence |
| --- | --- | --- | --- | --- |
| Hosted Linux | Linux with glibc | `x86_64`, `aarch64` | GCC and Clang | Native compile, link, run, sanitizer, and C/C++ consumer lanes |
| Hosted macOS | Current supported macOS | `arm64`, `x86_64` | Apple Clang | Native compile, link, run, and C/C++ consumer lanes |
| Hosted Windows | Windows with UCRT/MSVC ABI | `x86_64` | `clang-cl` | Native compile, link, run, DLL/static-library, and C/C++ consumer lanes |
| Freestanding metal | Declared ARM Cortex-M board or emulator capability profile | at least one release-declared Cortex-M ISA plus `arm-none-eabi` tuple | GNU Arm Embedded GCC | Compile, link, map/symbol inspection, and emulator or hardware smoke with `metal` plus `none`/allowed `minimal` runtime |

The hosted lanes own the applicable portable Haxe standard-library contract.
The freestanding lane is an explicit capability-limited metal contract; it does
not imply hosted `sys`, tracing collection, general exceptions, or libc.

Tier 2 covers combinations with maintained compile or conformance evidence but
which are not 1.0 release blockers: native MSVC `cl`, Linux/musl and additional
architectures, WASI SDK, Emscripten, and additional Clang freestanding profiles.
Tier 3 covers documented embedded vendor toolchains. A release may promote a
combination only by updating the support matrix and adding non-skippable CI or
archived external-run evidence.

The planner resolves support from the whole tuple. Unsupported combinations
fail before native compilation with the missing capability; they do not inherit
support from a matching operating-system name. Native-run obligations are
recorded separately from compile-only or cross-compile obligations.

### M0 production carrier: exact Haxe 5 custom target

The production typing carrier is the exact official
`Haxe 5.0.0-preview.1` release at source revision
`2c1e544e0a2c7524ef4c8e103f1b0580362ea538`. Haxe 5 custom targets provide the
early platform-configuration hook that C needs. `c.Init` installs this initial
configuration:

- static type system and block scoping;
- Unicode support with scalar, non-UTF-16 strings;
- `target.sys` only for the resolved `hosted` environment;
- no advertised thread or atomic support until an adapter proves it;
- no inherited platform capabilities from whichever host executes Haxe.

The exact preview is checksum-locked for Linux, macOS, and Windows. The custom
target API is preview surface and may change, so a broader Haxe range is not
permitted without a pin update, API/source audit, snapshot review, and complete
toolchain test evidence. Moving back to Haxe 4 would require maintaining and
distributing a compiler fork and is not the selected baseline.

The earlier Haxe 4.3.7 experiment remains important negative evidence:

- a Reflaxe invocation without a native Haxe output target uses Haxe 4.3.7's
  `Cross` platform configuration;
- `Cross` initializes `target.name=cross`, `target.utf16`, `utf16`,
  `target.sys`, and static-platform facts before initialization macros run;
- `haxe.macro.Compiler.define` can add or overwrite a value, but Haxe 4.3.7's
  public macro API cannot remove an existing define. `Context.getDefines()` is
  explicitly a copy;
- overwriting only `target.name` would therefore leave upstream and application
  `target.utf16` branches active and violate this ADR and ADR 0004.

The lifecycle harness now exercises the real `CustomTarget(c)` platform and
uses its probe switch only to stop before the intentionally unimplemented
lowering boundary. It snapshots the exact platform configuration, target
defines, upstream `String`/`StringTools`/`UnicodeString` branches, package
layout, and compiler-server isolation. A non-probe production request reaches
the target-owned compiler and then fails at the source root with `HXC1000`,
without emitting an artifact. Eval remains a target-neutral host for future
`hxc` bootstrap code, a non-C isolation test, and a differential semantic
oracle; it is never the production carrier for a user program.

A legacy `Cross` request continues to fail with source-anchored `HXC0003`.
That regression guard prevents a future refactor from reviving contradictory
UTF-16 state by relabeling Cross.

## Consequences

- The C AST and runtime can rely on one conservative syntax floor.
- C17/C23 and extension flags are reproducible toolchain selections rather
  than hidden semantic profiles.
- Haxe standard-library conditionals see one compiler-owned target identity and
  a scalar, non-UTF-16 Unicode model during real production typing.
- Depending on a preview compiler API is an explicit, bounded bootstrap risk;
  exact pins, artifact hashes, and compile-backed snapshots make that risk
  reviewable.
- Windows is a 1.0 hosted platform without requiring the initial compiler to
  normalize every optional C11 facility to native MSVC syntax.
- Embedded use is release-tested without overclaiming the hosted portable
  standard library on a freestanding target.
- CI and release metadata must distinguish native execution, emulated
  execution, cross compilation, and consumer-header compatibility.

## Rejected alternatives

- GNU C as the default: it would make portability and no-extension claims
  false at the first implementation layer.
- C23 as the initial floor: its compiler reach is narrower and it provides no
  semantic benefit worth making public headers less consumable.
- Treating `c_output` as the application conditional: it couples source
  semantics to a build-path transport detail.
- Using Haxe 4 Cross and relabeling only `target.name`: its unremovable UTF-16
  facts contradict the accepted String contract.
- Maintaining a Haxe 4 compiler fork for M0: it adds a compiler distribution
  and patch-maintenance burden when the official Haxe 5 custom-target hook
  supplies the required lifecycle.
- Calling an operating system “supported” after one cross-compile: it does not
  test runtime, ABI, platform adapter, or standard-library behavior.
- Claiming the full portable standard library for freestanding builds: those
  environments deliberately lack many hosted capabilities.
