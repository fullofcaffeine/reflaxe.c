# HxcIR semantic contract

`HxcIR` is the target-owned semantic layer between normalized Haxe input and
the structural C AST. Its schema is internal to the compiler: schema version 9
is deterministic and validation-backed, but it is not a public file format or
ABI promise. E2.T02 connects real primitive bodies to this layer; E2.T03 adds
typed parameters, ordered direct calls, explicit argument conversions, and a
narrow production C consumer. E2.T04 adds primitive global places plus explicit
branch/jump graphs for lazy and expression-valued control flow. E2.T05 consumes
typed unary/binary operation IDs with direct or request-local implementation
intent. E2.T06 consumes branch, jump, and switch terminators for the admitted
primitive statement/value control-flow slice. E2.T07 adds validated immutable
UTF-8 constants and explicit literal-only hosted output intent. E2.T08 adds
fixed-array/span type identity, ordered literal and zero initialization, and
explicit checked/proven bounds policies.
E3.T01 consumes aggregate type declarations/instances, named construction,
projection, and structural field places for the bounded closed-record slice.
E3.T02 adds ordered tagged cases, checked payload projection, tag matching,
exhaustive tag-switch edges, and finite recursive enum representation for the
bounded direct-value slice. E3.T04 adds concrete class layouts, explicit header
intent, nullable class-reference checks, and inspectable representation-safe
upcasts. E3.T05 adds default object initialization plus explicit infallible or
typed-status function failure conventions for bounded constructor propagation
and cleanup, advancing the schema to version 6. E3.T06 adds a reachable
whole-program virtual-dispatch plan, named hierarchy layouts and slots,
root-header intent, table binding, and receiver-bearing virtual calls,
advancing the schema to version 7.
The bounded E6 direct-import slice adds a distinct borrowed literal-C-string
type, header-owned nominal values and constants, and exact native dispatch,
advancing the schema to version 8.
The bounded zero-initialized fixed-array extension adds a distinct structural
storage operation and shared automatic-storage validation, advancing the schema
to version 9.
All other frontend and C lowering remains explicitly gated.

The IR exists because C syntax cannot safely carry several Haxe decisions by
itself. It records evaluation order, immutable values, mutable places,
initialization and lifetime transitions, call dispatch, conversion and
primitive-operation strategy, allocation intent, failure successors, and
cleanup execution before a C expression or statement is selected.

## Why a second IR when Haxe already has one?

Haxe's typed AST is already an intermediate representation. After parsing and
typing, the pinned Haxe compiler gives the target a `TypedExpr` tree containing
resolved types, declarations, compiler desugaring, and source positions. Haxe
itself also provides algebraic enums and pattern matching, so HxcIR does not
exist because the implementation language lacks suitable compiler-building
features.

The compiler therefore has four distinct layers rather than one undifferentiated
"AST":

| Layer | Owns | Deliberately does not own |
| --- | --- | --- |
| Haxe source and libraries | Portable application algorithms and source-level abstractions | A particular target representation |
| Haxe `TypedExpr` | Shared typed Haxe meaning and the complete frontend result | C-specific sequencing, representation, lifetime, or runtime decisions |
| HxcIR | The admitted program's normalized semantic operations and the proofs required before C is selected | Haxe surface syntax and C punctuation |
| structural `CAST` | Valid C11 declarations, expressions, statements, association, and printing | Recovery or reinterpretation of Haxe semantics |

Cross-target behavior does not require HxcIR. Portable behavior can and should
live in ordinary Haxe, in target-neutral libraries, in the Haxe language
contract, and in differential tests. A C, Go, Rust, JavaScript, or Elixir
backend may use a different representation while preserving the same observable
Haxe result. HxcIR is a target-owned contract for the Haxe-to-C gap; it is not a
universal Reflaxe IR and is not currently consumed by another backend.

### The credible simpler design

A bounded compiler could lower `TypedExpr` directly to `CAST`. It could insert
C temporaries while recursively visiting expressions, keep representation and
runtime decisions in side tables, and validate the resulting C with native
compilers. For a deliberately C-shaped Haxe subset, that architecture can be
smaller and entirely reasonable. Individual features such as a fixed array or
a span parameter do not, by themselves, justify a second whole-program IR.

That alternative becomes less simple when the same source node participates in
several dependent semantic decisions. For example, a call whose arguments have
effects must become an ordered sequence before it becomes a C call expression:

```text
Haxe:       consume(next(), next())

HxcIR:     first  = call next
           second = call next
           result = call consume(first, second)
```

The eventual C call syntax alone cannot carry that sequencing proof. The same
problem recurs for a value crossing blocks, an lvalue versus a copied value, a
checked conversion, a borrow with a forwarded length, a cleanup on several
exits, a throwing call with normal and failure successors, or an operation that
must choose between direct C, a program-local helper, and `hxrt`.

Those facts could all be attached to `TypedExpr` through independent maps and
plans. Once the compiler has an instruction-order plan, a control-flow plan, a
place model, a cleanup graph, a bounds-proof table, a representation plan, and
a runtime-intent table, however, it has created an implicit intermediate
representation. HxcIR makes that boundary typed, deterministic, dumpable, and
validatable in one place before target syntax exists. It also prevents raw
Haxe compiler objects from becoming a serialized or cross-request compiler
contract, which is important while the production carrier uses a pinned Haxe
5 preview API.

### Guardrails against overengineering

HxcIR must remain smaller and more semantic than `TypedExpr`. It is not a second
copy of every Haxe node and it is not the mandatory home for every compiler
fact. The operative rules are:

- use `TypedExpr` directly for frontend discovery, source-shape recognition,
  declaration capture, reachability, and other analyses that need no lower
  semantic form;
- add or extend HxcIR only when a concrete invariant must survive independently
  of Haxe source shape and be validated before C selection;
- require each new IR operation to state the ordering, control-flow, place,
  lifetime, failure, representation, or runtime fact that would otherwise be
  implicit;
- normalize multiple Haxe spellings to one semantic operation rather than
  mirroring each spelling;
- keep C grammar, declarator association, precedence, and token safety in
  `CAST`; and
- do not add speculative nodes solely for a possible future backend.

If a proposed HxcIR node merely renames a `TypedExpr` case and adds no invariant
or independently testable decision, the default is to keep the analysis at the
typed-AST boundary. Conversely, if removing an HxcIR operation would recreate
several loosely synchronized side tables or force the C emitter to infer Haxe
meaning, the explicit semantic form is earning its cost.

## Sibling Reflaxe architectures

The sibling repositories use the same Haxe typed frontend but choose different
target-owned layers. The current local checkouts show these broad pipelines:

- `haxe.go` lowers `TypedExpr` into a typed Go AST, applies Go AST transforms
  and structural import analysis, then prints and formats Go. Its documented
  typed Go IR primarily models target syntax; it explicitly leaves Haxe
  portability and native-boundary admission to earlier policy.
- `haxe.rust` lowers into structural `RustFile` and `RustExpr` values, applies
  representation analyses and Rust AST transforms, and then prints Rust. Rust
  itself structurally expresses many ownership, enum, result, and reference
  decisions that C cannot express, so the target AST and analysis plans can
  carry more of the bridge.
- `haxe.ruby` lowers into a typed `RubyFile`/`RubyExpr` model and then a Ruby
  printer. It also has focused block and keyword-semantics analyzers plus a
  separate Rails route IR. Its fail-closed typed-expression contract is a strong
  boundary, but the small target AST does not yet cover several ordinary Ruby
  forms; the large compiler consequently renders some structural fragments
  back into `RubyRawExpr` or `RubyRawStatement`. The appropriate direction is
  a richer Ruby AST and focused block/call/control plans, not a C-shaped CFG.
- `haxe.elixir.codex` uses `TypedExpr -> ElixirAST -> ordered transforms ->
  printer`. It introduces the narrower `LoopIR` only where several Haxe loop
  forms need shared analysis before choosing an idiomatic Elixir emission
  strategy, and validates function-result invariants between named passes. That
  is useful precedent for adding a semantic sub-IR only at a demonstrated gap.
  Its remaining pressure is concentrated in large builder/transformer stages
  and places where printed or raw Elixir becomes an early semantic boundary;
  focused mutation, managed-reference, failure, framework-intent, and runtime
  plans can strengthen that seam without replacing `ElixirAST`.
- `haxe.ocaml`'s `reflaxe.ocaml` path uses `TypedExpr -> OcamlExpr -> OCaml
  printer`. `OcamlExpr` is a structural target AST, not a separate general
  semantic IR. Its `hxhx` compiler has its own typed program and expression
  models; the currently named `GenIrProgram` is explicitly an alias for the
  macro-expanded typed program, not yet a normalized target-neutral IR.

These architectures do not prove that one target is missing HxcIR. Go, Rust,
Ruby, Elixir, OCaml, and C expose different semantic gaps. A managed target with
bounds checks, exceptions, strings, and garbage collection can often make a
decision directly in its target AST. Strict C11 has weaker expression-order
guarantees, undefined and implementation-defined primitive behavior, and no
built-in ownership, cleanup, exception, object, string, or collection model.
That combination creates stronger pressure for a distinct validated semantic
layer.

The OCaml target also illustrates an important middle ground. Its structural
`OcamlExpr` layer is appropriate for idiomatic target code; replacing it with a
C-like whole-program CFG would not automatically improve the compiler. As its
single large builder accumulates representation, coercion, loop, exception,
null, and runtime-selection decisions, the appropriate next step is narrower
typed plans and clearer pass boundaries where evidence demands them—not an
automatic clone of HxcIR.

## Possible future shared Reflaxe semantic layer

A shared semantic layer across Reflaxe compilers may eventually be useful, but
extracting HxcIR today would couple other targets to C's current needs. Shared
code should be admitted only after at least two backends independently require
the same semantic operation and executable tests show that the observable Haxe
contract is genuinely identical.

A candidate extraction must satisfy all of these conditions:

1. Two or more target implementations currently duplicate the same semantic
   decision; a hypothetical future backend is not sufficient evidence.
2. The shared node describes Haxe behavior, not target syntax, runtime symbols,
   ABI spellings, or one backend's optimization strategy.
3. Each backend remains free to select its own representation and target AST.
4. Differential fixtures define the shared observable behavior, including
   failure and evaluation order where applicable.
5. The model has a closed typed schema, deterministic traversal, normalized
   source positions, validation, and explicit unsupported behavior.
6. Adoption can be incremental; a backend need not translate its entire typed
   program merely to consume one shared semantic family.

Plausible future candidates include ordered effect sequences, normalized
failure/result edges, source-span normalization, or shared runtime-requirement
reason categories. C-specific place rules, `size_t` representation, `CAST`
nodes, `hxrt` feature IDs, and C ABI policy are not shared candidates in their
current form. Until the admission conditions are met, common semantics belong
in Haxe libraries, specifications, conformance fixtures, and small reusable
analyses rather than a universal IR package.

## The implementation language is a separate decision

Reimplementing this compiler in OCaml would make algebraic data models,
immutable transformations, and exhaustive matching pleasant, but it would not
remove the semantic distance between Haxe and C. An OCaml implementation close
to the upstream Haxe compiler might reuse its native typed-tree structures more
directly and could represent some lower decisions as transformed typed trees
plus typed plans. If those transformed trees make evaluation sequence,
control-flow edges, places, cleanup, lifetime, and failure explicit, they are
still serving the role of a lower IR even if no file is named `HxcIR`.

The converse also holds: Haxe's enums, pattern matching, generics, and macro
integration are sufficient to implement the current typed model. Host-language
ergonomics may change the amount of boilerplate, but they do not determine
whether a compiler phase needs an explicit contract. The current architecture
therefore does not imply an OCaml rewrite, and the `haxe.ocaml` repository name
must not be mistaken for a compiler implemented in OCaml: its Reflaxe compiler
is Haxe code that emits OCaml.

## Structural model

A program contains modules. Modules own semantic type declarations, concrete
type instances, global/static storage, and functions. Functions contain typed
parameters, an explicit failure convention, addressable locals, cleanup
regions, and basic blocks. A block owns an ordered instruction array and
exactly one terminator.

An `HxcIRTypeRef` describes Haxe-level semantic types and selected type
instances, not C declarator spelling. `IRTDynamic` is the explicit Haxe
`Dynamic` type; it must never be used as a placeholder for an unsupported typed
AST node. Exact-width integers retain width and signedness, while `size_t`,
`ptrdiff_t`, `intptr_t`, and `uintptr_t` retain distinct unresolved target-ABI
identities. Nullability records either a tagged scalar payload or a pointer
representation rather than being inferred from the eventual C spelling.
`IRTFixedArray` retains the element type, validated compile-time length, and
phantom Haxe witness identity. `IRTSpan` retains element type and mutability without
pretending a borrowed view is an owned aggregate or runtime object.
`IRTInstance` names a validated concrete HxcIR instance rather than a C tag.
For closed records, the declaration owns canonical ordered fields and the
instance records direct representation; C naming and layout remain later
emission decisions.
For Haxe enums, the declaration owns source-ordered cases, explicit integer
discriminants, and named source-ordered payload fields. A concrete instance
records either direct or tagged representation intent. Construction names the
instance, case, and ordered payload values; matching and projection retain the
case identity; and `IRTTagSwitch` carries case-to-edge mappings rather than a C
switch fragment. Recursive payload storage is an explicit pointer type, never
an infinitely recursive by-value instance.
For Haxe classes, the declaration owns one optional direct base instance,
source-ordered own storage fields, and explicit absent, named virtual-layout,
or named-runtime header intent. The concrete class instance describes private
direct storage, while a source-level class value is a nullable pointer to that
instance. A safe upcast is retained as a representation conversion rather than
inferred from C pointer spelling.
`IRTString` is the immutable valid-UTF-8 Haxe value contract. Its
`IRCString` constant records source text and an independently checked byte
length, so embedded NUL and non-ASCII scalars cannot be confused with C string
termination or character count.
`IRTCString` is a separate borrowed, NUL-terminated C view. Its only admitted
constructor is `IRCCStringLiteral`: immutable literal storage with a validated
UTF-8 byte count and no embedded NUL. `IRCNativeConstant` retains the exact
header-owned constant identity and a nominal imported value retains its Haxe
type identity through `IRTInstance`; neither is guessed from a host value or
redeclared as generated C.

Calls have one of seven exhaustive dispatch forms: direct, virtual, interface,
closure, native, runtime, or intrinsic. Direct and native calls therefore
cannot be confused with runtime fallback, and later devirtualization does not
need to recover intent from target syntax. A virtual call names its validated
slot and receiver separately from the explicit source arguments.

## Required invariants

`HxcIRValidator` rejects a program unless all of these conditions hold:

- program schema, module/type/instance/global/function IDs, function-local
  value/place/block/instruction/cleanup IDs, and every reference are valid and
  unique in their declared namespace;
- source spans use normalized repository-relative `/` paths, one-based
  line/column coordinates, and an end point that does not precede the start;
  invalid absolute spellings are redacted from diagnostics;
- immutable values and mutable addressable places remain different types;
  loads, stores, addresses, dereferences, fields, and indices cannot silently
  change value category;
- instruction array order is semantic order. An instruction can use only a
  function parameter, a parameter of its own block, or an earlier result in
  that block;
- values never leak directly between blocks. A control or failure edge passes
  them through typed target-block parameters, with arity and type validation;
- direct and closure calls match known parameter and return signatures;
- every value-producing instruction has exactly one typed result, every
  effect-only instruction has none, every block terminates, and every branch,
  switch, failure, and `finally` target exists;
- every switch constant matches the subject's literal/type family, case values
  are unique, and every case/default edge validates arguments and cleanup;
- constant families match their result types; for structurally resolvable
  places, loads match the place type, initialization/store values match their
  destination, and address results are pointers to the addressed type;
- every Haxe string constant is a valid Unicode-scalar sequence and its
  recorded UTF-8 byte length is exact, including embedded NUL bytes; every C
  string literal also has an exact byte length and rejects embedded NUL;
- element-list fixed-array initialization supplies exactly the declared number
  and element type of values; zero initialization targets exact-size integer or
  binary64 storage, has a positive length, cannot overflow its byte count, and
  remains within the shared 65,536-byte per-array automatic-storage ceiling;
  span initialization borrows compatible fixed storage; index places resolve
  to the collection element type; and every admitted collection access has a
  preceding checked-abort, compile-time static, or compiler loop-guard proof
  for the same collection and immutable index;
- aggregate construction names every declared field exactly once, in canonical
  declaration order, with a matching value type; projection names a real field
  and returns its exact type; and a field place resolves only from a compatible
  aggregate base;
- every class base names a known direct class instance, class storage fields are
  unique and typed, direct layouts are finite, and a class instance is
  specialized before emission;
- virtual layouts, slots, and tables have unique stable IDs and strict UTF-8
  order; one root selects each layout; every slot belongs to exactly one layout;
  every table class descends from that root; and every table entry follows the
  layout's exact slot order;
- each applicable virtual entry names a reachable infallible implementation
  whose explicit arguments and result preserve the slot representation. Its
  typed receiver descends from the slot owner and owns the table class;
- a virtual call names a known slot, uses the separately named receiver as its
  first semantic argument, has a preceding null proof when required, and
  preserves the slot argument/result types. A table bind targets direct storage
  of that table's exact concrete class;
- class references use matching nullable pointers to concrete class instances;
  equality/inequality is a static `Bool` identity operation, and representation
  conversion admits only a proven derived-to-ancestor path with matching
  nullability;
- dereferencing a nullable class reference requires an earlier same-block
  `IRIONullCheck` for that immutable value. Its checked-abort policy records a
  valid profile and build mode rather than relying on C undefined behavior;
- tagged construction names a declared case and supplies exactly its ordered
  payload types; matching uses a case from the value's concrete instance;
  payload projection returns the exact named field type and carries the
  resolved profile/build checked-abort policy;
- tag-switch cases are unique and compatible with the subject instance. An
  exhaustive switch names every constructor exactly once and has no default
  edge; every case edge still validates block arguments and cleanup;
- direct by-value instance dependencies are acyclic. Recursive enum edges must
  be represented indirectly so every emitted C object has finite size;
- return terminators carry no value for `Void`, carry exactly one value for a
  non-`Void` function, and match the declared return type;
- calls, conversions, allocation, deallocation, retain, and trace operations
  identify their implementation as static/direct, program-local specialized,
  or a named runtime feature;
- primitive conversions identify exact, wrapping, checked, saturating,
  nullable-inject, or nullable-unwrap intent. Checked numeric and nullable
  unwrap forms require an explicit failure edge; the remaining primitive forms
  reject one;
- exact, wrapping, checked, saturating, and nullable primitive operations may
  use only direct/static or program-local implementation. A named runtime
  feature is an internal invariant failure, not a fallback;
- local entry states and every explicit lifetime transition are legal. An
  initialization ends in `initialized`; a destroy cleanup ends in `destroyed`;
- default initialization targets a structurally resolvable class-instance
  place whose storage type matches the instruction exactly;
- a deferred global names an existing zero-argument `Void` initializer, and
  that function contains exactly one `uninitialized -> initialized`
  `initialize-global` instruction for the named global;
- cleanup actions are listed in registration order. An edge lists actual
  execution order: reverse registration order within a region, then the direct
  parent region moving outward. Steps are unique and refer to stable
  local/global places;
- call/allocation failure edges and throw terminators identify exception,
  result-error, allocation-failure, or native-status behavior explicitly. The
  normal continuation of a successful instruction is the next instruction (or
  its block terminator); the non-normal successor is never implicit;
- an infallible function cannot propagate a throw, while a status-bearing
  function names the exact failure kind represented by its native status. A
  direct constructor call may carry a failure successor only when the called
  constructor has the matching status convention;
- the admitted `io` runtime call is exactly `sys-println-literal` or
  `trace-literal`, accepts one `IRTString`, returns `Void`, and retains the
  cleanup-free native-status abort edge used by the hosted fail-stop policy.
- a direct imported call is represented as native dispatch with exact argument
  order and value types. Header-owned scalar, typedef, enum/constant, and
  by-value struct identities remain structural; they select no runtime intent.

These checks make a malformed internal IR an `HXC9000` compiler invariant
failure. They do not replace later semantic passes such as representation
selection, ownership proofs, runtime-feature finalization, or exception-
strategy selection. For the admitted primitive slice, E2.T05 now consumes the
implementation intent with UB-safe structural C and request-local helpers;
E2.T06 consumes branch/jump/switch terminators for primitive statement and
value control flow. E2.T08 consumes array/span initialization and bounds intent.
Schema 9 distinguishes ordered element-list initialization from
`IRIOZeroInitializeFixedArray`; the latter carries an explicit storage-state
transition and must pass `HxcIRFixedArrayPolicy` before either validation or C
emission. A static proof retains its literal length/index, a loop proof names
the guard, compiler index local, and exact length, and a dynamic policy records
the resolved profile and build mode. The validator checks that the loop guard
is an exact `size_t index < length` comparison whose true edge enters the
checked body and that the body immediately reloads the same index local. Static
proofs must name the exact in-block `Int` constant, and both static and loop
proofs must tie their length to fixed-array storage behind the local span.
Whole-view assignment, taking a view's address, escaping a borrow, and whole-
frame automatic-storage accounting remain outside this admitted proof model.
Future operation and control-flow families still require their own proof.

E2.T09 consumes the deferred-global and initialization-transition model for
real Haxe static fields. Each explicit primitive field is emitted as
zero-initialized C storage plus one private HxcIR initializer; the separately
inspectable whole-program plan determines when those functions run. HxcIR
retains per-field semantic work and the exact-once link, while the plan retains
cross-function/type order and dependency reasons. See [deterministic static
initialization](static-initialization.md).

For E3.T01, local aggregate-field reads remain explicit field-place,
address, and dereference instructions, while immutable parameter or temporary
reads use value projection. By-value initialization and loads retain the
instance type. This prevents C syntax from erasing the semantic distinction
between an addressable place and a copied record value.

For E3.T04, a class declaration retains its base-prefix and source-order own
fields, while values remain nullable instance pointers. Inherited field places
resolve through the declaration chain only after an explicit null proof. A
derived-to-base conversion remains `IRCRepresentation` with static intent, so
the emitter can select a null-preserving embedded-member address without an
unchecked cast. See [concrete class instance layouts](class-layout.md).

For E3.T05, a stack-constructed object begins as default-initialized direct
storage, transitions through `initializing`, and becomes `initialized` only
after its constructor succeeds. Constructor functions remain semantically
`Void`, while their explicit failure convention lets structural C use a
private `bool` status without disguising that status as a Haxe return value.
Failure edges carry the already-ordered partial and initialized cleanup steps;
the emitter never reconstructs them from C lexical scope. See [bounded
constructor lowering](constructor-lowering.md).

For E3.T06, the whole-program dispatch plan contains only reachable hierarchy
slots and tables for constructed concrete dynamic classes. The hierarchy root
selects the one virtual-layout header, construction binds a named table before
user constructor code, and each indirect call retains its receiver and slot.
Override implementations must preserve the slot's target representation;
typed receiver adapters are explicit program-local C emission facts rather
than runtime intent. See [closed-world virtual dispatch](virtual-dispatch.md).

For the bounded E6 direct-import slice, reached typed extern declarations lower
to exact native dispatch only after their names, headers, fixed arity, calling
convention, argument types, and return type validate. Imported struct field
places remain ordinary structural HxcIR places; the authoritative header owns
their layout and definition. Literal `String -> c.CString` borrowing is static,
allocation-free storage local to the generated translation unit. Callbacks,
variadics, pointer lifetimes, retained strings, opaque ownership, and inferred
ABI facts remain fail-closed with `HXC3000`.

## Runtime and profile policy

The IR has no unconditional runtime concept. `IRIStatic` is a direct semantic
operation, `IRIProgramLocal` names a compilation-local specialized helper, and
`IRIRuntime` names the exact requested feature. Runtime call dispatch likewise
records a feature and operation. Merely constructing a type, calling a direct
function, constructing or copying a closed record, initializing a fixed
array/span, constructing/matching a bounded enum, laying out or stack-
constructing a concrete class, checking a class reference/tag/bounds,
performing a safe upcast, binding or calling a program-local virtual table, or
using cleanup selects nothing from `hxrt`.

E2.T07 is the first bounded compiler-selected exception: a compiler-known
literal remains direct static UTF-8 storage, while its observable hosted output
requests the `io` feature. The exact closure is `runtime-base + status +
string-literal + io`; it contains no allocator, full string operations, object,
collector, dynamic, reflection, or exception machinery. Each source call is a
root reason, and transitive dependencies remain plan evidence rather than
duplicate warnings.

Portable and metal share this IR. Later analyses decide whether a representation
is legal under the resolved profile and runtime policy. The IR must preserve the
reason and source span needed for those decisions; it must not preselect a broad
runtime slice to make validation pass.

## Unsupported typed AST nodes

The body frontend calls
`HxcIRDiagnostic.unsupportedTypedAstNode(profile, nodeKind, context, span)` and
stops at the first unsupported typed node with stable diagnostic `HXC1001`. It
must not insert a `Dynamic`, null, raw C string, or invented constant in place
of an unsupported node. A fully admitted primitive, local fixed-array/span,
closed anonymous-record, bounded direct-value enum, concrete class-reference,
bounded nonescaping constructor, or reachable closed-world class-dispatch
static graph reaches
validated HxcIR, structural C, and an owned runtime-free executable project.
Recursive enum parameters and returns remain rejected until escape/lifetime
analysis can choose owned storage. The separately admitted literal-output edge
reaches the exact selective runtime plan above; nonliteral strings and broader
output APIs still stop without output. See
[primitive function-body lowering](body-lowering.md) and [static function
lowering](function-lowering.md), plus [closed anonymous-record
lowering](aggregate-lowering.md), [Haxe enum lowering](enum-lowering.md), and
[concrete class instance layouts](class-layout.md), and [bounded constructor
lowering](constructor-lowering.md), and [closed-world virtual
dispatch](virtual-dispatch.md).

## Canonical dump

`HxcIRDumper` emits UTF-8 text with LF line endings. It sorts semantically
unordered modules, declarations, instances, globals, functions, locals,
cleanup regions, and blocks by UTF-8 ID (with the entry block first). It
preserves every semantically ordered list: parameters, fields, tag payloads,
instructions, switch cases, cleanup registration, edge arguments, and cleanup
execution steps. Every source-bearing module, declaration, storage, function,
block, instruction, terminator, and cleanup node includes its span; nested edge
data inherits the owning instruction or terminator span. Dumps contain no
timestamps, random IDs, locale-dependent formatting, or normalized-away checkout
paths.
Control characters in dump strings use deterministic JSON-style escapes, so an
embedded NUL appears as `\u0000` rather than becoming a raw byte in an
inspection envelope.

The executable contract is:

```sh
npm run test:hxc-ir
```

`test/hxc_ir/expected/semantic.hxcir` demonstrates the ordered lowering intent
for `arr[nextIndex()] += produce()` and its nested cleanup path without a
runtime request. The checked-in Haxe fixture runs independently under Eval and
asserts the oracle trace `nextIndex,produce:8`. `coverage.hxcir` exercises all
dispatch forms plus ABI integers, tagged/pointer nullability, an exact UTF-8
string constant and hosted-output failure edge, explicit primitive
conversion/failure forms, aggregate/tag, allocation, retain/trace, lifetime,
default initialization, and function failure conventions. Its named runtime
requests are explicit non-primitive coverage.
`diagnostics.json` covers missing termination, use-before-definition, invalid
cleanup order, path redaction, constant/load/address/store/initializer type
mismatches, primitive runtime rejection, missing nullable-unwrap failure,
switch subject/case-family mismatch, void/value return mismatches, return-type
mismatch, tag construction/projection mismatches, invalid enum representation,
non-exhaustive and redundant-default tag switches, illegal direct recursive
layout, unchecked class dereference, unsafe class representation conversion,
mismatched class-reference equality, a bad UTF-8 byte length, missing
hosted-output failure, invalid default initialization, inconsistent status
conventions, a throw without status propagation, and `HXC1001`.
The runner renders twice and reverses unordered inputs before comparing the
canonical bytes.

The direct HxcIR suite itself emits no C. The body-lowering suite generates a
test translation unit from real typed Haxe. The function-lowering suite extends
that evidence to parameters, conversions, calls, recursive prototypes, and the
production private header/source-set/entry project. The evaluation-order suite
adds source-backed calls, assignments, static fields, short circuit, ternary,
and increments, and retains the representation-neutral general indexed
compound-assignment IR. The span-lowering suite adds source-backed literal and
zero-initialized fixed arrays, exact-width mutable/const views,
checked/static/loop bounds policies, ordinary-Haxe three-dimensional indexing,
direct guarded iteration, storage-budget negatives, and strict generated-C
execution. The arithmetic
suite adds source-backed operation/helper decisions, `Std.int`, boundary
execution, and eligible UBSan. All select no runtime files or public C ABI and
compile/run as strict C11 with available GCC and Clang at `-O0` and `-O2`.
The aggregate-lowering suite adds source-backed named construction, direct
record instances, explicit copies and field addresses, dependency-first private
structs, and exact C/C++17 layout agreement under both required compiler
families.
The enum-lowering suite adds source-backed native fieldless enums and payload
tagged unions, two concrete primitive generic specializations, checked payload
projection, exhaustive tag switches, and a recursive local value with explicit
indirect storage. Its C and C++17 consumers verify private tags, size,
alignment, offsets, construction, and recursive layout under both compiler
families without claiming general generic monomorphization or public ABI.
The class-layout suite adds source-backed private concrete structs, embedded
base prefixes, explicit null checks, inherited field places, identity
operations, and null-preserving upcasts. Its independent C and C++17 consumers
verify private size, alignment, offsets, base paths, field access, and reference
behavior without claiming construction or ownership.
The constructor-lowering suite adds source-backed `new`, ordinary private C
constructor functions, exact pinned-Haxe field/`super`/body ordering,
default-initialized automatic storage, trivial-chain elision, typed status
propagation, and ordered partial-initialization cleanup. It remains bounded to
unconditional nonescaping locals and does not claim heap allocation, general
exceptions, dispatch, object runtime, or public ABI.
The separate string-output suite selects only the four-feature literal/I/O
closure, compares exact UTF-8 and embedded-NUL stdout against Eval, forces a
closed-stdout failure, and runs the generated project at both optimization
levels.
