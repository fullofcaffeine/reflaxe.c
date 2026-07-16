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
  -> captures the complete Haxe module-type set before Reflaxe filters
  -> resets CompilationContext and request-local adapter state
  -> invokes TypedAstNormalizer
  -> invokes CCompiler
  -> validates and writes GeneratedFile records through Reflaxe OutputManager

CCompiler
  -> consumes TypedProgramInput from CompilationContext
  -> current E2.T06: collects reachable typed functions and captured primitive globals
  -> CBodyLowering maps admitted signatures, ordered expressions, calls, bodies, and UB-safe primitive operations
  -> selects the deterministic request-local helper closure before symbol finalization
  -> validates HxcIR before CBodyEmitter constructs structural C
  -> emits a private prototype header, source definitions, and int main(void)
  -> invokes ProjectEmitter with analyzed zero-runtime primitive facts

ProjectEmitter
  -> public/private headers
  -> source files
  -> boot/export/reflection units
  -> runtime slices
  -> tool-neutral manifest and typed build facts
  -> runtime, symbol, stdlib, ABI, and lowering reports

Build adapters (future E1.T08/E8)
  -> consume the one neutral manifest
  -> CMake/Meson/direct compiler plans
```

## Why `Manual` Reflaxe output

A compilation produces multiple file categories and sidecar reports. `Manual`
lets the compiler control paths while retaining Reflaxe's changed-file and
stale-file ownership behavior. `GeneratedFile` admits only normalized relative
paths, closed artifact kinds, canonical text, and verified SHA-256 content.
`ReflaxeOutputWriter` validates both the old ownership set and the new file set
before the first write, rejects descendant symlinks or unowned destinations,
then delegates every save and stale deletion to `OutputManager`. The pure typed
emitter contains no output root or host fact. See [project-emission
boundary](project-emission.md).

Adversarial determinism checks compare normal compiler artifacts as raw bytes
across isolated roots, input order, locale, CRLF source/configuration input,
renamed ownership, and warm compiler-server requests. They sort paths by UTF-8
and identify the first differing path and byte offset. Reflaxe invocation state
in `_GeneratedFiles.json` is validated as engine activity metadata rather than
normalized into the compiler-artifact comparison.

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

The schema-2 semantic core is implemented under `src/reflaxe/c/ir/` and its
normative internal invariants are documented in [HxcIR semantic
contract](hxc-ir.md). Immutable values are block-local and definition-ordered;
mutable storage uses structural places; cross-block data uses typed block
parameters. Cleanup actions are registered in source order while every edge
records their validated reverse, inner-to-outer execution order. Calls and
memory operations distinguish static/direct, program-local, and named runtime
implementations, so the IR never selects an implicit runtime core.

Primitive representation is owned by the typed
`src/reflaxe/c/semantics/` layer. It maps real Haxe compiler types to exact
fixed-width or unresolved target-ABI integer identities, preserves tagged
scalar versus pointer nullability, and records exact, wrapping, checked, or
saturating conversions before C syntax exists. Typed unary and binary operation
decisions likewise record direct or named program-local implementation intent.
Portable and metal share these ordinary Haxe mappings and operation rules.
Primitive decisions cannot select `hxrt`; see the [primitive semantic
contract](primitive-semantics.md), [UB-safe arithmetic
lowering](arithmetic-semantics.md), and [ADR
0008](adr/0008-primitive-representations-and-conversions.md).

The target-owned typed-input boundary is implemented under
`src/reflaxe/c/frontend/`. `filterTypes` captures and normalizes the complete
request before Reflaxe suppresses or preprocesses callback inputs, so externs,
typedefs, and original field expressions remain available.
`TypedAstNormalizer` sorts logical module/declaration sets, preserves ordered
field, enum-constructor, metadata, and expression-root sequences with explicit
ordinals, records primary/secondary module ownership, retains raw compiler
objects for lowering, and carries `getMainModule()` plus
`getMainExpr()` as the entry point, including an eagerly captured static target
for the omitted-main-module fallback. A fresh `CompilationContext` owns the
result once plus a fresh empty `CSymbolRegistry`; request-local captures are cleared
before `CCompiler` runs. See
[typed-AST input boundary](typed-ast-input.md).

`TypedAstInventory` exposes a path-stable implementation report for unsupported
node planning. It classifies declarations, class/field kinds, expression nodes,
types, and metadata without serializing raw host positions. The exact
Reflaxe-injected build-cache hook is excluded from the normalized metadata view
because it appears only after cache reuse; source-authored metadata and the raw
declaration remain intact. Forward/reversed input and cold/compiler-server
fixtures are byte-identical. The inventory itself still performs no semantic
lowering; its report is emitted immediately before the E2 body pipeline
consumes the retained raw typed expression.

`CDiagnostic` is the single target-owned diagnostic boundary. Its typed ID
registry is checked against the schema-2 catalog, while `CDiagnosticRecord`
provides deterministic schema-1 JSON with severity, phase, kind, profile,
normalized primary source range, remediation, notes, and related locations.
Production call sites cannot embed raw ID prefixes. Unsupported source
(`HXC1001`), the scaffold capability boundary (`HXC1000`), and internal
compiler failure (`HXC9000`) remain structurally distinct. Public CLI/event
framing and native-tool attachments remain E8.T09 rather than leaking into the
compiler core. See [diagnostic contract](diagnostics.md).

`HxcIRDumper` canonicalizes only semantically unordered collections and retains
ordered instructions, edge arguments, and cleanup steps with repository-relative
source spans. `HxcIRValidator` rejects missing targets/results/terminators,
use-before-definition, illegal lifetime transitions, malformed cleanup paths,
primitive runtime fallback, or checked/nullable unwraps without a failure
edge. It also checks constant/result and return types plus load, initialization,
store, and address types whenever the place type is structurally resolvable;
the E2.T02 slice uses local places. Unsupported typed nodes use exact
source-positioned `HXC1001`; they never become an opaque value. E2.T02 lowers
primitive constants, initialized locals/reads, nested cleanup-free blocks, and
returns. E2.T03 adds primitive parameters, explicit argument conversions, and
direct static calls, then emits the reachable graph with a private prototype
header, deterministic source partition, and hosted `int main(void)` wrapper.
E2.T04 adds captured primitive globals, stable loads, local/global assignment,
lazy Boolean and value-conditional block graphs, and unsigned increment. E2.T05
adds typed arithmetic/comparison operations, wrapping signed updates, compound
assignment, masked shifts, zero-safe division/modulo, bit operations, and the
defined `Std.int` conversion. E2.T06 adds statement conditionals, primitive
pre/post-test and range-loop graphs, innermost loop jumps, and `Int`
statement/value switches. Every C switch arm jumps explicitly to its HxcIR
target, and a compiler-exposed value-switch carrier is admitted only after all
case/default assignments are structurally proven. Within the remaining
unconditional single-block subset, a
closed direct-call cycle is compiler-proven non-returning and emitted with
structural C11 `_Noreturn` plus no unreachable return. Direct
self-tail calls use registry-named typed argument temporaries and a structural
loop; other closed-cycle members are isolated in deterministic translation
units behind the shared header so strict optimized compilation remains
warning-clean. See
[primitive function-body lowering](body-lowering.md) and [static function
lowering](function-lowering.md). The E2.T04 stable-value and CFG contract is in
[explicit evaluation order and control flow](evaluation-order.md); E2.T05 operation selection is
in [UB-safe primitive arithmetic](arithmetic-semantics.md). E2.T08 adds typed
fixed-array storage, borrowed span views, explicit bounds policies, and direct
guarded span iteration; its representation and proof matrix are in
[fixed arrays and span-based iteration](span-lowering.md).

`CBodyLowering` prepares the complete admitted HxcIR function set, scans it for
program-local primitive helper IDs, computes their dependency closure, and
registers every helper/parameter/standard symbol before sealing the
per-compilation symbol registry. Function requests use translation-unit
ordinary namespace; locals use the finalized function scope plus lexical source
ordinals, so shadowing is stable without deriving C identifiers from Haxe text.
`CBodyEmitter` receives only validated HxcIR and finalized `CIdentifier` values,
and builds strict structural statements plus optional typed `#line` nodes.
Direct-call arguments remain ordered HxcIR instructions, conversions precede
their calls, and every load plus each consumed call result becomes a typed
stable-value temporary instead of a C subexpression with weaker evaluation
order. Lazy/conditional expressions remain explicit labels and edges rather
than C operators. `CPrimitiveHelperEmitter` builds only selected private
`static inline` helpers through structural CAST; safe unsigned fast paths remain
direct C, and floating modulo contributes the exact `m` build fact without a
runtime feature. Fixed arrays remain structural array declarators, while spans
remain typed pointer-plus-`size_t` views. Bounds proof nodes survive through
validated HxcIR; only exact static constants or typed `size_t` loop guards
remove a check, and a dynamic index emits a signed/element-count fail-stop
branch. The typed Haxe
iterator surface is consumed at this boundary and never becomes an IR or C
runtime object. `CStaticFunctionProjectEmitter`
places all prototypes before definitions, partitions proven non-returning cycle
members, and produces a runtime-free project in
both portable and metal. None of these stages can select `hxrt`.

The type model normalizes base specifiers and keeps them separate from a
grammar-level `CDeclarator` tree. Pointer, array, function, parenthesized, and
abstract declarators therefore retain their association without reconstructing
or parsing target-code strings. Final C tokens use validated `CIdentifier`
values; array bounds, parameter-list forms, C11 alignment/atomics, anonymous
aggregate specifiers, and compiler attributes remain typed nodes.

Expressions likewise retain their exact AST association. The printer combines
C11 precedence with grammar-category checks for assignment and prefix
increment/decrement operands, and inserts token boundaries where adjacent
unary operators could become a different token. Integer and floating constants
are validated representations rather than source fragments; `_Generic`
associations, comments, strings, byte characters, and `#line` anchors are also
structured or escaped at their owning boundary. There is no raw expression or
declaration node. Line anchors are syntax capability only: source-map policy,
sidecar provenance, and diagnostic remapping remain owned by E8.T08.

The printer's default dialect is strict ISO C11. Compiler attributes are
explicit extension nodes and require an explicit GNU or Clang mode. The
`test/c_ast` Haxe fixtures render deterministic declarator and
expression/statement goldens. The latter includes a literal 6×6 ordered
precedence-family matrix, every unary and binary operator, adversarial escaping,
and all statement shapes. Required GCC and Clang lanes compile and execute both
checked-in C files with no `hxrt` selection. Those broad printer fixtures remain
direct AST proofs; the separate body suite exercises only the admitted real
TypedExpr-to-HxcIR/C subset. Broader lowering remains fail-closed.

`CSymbolRegistry` owns the boundary before any finalized name reaches that C
AST. It batch-finalizes path-independent semantic requests against C's ordinary,
tag, member, and label namespaces; exact `@:c.name` values are preserved or
rejected, while compiler-owned defaults use readable `hxc_` provenance and
stable hash suffixes only for collisions or length limits. The schema-1 table
and collision ledger are the in-memory shape of `hxc.symbols.json`.
`TypedCNameFinalizer` applies that policy structurally to schema-2 typed C
snapshots before declaration planning. See [deterministic symbol
naming](symbol-naming.md).

## Target and native baseline

The Haxe target identity is `c`: production typing uses
`--custom-target c=<directory>`, Haxe automatically runs `c.Init.init()`, and
the target installs its platform configuration before standard-library and user
typing. Initialization exposes the `c` conditional. `reflaxe_c` is an
implementation compatibility marker, not the application portability contract;
`c_output` is derived internal Reflaxe transport, not an application identity.

The target enables `target.unicode` without `target.utf16`. Other platform
defines are capability facts: `target.sys`, `target.threaded`, and
`target.atomics` are enabled only by adapters that implement them.

The exact Haxe 5.0.0-preview.1 carrier installs a static, scalar-Unicode
`PlatformConfig`. Hosted owns `target.sys`; other environment and concurrency
facts remain disabled until adapters prove them. The lifecycle snapshot uses
that real custom target. Full registration now reports exact `HXC1001` at the
bootstrap fixture's first unsupported typed call without output; a completely
admitted primitive static graph emits an owned runtime-free C project. Legacy
Cross remains an `HXC0003` negative because relabeling its UTF-16 state is not
an architectural substitute. Eval remains only the future CLI bootstrap host,
an oracle, and a non-C isolation target.

Strict ISO C11 without extensions is the generated-source, runtime, fixture, and
public-header floor. C17 preserves the same contract; C23 syntax remains an
opt-in internal experiment and may not change ABI. Support is resolved from an
environment/OS-runtime/architecture/compiler/capability tuple rather than an OS
name. See [ADR 0007](adr/0007-strict-c11-target-and-platform-baseline.md).

## Fail-closed bootstrap

The scaffold's compiler classes establish lifecycle and interfaces, but broad language lowering is not marked complete. Until a construct has a tested lowering, the implementation must issue a stable diagnostic. Silent placeholder code is prohibited.

## Evidence and fixture boundary

The machine-readable fixture catalog in
[`docs/specs/fixture-taxonomy.json`](specs/fixture-taxonomy.json) maps every
current runner to one or more of eight evidence lanes: positive, negative,
AST/IR, snapshot, runtime, differential, ABI, and performance. New cases use a
schema-valid `case.json` in the corresponding canonical `test/<lane>/`
directory; existing M0 suites remain mapped in place to avoid a path-only
migration. The detailed runner and assertion contract lives in
[`docs/testing.md`](testing.md).

Snapshots are a review surface, not an architectural shortcut. The central
registry owns every checked-in expected tree, rerenders it twice, compares JSON
semantically and structural text byte-for-byte, and exposes explicit check and
update paths. Update mode prints the diff before per-file atomic replacement,
runs the owning suite, and is forbidden in CI.

Evidence does not leak across compiler layers. A direct HxcIR or C AST golden
proves that model/printer boundary only. A separately authored native C/C++
fixture proves a prospective runtime or interop contract only. Generated-Haxe
runtime, differential, ABI, sanitizer, and performance claims require their own
declared assertions and may not be inferred from either snapshot class.
Examples follow the same rule: they are run only through explicit case
manifests and never through an undeclared directory glob.

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
  -> TypedCNameFinalizer / CSymbolRegistry
  -> CDeclarationPlanner (complete/forward/include/header decisions)
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
writes no files. Schema 2 build facts preserve sorted declaration owners so
`CDeclarationPlanner` can keep headers minimal and retain source reasons. The
planner is likewise pure: complete edges order or include definitions,
pointer-only edges forward-declare, and authoritative external opaque includes
are propagated. Its report/header adapter remains test-only. The implemented
project emitter can package finalized structural header/source artifacts and
typed build facts through Reflaxe ownership, but production declaration/HxcIR
lowering still does not reach it. See [typed C authoring](typed-c-authoring.md)
and [project emission](project-emission.md).

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
