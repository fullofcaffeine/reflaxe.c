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
  -> plans deterministic class/static initialization and retains its request-local graph
  -> collects reachable typed functions from entry and initializer bodies plus captured primitive globals
  -> CBodyLowering maps admitted signatures, ordered expressions, calls, bodies, UB-safe primitive operations, records, and bounded enum values
  -> CBodyAggregateRegistry deduplicates structural shapes and requests dependency-first type/member symbols
  -> CBodyEnumRegistry specializes concrete enum instances and selects finite direct/indirect storage
  -> selects the deterministic request-local helper closure before symbol finalization
  -> validates HxcIR before CBodyEmitter constructs structural C
  -> emits a private prototype header, source definitions, an initialization wrapper when needed, and int main(void)
  -> resolves an empty direct-value plan or exact source-rooted literal-output runtime requirements
  -> packages selected runtime artifacts as GeneratedFile records
  -> invokes ProjectEmitter with the analyzed runtime, stdlib, primitive, and direct-value facts

ProjectEmitter
  -> public/private headers
  -> source files
  -> boot/export/reflection units
  -> runtime slices
  -> tool-neutral manifest and typed build facts
  -> initialization, runtime, symbol, stdlib, ABI, and lowering reports

Build adapter seeds (E1.T08)
  -> consume the one neutral manifest
  -> optional CMake/Meson projects and direct argument-array consumption
  -> executable-only; full toolchain/export orchestration remains E7/E8
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

The schema-9 semantic core is implemented under `src/reflaxe/c/ir/` and its
normative internal invariants are documented in [HxcIR semantic
contract](hxc-ir.md). Immutable values are block-local and definition-ordered;
mutable storage uses structural places; cross-block data uses typed block
parameters. Cleanup actions are registered in source order while every edge
records their validated reverse, inner-to-outer execution order. Calls and
memory operations distinguish static/direct, program-local, and named runtime
implementations, so the IR never selects an implicit runtime core.
Validated UTF-8 String constants additionally retain their exact byte length;
the only admitted String consumer is the explicit hosted literal-output call
with a native-status abort edge.
The bounded direct-import slice keeps borrowed literal C strings distinct as
`IRTCString`, retains header-owned constants as `IRCNativeConstant`, and uses
`IRCDNative` only after exact import validation. Imported nominal values remain
structural HxcIR types and places even though their definitions belong to the
authoritative header; no import selects runtime intent.
Concrete class declarations retain their private base-prefix layout, own fields,
and explicit header intent; nullable references require validated null proofs,
and derived-to-base conversion stays inspectable until the C emitter selects a
null-preserving embedded-member address. See [concrete class instance
layouts](class-layout.md).
Bounded constructors add default-initialized automatic storage, explicit
function failure conventions, ordinary private C constructor calls, and
validated partial/initialized cleanup edges. They remain limited to proven
nonescaping entry-block locals and select no runtime; see [bounded constructor
lowering](constructor-lowering.md).
Closed-world class dispatch adds a request-local reachable call catalog,
hierarchy-root table layouts, representation-checked slots, typed receiver
adapters, and explicit table binding. Direct calls stay direct, unused
hierarchies retain no table header, and the plan selects no runtime; see
[closed-world virtual dispatch](virtual-dispatch.md).

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
result once plus a fresh empty `CSymbolRegistry`; E2.T09 adds the once-set typed
static-initialization snapshot after planning. Neither input nor graph can
survive the request, and adapter captures are cleared before `CCompiler` runs. See
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

Repository-owned Haxe is also protected by the [typed boundary
policy](typed-boundaries.md). Compiler semantics contain no `Dynamic`, `Any`,
`Reflect`, `untyped`, open-ended record carrier, or unchecked cast. JSON
decoding is isolated in the output-ownership adapter and converted immediately
to a closed validated record. The only other allowlisted occurrences are a
diagnostic negative test and an intentional Haxe-`Dynamic` semantic fixture.
The source-exact machine inventory rejects new or stale exceptions before they
can become compiler precedent.

`HxcIRDumper` canonicalizes only semantically unordered collections and retains
ordered instructions, edge arguments, and cleanup steps with repository-relative
source spans. `HxcIRValidator` rejects missing targets/results/terminators,
use-before-definition, illegal lifetime transitions, malformed cleanup paths,
primitive runtime fallback, or checked/nullable unwraps without a failure
edge. It also checks constant/result and return types plus load, initialization,
store, and address types whenever the place type is structurally resolvable.
Unicode-scalar String constants, exact UTF-8 byte lengths, and the narrow
hosted-output signature/failure policy are validated too. Schema 4 additionally
validates ordered tag declarations, construction, matching, checked payload
projection, exhaustive tag-switch edges, and finite direct instance layout.
The E2.T02 slice uses
local places. Unsupported typed nodes use exact
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
in [UB-safe primitive arithmetic](arithmetic-semantics.md). E2.T08 and its
bounded storage extension add typed literal and compile-time-sized zero-
initialized fixed arrays, a shared 65,536-byte per-array automatic-storage
ceiling, borrowed span views, explicit static/dynamic bounds policies, and
direct guarded exact-width span iteration; their representation and proof
matrix are in
[fixed arrays and span-based iteration](span-lowering.md).

E2.T09 adds a whole-program static-initialization graph before body lowering.
It follows source-positioned superclass/interface/type/static/constructor
dependencies, including static-function bodies, and computes dependency-first
type order from UTF-8-sorted roots and edges. Class `__init__` functions form
the first execution phase; ordinary static fields form the second in type and
source order; the Haxe entry is third. Explicit fields are HxcIR deferred
globals whose private `():Void` initializer contains exactly one
`initialize-global`. The validator proves that link, and the C emitter assigns
zero-initialized storage through file-local initializer functions and one
compiler-owned wrapper called once before Haxe `main`; the initializer
prototypes stay out of the shared header, and an empty plan elides the wrapper
and call. Cross-type cycles fail as `HXC1002`. See [deterministic static
initialization](static-initialization.md).

E3.T01 adds request-local closed-record representation selection.
Typedef-expanded anonymous shapes are accepted only when every field is a direct
primitive or a nested admitted record. Canonical UTF-8 field structure
determines one shared HxcIR instance and finalized C tag across structurally
equal aliases; source expression evaluation remains ordered separately.
Construction and projection stay typed HxcIR operations, addressable local
fields use structural field/address/dereference places, and dependency-first
private `DStruct` declarations carry structural layout assertions. See [closed
anonymous-record lowering](aggregate-lowering.md).

E3.T02 adds request-local concrete enum representation. Fieldless declarations
may use native C enums; payload declarations become discriminant enums,
constructor payload structs, a payload union, and an outer value struct.
Constructor operands remain ordered before named HxcIR construction, payload
projection retains a profile/build tag check, and exhaustive Haxe matches use
typed tag-switch edges. Recursive local payload edges use explicit pointers to
stable automatic backing storage, while recursive parameters and returns fail
closed pending escape/lifetime analysis. Concrete primitive type arguments are
specialized deterministically and now participate in the shared E3.T03 report.
See [Haxe enum lowering](enum-lowering.md).

E3.T03 extends the reachable static-function worklist with closed generic
instances. It infers typed arguments at direct calls, expands aliases, accepts
only already-proven primitive and enum representations, and shares equivalent
instances by a length-prefixed full semantic key. SHA-256 is only a compact
instance suffix; request-local registries retain the full key and reject digest
collisions. Registering an instance before scanning its body closes recursive
generic calls, while hard function/type-count and estimated-C-byte budgets stop
expanding graphs with source-positioned `HXC1001` instead of silently boxing.
The typed `hxc.specializations.json` sidecar records canonical instances,
source-rooted reasons, recursion, and code-size attribution. See
[deterministic generic specialization](generic-specialization.md).

E3.T06 extends that worklist with reachable ordinary instance methods and
constructed concrete dynamic classes. Final/private/metadata/`super` calls
remain direct; ordinary overridable calls select UTF-8-ordered hierarchy slots
and only the tables they can reach. HxcIR validates the whole plan before the C
emitter writes root-only table pointers, typed adapters, or indirect calls.
The conditional `hxc.dispatch.json` sidecar explains every choice and carries
no runtime or public-ABI claim. See [closed-world virtual
dispatch](virtual-dispatch.md).

`CBodyLowering` prepares the complete admitted HxcIR function, closed-record,
bounded enum, and closed generic-function sets, scans them for program-local
primitive helper IDs,
computes their dependency closure, and
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
runtime feature. Closed records become dependency-first private struct
declarations and typed compound literals. Bounded enums become private native
enums or tagged-union declarations with structural tag/layout assertions;
checked payload reads emit a tag comparison before union access, and tag
switches remain structural statements. Fixed arrays remain structural array
declarators, while spans remain typed pointer-plus-`size_t` views. Bounds proof
nodes survive through
validated HxcIR; only exact static constants or typed `size_t` loop guards
remove a check, and a dynamic index emits a signed/element-count fail-stop
branch. The typed Haxe
iterator surface is consumed at this boundary and never becomes an IR or C
runtime object. `CStaticFunctionProjectEmitter`
places all prototypes before definitions, partitions proven non-returning cycle
members, emits the ordered initialization wrapper for a non-empty plan, and produces a runtime-free project in
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
`test/c_ast` fixture compiler builds the complete corpus from typed nodes and
writes it only through Reflaxe ownership. In addition to the difficult
declarator and literal 6×6 expression-family goldens, the corpus contains every
statement family, designated initializers, an explicit GNU-attribute unit, and
a structurally guarded header with separately compiled implementation and
consumer units. Renders under unrelated absolute roots are byte-identical.

The reusable `scripts/test/c_fixture_harness.py` boundary accepts closed project
records and invokes the native compiler only through argument arrays. Its
schema-1 run report records the identity-verified compiler family/version,
strict flags, normalized compile/link/run arguments, input SHA-256 values,
working directories, exits, stdout, and stderr. Temporary checkout/build paths
never enter that record. Required GCC and Clang native lanes execute every
project with no `hxrt` selection. These remain direct AST proofs; the separate
body suite exercises only the admitted real TypedExpr-to-HxcIR/C subset.
Broader lowering remains fail-closed.

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

`RuntimeFeatureRegistry` now validates stable definitions, artifact and symbol
ownership, environment availability, and an acyclic dependency graph.
`RuntimeRequirementAnalyzer` walks every reachable validated HxcIR runtime
intent and reconciles it one-to-one with typed operation/surface/source
candidates; raw typed-input sightings do not become requirements.
`RuntimeFeaturePlanner` resolves source-rooted requests deterministically while
preserving root-versus-transitive provenance; every selected feature and edge
retains at least one root reason ID. `RuntimeFeaturePackager` turns exactly the
selected artifact records into typed `GeneratedFile` values and performs no
artifact read for an empty plan.

The schema-2 feature catalog also owns internal runtime ABI 0.5.0 and exact
source/build provenance. Every artifact has a reviewed SHA-256, packaging
rechecks those bytes, and the sorted source set has one aggregate digest. Every
nonempty closure contains `runtime-base`; generated private headers therefore
emit a structural same-major assertion against `HXC_RUNTIME_ABI_MAJOR`. Empty
plans emit no runtime version metadata or assertion. Runtime layouts are
explicitly forbidden from future application exports; the current production
slice admits no public headers, and E7 remains responsible for enforcing that
policy in its future typed export model.

The admitted primitive compiler path uses this planner for its positive empty
plan. Its schema-2 `hxc.runtime-plan.json` contains a structured reachable-
whole-program proof with zero runtime intents and no `hxrt` include, source,
define, library, or symbol in the build. `hxc_runtime=none` instead reports one
sorted `HXC2000` containing every root operation, typed surface, source span,
dependency chain, and available alternative before output. E2.T07 adds one exact compiler-selected
edge: literal-only hosted `Sys.println` and default `trace` request `io`, whose
closure is `runtime-base + status + string-literal + io`. It packages one C
source and no allocator, full string operations, objects, collector, dynamic,
reflection, or exceptions. New semantic lowerings remain responsible for
supplying typed candidates for any explicit runtime intent and fail internally
if they do not. The checked-in allocator contract has E4.T02 native evidence for checked
sizes, over-alignment, failure atomicity, custom freestanding allocation,
cross-boundary identity, and C/C++ layout agreement. The E4.T03 string contract
adds valid UTF-8 scalar storage and indexing, checked/maximal-subpart decoding,
allocation-aware concatenation and building, and explicit borrowed/owned
CString lifetimes without object, GC, reflection, or dynamic dependencies. The
E4.T04 array contract adds overflow-safe contiguous growth, exact-slot aliasing,
and optional typed element lifecycle callbacks without selecting those adjacent
features. The full allocator, string-operation, and array features remain
`native-seed-only`; only
the runtime foundation, status definitions, literal carrier, and minimal hosted
output are compiler-selectable. See [allocator ownership](allocator-abi.md),
[string runtime](string-runtime.md), [array runtime](array-runtime.md), and
[runtime feature planning](runtime-feature-planning.md).

Portable defaults to `auto + summary`; metal defaults to `minimal + warn`.
These are presets over independent axes, not separate compiler pipelines. See
[ADR 0001](adr/0001-direct-c-and-selective-runtime.md).

## Standard-library ownership boundary

The schema-2 [standard-library ledger](stdlib-ledger.md) is generated from the
exact pinned Haxe typed API surface. Its real `CustomTarget(c)` probe inventories
the common and hosted declarations; a separately labeled, no-generator test
custom target reads only the upstream declarations that intentionally require
thread/atomic capabilities C does not yet advertise. The latter is planning
input, never a production carrier or behavior claim.

Every applicable module matches exactly one explicit ownership rule, and every
public typed API receives profile, environment, candidate runtime-feature,
implementation-Beads, test-Beads, diagnostic, status, source-hash, and test
provenance. There is no catch-all: a Haxe pin that adds or changes surface fails
the snapshot until the ownership decision is reviewed. Exact API overrides must
match once, and only conformant/partial rows with executable evidence are
admitted. The generated CSV is a review view of the JSON authority.

The ledger applies the same hard compiler-first order as runtime planning:
direct idiomatic C or a compatible native operation, then a program-local
specialization, then the smallest proven `hxrt` slice, then a source-positioned
diagnostic. Candidate runtime ownership never selects a feature from an import
or type mention. The global parity ledger is also distinct from a program's
`hxc.stdlib-report.json`: the former owns the pinned product surface, while the
latter reports only reachable operations analyzed in one build. The current
bounded report does that for the literal `Sys.println` and default `trace`
capabilities; all other stdlib use remains fail-closed.

## String and managed-memory model

Portable `String` is a private immutable valid-UTF-8 representation. Every Haxe
index counts Unicode scalar values, embedded NUL is content, normalization is
never implicit, and malformed external UTF-8 has distinct lossy and checked
paths. Binary `Bytes`, NUL-terminated `c.CString`, and exported UTF-8 views keep
their own units, ownership, and lifetime contracts. The literal carrier is an
independent allocation-free feature; full String operations remain a separate
native seed. Neither by itself requires objects, reflection, or the collector.
See [ADR 0004](adr/0004-utf8-scalar-string-contract.md).

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
  -> CDeclarationPlanner or reached CImportRegistry
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
project emitter packages finalized structural header/source artifacts and typed
build facts through Reflaxe ownership. Production lowering now reaches it for
exact fixed-arity scalar/typedef/enum/constant/by-value-struct imports and
literal borrowed C strings. The authoritative header remains the ABI source,
compiled probes verify layout/constants, and unsupported callbacks, variadics,
pointer lifetimes, ownership, and broader bindgen remain fail-closed. See
[typed C authoring](typed-c-authoring.md) and [project
emission](project-emission.md).

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

The compiler emits one typed schema-1 executable plan inside the tool-neutral
`hxc.manifest.json`. Optional `cmake/CMakeLists.txt` and `meson.build` files are
pure derived views over its sources, include directories, typed definitions,
logical libraries, pkg-config packages, frameworks, C standard, and strict
warning policy. Existing build systems can consume the manifest and generated
source tree directly with argument arrays; no adapter or shell command is the
semantic authority. Build-language literals and C definition values use
separate validators/escapers.

The current seed deliberately has one non-installing executable target. Library
artifacts, exports, configurable targets, toolchain/sysroot/cross files, and
CLI/Ninja orchestration remain E7/E8 responsibilities.

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
