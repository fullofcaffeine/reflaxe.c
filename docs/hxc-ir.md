# HxcIR semantic contract

`HxcIR` is the target-owned semantic layer between normalized Haxe input and
the structural C AST. Its schema is internal to the compiler: schema version 19
is deterministic and validation-backed, but it is not a public file format or
ABI promise.

C is deceptively close to Haxe syntactically, but semantically quite distant.
HxcIR makes those differences explicit before selecting C syntax. This is why
the layer records meaning such as evaluation order, ownership, failure, and
cleanup rather than merely copying the spelling of Haxe expressions.

E2.T02 connects real primitive bodies to this layer; E2.T03 adds
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
The reusable C-facing binary32 carrier adds distinct, validated
binary64-to-binary32 rounding and binary32-to-binary64 widening operations,
advancing the schema to version 10.
The bounded caller-owned class-reference extension marks ordinary function and
method parameters that may use an object for the duration of a known direct
call without taking ownership, advancing the schema to version 11. Validation
allows reads, scalar field mutation, and nested direct calls with the same
declared contract. It rejects storing, returning, throwing, aliasing through a
local, carrying the reference across a block edge, or forwarding it to a call
whose ownership behavior is not proven.
Schema version 12 distinguishes borrowing an embedded class field from taking
an ordinary address. It also lets a function declare an automatic pointer local
as a borrowed alias. That local may be initialized once from a proven borrowed
class value and reloaded after a branch, which is how an inlined Haxe method's
compiler-created `_this` survives lazy `&&` or `||` evaluation. The alias never
owns or extends the child's lifetime: validation rejects reassignment, return,
storage into an owning place, unchecked forwarding, and every non-automatic or
non-class alias declaration. The ordinary address operation remains available
for construction storage and does not silently acquire borrow semantics.
Schema version 13 adds explicit interface-value construction. The operation
names the interface type, the concrete object reference, and the selected
interface table as three separate semantic facts. Validation proves that the
result has the named interface type, the object is a concrete class reference,
the table belongs to that interface, and the table's concrete class is
compatible with the object. This is intentionally earlier than C emission:
the later C layer may spell the value as a two-pointer struct, but it may not
guess or repair an object/table pairing. Production construction, interface
calls in generated C, inherited interfaces, and casts remain owned by E3.T07.
Schema version 14 adds `IRIORelease`, the explicit counterpart to
`IRIORetain`. A release names the owned place and the selected direct,
program-local, or runtime implementation before C syntax is chosen. This lets
the validator and cleanup planner see exactly where a managed enum, Array, or
closed record gives up ownership; the emitter may not hide that decision in a
printer helper. Managed closed records compose their fields through one typed
retain/destroy plan, and recursive enums use allocator-backed uniquely owned
tree links. Cycle-capable graphs remain outside this bounded ownership model.
Schema version 15 adds an explicit `managedRoots` plan to every function. A
root names one immutable HxcIR parameter or instruction result whose finalized
type representation is `IRRManaged("gc")`. The validator rejects unknown,
duplicate, scalar, direct, or block-parameter roots before C is selected.
Block parameters are intentionally not admitted yet: their value changes on an
incoming edge, so safely rooting one requires an edge-owned slot update rather
than the simpler update-after-definition rule.
The first production consumer is the bounded concrete `Array<Class>` graph.
Its class and array allocations name `gc` explicitly, a fresh class result is
rooted before constructor execution, and array `create`, `push`, `get`, and
`set` remain typed operations rather than printer-invented runtime calls.
Exact descriptor generation happens only after the same settled
representations are validated. Direct classes and ordinary arrays therefore do
not acquire roots or a collector merely because this path exists.
Schema version 16 extends each managed root with an exact typed projection
path. A root can now start at a direct record, an active enum payload, or a
present optional and walk to each embedded collector-managed reference. The
validator proves every step before C syntax is selected. This keeps useful
by-value C records and tagged unions without hiding their managed children from
the collector or conservatively treating unrelated memory as a pointer.
Schema version 17 adds typed references to reachable non-capturing functions.
The reference names one known HxcIR function and its result retains the exact
parameter and return types. Calling the value uses the existing callable-value
dispatch form. That dispatch name does not imply that a capture environment
exists: in this slice it emits an ordinary C function pointer. Haxe enum
constructors used as values receive deterministic HxcIR adapter functions, so
constructing the enum remains visible to validation rather than becoming a
printer repair. Capturing closures and managed or recursive constructor
payload adapters remain explicit unsupported boundaries.
Schema version 18 adds an explicitly uninitialized local carrier for
conditional results that are complete unmanaged values, such as a closed Haxe
record, an unmanaged tagged enum, or a header-owned C struct. This is not a
general permission to read uninitialized storage. The validator requires an
automatic direct-value local, rejects managed or recursive representations, and
walks both branches plus nested joins to prove that every reachable read follows
an assignment on that path. C lowering can therefore emit the familiar
handwritten shape `T selected; if (condition) selected = left; else selected =
right;` without fabricating a zero value. Managed conditional joins remain a
separate ownership problem because selecting a branch may require retain,
transfer, and cleanup.
Schema version 19 solves that separate problem for managed tagged enums. It
declares one initially empty carrier before the branch, then records how the
selected arm gives that carrier exactly one owner. A newly constructed enum or
owned call result moves into the carrier; a parameter, local, or other borrowed
value is copied and retained through the enum family's exact active-payload
helper. The join moves the one owner out. Validation rejects an unowned branch,
two acquisitions, a borrowed value mislabeled as fresh, ordinary loads or
stores of the carrier, a second move, or a path that abandons an acquired owner.
This lets C emission use an ordinary local plus structured `if`/`else` while
keeping retain, cleanup, and transfer decisions explicit before C syntax is
chosen.
All other frontend and C lowering remains explicitly gated.

The IR exists because C syntax cannot safely carry several Haxe decisions by
itself. It records evaluation order, immutable values, mutable places,
initialization and lifetime transitions, call dispatch, conversion and
primitive-operation strategy, allocation intent, failure successors, and
cleanup execution before a C expression or statement is selected.

HxcIR deliberately keeps control flow as explicit basic blocks and edges even
when the eventual C should look like handwritten structured code. The CFG is
the semantic truth used to validate evaluation order, failure, cleanup, and
completion without depending on C lexical nesting. Only after that validation
does `CBodyControlFlowPlanner` derive a temporary C-specific region plan. The
plan references HxcIR blocks, proves reducibility and ownership, and is itself
verified before CAST construction. This separation keeps readable C from
becoming a reason to weaken the IR or move semantic repair into the printer.
See [HxcIR-to-C control-flow
structuralization](control-flow-structuralization.md).

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

A universal low- or middle-level executable IR is therefore deliberately not
the current goal. Across C, Rust, Go, JavaScript, Ruby, and Elixir, such a model
would tend toward one of two failure modes: a lowest-common-denominator form
that discards useful target structure, or a growing set of target-specific
variants that merely relocates every backend into one harder-to-evolve schema.
It could also force representation decisions too early: C cleanup and
undefined-behavior proofs, Rust ownership, Go interfaces and multiple results,
BEAM processes and immutable values, and dynamic Ruby/JavaScript operations do
not become clearer when encoded as exceptions to a single target-neutral
machine model. That coupling would be counterproductive even if the shared IR
were technically expressive enough.

The useful universal layer is currently the observable contract rather than a
universal backend program representation: Haxe source and `TypedExpr`, portable
Haxe APIs, capability declarations, semantic specifications, normalized test
fixtures, and cross-target differential evidence. Each backend can preserve
that contract through its own focused semantic plans and target AST. This is
not a permanent prohibition on shared IR; it is a requirement that any shared
form be extracted from proven duplication without becoming the mandatory path
for unrelated target semantics.

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
identities. Nullability records either a tagged scalar/direct-record payload or a pointer
representation rather than being inferred from the eventual C spelling.
`IRTFloat(64)` is ordinary Haxe `Float`; `IRTFloat(32)` is the explicit
`c.Float32` foreign carrier. `IRCNumericRoundBinary32` and
`IRCNumericWidenBinary64` retain the lossy-versus-exact distinction rather than
collapsing both operations into a generic C cast.
`IRTFixedArray` retains the element type, validated compile-time length, and
phantom Haxe witness identity. `IRTSpan` retains element type and mutability without
pretending a borrowed view is an owned aggregate or runtime object.
`IRTInstance` names a validated concrete HxcIR instance rather than a C tag.
For closed records, the declaration owns canonical ordered fields and the
instance records direct representation; C naming and layout remain later
emission decisions.
`IRTNullable(value, IRNTagged)` keeps a `Null<Record>` or `Null<Enum>`
distinction that the selected C value cannot express by itself.
`IRCNullableInject` names the present case,
`IRIOConstant(IRCNull)` names the absent case, and
`IRCNullableUnwrap` may expose the payload only after a dominating
`IRIONullCheck`. CAST later spells this as a small `{has_value, value}` struct;
the semantic IR never depends on those C member names. When the record is
managed, explicit `IRIORetain`/`IRIORelease` instructions name the optional's
program-local lifecycle helper. That helper consults the presence tag before
delegating to the record lifecycle, so inactive storage is never treated as an
owner. A managed enum uses the same optional operations and delegates to the
enum's tag-aware lifecycle helper. Pattern matching first checks and unwraps a
nullable enum, then dispatches on the ordinary enum value; no C pass is allowed
to infer that the optional payload was present.
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
direct storage, while an ordinary source-level class value is a nullable pointer
to that instance. A compiler-proven nonescaping `final` child field instead
retains the direct `IRTInstance`: the parent owns the child's storage and later
operations borrow its address without turning that borrow into independent
ownership. A safe upcast is retained as a representation conversion rather
than inferred from C pointer spelling.
`IRTString` is the immutable valid-UTF-8 Haxe value contract. Its
`IRCString` constant records source text and an independently checked byte
length, so embedded NUL and non-ASCII scalars cannot be confused with C string
termination or character count. A constant's bytes have program-long storage,
so the resulting view can be copied through direct values without ownership
work. `haxe.string.equal` and `haxe.string.not-equal` compare canonical byte
length and content; they do not compare the view's data pointers. Operations
that create new bytes at run time still require the separately owned String
lifetime slice.
`IRTCString` is a separate borrowed, NUL-terminated C view. Its only admitted
constructor is `IRCCStringLiteral`: immutable literal storage with a validated
UTF-8 byte count and no embedded NUL. Typed locals, block parameters,
conditionals, switches, and program-local calls may propagate that carrier, but
none can construct it from a dynamic Haxe `String`. `IRCNativeConstant` retains the exact
header-owned constant identity and a nominal imported value retains its Haxe
type identity through `IRTInstance`; neither is guessed from a host value or
redeclared as generated C. A native constant may use a scalar, imported extern,
or direct aggregate result. The import contract supplies its type; HxcIR never
parses a C macro body.

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
- a function reference names one reachable, infallible function whose exact
  parameter and return types match the callable result; direct calls and calls
  through a callable value preserve those same signatures;
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
  and element type of values; local zero initialization targets exact-size
  integer or binary32 or binary64 storage, has a positive length, cannot
  overflow its byte count, and remains within the shared 65,536-byte per-array
  automatic-storage ceiling; a fixed-array class field is validated under the
  same policy and initialized as part of its enclosing nonescaping object's
  structural zero state; span initialization borrows compatible local or field
  fixed storage through a validated place; index places resolve
  to the collection element type; and every admitted collection access has a
  preceding checked-abort, compile-time static, or compiler loop-guard proof
  for the same collection and immutable index;
- aggregate construction—including `c.StructInit.make` for a header-owned
  struct—names every declared field exactly once, in canonical
  declaration order, with a matching value type; projection names a real field
  and returns its exact type; and a field place resolves only from a compatible
  aggregate base;
- every class base and owned-child field names a known direct class instance,
  class storage fields are unique and typed, direct layouts are finite, and a
  class instance is specialized before emission;
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
- enum-tag equality/inequality uses matching instances of one payload-free
  direct enum, returns `Bool`, and has a static implementation. Tagged payload
  structs are deliberately excluded until their recursive value semantics are
  defined;
- dereferencing a nullable class reference or unwrapping a tagged nullable
  record requires an earlier dominating `IRIONullCheck` for that exact
  immutable value. “Dominating” means the check runs on every route to the use.
  The validator rejects branch-local proofs at a later join and checks the
  recorded profile/build policy instead of relying on C undefined behavior;
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
- calls, conversions, allocation, deallocation, retain, release, and trace operations
  identify their implementation as static/direct, program-local specialized,
  or a named runtime feature;
- primitive conversions identify exact, binary32 rounding, binary64 widening,
  wrapping, checked, saturating, nullable-inject, or nullable-unwrap intent.
  Binary32 rounding is exactly `f64 -> f32` with direct static intent;
  binary64 widening is exactly `f32 -> f64` with direct static intent. Checked
  numeric conversions require an explicit failure edge. A nullable unwrap
  instead requires either its own explicit failure edge or an earlier
  dominating null check; a proven unwrap must not repeat the failure edge. The
  remaining primitive forms reject one;
- exact, wrapping, checked, saturating, and nullable primitive operations may
  use only direct/static or program-local implementation. A named runtime
  feature is an internal invariant failure, not a fallback;
- local entry states and every explicit lifetime transition are legal. An
  initialization ends in `initialized`; retain/release operations name a
  type-compatible owned place; a destroy cleanup ends in `destroyed`;
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
Schema 10 retains the earlier distinction between ordered element-list initialization and
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

For the first E3.T07 semantic slice, an interface layout uses a direct reference
instance as its root and each implementation table still names its concrete
class. `IRIOConstructInterface` then pairs one concrete object reference with
one of those tables. An interface call consumes that validated value and names
the interface and slot explicitly. The C representation and cast rules are not
inferred by the printer and are not yet claimed complete.

For the bounded E6 direct-import slice, reached typed extern declarations lower
to exact native dispatch only after their names, headers, fixed arity, calling
convention, argument types, and return type validate. Imported struct field
places remain ordinary structural HxcIR places; the authoritative header owns
their layout and definition. A literal or proven literal-only selection can
construct `c.CString`; propagation through typed program-local flow preserves
its static, allocation-free translation-unit storage. Callbacks,
variadics, pointer lifetimes, retained strings, opaque ownership, and inferred
ABI facts remain fail-closed with `HXC3000`.

The `c.Float32` extension uses that same import path. Header-owned C `float`
fields, constants, arguments, and returns remain `IRTFloat(32)`. Explicit
source conversions become the two dedicated operations above and emit direct
structural casts only after validation. The generated project layer separately
adds target-side binary32 `_Static_assert` checks; HxcIR does not guess ABI
facts from the Haxe host and does not select `hxrt` for either conversion.

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

A compiler-known String literal remains direct static UTF-8 storage and can
flow through `IRTString` values without allocation. Its `string-literal`
requirement records the private carrier needed by generated C; it does not
select the full String runtime. `haxe.string.equal` and
`haxe.string.not-equal` compare the canonical byte length and bytes, never the
storage pointer. Observable hosted output separately requests `io`. An output
program's exact closure is `runtime-base + status + string-literal + io`; it
contains no allocator, full string operations, object, collector, dynamic,
reflection, or exception machinery. Each semantic requirement is a root reason,
and transitive dependencies remain plan evidence rather than duplicate
warnings.

Portable and metal share this IR. Later analyses decide whether a representation
is legal under the resolved profile and runtime policy. The IR must preserve the
reason and source span needed for those decisions; it must not preselect a broad
runtime slice to make validation pass.

Ordinary resizable `Array<T>` is an explicit managed representation, not a C
pointer guessed by the printer. Its runtime calls name the operation—create,
length, checked copy, or push—and retain the concrete element type. When
checked indexing copies an element that owns Bytes fields, HxcIR gives the copy
a compiler-owned local, exposes only a short-lived borrow to the enclosing
expression, and attaches the matching program-local typed destroy cleanup.
Validation proves that the element type encoded by that cleanup matches the
local being destroyed. Managed enum and record values use the same explicit
owned-local rule. A record-level plan retains fields in order, rolls back a
partial retain on failure, and releases them in reverse order. Recursive enum
children are unique allocator-backed owners, so a copy clones the tree and a
release recursively destroys it. Borrows still cannot escape their proven
lifetime, and cyclic graphs remain rejected.

## Exact managed roots

The root planner runs after type representations settle. It scans each function
in stable program order, roots collector-managed parameters at entry, and roots
collector-managed instruction results immediately after their defining
instruction. A root may be the value itself or a typed path through ordinary
value storage: a record field, an active enum payload, or a present optional
payload. These paths let a value such as `Array<EnumWithClass>` remain a normal,
readable C struct while still telling the collector exactly where its class
references live. Enum tags and optional-presence flags guard union reads, so
inactive storage is never inspected. The first implementation conservatively
keeps those slots live until the function exits. Shortening a slot's lifetime
later is an optimization; it must not change which objects remain reachable at
a collector safepoint.

The C emitter turns the plan into one stack array of `const void *` values and
one `hxc_gc_root_frame`. Parameter slots start with the incoming pointer or the
guarded pointer selected by their path; result slots start null and are assigned
as soon as the result exists. The
frame is pushed before the body can allocate and popped after semantic cleanup
on ordinary returns, throws, and early propagated constructor failures. Abort
paths do not resume and therefore need no recovery. A managed function that
would currently use the tail-loop rewrite fails closed because reusing one C
stack frame across logical iterations needs a separate root-reset proof.

This plan belongs in HxcIR because a C local declaration cannot say whether an
address is a collector-managed base or merely an ordinary pointer. `CAST` owns
the safe declarator and calls; it does not rediscover liveness or repair a
missing root.

## Unsupported typed AST nodes

The body frontend calls
`HxcIRDiagnostic.unsupportedTypedAstNode(profile, nodeKind, context, span)` and
stops at the first unsupported typed node with stable diagnostic `HXC1001`. It
must not insert a `Dynamic`, null, raw C string, or invented constant in place
of an unsupported node. A fully admitted primitive, local fixed-array/span,
closed anonymous-record, bounded direct-value enum, concrete class-reference,
bounded nonescaping constructor, or reachable closed-world class-dispatch
static graph reaches validated HxcIR and structural C. A program containing
only direct values remains runtime-free; admitted Arrays or recursive enum
trees instead select the narrow dependency-closed runtime slices recorded in
the plan. Literal-backed String values can cross
parameters, returns, direct closed aggregates, tagged enums, tagged optionals,
and managed Array elements because their bytes have program-long storage.
Runtime-created or owned Strings and broader output APIs still stop without
output. See
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
conversion/failure forms, aggregate/tag, allocation, retain/release/trace, lifetime,
default initialization, and function failure conventions. Its named runtime
requests are explicit non-primitive coverage.
`diagnostics.json` covers missing termination, use-before-definition, invalid
cleanup order, path redaction, constant/load/address/store/initializer type
mismatches, primitive runtime rejection, missing nullable-unwrap failure,
switch subject/case-family mismatch, void/value return mismatches, return-type
mismatch, tag construction/projection mismatches, invalid enum representation,
non-exhaustive and redundant-default tag switches, illegal direct recursive
layout, unchecked and non-dominating class null proofs, unsafe class representation conversion,
mismatched class-reference equality, missing payload-free representation for
enum-tag equality, a bad UTF-8 byte length, missing hosted-output failure,
invalid default initialization, inconsistent status
conventions, a throw without status propagation, and `HXC1001`.
The runner renders twice and reverses unordered inputs before comparing the
canonical bytes.
It also runs the receiver-check coalescer over a mutable local that changes from
one class reference to another: duplicate checks of each immutable load collapse,
while the post-assignment load keeps its own proof.

The direct HxcIR suite itself emits no C. The body-lowering suite generates a
test translation unit from real typed Haxe. The function-lowering suite extends
that evidence to parameters, conversions, calls, recursive prototypes, and the
production private header/source-set/entry project. The evaluation-order suite
adds source-backed calls, assignments, static fields, short circuit, ternary,
and increments, and retains the representation-neutral general indexed
compound-assignment IR. The span-lowering suite adds source-backed literal and
zero-initialized local arrays, zero-initialized inline arrays owned by a
nonescaping class object, exact-width mutable/const views from either place,
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
The constructor suite additionally embeds a constructed child class directly
inside a nonescaping parent, verifies child-before-parent C definitions and
virtual-table binding before construction, and rejects every admitted path
that would escape or reassign the borrowed child reference.
The enum-lowering suite adds source-backed native fieldless enums and payload
tagged unions, two concrete primitive generic specializations, checked payload
projection, exhaustive tag switches, and allocator-backed uniquely owned
recursive trees. The managed fixture adds a closed record containing managed
and recursive enums, Array element copies, call/return ownership, failure
rollback, and lexical cleanup. Its C and C++17 consumers verify private tags,
size, alignment, offsets, construction, and recursive layout under both
compiler families without claiming cyclic graphs, general generic
monomorphization, or public ABI.
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
The enum suite includes a nominal abstract-over-String payload and proves
construction, copy, projection, and byte-content equality across all project
layouts. The separate string-output suite selects only the four-feature
literal/I/O closure, compares exact UTF-8 and embedded-NUL stdout against Eval,
forces a closed-stdout failure, and runs the generated project at both
optimization levels.
