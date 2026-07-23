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
  -> validates HxcIR before CBodyControlFlowPlanner derives and verifies a closed structural region plan
  -> CBodyEmitter consumes that plan to construct structural C without reinterpreting Haxe control flow
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

## How Reflaxe participates

haxe.c does use Reflaxe's compiler base classes. The important distinction is
that Reflaxe hosts the target and delivers Haxe's typed program; it does not
decide what correct C means or turn haxe.c's intermediate representation into
C automatically.

This checkout uses Reflaxe `4.0.0-beta` at exact upstream revision
`73a983112e039daad46b37912ab238df6bf0cf53`. Its source is committed under
[`vendor/reflaxe`](../vendor/reflaxe), not downloaded from GitHub or installed
globally during an ordinary build. Lix resolves the checked-in
[`haxe_libraries/reflaxe.hxml`](../haxe_libraries/reflaxe.hxml), which adds that
vendored source directory to Haxe's class path. The toolchain lock and
[third-party provenance inventory](specs/third-party-provenance.json) retain
the exact revision and license evidence, so a clean build does not silently
select a different Reflaxe installation. See the
[machine-readable toolchain lock](specs/toolchain-lock.json) for the bundle
tree, file count, and content digest.

The actual inheritance chain is:

```text
CReflaxeCompiler
  extends GenericCompiler<Bool, Bool, Bool, Bool, Bool>
  extends BaseCompiler
```

[`CReflaxeCompiler`](../src/reflaxe/c/CReflaxeCompiler.hx) is therefore a real
Reflaxe compiler. The five `Bool` parameters satisfy `GenericCompiler`'s typed
per-class, per-enum, per-expression, per-typedef, and per-abstract result
contract. haxe.c does not produce those individual results: its corresponding
`compile*Impl` methods return `null`, so no `Bool` is emitted. Instead, it
captures the complete typed program and compiles it as one request. This is a
deliberate use of the framework's lifecycle, not an attempt to bypass it.

### The lifecycle, one step at a time

1. [`CompilerBootstrap`](../src/reflaxe/c/CompilerBootstrap.hx) verifies that
   the pinned Reflaxe sources are visible before target initialization. The
   scoped HXML normally supplies them; the bootstrap can add the same vendored
   class path when that resolution has not happened yet.
2. [`CompilerInit`](../src/reflaxe/c/CompilerInit.hx) calls
   `ReflectCompiler.Start()` and registers one `CReflaxeCompiler` through
   `ReflectCompiler.AddCompiler(...)`.
3. Reflaxe receives Haxe's `ModuleType` values after typing. A `ModuleType` is
   Haxe's typed description of one class, enum, typedef, or abstract.
4. Reflaxe calls `filterTypes` before its later filters and per-type callbacks.
   haxe.c uses this earliest hook to preserve the complete program, including
   declarations and original field expressions that later preprocessing may
   hide or replace.
5. `onCompileStart` creates fresh request-local state. This is essential for a
   compilation server, where several builds can run in the same Haxe process.
6. Reflaxe still walks its normal class, enum, typedef, and abstract callback
   lifecycle. Their `compile*Impl` methods deliberately return `null`, as does
   haxe.c's expression callback if invoked. The target does not build unrelated
   fragments independently.
7. `onCompileEnd` gives the captured program to `CCompiler`, which performs the
   whole-program semantic and C-emission pipeline.
8. Because the target registered Reflaxe's `Manual` output mode,
   `generateFilesManually` sends the completed `GeneratedFile` set through
   [`ReflaxeOutputWriter`](../src/reflaxe/c/emit/ReflaxeOutputWriter.hx). That
   boundary validates every path and ownership record before delegating saves
   to Reflaxe's `OutputManager`.

The current vendored framework code that defines these hooks lives in
[`GenericCompiler.hx`](../vendor/reflaxe/src/reflaxe/GenericCompiler.hx),
[`BaseCompiler.hx`](../vendor/reflaxe/src/reflaxe/BaseCompiler.hx), and
[`ReflectCompiler.hx`](../vendor/reflaxe/src/reflaxe/ReflectCompiler.hx).
Those links are useful when adapting another Reflaxe target: they show the
framework contract separately from haxe.c's policy.

### Framework responsibility versus target responsibility

| Reflaxe provides | haxe.c owns |
| --- | --- |
| Registration and callbacks at Haxe compiler lifecycle boundaries | Which Haxe programs and language forms are admitted |
| Access to Haxe's typed modules, declarations, fields, and expressions | Whole-program reachability, specialization, representation, ownership, and runtime planning |
| A typed base-class contract for target compilers | HxcIR construction and independent semantic validation |
| Common per-type callback scheduling and selected typed-input helpers | HxcIR control-flow structuralization and structural `CAST` construction |
| `OutputManager`, changed-file avoidance, and stale generated-file ownership | C names, declarations, ABI, headers, sources, build facts, and exact runtime slices |
| The point at which target output is requested | `CASTPrinter` formatting, escaping, precedence, and final C bytes |

In short, `GenericCompiler` means “a Reflaxe compiler may choose typed result
types for its callbacks.” It does **not** mean “a generic backend already knows
how to lower every Haxe operation.” Reflaxe deliberately leaves target
semantics to the target implementation.

Whole-program ownership matters especially for C. A class representation may
depend on whether its values escape; a call may require an exact runtime
feature; one source type may need declarations in several headers and source
files; and reachable subclasses may determine whether dispatch needs a table.
Producing one isolated target string in each class callback would either decide
those facts too early or reconstruct them later from partially generated C.
Capturing first and lowering once gives the compiler one typed, deterministic
place to make and validate those decisions.

## Performance-observation boundary

Compiler profiling is opt-in and must not become hidden process-global state.
`reflaxe_c_phase_timing` creates request-local clocks around typed-input
capture, HxcIR construction and validation, semantic analyses and naming,
structural CAST construction, project planning, printing, artifact planning,
and output ownership. Each clock has a closed `CPhaseTimingId`; adding or
renaming a phase therefore requires an intentional compiler and profiler
change rather than a new free-form string in a hot path. Normal builds create
no phase objects and emit no timing records.

The Caxecraft profiler combines those exact target wall times with Haxe's
opt-in `--times` table. Parent durations are not added to their children: the
report derives non-overlapping “remainder” records so its bottleneck ranking
does not count the same time twice. Durations are diagnostic observations, not
compiler artifacts, and live under ignored `_build/`; normal generated files
remain byte-deterministic. The profiler starts a fresh process for cold samples
and owns an ephemeral loopback compiler server for post-prime warm samples. It
never caches `TypedExpr`, `CompilationContext`, symbols, output ownership, or
other mutable request state.

The broad CAST-body phase also has three closed, opt-in detail clocks: body
setup/value planning, control-flow planning, and CAST emission. Repeated calls
are added together in the profiler report. This is diagnostic structure, not a
new compiler layer: the HxcIR control-flow plan remains the semantic authority,
and CAST emission still only turns that verified plan into C syntax.

See [test feedback-loop and CI performance](test-performance.md) for the
measurement method, the pinned-Haxe macOS timer caveat, and current Caxecraft
results.

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

C is deceptively close to Haxe syntactically, but semantically quite distant.
Both languages have familiar functions, loops, conditionals, and arithmetic,
yet the same-looking expression can carry different guarantees. Haxe defines
language behavior that C syntax alone does not record: evaluation sequence,
expression-valued control flow, object identity, null behavior, managed
lifetimes, and cleanup. C also has undefined behavior, meaning some operations
have no valid result at all when their exact preconditions are violated.
HxcIR makes these differences explicit and independently checkable before the
compiler selects C syntax.

The pinned Haxe compiler's `TypedExpr` is already the shared high-level IR. It
owns typed Haxe meaning and remains the preferred input for discovery,
source-shape recognition, declaration capture, reachability, and any analysis
that does not need a lower semantic contract. Portable source sharing happens
at the Haxe/library/specification level; it does not depend on HxcIR.

HxcIR is the narrower target-owned layer for decisions that must become
explicit and independently valid before strict C syntax is selected. `CAST` is
the later target-syntax layer. This gives the pipeline three durable compiler
forms plus one derived emission plan:

```text
TypedExpr
  -- admitted Haxe meaning --> HxcIR
  -- validated CFG facts --> derived CBodyControlFlowPlan
  -- verified structural choice --> CAST
```

`CBodyControlFlowPlan` is deliberately not another durable semantic IR. It
references HxcIR blocks instead of copying their instructions and exists only
long enough to prove that a reducible graph can be rendered as C sequences,
`if`/`else`, loops, switches, returns, and lexical escapes. HxcIR remains the
semantic authority and keeps explicit edges; CAST remains the syntax authority.
See [HxcIR-to-C control-flow structuralization](control-flow-structuralization.md).

This is an engineering boundary, not a rule that every `TypedExpr` case needs a
matching HxcIR node. A new IR form is justified only when it preserves an
ordering, control-flow, place, lifetime, failure, representation, or runtime
invariant that would otherwise be implicit or reconstructed by the C emitter.
A node that merely renames Haxe syntax belongs at the typed-AST boundary.

`HxcIR` exists to normalize semantics that C syntax cannot safely express directly:

- evaluation sequence;
- expression-valued control flow;
- cleanup and exception edges;
- addressability;
- representation choices;
- allocation/ownership;
- dispatch classification.

`CAST` models C declarations and syntax precisely. It does not decide Haxe semantics.

Sibling targets demonstrate that this extra semantic layer is not a universal
Reflaxe requirement. Go, Rust, Ruby, Elixir, and OCaml use typed target ASTs and
target-specific passes; Elixir additionally uses a focused loop IR and Ruby a
focused Rails route IR at demonstrated semantic/domain gaps. C has stronger
pressure for a separate layer because strict C11 combines weaker operand-order
guarantees and undefined primitive behavior with no built-in ownership,
cleanup, exception, object, string, or collection semantics. A future shared
Reflaxe semantic package remains possible, but only after at least two backends
prove the same behavior-tested operation; HxcIR itself is not that package. The
complete rationale, direct-lowering alternative, sibling comparison, extraction
criteria, and implementation-language analysis are in [the HxcIR semantic
contract](hxc-ir.md#why-a-second-ir-when-haxe-already-has-one).

The schema-17 semantic core is implemented under `src/reflaxe/c/ir/` and its
normative internal invariants are documented in [HxcIR semantic
contract](hxc-ir.md). Immutable values are block-local and definition-ordered;
mutable storage uses structural places; cross-block data uses typed block
parameters. Cleanup actions are registered in source order while every edge
records their validated reverse, inner-to-outer execution order. Calls and
memory operations distinguish static/direct, program-local, and named runtime
implementations, so the IR never selects an implicit runtime core.
Explicit retain and release instructions make managed ownership visible before
C syntax. Closed records compose field lifetimes in one typed helper plan;
recursive enums use uniquely owned allocator-backed tree links. Copies clone a
recursive tree, cleanup destroys it, and graphs that require cycle collection
remain rejected.
An embedded class-field borrow is distinct from an ordinary address. A named
automatic borrow alias may reload that pointer in a later control-flow block,
but cannot be reassigned, returned, stored as an owner, or forwarded without a
checked borrow contract.
Validated UTF-8 String constants additionally retain their exact byte length.
Their immutable views may flow through parameters, returns, closed aggregates,
tagged enums, tagged optionals, and managed Arrays because literal storage lives
for the whole program. Byte-content equality is explicit HxcIR; hosted literal
output remains a separate operation with a native-status abort edge. This does
not admit runtime-created or owned Strings.
The bounded direct-import slice keeps borrowed, statically selected literal C
strings distinct as `IRTCString`. Typed program-local flow may propagate the
carrier but cannot create it from a dynamic Haxe String. The slice retains
header-owned constants as `IRCNativeConstant`, and uses
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
validated partial/initialized cleanup edges. A nonescaping parent may also own
a `final` child object directly in its C struct: HxcIR records the child as a
by-value instance, definition planning emits the child first, and uses of its
address remain parent-bound borrows. Locals and owned children select no
runtime, while aliases, escapes, reassignment, recursive direct layouts, and
fallible child construction remain fail-closed; see [bounded constructor
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
statement/value switches. The validated HxcIR still names every arm and edge;
the later structural region plan groups cases that share a target, emits each
arm body in place, and ends a continuing C arm with `break`, so Haxe never
acquires accidental C fallthrough. A compiler-exposed value-switch carrier is
admitted only after all case/default assignments are structurally proven.
Within the remaining
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
ceiling, bounded inline fixed-array fields in nonescaping class objects,
borrowed span views from local or owned field storage, explicit static/dynamic bounds policies, and
direct guarded exact-width span iteration; their representation and proof
matrix are in
[fixed arrays and span-based iteration](span-lowering.md).

Generated-C layout is a pure assignment made after that semantic and
declaration planning, not another IR or an emitter-side reconstruction. The
default `split` layout puts program-wide includes, ABI assertions, inline
primitive helpers, and dependency-neutral forwards in a common private types
header, assigns complete value-type definitions and private
globals/prototypes to their owning Haxe module headers, includes direct
definition-time complete-type dependencies, and uses common struct forwards
for declaration-only aggregate/class/tagged-enum references as well as
pointer-like edges. Native enums and embedded by-value fields remain hard
edges; forward-declarable prototypes and `extern` objects remain soft, so a
mixed hard/soft module cycle does not become a false include cycle. Anonymous-
record typedef ownership is captured before alias unwrapping,
while structural shape identity remains the representation key. Function/global
definitions follow deterministic package-shaped module sources; compiler
support and the host entry have small dedicated sources. `package` coalesces
those module-owned definitions into one header/source pair per Haxe package,
while retaining hard complete-type dependency order and soft forward-declarable
edges. `unity` assigns the same finalized declarations and function bodies to
one implementation file. All layouts use the same names, HxcIR, runtime
decisions, and structural CAST; printers neither rediscover ownership nor
choose layout. `CProjectEmitter` receives the complete typed artifact set
before any output is written, so manifest/build facts and stale-file ownership
describe the selected tree exactly. See
[project emission](project-emission.md#split-package-and-unity-generated-c).

E2.T09 adds a whole-program static-initialization graph before body lowering.
It follows source-positioned superclass/interface/type/static/constructor
dependencies, including static-function bodies, and computes dependency-first
type order from UTF-8-sorted roots and edges. Class `__init__` functions form
the first execution phase; ordinary static fields form the second in type and
source order; the Haxe entry is third. Explicit fields are HxcIR deferred
globals whose private `():Void` initializer contains exactly one
`initialize-global`. The validator proves that link, and the C emitter assigns
zero-initialized storage through project-private initializer functions and one
file-local compiler-owned wrapper called once before Haxe `main`; initializer
prototypes have identical project-private external linkage in split, package,
and unity layouts so a separate entry unit can call owned definitions, and an
empty plan elides the wrapper and call. Cross-type cycles fail as `HXC1002`. See [deterministic static
initialization](static-initialization.md).

E3.T01 adds request-local closed-record representation selection.
Typedef-expanded anonymous shapes are accepted only when every field is a direct
primitive, a nested admitted record, a fieldless ordinary enum, or an admitted
header-owned by-value C struct. Canonical UTF-8 field structure determines one
shared HxcIR instance and finalized C tag across structurally equal aliases,
while enum fields contribute their full nominal instance identity; source
expression evaluation remains ordered separately. Construction and projection
stay typed HxcIR operations, addressable local fields use structural
field/address/dereference places, and dependency-first private declarations
define every complete native enum or struct before a record stores it by value.
An admitted managed enum or Array field gives the record one composed
retain/destroy plan with failure rollback and reverse-order cleanup; unmanaged
records remain direct and runtime-free. See [closed anonymous-record
lowering](aggregate-lowering.md).

E3.T02 adds request-local concrete enum representation. Fieldless declarations
may use native C enums; payload declarations become discriminant enums,
constructor payload structs, a payload union, and an outer value struct.
Constructor operands remain ordered before named HxcIR construction, payload
projection retains a profile/build tag check, and exhaustive Haxe matches use
typed tag-switch edges. Equality and inequality on the same fieldless enum are
explicit validated enum-tag operations and become direct C tag comparisons;
payload-enum equality remains fail-closed rather than comparing C structs.
Recursive payload edges use explicit pointers to uniquely owned
allocator-backed children. Copying deep-copies the tree; calls borrow; returns
transfer or retain; destruction follows the active tag recursively. Cyclic
graphs still fail closed pending tracing-collector ownership. Concrete primitive type arguments are specialized
deterministically and now participate in the shared E3.T03 report.
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
ordinals, so shadowing remains stable even though routine C spellings preserve
recognizable Haxe source words. Those display words are separate from the full
semantic identity retained in `hxc.symbols.json`; a compact hash appears only
when the whole-program namespace pass finds a real collision or length limit.
`CBodyEmitter` receives only validated HxcIR and finalized `CIdentifier` values,
and builds strict structural statements plus optional typed `#line` nodes.
Before validation, `CBodyNullCheckCoalescing` uses the shared HxcIR control-flow
analysis to remove repeated checks of one immutable class receiver only when an
earlier check runs on every route to the use. The validator recomputes that
dominance proof; neither the CAST builder nor printer repairs missing safety.
Direct-call arguments remain ordered HxcIR instructions, conversions precede
their calls, and each consumed call result plus every observable or aliased load
becomes a typed stable-value temporary instead of a C subexpression with weaker
evaluation order. A separate per-function proof may coalesce one pure,
single-use compiler-owned value or private local/field read only inside the same
block and across no read, effect, failure, cleanup, alias, or lifetime barrier.
Lazy/conditional expressions remain explicit HxcIR blocks and edges rather than
C operators, then become verified structural regions before CAST.
`CPrimitiveHelperEmitter` builds only selected private
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

The direct-import registry also preserves one-level header-owned struct typedef
aliases. The exact alias remains part of the C import/signature plan, while its
semantic HxcIR value and structural field places share the target struct's
representation. This is a zero-conversion boundary: no wrapper, duplicate
layout, cast, allocation, or runtime feature is emitted. Alias chains, cycles,
and aliases across semantic families are rejected rather than guessed.
Header-owned extern names use the ordinary Haxe spelling as their validated
external identity when `@:c.name` is absent; an explicit name remains the exact
override only when the C spelling differs. Default fixed-arity C convention is
resolved the same way. Both facts still enter the symbol/import plans before
HxcIR emission.

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
rejected. Header-owned extern declarations use their identical Haxe spelling as
a validated exact external name when no override is present, while
compiler-owned defaults use readable `hxc_` provenance and stable hash suffixes
only for collisions or length limits. The schema-2 table
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

The schema-3 feature catalog also owns each feature's semantic contract,
selection roots, rejected direct/local alternatives, executable evidence,
internal runtime ABI 0.8.0, and exact source/build provenance. Every artifact has a reviewed SHA-256, packaging
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
features. Bounded Array and Bytes lowering can now select their exact managed
storage closures. E4.T05 adds immutable, versioned object descriptors with
exact trace/finalizer callbacks; a nonempty managed representation plan emits
sorted `static const` descriptors, while direct classes and empty plans remain
header-free and runtime-free. This contract does not allocate or collect an
object. E4.T06 supplies the selective precise, non-moving collector backend,
explicit root chains/pins, pressure thresholds, and reports. Schema-15 HxcIR
now names exact collector-managed parameters and instruction results; generated
C registers those stack slots and the project emitter owns one executable
collector context across all three file layouts. The bounded concrete
`Array<Class>` path now settles its class and array as collector-managed, emits
exact trace/finalizer descriptors, roots allocation before construction, and
preserves identity through array operations. Direct classes and primitive
arrays remain collector-free; broader escaping object shapes still fail
closed. Full runtime String operations remain `native-seed-only`. See [allocator ownership](allocator-abi.md),
[string runtime](string-runtime.md), [array runtime](array-runtime.md),
[object and type descriptors](object-descriptors.md),
[precise non-moving collection](gc-runtime.md), and
[runtime feature planning](runtime-feature-planning.md). The normative
[hxrt architecture and source guide](hxrt.md) explains plan inspection,
runtime minimization, file-level contracts, and source classifications.

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
exact fixed-arity scalar/typedef/enum/constant/by-value-struct imports,
one-level imported struct aliases, and
literal borrowed C strings. The authoritative header remains the ABI source,
compiled probes verify layout/constants, and unsupported callbacks, variadics,
pointer lifetimes, ownership, and broader bindgen remain fail-closed. See
[typed C authoring](typed-c-authoring.md) and [project
emission](project-emission.md).

Imported by-value structs remain nominal HxcIR instances whose layout authority
is the C header. `c.StructInit.make` lowers a checked closed object literal to
`IRIOConstructAggregate`; validation uses the same ordered field contract as an
owned aggregate, while C emission uses the import registry's exact member and
tag names. Header-owned aggregate macros are direct `IRCNativeConstant` values.
Neither path creates a second layout or runtime object.

Transparent non-core Haxe abstracts may preserve an already admitted underlying
representation. This lets a semantic binding add compile-time domain safety and
inline helpers without changing the C ABI. Core target carriers retain their
dedicated rules, and an abstract never makes an unsupported underlying
representation legal.

When a closed generic record contains such an abstract, specialization records
both facts before substituting the record type: the nominal Haxe path prevents
unrelated abstracts from sharing one instance, and the normalized carrier key
proves the later aggregate plan already understands its storage. This is a
bounded record-field rule, not blanket abstract erasure or general generic
class support.

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

The implemented configuration slice follows that boundary. `src/hxc/config`
parses untrusted JSON into a closed typed tree without `Dynamic` or reflection,
normalizes project paths relative to the directory containing `hxc.json`, and
resolves compiler defaults, environment presets, the project file, a named
overlay, CLI inputs, and direct Haxe defines into one immutable effective model.
Every value carries its winning source and priority. The model is reusable by
the Eval and future native CLI hosts; the compiler itself does not search for or
read an ambient project file. E8.T04 will pass the already resolved build model
into planning and manifest emission.

The bootstrap host is Haxe Eval, exposed through `haxe --run` and
`haxelib run reflaxe.c`. Once the C target supports the CLI's required semantic
and `sys` surface, the same core is compiled through `reflaxe.c` into the native
release executable. Eval remains the recovery implementation and differential
oracle. Native `hxc` must be rebuildable from direct Haxe/HXML plus the C
toolchain, never only by invoking an existing native `hxc`.
