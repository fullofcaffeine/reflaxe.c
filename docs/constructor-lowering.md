# Bounded constructor lowering

E3.T05 admits constructors for concrete, non-generic Haxe classes when the
complete object lifetime is proven inside one generated C function. The
compiler lowers the real pinned-Haxe `TypedExpr` through schema-19 HxcIR and
structural C AST nodes. The direct scalar/class slice does not allocate or
select `hxrt`; an independently admitted managed parameter or field selects
only its dependency-closed runtime features. No path uses C++ constructor
syntax or establishes a public C ABI.

This is deliberately a useful but narrow construction model. A direct local
initializer such as `var item = new Item(7)` receives automatic C storage when
it is in the function's unconditional entry block and the reference cannot
escape that function. A nonescaping parent may also own a child created by a
`final` field initializer such as `public final inventory = new Inventory()`.
The child is stored directly inside the parent's C struct, so it has a stable
address for the complete parent lifetime without a heap allocation.

Returning a local or owned-child reference, assigning it into longer-lived
storage, constructing a local conditionally, or storing `this` into a field
fails with source-positioned `HXC1001`. One same-function automatic alias is
safe: `var second = first` merely names the same stack object, and Haxe may
generate the equivalent `_this` alias when it inlines a method. The compiler
keeps that alias marked as stack-backed, so returning, storing, throwing, or
forwarding it still fails rather than losing the original lifetime.

A local or owned child may also be passed to a known ordinary Haxe function:
the callee receives a checked caller-owned parameter, which means it may read
and mutate the object only for that call. This separate parameter-borrow slice
still forbids creating another local alias inside the callee, as well as
returning, storing, throwing, capturing in a constructor, or forwarding the
borrow to an unproven call. HxcIR records function parameters as
`ownership=borrowed-class` and validates their no-escape rule before C is
chosen. The generated private C function receives an ordinary pointer; neither
form adds allocation or reference-counting machinery.

An owned-child field must be `final`, have the exact concrete class type, and
use an infallible constructor in this bounded slice. General escaping objects
remain owned by the allocator, lifetime, and tracing work in E4.

## Discovery and order

`CStaticFunctionGraphCollector` discovers every constructor reachable from an
admitted `new` or `super` expression. A constructor has a stable nominal ID
such as `constructor.app.Widget`; ordinary static functions referenced by its
body join the same request-local worklist. Dependencies are sorted only where
the model declares them unordered. The original constructor expression remains
ordered compiler input.

The generated call site evaluates source constructor arguments first, then
default-initializes direct object storage, obtains a non-null `self` pointer,
calls the constructor, completes the lifetime transition, and finally creates
the nullable Haxe local reference. The backing declaration uses a structural C
initializer:

```c
struct hxc_widget storage = { 0 };
```

That supplies Haxe defaults for the admitted direct fields before any explicit
field initializer or constructor statement runs. It is not a raw C fragment.

### Immediate constructed receivers

Haxe does not require an object to have a local name before calling one of its
methods:

```haxe
return new ScenarioDocumentReader(records).read();
```

When the complete object is proven not to escape, haxe.c gives this unnamed
receiver the same bounded storage as a named local. “Receiver” here simply
means the value to the left of the method call. Generated C creates one
compiler-named automatic object, calls its ordinary constructor, and borrows
the object's address for `read`:

```c
struct hxc_ScenarioDocumentReader object_storage = { 0 };
hxc_compiler_constructor_ScenarioDocumentReader(&object_storage, records);
result = hxc_ScenarioDocumentReader_read(&object_storage);
```

The borrow is not a second owner. It is valid only while `object_storage`
exists, and the usual HxcIR checks reject returning, storing, or forwarding
that pointer beyond a proven call. This keeps the natural Haxe expression
without paying for heap allocation or requiring the application to introduce a
local solely to satisfy C storage rules.

Constructor arguments still run first and exactly once. If a nested call
creates a fresh managed value such as an `Array`, the caller gives it a short
owner before construction. The constructor may retain it into a field; cleanup
then releases the receiver's field and the short argument owner once each. A
managed method result follows the ordinary function-return contract: the
callee returns one owner, so the caller transfers that owner rather than
retaining it again.

This first direct-receiver slice remains limited to the function's
unconditional entry block. Constructing the receiver inside a branch needs
path-specific destruction at the branch exit; until that separate lifetime
work is implemented, haxe.c reports `HXC1001` instead of extending the
receiver's lifetime or leaking it.

An owned child is a direct `IRTInstance` field in HxcIR rather than the
`IRTPointer` used for an ordinary Haxe class reference. That explicit semantic
choice lets the HxcIR validator reject recursive by-value layouts before C
syntax exists. C definition planning emits the complete child type before its
parent, and the parent constructor takes the child's address, binds any virtual
table required by the child, and then calls the ordinary child constructor:

```c
struct hxc_Parent {
  struct hxc_Child child;
};

hxc_Child_ctor(&self->child);
```

The compiler tracks references derived from that address as borrowed views of
the parent-owned storage. They may be used for immediate field and method
operations or named by a same-function automatic alias, but the lowering
boundary rejects any operation that would let the borrow outlive or become
independent from its parent.

The same rule applies when Haxe inlines a child method into an ordinary parent
instance method. The pinned front end may introduce a local named `_this` for
the embedded child. That local is only another name for the child during the
current parent call; haxe.c preserves its parent-bound borrow instead of
mistaking it for new ownership. The usual return, storage, throw, constructor,
and unproven-call checks still reject a real escape.

Constructor bodies preserve the exact sequence already produced by the pinned
Haxe compiler. The current Haxe 5.0.0-preview.1 filter expands a derived
constructor's own field initializers before its explicit `super` call, and its
fold currently presents those derived initializers in reverse declaration
order. The executable oracle therefore records the intentionally surprising
trace `1,3,2,6,4,5,7`: caller argument, second derived field, first derived
field, `super` argument, base field, base body, derived body. Reflaxe.c neither
reconstructs source order nor silently “corrects” the typed expression; its
Eval oracle and generated C must agree for the pinned compiler.

## C function and elision model

Every non-trivial constructor becomes an ordinary private C function. Its
first parameter is a non-null pointer to the complete class instance. A
`super` call structurally takes the address of the embedded base-prefix member,
so no cast or C++ syntax is involved. Constructor declarations and definitions
use the same `CSymbolRegistry`, C declarator tree, and private generated header
as other functions.

Constructor symbol identity is planned from each prepared body type, not from
the HxcIR constructor name alone. This distinction matters because several
unrelated Haxe families eventually use `IRTInstance`: closed records, enums,
arrays, maps, interface values, and other managed values all need a nominal IR
instance even though their call, copy, ownership, and cleanup rules differ.
Treating every `IRTInstance` as interchangeable would make a new record
capability silently authorize all of those families.

The bounded constructor slice therefore admits a closed anonymous record
parameter through its already validated aggregate identity. Generated C passes
the record by value, while the constructed class keeps its separate automatic
storage and identity:

```c
void hxc_compiler_constructor_ConfiguredSpawn(
  struct hxc_ConfiguredSpawn *self,
  struct hxc_SpawnPoint point
);
```

The full record identity participates in the constructor's semantic symbol key;
the readable C name stays source-shaped because that key is only a collision
and determinism input.

### Optional and default constructor arguments

Haxe may omit trailing constructor arguments whose declarations provide a
default. C has no omitted-call syntax: every call must match its prototype
exactly. Haxe.c therefore settles the argument list before HxcIR, using the same
rule as proven direct function calls:

```haxe
new RestoreReader();                 // inserts both declaration defaults
new RestoreReader(7, null);          // explicit null stays explicit
new RestoreReader(7, {tick: 12});    // supplied record stays present
```

The constructor declaration keeps each default as a typed Haxe expression.
When `new` or `super` supplies fewer trailing arguments, the call lowerer
appends those typed defaults and then lowers the completed list from left to
right. Haxe requires default declarations to be compile-time constants, so a
side-effecting call cannot legally appear as a declaration default. Supplied
argument expressions can have effects and are still evaluated exactly once
before object initialization and the constructor body.

An optional `?restore:RestorePoint` uses a typed `null` default. Omission and an
explicitly written `null` both produce an absent tagged optional, while a
supplied record sets its presence flag and carries the record by value. The
generated constructor remains one ordinary fixed-arity C function; it has no
overload wrapper or runtime “was this argument supplied?” test.

This bounded constructor slice admits tagged optionals only when their payload
has no managed lifetime. A managed optional may need retain/release or tracing
when copied or stored, so it remains closed until its constructor-call
ownership contract is explicit. Function values and other unproven parameter
families remain closed for the same reason. HxcIR receives only the completed
argument list, then independently verifies its count and exact type against the
direct constructor function before CAST selects C syntax.

### Literal-backed nominal String parameters

A Haxe abstract such as `ScenarioId(String)` gives an ordinary String a
distinct source type without adding a wrapper object. For the currently
admitted literal-backed String slice, generated C passes the immutable
`hxc_string` view by value:

```c
void hxc_compiler_constructor_RuleState(
  struct hxc_RuleState *self,
  hxc_string id
);
```

The constructor's semantic symbol key retains the full nominal Haxe identity,
such as `caxecraft.scenario.ScenarioId`, even though its C carrier is the same
two-field String view. This prevents two application ID types from becoming
the same compiler symbol merely because their generated C representation
matches.

The first `this.id = id` assignment to the constructed object's own `final`
field copies that view by value. Its bytes come from compiler-owned literal
storage and remain valid for the whole program, so the field needs no
allocation, retain, release, or tracing. Later writes still fail as writes to
an immutable field.

This is deliberately not a general owned-String rule. Parsing, concatenation,
input, and other runtime-created Strings remain unsupported until E5.T02 gives
their byte storage an explicit owner and cleanup contract. A future owned
String must not inherit this program-long lifetime merely because it uses a
similar C view.

### Unmanaged enum parameters

A Haxe enum whose cases carry no payload is one nominal C enum tag. A
constructor receives that value directly:

```c
void hxc_compiler_constructor_RuntimeObjective(
  struct hxc_RuntimeObjective *self,
  enum hxc_ObjectiveState state
);
```

The exact prepared enum instance participates in the constructor symbol key,
so unrelated enum types remain distinct even if both use the same C integer
representation. A constructor may compare the parameter or copy it into its
own final field. Because the value has no payload, pointer, or cleanup
obligation, both operations are allocation-free and select no runtime feature.
Normal assignment still rejects a later write to the final field.

A payload enum uses a tagged C struct: one discriminant says which case is
active, and one union stores that case's fields. The constructor may also copy
this complete value when the prepared enum graph proves
`managedLifetime == false`. For example, a `FlowValue` whose variants carry
`Bool`, `Int`, or a literal-backed `ContentId(String)` has no owned storage.
The tag and active payload therefore pass and store by value without allocation
or cleanup. The exact enum instance still participates in the symbol key.

That lifecycle fact is computed from the complete reached enum graph before
constructor symbol admission. This ordering matters for recursive or nested
types: default `false` flags during discovery must never make an unfinished
enum look safe.

Payload enums that own Arrays, Bytes, recursive values, or other managed state
remain outside this rule. Passing or retaining one requires an explicit
active-case-aware retain, transfer, tracing, rollback, and cleanup contract.
Focused managed and recursive negatives keep those broader families
fail-closed instead of treating every `IRTInstance` as interchangeable.

### Shared Array parameters

An ordinary Haxe `Array<T>` has shared identity: two variables can name the
same mutable container. A constructor therefore receives the prepared Array
reference by value, which is a borrow for the duration of that call. It may
read the Array or pass it to another proven direct call without copying its
elements.

If the first constructor assignment stores the parameter in the object being
built, `this.values = values`, that field becomes a separate owner. For a
reference-counted specialization such as `Array<Int>`, generated C stores the
same pointer and retains it once:

```c
self->values = values;
hxc_array_ref_retain(self->values);
```

The constructed object's cleanup releases that field exactly once. A fresh
literal argument first receives a short-lived caller owner because a borrowed
parameter cannot consume an ownerless value. The field takes its retain, and
the caller releases the temporary on its normal or failure cleanup path. An
Array whose elements can reach collector-managed objects instead uses the
existing exact root-and-trace graph, so that specialization needs no
reference-count retain.

This preserves identity rather than cloning the Array. The fixture mutates the
caller's Array after construction and observes the change through the stored
field. It also applies repeated allocation/release pressure to `Array<Int>`.
A tracing-collector stress loop would test the wrong lifetime mechanism for
that reference-counted specialization.

The owning boundary remains narrow. The first typed initialization of the
constructed object's own final Array field is admitted. Replacing a managed
Array field later or storing the parameter through another object still fails
until general assignment can retain the new owner and release the old owner
exactly once. HxcIR validates the constructor's exact Array instance type
before CAST chooses its C pointer spelling.

The independently proven interface-dispatch family also has a bounded
constructor contract. A Haxe interface value becomes a small C value containing
two pointers: `object` points at the concrete instance and `table` points at the
method table for that exact interface. A constructor receives that pair by
value and may call its methods during the constructor call:

```c
void hxc_compiler_constructor_ConfiguredScore(
  struct hxc_ConfiguredScore *self,
  struct hxc_compiler_interface_dispatch_ScoreSource_value source
);
```

Copying the pair does **not** copy the object. The compiler therefore chooses
one of two lifetime plans from the typed constructor body:

- If the constructor only reads the parameter or calls its methods, the object
  can remain a caller-owned local. HxcIR records
  `ownership=borrowed-interface` and rejects storage, return, throw, capture, or
  unproved forwarding. This path stays allocation-free.
- If the constructor assigns the value to one of its own interface fields, the
  pair is retained beyond the call. The enclosing class and each reachable
  concrete implementation use collector-managed storage. The enclosing
  object's exact trace function visits the field's `object` pointer; the
  separate `table` pointer is immutable program data and needs no tracing.

Because a C struct field stored by value needs its full type definition—not
only a forward declaration—the project planner emits the interface pair before
class definitions. Split and package layouts put it in the shared private type
header; unity output uses the same dependency order in `hxc/program.h`.

The retained slice is deliberately narrow: the only admitted escape is the
first typed `this.field = parameter` initialization. The compiler does not
weaken a borrow after seeing a generic assignment. Alias storage, returns,
throws, closure capture, and storage through another object remain
source-positioned failures unless another ownership rule proves them. The
source fixture forces a collection and then calls the retained interface,
showing that the dispatch pair did not become a dangling pointer.

Enums, collections other than the admitted Array slice, and every other
`IRTInstance` family remain
source-positioned `HXC1001` failures until their own call, copy, ownership, and
lifetime contracts are proven.

An empty constructor, or an empty zero-argument `super` chain whose base is
also proven trivial, has no C symbol and no call. Source arguments are still
evaluated before elision, so optimization never removes observable effects.
The report retains the constructor with `elided: true` and `cName: null`; this
makes absence reviewable instead of relying on a text search alone.

E3.T06 builds on this same storage path: when a constructed class needs a
reachable virtual table, construction binds the table before user constructor
code. See [closed-world virtual dispatch](virtual-dispatch.md).

## Failure and cleanup

Schema-7 HxcIR gives every function an explicit failure convention. Ordinary
functions are `infallible`. A reachable constructor containing `throw`, or
depending on a constructor that can throw, uses `status(exception)`. Its Haxe
semantic return remains `Void`; structural C emission uses `bool`, returns
`true` on normal completion, and returns `false` when propagating failure.
Derived constructors test and propagate a failing base status. An executable
entry that has no admitted catch policy performs the ordered cleanup and then
calls `abort()`.

Every constructed object registers two typed cleanup actions against its
stable backing place: one for `initializing -> destroyed` and one for
`initialized -> destroyed`. A failing constructor call releases the current
object's managed fields in reverse order, marks the partial object destroyed,
and then executes older cleanup actions in reverse construction order. Fields
the constructor did not reach still contain their zero/null defaults, and each
admitted release operation is null-safe; no hidden per-field “was initialized”
flags are needed. Normal returns execute the initialized object's field
releases and lifetime action in the same strict reverse registration order.
Direct scalar/class-reference storage currently needs no native destructor
statement, but the validated HxcIR order is still mandatory; owned fields
consume these edges without trying to recover lifetime facts from C lexical
nesting.

This bounded status path does not claim general Haxe exceptions. There is no
catch/finally surface, exception payload transport, foreign-frame transfer, or
runtime exception frame in E3.T05. Those remain E4.T09. The admitted top-level
failure is fail-stop, while nested constructors preserve propagation and
cleanup exactly enough for the bounded no-catch graph.

## Fail-closed boundaries

The compiler reports exact `HXC1001` diagnostics and emits no project for:

- an assigned or returned stack reference, or an automatic alias that is later
  stored, thrown, captured, or forwarded beyond a known borrow contract;
- an assigned, reassigned, or returned owned-child reference, or a bounded
  alias that later outlives or becomes independent from its parent;
- a mutable owned-child field, a mismatched declared child type, or a fallible
  owned-child constructor;
- conditional or otherwise non-entry local construction;
- constructor dependency cycles, with the canonical nominal cycle path;
- extern or `@:c.layout` native construction, because imported construction
  and destruction policy is not inferred from a Haxe declaration;
- generic class construction without a closed class specialization; and
- constructor parameters whose prepared value family has no constructor-call
  contract, even when the family shares the generic `IRTInstance` IR shape;
- managed optional, callable, or other constructor parameters whose copy and
  lifetime contract has not yet been admitted;
- replacement of an existing managed Array field, or storage of an Array
  parameter through an object other than the one being constructed;
- an interface constructor parameter that is stored, returned, thrown,
  captured, or otherwise allowed to outlive its call; and
- broader exceptions, runtime-checked interface casts, dynamic/generic
  dispatch, allocation,
  ownership, or public ABI.

Haxe itself rejects attempts to instantiate an interface before the custom
target receives typed input. That language-level rejection is not relabeled as
an `hxc` diagnostic.

Constructor cycles are rejected during reachable-graph discovery before a C
declaration is planned. Unsupported native layouts are diagnosed at the source
`new`, rather than being mistaken for an ordinary private class layout.

## Evidence

Run:

```sh
npm run test:constructor-lowering
npm run test:constructor-direct-receiver
npm run test:virtual-dispatch
npm run test:class-layout
npm run test:hxc-ir
npm run test:all-sources
npm run test:typed-boundaries
npm run snapshots:check
```

Use `test:constructor-direct-receiver` while changing the immediate-receiver
slice. It runs that slice's complete Eval, layout, order, compiler-server,
native, sanitizer, C++ header, and escape matrix without recompiling unrelated
constructor families. `test:constructor-lowering` remains the exhaustive
reference gate before integration.

`test/constructor_lowering/fixtures/minimal/Main.hx` is the small readable
example. The focused `record_parameter` fixture proves a direct closed record
argument in split, package, and unity output, reversed discovery, warm compiler
server reuse, and strict native execution. `interface_parameter` proves the
same output and native matrix for a by-value interface pair whose constructor
performs real interface dispatch; `interface_parameter_escape` proves that the
pair cannot be retained after the call. `instance_parameter` proves that a
fresh payload enum with an owned Array cannot silently cross a constructor call
before caller-side transfer or cleanup is planned.
`default_arguments` proves omitted and supplied defaults, explicit `null`, a
present optional record, exactly-once supplied-argument evaluation, and
`super()` completion across split/package/unity, reversed discovery, warm
server reuse, Eval, strict C11, and sanitizer lanes. `default_callable` proves
that valid Haxe with an unproven callable representation still fails before any
C is emitted. `array_parameter` proves call-only borrowing, shared identity,
one retained final-field owner, a caller-owned fresh literal, repeated
reference-counted allocation pressure, exact runtime selection, and balanced
cleanup across the same layout/order/server/native/sanitizer matrix.
`array_parameter_escape` keeps storage through another object's existing Array
field fail-closed. The positive
`string_parameter` fixture proves nominal constructor identity, by-value
literal-backed borrowing, final-field storage, header-only runtime selection,
and Eval/native/sanitizer parity across the same deterministic layouts.
`enum_parameter` proves exact fieldless-enum identity, by-value tag passing,
comparison, final-field storage, and runtime-free output across that matrix.
`enum_payload_parameter` proves all active unmanaged payload variants,
by-value tagged-struct passing, final-field storage, exact identity, and
allocation-free header-only String-literal support. `direct_receiver` proves
`new Reader(records).read()` with a retained Array input, an owned Array
result, a fresh nested argument, automatic object storage, and no collector or
`goto`. It compares Eval with split, package, and unity C; reversed discovery;
warm compiler-server reuse; strict GCC and Clang at `-O0` and `-O2`; C++17
header consumption; and sanitizer execution. `direct_receiver_failure` proves
that a constructor which throws after retaining its Array field releases both
that field and the caller's fresh argument owner before the fail-closed abort.
`direct_receiver_escape` keeps a child borrowed through the temporary parent
from becoming an independent local. The revised
`instance_parameter` negative reaches the more precise
`function-exit:unowned-fresh-managed-enum-value` boundary: the constructor can
read its admitted payload enum, but the caller cannot yet transfer or release
the fresh Array owner stored inside it. `recursive_enum_parameter` reaches the
same caller-cleanup boundary with an indirect recursive payload. Together they
prove that understanding a managed tagged union's representation does not
silently authorize an incomplete ownership lifecycle.
The positive
semantic corpus adds inheritance, default fields,
side-effecting arguments and initializers, a throwing base constructor, an
inner temporary, empty-constructor elision, a same-function stack alias, and a
parent with an inline owned child whose constructor, stable identity, and later mutation are observed. It
compares Eval with repeated, reversed-input, portable, metal, and explicit
runtime-none production builds. Negative fixtures keep child reassignment,
return, storing a bounded alias, unsafe borrow forwarding, constructor capture,
fallible construction, and recursive direct layout fail-closed. Focused HxcIR
fixtures prove that a declared automatic borrow alias may be initialized and
reloaded, while the same pointer still cannot initialize an ordinary owning
local or escape after that reload.
Because Eval is a dynamic platform whose uninitialized primitive fields are
`null`, the separate target-native default-field fixture proves the C target's
static-platform `0`/`false`/`0.0`/null defaults without pretending Eval is an
oracle for that target-specific representation fact.
The native harness compiles and runs strict C11 under identity-verified GCC and
Clang at `-O0` and `-O2`, verifies the intentional abort path, and compiles the
private header as C++17. The checked-in HxcIR/C/symbol snapshots contain no
checkout path or runtime dependency.
