# Architecture overview

## Core invariant

One compiler pipeline serves both `portable` and `metal`. Profiles select representation policy, fallback permission, diagnostics, and runtime constraints; they do not fork the compiler.

## Components

```text
BuildDetection
CompilerBootstrap
CompilerInit
  -> BuildContextResolver
  -> boundary macros
  -> Reflaxe registration

CReflaxeCompiler
  -> collects Haxe module types
  -> resets CompilationContext
  -> invokes CCompiler
  -> writes GeneratedFile records through OutputManager

CCompiler
  -> validates semantic contract
  -> lowers typed AST to HxcIR
  -> runs analyses and HxcIR passes
  -> lowers to C AST
  -> runs C passes
  -> invokes ProjectEmitter

ProjectEmitter
  -> public/private headers
  -> source files
  -> boot/export/reflection units
  -> runtime slices
  -> CMake/Meson/tool-neutral manifest
  -> runtime, symbol, stdlib, ABI, and lowering reports
```

## Why `Manual` Reflaxe output

A compilation produces multiple file categories and sidecar reports. `Manual` lets the compiler control paths while retaining Reflaxe's changed-file and stale-file ownership behavior.

## IR boundary

`HxcIR` exists to normalize semantics that C syntax cannot safely express directly:

- evaluation sequence;
- expression-valued control flow;
- cleanup and exception edges;
- addressability;
- representation choices;
- allocation/ownership;
- dispatch classification.

`CAST` models C declarations and syntax precisely. It does not decide Haxe semantics.

## Target and native baseline

The Haxe target identity is `c`: `c_output=<directory>` is the reliable Haxe 4
bootstrap/output signal, `target.name=c` is used where custom-target plumbing
supplies it, and initialization exposes the `c` conditional. `reflaxe_c` is an
implementation compatibility marker, not the application portability contract.

The target enables `target.unicode` without `target.utf16`. Other platform
defines are capability facts: `target.sys`, `target.threaded`, and
`target.atomics` are enabled only by adapters that implement them.

The pinned Haxe 4.3.7 implementation currently has a verified carrier gap.
Its default Reflaxe `Cross` configuration installs `target.utf16`/`utf16`
before macros and offers no public undefine operation. `CompilerInit` therefore
normalizes identity and scalar-Unicode branches only under the explicit
lifecycle fixture, while a real Cross request fails with `HXC0003`. Decision
`haxe_c-od2.6` blocks production bootstrap and String work; relabeling Cross or
copying Eval's `target.sys`/`target.threaded` observations is not an
architectural substitute.

Strict ISO C11 without extensions is the generated-source, runtime, fixture, and
public-header floor. C17 preserves the same contract; C23 syntax remains an
opt-in internal experiment and may not change ABI. Support is resolved from an
environment/OS-runtime/architecture/compiler/capability tuple rather than an OS
name. See [ADR 0007](adr/0007-strict-c11-target-and-platform-baseline.md).

## Fail-closed bootstrap

The scaffold's compiler classes establish lifecycle and interfaces, but broad language lowering is not marked complete. Until a construct has a tested lowering, the implementation must issue a stable diagnostic. Silent placeholder code is prohibited.

## Runtime feature graph

Runtime planning follows representation selection, escape/lifetime analysis, and
specialization. For each operation the compiler prefers, in order: direct
idiomatic C, a program-local specialized helper, the narrowest dependency-closed
`hxrt` feature, or a policy diagnostic. There is no unconditional runtime core.

`RuntimeRequirementAnalyzer` records stable root reason kinds, consumed typed
surfaces, and source spans. `RuntimeFeatureRegistry` resolves deterministic
dependencies while preserving root-versus-transitive provenance.
`ProjectEmitter` copies/emits only selected features.
`hxc_runtime=none` asks `NoRuntimeEligibilityAnalyzer` for a proof; a failed
proof returns every blocking feature and source site. A successful proof is also
written to `hxc.runtime-plan.json` and means no `hxrt` include, source, define,
library, or symbol exists in the build.

Portable defaults to `auto + summary`; metal defaults to `minimal + warn`.
These are presets over independent axes, not separate compiler pipelines. See
[ADR 0001](adr/0001-direct-c-and-selective-runtime.md).

## String and managed-memory model

Portable `String` is a private immutable valid-UTF-8 representation. Every Haxe
index counts Unicode scalar values, embedded NUL is content, normalization is
never implicit, and malformed external UTF-8 has distinct lossy and checked
paths. Binary `Bytes`, NUL-terminated `c.CString`, and exported UTF-8 views keep
their own units, ownership, and lifetime contracts. String support is a separate
runtime feature and does not by itself require objects, reflection, or the
collector. See [ADR 0004](adr/0004-utf8-scalar-string-contract.md).

When object-graph semantics still require tracing after escape/region analysis,
the default `gc` slice is precise, non-moving, stop-the-world mark-and-sweep.
Generated trace descriptors plus explicit global/stack/thread root chains are
the authority; arbitrary C addresses are not scanned for roots. Interior and
foreign pointers require a live typed base/root or pinned handle. Backend
details never enter exported layouts, and `hxc_runtime=none` emits none of this
machinery. See [ADR 0005](adr/0005-precise-nonmoving-collector.md).

## Failure and unwind model

HxcIR represents exceptional successors, cleanup checkpoints, and all exits
before choosing C control flow. Closed regions use reported result/status
lowering when it is provably equivalent. General portable behavior selects the
isolated `exception` slice, whose strict-C11 frame/cleanup chain contains
`setjmp`/`longjmp` and handles C automatic-local rules explicitly.

Exports and callback trampolines catch and translate every Haxe exception to a
declared C status/error contract. A non-local transfer never crosses a foreign
frame, public ABI, signal, or thread boundary. See
[ADR 0006](adr/0006-explicit-failure-edges-and-contained-unwinding.md).

## Typed C authoring boundary

The compiler consumes C-native intent as structured typed facts, not normal-path
code strings:

```text
Haxe declarations and types
  -> typed c.* abstractions
  -> validated metadata/macros
  -> deterministic TypedCContractSnapshot
  -> declaration, header, layout, ownership, and build facts
  -> HxcIR / C AST / neutral manifest
```

Header/source grouping, forward declarations, includes, linkage, visibility,
calling conventions, qualifiers, layout, ownership, and compile-time assertions
share one declaration model with bindgen and ABI export. Haxe macros can reject
invalid combinations at the original source span; generated `_Static_assert`,
Clang-derived header facts, compiled probes, and C/C++ consumers provide the
native verification layer.

Ordinary Haxe comes first, followed by typed `c.*`, validated metadata/macros, a
narrow typed DSL only for a real language gap, and finally explicit raw C
authority. Every macro/DSL must expose its expansion and allocation, ownership,
unsafe, portability, and runtime effects. See
[ADR 0002](adr/0002-haxe-first-typed-c-authoring.md).

The M0 collector is installed once per C compilation and rebuilds its snapshot
from that compilation's typed module set. It owns no cross-build registry and
writes no files. Its report seam is test-only; the future compiler consumes the
snapshot and routes manifests and headers through Reflaxe output ownership. See
[typed C authoring](typed-c-authoring.md).

## ABI boundary

The internal object representation is not the public ABI. Export lowering maps Haxe values to a separate ABI model with:

- fixed scalars;
- frozen plain structs where explicitly selected;
- opaque handles;
- string/byte views;
- callbacks plus context;
- status and structured error outputs;
- explicit ownership.

## Build boundary

The compiler emits a tool-neutral `hxc.manifest.json`. CMake and Meson are adapters. Existing build systems can consume the manifest and generated source tree directly.

## CLI bootstrap boundary

The optional `hxc` orchestration layer is one target-neutral Haxe core with
narrow host-service seams for files, processes, environment, and terminal I/O.
It is not part of Reflaxe compiler registration and cannot be required for
direct Haxe/HXML compilation.

The bootstrap host is Haxe Eval, exposed through `haxe --run` and
`haxelib run reflaxe.c`. Once the C target supports the CLI's required semantic
and `sys` surface, the same core is compiled through `reflaxe.c` into the native
release executable. Eval remains the recovery implementation and differential
oracle. Native `hxc` must be rebuildable from direct Haxe/HXML plus the C
toolchain, never only by invoking an existing native `hxc`.
