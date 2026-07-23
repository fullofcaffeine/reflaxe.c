# Haxe code architecture: clear source and careful C

Status: design and teaching guide. The support descriptions below are bounded
by executable evidence in the current checkout; this is not a claim of complete
Haxe language support.

## The source is part of the product

Reading haxe.c and Caxecraft should be a pleasant way to learn three things at
once: modern Haxe, careful software architecture, and practical game-engine
design. Correct output is necessary, but it is not enough. Names, module
boundaries, HxDoc, examples, and generated C should help a curious newcomer
build an accurate mental model.

Choose the smallest Haxe construct that tells the truth about a value:

- Use a value when identity does not matter.
- Use an object when identity, ownership, or lifetime does matter.
- Use a closed type when the valid choices are known.
- Use a module function when an operation owns no lasting state.
- Use an extern only at a foreign application binary interface (ABI), such as
  Raylib's C API. An ABI is the exact binary contract two compiled components
  use to call each other and exchange data.
- Use a macro only when ordinary typed Haxe would otherwise repeat mechanical
  compile-time glue.

This produces Haxe that is familiar to a TypeScript developer without copying
JavaScript's runtime model. Haxe classes do not become JavaScript prototype
objects on the C target, a `typedef` does not create a runtime type descriptor,
and a module does not become a namespace singleton. Haxe.c selects direct C
storage and functions from the typed meaning of the program.

Caxecraft applies this as a **functional core with a stateful shell**.
Deterministic rules are small functions over explicit values, while a few
objects own the live session, platform resources, and update order. Authored
characters, dialogue, maps, and quests remain data; choosing a Haxe type does
not move content back into engine code.

## Quick choice guide

| Question | Usually choose | Why |
| --- | --- | --- |
| Is this a small snapshot, request, result, or configuration value? | `typedef` record | It has shape but no separate identity or lifetime. |
| Is this one meaningful scalar such as an entity ID or distance? | `abstract` over a scalar | It prevents accidental mixing while keeping compact storage. |
| Is this a closed set of named scalar choices? | `enum abstract` | Source gets named values while C can keep the scalar carrier. |
| Is this a closed set of alternatives, possibly with different payloads? | algebraic `enum` | Pattern matching is exhaustive and payloads stay typed. |
| Does it own cohesive mutable state, a resource, or a stable lifetime? | usually a `final class` | One object owns the state and methods that protect it. |
| Is this a stateless calculation or constructor for a value? | module-level function | It needs no fake class or singleton namespace. |
| Does C already own this declaration or function? | raw `extern` plus a semantic Haxe facade | The extern preserves the ABI; the facade improves safety and use. |
| Are several real implementations interchangeable behind one behavior contract? | narrow interface or abstract base | Only after the target supports it and composition is insufficient. |
| Is typed boilerplate repeated across codecs, registries, editor facts, and tests? | possibly a macro | Generate facts from one declaration; never duplicate the behavior. |

This is a decision aid, not a ranking. A class is not more advanced than a
record, and a record is not automatically faster. The correct choice makes
ownership and valid operations obvious, which gives the compiler better facts
for readable and efficient C.

## Programming styles are complementary tools

Haxe supports several programming styles in one language. Good architecture
does not choose one style as a religion. It puts each kind of work in the style
that makes its state, dependencies, and effects easiest to see.

### Functional style: calculate a result from explicit inputs

A functional operation takes everything it needs as arguments and returns a
new value. It avoids changing hidden state. For example:

```haxe
function applyAttack(vitals:VitalsState, attacked:Bool):VitalsState {
	return attacked ? damage(vitals, 1) : vitals;
}
```

This style is strong for collision decisions, damage, inventory transfers,
validation, parsing, state transitions, and artificial-intelligence decisions.
The same input produces the same result, so a focused test needs little setup.
It also gives haxe.c an explicit value flow to optimize and validate.

“Functional” does not require copying the whole world on every tick. Keep the
calculation focused: pass the component or view it needs, return the small
result, and let the state owner commit it. A function that accepts the entire
`GameSession` merely to read one health value is still tightly coupled even if
it returns an immutable record.

### Imperative style: perform an ordered sequence of effects

Imperative code says “do this, then this.” Games need it. Polling input,
advancing a fixed simulation, committing results, drawing, playing sounds, and
releasing native resources all have important order.

The goal is not to remove mutation. It is to make mutation:

- owned by a clear object or narrow scope;
- explicit in names and parameters;
- ordered in one reviewable schedule;
- unavailable to presentation-only code; and
- deterministic when it belongs to simulation.

`GameSession.tick` is an imperative boundary: it advances water, character
movement, aquatics, and health in a documented order. The calculations it
calls can still be functional. This combination is often called a **functional
core with an imperative shell**.

### Object-oriented style: put behavior beside the state it protects

Object-oriented programming (OOP) is useful when one instance owns cohesive
state or a resource. `GameSession` and `WaterSimulation` are objects because
their state lives across calls and their methods maintain invariants.

OOP does not mean turning every noun into a class. An input, event, coordinate,
or result can remain a value. Nor does it require deep inheritance. Prefer
composition--“a session has a water simulation”--when parts have separate
responsibilities. Use inheritance only when callers truly need one subtype to
stand in for another through the same behavior contract.

### Data-driven style: let content select reusable behavior

Data-driven design keeps changing content outside engine control flow. A map or
content pack chooses an NPC profile, placement, localized message, item, or
CaxeFlow action. Typed Haxe implements the reusable mechanic once.

This separation lets a child or modder create a new adventure without editing
engine source. The current built-in Caxecraft content still runs a validator
and generates a typed adapter during the build; arbitrary native runtime pack
loading is planned, not shipped. When that loader lands, ordinary content edits
will no longer require invoking Haxe. This separation also prevents a giant
`Main.hx` switch over character names. Data is validated into closed engine
concepts before the simulation uses it; “data-driven” does not mean accepting
arbitrary strings or executable code everywhere.

### Data-oriented style: shape hot storage around measured access

Data-oriented design asks how a hot operation reads memory. A voxel world is a
good candidate for compact contiguous cells because simulation repeatedly
walks neighboring blocks. A menu controller usually is not.

Use this style after identifying a real access pattern or budget. Do not split
every object into parallel arrays or import a full entity-component system
(ECS) merely because games sometimes do so. Caxecraft currently borrows the
useful ECS ideas--stable entity IDs, composed capabilities, focused systems,
and an explicit schedule--without paying for a query engine or dynamic
component registry it has not needed.

### Declarative style: describe intent, then validate and execute it

CaxeMap and CaxeFlow are declarative. Authored data says what exists, which
condition matters, and which admitted action should happen. The engine owns how
to validate and execute that request.

Declarative data is appropriate for content, cutscenes, simple creator logic,
and test journeys. It is not a reason to create an untyped string command bus.
Every admitted event, condition, and action reaches one typed Haxe mechanic.

## Architecture principles behind the styles

### One owner, many readers

Every mutable value needs one clear owner. `GameSession` now owns the first
committed game-state slice, and `EntityStore` owns each admitted character once;
`PlayerAgent` refers to a character by stable ID rather than copying it. The
read-only renderer view is the target boundary. `Main` still has a documented
narrow friend-access migration seam until that view is implemented. This rule
prevents two “authoritative” values from drifting apart.

### Dependencies point toward stable rules

The intended dependency flow makes platform input, artificial intelligence,
cutscenes, and pilots produce the same semantic intent. The first player and
non-player fixtures already share the intent and character mechanics; general
runtime actor controllers remain planned. The session consumes the intent and
presentation observes the committed result. Raylib stays at the edge rather
than inside collision or health rules:

```text
keyboard / controller / AI / pilot
                 |
                 v
          CharacterIntent
                 |
                 v
       GameSession fixed tick
                 |
        committed state/events
                 |
                 v
        renderer / audio / HUD
```

The arrows do not point back from a renderer into simulation. This dependency
direction makes headless tests and future target adapters possible without a
second gameplay implementation.

### Make invalid states difficult to express

Use `EntityId` instead of an arbitrary `Int`, an enum instead of a string tag,
and a validated record instead of a bag of optional fields. Parse untrusted
JSON or authored text once at a narrow boundary, report useful errors, and
convert it into typed engine values immediately.

### Model expected failure as data

“Inventory full,” “interaction out of range,” and “unknown content ID” are
ordinary expected outcomes. Closed enums and result records let callers handle
them explicitly. Assertions or fail-stop checks remain appropriate for broken
internal invariants that valid source and validated content cannot produce.

### Separate simulation time from presentation time

The outer frame loop polls devices and renders. The inner loop advances the
simulation in fixed-size steps. Fixed steps make mechanics and tests stable
across display rates; rendering may interpolate between committed states for
smooth motion. Bound the catch-up work so a slow frame cannot create an
unlimited “spiral of death.”

### Optimize from evidence

Begin with the clearest type that expresses ownership and behavior. Measure
compile time, frame/update cost, allocations, generated size, and memory access
before changing the representation. A more complicated ECS, object pool,
macro, cache, or runtime helper must earn its maintenance cost with a concrete
problem and regression gate.

### Test at the smallest truthful boundary

Test pure rules with small Haxe inputs and outputs. Test a session tick when
ordering or ownership matters. Use a native pilot for a few real journeys that
connect loading, input, simulation, rendering, and presentation. When a game
uncovers a compiler bug, keep both the small compiler fixture that owns the
language rule and the flagship path that proves the whole stack works.

## Module-level functions

Haxe permits a function directly in a module:

```haxe
package caxecraft.domain;

function applyDamage(vitals:VitalsState, amount:Int):VitalsState {
	// Return the next value.
}
```

Use this for a stateless rule whose inputs and result identify all of its
dependencies. This is similar to an exported function in an ECMAScript or
TypeScript module. It is clearer than a static-only wrapper:

```haxe
class VitalsTools {
	public static function applyDamage(...):VitalsState;
}
```

The Haxe compiler internally represents module fields with a hidden synthetic
class, but that is a compiler detail. Haxe.c preserves the complete hidden
identity for collision safety and emits a readable ordinary C function based
on the source module, conceptually:

```c
struct hxc_VitalsState hxc_Vitals_applyDamage(
    struct hxc_VitalsState vitals,
    int32_t amount);
```

No module object is allocated. Use a class instead when several operations
must protect the same persistent mutable state or resource.

## `typedef`: a name for a value shape

A Haxe `typedef` names another type. A closed anonymous record is useful for a
small immutable snapshot, input, result, or configuration:

```haxe
typedef CharacterIntent = {
	final moveX:Float;
	final moveZ:Float;
	final jump:Bool;
}
```

This resembles a TypeScript object type or interface used only as data. It does
not give each value object identity, inheritance, a constructor, or a runtime
type test. Two typedefs with the same structural fields can describe the same
Haxe shape, so use a nominal abstract or class when names must not be
interchangeable.

For the admitted closed shapes, haxe.c emits a private C struct and passes or
returns it as a value. Nested admitted records remain structural values. The
actual compiler-generated names are collision-safe; simplified C looks like:

```c
struct hxc_CharacterIntent {
  double moveX;
  double moveZ;
  bool jump;
};
```

Small values make state transitions explicit and easy to compare, serialize,
and test. Do not put an entire mutable game world into one record merely to
avoid classes: large value copies and unclear ownership would be worse.

`PlayerAgent` is currently a typedef for exactly this reason. It only binds a
stable `EntityId`; it does not yet own a device, input buffer, camera policy, or
lifetime. Its own HxDoc records what future evidence would justify revisiting
that choice.

## `abstract`: a meaningful view of another type

Haxe's `abstract Name(Carrier)` creates a source-level type over an underlying
carrier:

```haxe
abstract EntityId(Int) {
	private inline function new(code:Int)
		this = code;

	public inline function isValid():Bool
		return this > 0;
}
```

`EntityId` is not an abstract class. It is a nominal, meaning name-sensitive,
view over an `Int`. Code cannot casually pass an unrelated integer where an
`EntityId` is required. Inline constructors and operations can enforce the
valid paths without adding a wrapper object.

For admitted operations, haxe.c uses the underlying representation. An
`EntityId` therefore remains an `int32_t` in C and its inline checks become
ordinary expressions or statements. There is no allocation and no runtime
wrapper solely for the abstract.

Use this for IDs, units, validated scalar values, flags with safe operators, or
an ergonomic zero-cost view of a foreign C value. Do not use it to hide a large
mutable object graph or pretend two values share runtime identity.

## `enum abstract`: closed names over one carrier

An `enum abstract` combines a scalar carrier with a declared set of named
values:

```haxe
enum abstract CharacterDamagePolicy(Int) {
	final Invulnerable = 0;
	final Survival = 1;
}
```

It is close to a TypeScript numeric enum or a branded literal union, but Haxe
can use the declared value set to check an exhaustive switch. Haxe.c keeps the
admitted underlying integer representation and emits a defensive fail-stop C
`default` when an external or forged integer violates the closed Haxe domain.

Choose this when all alternatives share the same simple carrier and need no
different payloads. If an alternative needs its own typed data, use an
algebraic enum instead.

## Algebraic `enum`: alternatives with typed payloads

A Haxe algebraic enum defines a closed family of constructors:

```haxe
enum InteractionResult {
	Ignored;
	Started(target:EntityId);
	Blocked(reason:InteractionFailure);
}
```

This is similar to a TypeScript discriminated union, but the constructors and
exhaustive pattern matching are built into the language. It is ideal for
states, events, commands, results, and failure reasons. It is much safer than
unrelated string tags plus nullable fields.

For the current bounded compiler support:

- a payload-free enum becomes a native private C enum; and
- an enum with admitted payloads becomes a tag plus C structs and a union.

The emitted shape is a private implementation detail, not a stable exported C
ABI. Current payload support is deliberately bounded; reference payloads and
some aggregate forms still fail closed instead of being boxed silently.

## Real class instances

Use a class when a value has a meaningful identity or owns state across calls:

```haxe
final class WaterSimulation {
	var revision:Int;

	public function new() {
		revision = 0;
	}

	public function tick(...):WaterTickResult {
		// Mutate the one owned simulation.
	}
}
```

A class is appropriate when:

- construction establishes an invariant;
- several fields have one cohesive lifetime;
- methods protect persistent mutable state;
- callers must refer to the same object rather than a copied snapshot; or
- an object owns a native resource that later needs explicit cleanup.

Haxe.c currently lowers bounded concrete, non-generic instances to private C
structs. Instance methods become ordinary C functions with an explicit `self`
pointer. A `final` class or otherwise proven direct method call needs no virtual
table. A reachable polymorphic hierarchy receives only the closed-world table
it actually needs.

```c
struct hxc_WaterSimulation { int32_t revision; };

struct hxc_WaterTickResult hxc_WaterSimulation_tick(
    struct hxc_WaterSimulation *self,
    /* explicit inputs */);
```

The current constructor model proves non-escaping automatic storage and direct
parent-owned `final` child objects. `GameSession`, for example, owns its
`EntityStore`, `WaterSimulation`, and fixed arrays directly. Haxe.c can embed
those children in the parent C struct without heap allocation. A known helper
may borrow such an object for one call, but cannot return, store, or otherwise
let the pointer outlive its owner.

General heap allocation, garbage-collected escaping objects, generic classes,
interfaces, reflection, and public class ABI are not implied by this support.
The compiler rejects an unsupported lifetime rather than quietly leaking or
emitting an unsafe pointer.

### Why `final class` is a useful default

Mark a class `final` when extension is not part of its design. This tells a
reader that behavior varies through composition or data, not hidden subclasses.
It also lets haxe.c keep exact method calls direct and omit a virtual table when
no other polymorphic use requires one. Remove `final` only for a real and tested
substitution boundary, not to preserve hypothetical flexibility.

## Other Haxe tools that keep the design clear

### `final`: say which reference or field cannot be replaced

Use `final` for locals and fields that receive one value. It reduces the number
of state changes a reader must track. A `final` field may still refer to a
mutable object; it means the reference is stable, not that the complete object
is deeply immutable. In the current owned-child class model, a `final` field is
also the proof that the parent will not replace its embedded child during the
parent's lifetime.

### Pattern matching: keep closed decisions together

Switch on enums and enum abstracts instead of scattering string or integer
comparisons. Exhaustive matching makes a newly added state visible at the code
that must handle it. Haxe.c lowers admitted matches to structural C `switch` or
`if` regions after validating every semantic edge; the C printer does not
invent a missing branch.

### Generics: share an algorithm without erasing its types

Generics are useful when the same operation is truly independent of the
concrete type. Do not add a type parameter used by only one call site, and do
not use a generic container when a domain name would explain an invariant.

Haxe.c currently specializes a bounded set of closed generic static functions
for admitted primitive and enum arguments. In other words, it produces one
direct C version per used concrete type. Generic classes, open type parameters,
reference arguments, and unrestricted specialization are not current general
capabilities. `c.CArray` is a dedicated typed C abstraction with its own
lowering, not proof of broad generic-class support.

### `inline`: remove a boundary only when its meaning stays clear

An inline function can provide a safe constructor or tiny adapter without
requiring a runtime call. Haxe may substitute its typed body at the call site
before haxe.c sees the final operation. This is useful for nominal abstracts
and exact C conversions.

Do not mark large functions inline as a guess about speed. It can increase code
size, compiler work, and debugging distance. Measure hot paths, and keep a
normal function when the call is a useful architecture boundary.

### Extension syntax: ergonomics without hidden ownership

Haxe's `using` feature can make a static function read like a method. It is
useful when the operation naturally belongs in a discoverable semantic facade,
but the value should not own that behavior as class state. The underlying
operation remains a static or module function; extension syntax must not hide
allocation, global state, or an expensive conversion.

### Packages and modules: organize by responsibility

A package groups related modules; a module is one Haxe source file and may
contain several types, functions, and values. Organize by engine responsibility
or vertical feature rather than placing every record in a miscellaneous
`types` folder and every function in a `utils` folder. Keep a small supporting
type beside its sole owner; give a broadly used concept its own searchable
module.

Metadata such as `@:allow`, `@:c.name`, or `@:build` changes a compiler-facing
contract. Use it only when ordinary Haxe cannot express the same fact, keep its
authority narrow, and explain the practical effect locally. Redundant metadata
is noise, while unexplained metadata is hidden architecture.

## Abstract classes and interfaces

These terms are easy to confuse with Haxe abstracts:

- `abstract EntityId(Int)` is a value abstraction over a carrier.
- `abstract class ActorController` is a class that cannot be instantiated
  directly and may require subclasses to provide behavior.
- `interface InputProvider` is a behavior contract without owned instance
  storage.

An abstract class can share fields and implementation across genuine subtypes.
An interface can allow unrelated types to be used through one narrow contract.
Neither should be the first tool for code reuse. Prefer composition when the
relationship is “has a capability,” such as `Character has Vitals`, rather than
“is substitutable for,” such as several input providers implementing the exact
same input contract.

The current haxe.c evidence covers concrete class inheritance and bounded
closed-world virtual methods. Interface lowering is explicitly still a later
compiler task, and abstract-class execution does not have a separate production
support gate. Caxecraft must not rely on either path until focused compiler and
native tests admit it. Today, final classes plus explicit composition and
module functions are the honest default.

## Static-only classes and singletons

Haxe has static fields and methods, but it has no special `singleton class`
language construct. A singleton is a design pattern that makes exactly one
runtime object globally reachable.

Use a module-level function for stateless behavior. A static-only class whose
name merely repeats its module usually adds a fake namespace and Java-shaped
ceremony. Keep a static class only when a real Haxe API requires a nominal
class owner, metadata must attach to that owner, or the type itself is the
public concept.

Avoid hidden mutable singletons for game state. They obscure construction,
test isolation, update order, and ownership; they can also leak state between
warm compiler-server or repeated game runs. If the application currently needs
one session, construct one explicit `GameSession` in the composition root and
pass or own it through typed fields. “There is one right now” does not mean
“make it a global singleton.”

Generated C follows the same distinction:

- a module function is an ordinary function;
- a static immutable value can become a private global or compile-time value;
- a static mutable field becomes process-global state and needs explicit
  startup ordering; and
- one explicitly constructed class instance remains one ordinary owned C
  object, without requiring a global pointer.

## Externs and semantic facades

An `extern` tells Haxe that a declaration exists outside the generated program.
Raylib's raw surface uses extern types and functions to preserve names, layouts,
calling conventions, and includes from `raylib.h`. Haxe.c does not generate a
second implementation for those declarations.

Raw ABI fidelity is necessary but not always pleasant application code. Put a
narrow semantic Haxe layer above it:

```text
raylib.raw.Rectangle  -> exact foreign C declaration
raylib.Rectangle      -> typed Haxe construction and conversions
Caxecraft renderer    -> game-facing use
```

The semantic layer may use abstracts, enum abstracts, inline constructors, and
safe defaults to improve ergonomics without hiding cost. Do not use externs to
write repository-owned behavior in C: compiler limitations should become
focused compiler work, not application escape hatches.

## Macros

A Haxe macro runs inside the Haxe compiler while building the program. It can
inspect or generate Haxe syntax and types. Haxe.c receives the resulting typed
program; the macro itself does not become a C runtime feature.

Macros are powerful enough to make architecture worse when they hide ordinary
control flow or copy behavior into many types. Start with ordinary Haxe. Admit a
macro only when one typed declaration should mechanically produce several
consistent views, for example:

- a capability registry;
- a validated codec or schema view;
- editor palette facts;
- localization references;
- console and CaxeTest descriptors; and
- deterministic documentation or snapshots.

The declaration remains the source of truth and one ordinary Haxe system still
owns runtime behavior. Expansion must be deterministic, inspectable, typed,
collision-checked, and source-positioned when invalid. A macro must not execute
user content, hide update order, perform undeclared network or filesystem work,
or create a second implementation of a mechanic.

Caxecraft's editor and content formats must work at runtime without invoking
the Haxe compiler. A macro may generate engine glue at build time; it cannot be
the map loader or the user's scripting runtime.

## Compile-time target conditionals

`#if c` selects one branch while Haxe compiles. The inactive branch disappears;
there is no generated runtime target check.

Caxecraft uses this narrowly when the representation genuinely differs:

```haxe
#if c
final world:CArray<UInt8, WorldVolume>;
#else
final world:Array<Int>;
#end
```

The C branch demonstrates compact fixed storage. The Eval branch is an
independent behavior oracle. The gameplay rules on either side are shared.
This is appropriate for storage, ABI, native resources, or platform services;
it is not appropriate for separate C and non-C combat, quests, physics, or
content rules. When conditionals spread through a mechanic, introduce a small
typed adapter or fix the missing compiler capability.

## Current haxe.c support boundary

“Haxe can express it” and “this compiler currently lowers it” are different
claims. The current checked-in evidence is intentionally bounded:

| Haxe construct | Current haxe.c evidence | Selected C shape or boundary |
| --- | --- | --- |
| module-level functions and fields | bounded and tested | direct functions and private globals with deterministic startup planning |
| closed `typedef` records | bounded and tested for admitted fields | private C structs passed as values |
| abstracts over admitted carriers | bounded and tested for the used operations | underlying carrier; no wrapper allocation solely for the abstract |
| `enum abstract` over admitted scalars | bounded and tested | underlying scalar plus checked structural switches |
| algebraic enums | bounded payload-free and selected payload support | C enum, or tag plus payload structs/union |
| concrete non-generic classes | bounded and tested | private C structs and nullable typed pointers |
| non-escaping constructors and parent-owned children | bounded and tested | automatic storage or direct embedded child; no heap allocation |
| caller-owned class parameters | bounded and tested | borrowed pointer valid only for the checked call |
| concrete inheritance and virtual methods | bounded closed-world support | embedded base prefix and only the required virtual table |
| abstract classes | no separate production gate | do not rely on them yet merely because concrete inheritance works |
| interfaces | explicitly not admitted yet | source-positioned failure; later interface-table work owns support |
| closed generic static functions | bounded primitive/enum specialization | one direct generated C function per admitted used instance |
| generic classes and methods | not generally admitted | source-positioned failure except dedicated target abstractions |
| function values and closures | not generally admitted | later lowering and lifetime work |
| escaping/heap class objects and reflection | not generally admitted | later allocation, tracing, and object-model work |
| typed C extern imports | bounded reviewed C-facing support | preserve the foreign declaration/call instead of regenerating it |
| Haxe macros | run on the compiler host | generated typed Haxe must still use constructs admitted above |

The linked compiler guides and executable fixtures are the authority if this
summary becomes stale. A new feature advances the table only with focused
positive, negative, native, and generated-output evidence; Caxecraft must not
simulate support with raw C or a game-specific workaround.

## How this maps to Caxecraft

| Game role | Haxe shape | Reason |
| --- | --- | --- |
| `GameSession` | `final class` | Owns the live simulation, update order, child objects, and fixed storage. |
| `EntityStore`, `WaterSimulation` | `final class` | Each owns cohesive mutable state for the session lifetime. |
| `Character`, `CharacterIntent`, tick inputs/results | `typedef` records | They are explicit snapshots and messages, not independent owners. |
| `EntityId` | nominal `abstract` | Keeps one compact integer while preventing unrelated integers from posing as IDs. |
| block kinds and interaction results | algebraic `enum` | They are closed semantic alternatives, sometimes with payloads. |
| damage policy and other scalar decisions | `enum abstract` | Named closed choices use one compact scalar carrier. |
| character, aquatics, vitals, and inventory rules | module functions when stateless | Every controller can call the same deterministic mechanic without constructing a tools object. |
| Raylib raw declarations | `extern` | Raylib owns the C implementation and ABI. |
| Raylib ergonomic layer | abstracts and small typed facades | Haxe users get safer construction and conversions without duplicating Raylib. |
| one running game/session | explicitly owned instance | The composition root owns it; no hidden mutable singleton is required. |
| authored NPCs, maps, dialogue, quests | validated data, with runtime loading planned | Content varies without creating a subclass per character; the current built-in adapter is still generated at build time. |

`PlayerAgent` remains a small typedef until it earns persistent identity or
state. Planned input buffering, controller assignment, camera preferences, and
later local-player indexing are reasons to review the decision, not proof that
all of them must live in one class. The runtime architecture bead
`haxe_c-xge.20.4.2` owns that migration; local two-player/controller behavior is
separately planned by `haxe_c-xge.28`.

## Document the decision where it lives

This guide explains reusable rules. Each important declaration must still teach
its local decision in HxDoc, using compact **what / why / how** order:

1. What does this value or operation mean and own?
2. When a real alternative exists, why is this a record, abstract, enum, class,
   module function, or extern instead?
3. How does state or control move through it, and what important C
   representation, allocation, ownership, or lifetime consequence follows?
4. What concrete change would justify revisiting the choice, when useful?

Do not invent a rejected alternative or repeat this whole document above every
type. Two or three precise paragraphs are better than boilerplate. A newcomer
should understand the local choice without leaving the file, then follow a
link here for the broader rule.

## Common architecture mistakes

- **God object:** one `Main` or `Game` class knows input, simulation, content,
  rendering, audio, and saving. Split ownership by reason to change, then keep
  one visible composition root.
- **Anemic object:** a class only stores fields while unrelated static helpers
  mutate them. Either make it a value with module functions or give a real
  owner the invariant-protecting behavior.
- **Primitive obsession:** unrelated IDs, units, states, and errors are all
  raw integers or strings. Introduce nominal abstracts and closed enums.
- **Boolean blindness:** several booleans combine into unclear or invalid
  states. Replace the closed combination with an enum or validated record.
- **Inheritance for reuse:** subclasses exist only to borrow code. Move the
  reusable rule into a component or module function and compose it.
- **Global singleton state:** tests and level changes depend on invisible
  process history. Construct explicit owners and pass narrow dependencies.
- **Event soup:** string events can trigger arbitrary callbacks in any order.
  Use closed typed events, commands, phases, and one scheduler.
- **Macro magic:** generated behavior cannot be found, debugged, or explained.
  Keep runtime semantics in ordinary Haxe and generate only mechanical views.
- **Premature ECS or data layout:** source complexity grows before a measured
  query or memory bottleneck exists. Keep an admission threshold and benchmark.
- **Target fork:** `#if c` and another branch independently implement gameplay.
  Share semantics and isolate only the representation or platform edge.
- **Content in code:** adding an NPC, line of dialogue, item, or level requires
  a Haxe branch. Put authored facts in validated content and keep mechanics
  reusable.

## Review checklist

Before adding or reshaping a Haxe type, ask:

- Does it have identity, or only a value shape?
- Who owns it, and how long does it live?
- Is mutation cohesive and protected, or would an explicit input/result be
  clearer?
- Are the valid alternatives closed?
- Could a module function express the behavior without a fake namespace?
- Is inheritance true substitutability, or is composition more accurate?
- Is a macro eliminating proven typed boilerplate, or hiding normal code?
- Is `#if c` isolating representation, or duplicating semantics?
- What allocation, copying, virtual dispatch, runtime support, and public ABI
  would the choice require?
- Does haxe.c have executable evidence for that exact construct and lifetime?
- When a plausible alternative exists, does local HxDoc explain the choice?

## Further learning

Repository guides connect these ideas to the exact shipped compiler:

- [Module-level functions and direct calls](function-lowering.md)
- [Closed anonymous-record lowering](aggregate-lowering.md)
- [Haxe enum lowering](enum-lowering.md)
- [Concrete class layouts](class-layout.md)
- [Bounded constructors and borrowed class parameters](constructor-lowering.md)
- [Closed-world virtual dispatch](virtual-dispatch.md)
- [Caxecraft runtime architecture](caxecraft-architecture.md)
- [Typed C and foreign-interface authoring](typed-c-authoring.md)

The following external references teach the source-language and game-design
ideas. They are background, not evidence that haxe.c supports every feature:

- [Haxe manual: the type families](https://haxe.org/manual/types.html),
  [class instances](https://haxe.org/manual/types-class-instance.html),
  [abstracts](https://haxe.org/manual/types-abstract.html), and
  [abstract classes](https://haxe.org/manual/types-abstract-class.html)
- [Haxe manual: macros](https://haxe.org/manual/macro.html) and
  [conditional compilation](https://haxe.org/manual/lf-condition-compilation.html)
- [TypeScript for functional programmers](https://www.typescriptlang.org/docs/handbook/typescript-in-5-minutes-func.html),
  useful for comparing type aliases and discriminated unions
- Robert Nystrom's [Game Programming Patterns](https://gameprogrammingpatterns.com/),
  especially the game-loop, state, component, singleton, and service-locator
  chapters; patterns are vocabulary and tradeoffs, not a checklist
- Glenn Fiedler's [Fix Your Timestep!](https://gafferongames.com/post/fix_your_timestep/),
  the background for fixed simulation steps, bounded catch-up, and render
  interpolation
- Godot's [scene-organization guide](https://docs.godotengine.org/en/latest/tutorials/best_practices/scene_organization.html),
  a useful comparison for focused ownership, loose coupling, dependency
  injection, and explicit application/world/user-interface structure
