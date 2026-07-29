# Bounded constructor lowering

E3.T05 admits constructors for concrete, non-generic Haxe classes when the
complete object lifetime is proven. The compiler lowers the real pinned-Haxe
`TypedExpr` through schema-21 HxcIR and structural C AST nodes. A class whose
object stays inside one function can remain allocation-free. A class reference
that crosses a function return or is retained by another object instead selects
the dependency-closed object and garbage-collector runtime features needed for
stable storage. No path uses C++ constructor syntax or establishes a public C
ABI.

This is deliberately a useful but narrow construction model. A direct local
initializer such as `var item = new Item(7)` receives automatic C storage when
it is in the function's outer statement sequence and the reference cannot
escape that function. An earlier guard may return before the declaration:

```haxe
if (!inputIsValid)
  return fallback;
final item = new Item(7);
return item.read();
```

The early return records its cleanup before `item` exists. Construction then
registers `item` for every later exit, so HxcIR can prove exactly which paths
destroy it without moving the constructor before the guard. This is different
from declaring the object inside one branch, loop body, or switch arm. Such an
object must be destroyed when that nested body ends, and path-scoped class
destruction is not admitted yet.

A nonescaping parent may also own a child created by a `final` field initializer
such as `public final inventory = new Inventory()`. The child is stored
directly inside the parent's C struct, so it has a stable address for the
complete parent lifetime without a heap allocation.

Assigning an otherwise local reference into longer-lived storage, conditionally
constructing an object that still uses automatic storage, or storing `this`
into an unsupported field fails with
source-positioned `HXC1001`. One same-function automatic alias is safe:
`var second = first` merely names the same stack object, and Haxe may generate
the equivalent `_this` alias when it inlines a method. The compiler keeps that
alias marked as stack-backed, so storing, throwing, or forwarding it beyond a
proved call still fails rather than losing the original lifetime.

A local or owned child may also be passed to a known ordinary Haxe function.
When that concrete class has no longer-lived use anywhere in the reachable
program, the callee receives a checked caller-owned parameter: it may read and
mutate the object only for that call. This parameter-borrow slice still forbids
creating another local alias inside the callee, as well as storing, throwing,
capturing in a constructor, or forwarding the borrow to an unproven call.
HxcIR records such function parameters as `ownership=borrowed-class` and
validates their no-escape rule before C is chosen. If a reachable signature
returns that class, whole-program planning instead selects the managed
representation described below, so passing and returning the stable pointer is
safe. The generated private C function still receives an ordinary typed
pointer in either case.

An owned-child field must be `final` and have the exact concrete class type.
Its constructor may fail: the generated call propagates the status through the
parent constructor, while the caller-owned partial parent releases every
managed field reached inside the child exactly once. The child still remains
inline because failure behavior does not change its successful lifetime.

Ordinary class references that are retained in object fields use a different
representation. Whole-program planning promotes the owner and referenced class
to the precise nonmoving collector, stores a typed nullable pointer, and traces
that pointer from the owner. This is the conservative correct baseline for an
object that can outlive its creating call; later escape analysis may recover
inline storage only when it proves the alias never exceeds the parent's
lifetime.

### Class references returned to a caller

A returned object must remain valid after the function that created it has
finished. In C terms, that means a factory cannot return the address of an
ordinary local variable: the local's storage ends when the function returns,
so the pointer would immediately dangle.

Haxe makes this lifetime visible in the function signature:

```haxe
static function create(value:Int):Null<ValidatedValue> {
  if (value <= 0)
    return null;
  return new ValidatedValue(value);
}
```

Before lowering any body, haxe.c inspects every prepared return type. When a
return can carry a class reference—including through an admitted nullable or
closed record—it promotes that concrete class to stable collector storage.
The guarded `new` can then run only on the successful path, and generated C
returns a typed pointer owned by the collector instead of the address of a
callee-local struct. The caller records an exact root while it keeps the
result, so later collections cannot reclaim the object too early.

The representation choice is currently conservative for the whole concrete
class. If one reachable function returns `ValidatedValue`, other reachable
instances of that same class also use the managed representation. This keeps
all producers and consumers on one C layout and is correct before more precise
per-allocation escape analysis exists. It may allocate more instances than a
future optimizer would need to; that is a performance opportunity, not a
reason to expose an unsafe pointer.

Passing an already managed instance through a helper preserves the same object
identity. It does not copy the object or create a second owner. HxcIR still
validates allocation, construction order, roots, and each return edge before
the compiler selects C syntax.

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
then releases the receiver's field and the short argument owner once each,
immediately after the method returns. A managed method result follows the
ordinary function-return contract: the callee returns one owner, so the caller
transfers that owner before ending the receiver and argument lifetimes rather
than retaining it again.

A method receiver and each earlier argument are also saved when a later
argument creates an `if` or `switch` join. HxcIR values belong to one basic
block, so the join cannot name the original receiver value directly. A typed
automatic local carries the already checked pointer through the branches,
preserves whether it is a parent-bound borrow, and reloads it once for the
call. The validator carries the receiver's non-null proof through that local;
generated C does not need a duplicate runtime check after the join. Because
the object has exactly one immediate call use, haxe.c can also construct it in
a reachable branch or after an earlier guard: constructor failure owns the
partial cleanup edge, and a successful call ends the temporary lifetime in the
same generated block. A named object declared inside a branch is still a
different, unsupported case because later statements in that branch may keep
using it and its scope needs a separate path-sensitive lifetime.

### Fresh objects passed directly to calls

Natural Haxe also permits a fresh object to be an argument:

```haxe
final plans = planActorComposition(objects, new BaseContentRegistry());
```

Before this capability, haxe.c rejected the `new` expression unless the author
first assigned it to a named local. That rewrite was safe but artificial: the
name existed only to expose addressable storage to C, not because the Haxe
program needed a longer-lived registry.

A **call-bounded borrow** means the known callee may read or mutate the object
while that call is running but cannot store, return, throw, capture, or forward
the reference into an unproved call. haxe.c establishes that rule from the
callee's typed body before lowering the caller. For a qualifying direct
function or final-method call, it then:

1. evaluates arguments from left to right;
2. creates compiler-owned automatic storage for the fresh class;
3. calls its constructor and cleans every initialized field if construction
   fails;
4. converts the pointer to the exact class or interface parameter value;
5. performs the known synchronous call; and
6. releases temporary argument owners and object fields in reverse creation
   order immediately after the call.

For an interface parameter, the conversion in step 4 produces the ordinary
by-value `{ object, table }` pair. The object pointer still refers to the same
automatic storage; the interface table only selects the correct method
implementation and does not extend the object's lifetime.

This rule does not make every call synchronous or safe by assumption. The
compiler admits it only when it has the exact target and the target's parameter
has passed the no-escape proof. Virtual dispatch, function values, constructors
that retain the parameter, and other unknown forwarding remain
source-positioned `HXC1001` failures for an unmanaged fresh object. A class
that whole-program analysis already placed in collector-managed storage uses
that stable representation instead of this bounded automatic-storage path.

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

### Retained concrete references and record fields

A concrete class field that keeps a reference after its creating method returns
forms the same kind of persistent object graph as a retained interface. The
compiler gives both objects stable collector storage, initializes the field
only from an exactly typed rooted value, and emits an exact trace edge.
Replacing a mutable reference updates that graph edge; clearing a nullable
field stores null. An inline `final child = new Child()` remains by value unless
the same child class must also participate as an independently retained object.

The constructing object may also publish `this` as the first value of one of
its own managed class-reference fields. That creates a real cycle—for example,
`peer = this`—but not an unbounded escape: the write targets storage owned by
the same rooted object while its constructor is running. The generated
descriptor traces that typed pointer like any other managed edge. The focused
fixture forces collection and then checks `peer == this`, proving that the
cycle remains live without a self-reference special case in C.

Constructor fields use the smallest ownership rule that matches their value:

- an ownership-free closed record is copied directly into its final field once;
- a managed record first captures one replacement owner, releases the prior
  nested owners, and only then stores the replacement, making aliasing safe;
- an inline child recursively contributes its nested trace and finalizer work
  to the outer managed object's descriptor; and
- a retained concrete class uses the typed collector pointer described above.

The focused retained-object fixture exercises all four rules under allocation
pressure, strict native compilation, Haxe Eval, and AddressSanitizer plus
UndefinedBehaviorSanitizer. Unsupported value families still report
source-positioned `HXC1001`; sharing the generic `IRTInstance` shape alone never
grants a copy or lifetime contract.

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
flags are needed. Named objects execute their initialized field releases and
lifetime action on normal return in the same strict reverse registration
order. An unnamed object with one proven synchronous call use executes that
same sequence immediately after the call returns.
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

- an assigned stack reference, or an automatic alias that is later stored,
  thrown, captured, or forwarded beyond a known borrow contract;
- an assigned or reassigned owned-child reference, or a bounded
  alias that later outlives or becomes independent from its parent;
- a mutable owned-child field or a mismatched declared child type;
- branch-, loop-, or switch-local automatic class construction whose
  destruction would occur before the surrounding function exits;
- an unmanaged fresh object passed to a virtual, indirect, retaining, or
  otherwise unproved call;
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
- an interface constructor parameter that escapes outside the exact retained
  field-initialization rule, including return, throw, capture, or unrelated
  storage; and
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
npm run test:constructor-early-exit
npm run test:constructor-direct-receiver
npm run test:constructor-direct-argument
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

Use `test:constructor-direct-argument` for the neighboring call-argument rule.
It proves direct class and interface parameters, final instance methods,
left-to-right evaluation around fresh managed arguments, fallible-constructor
cleanup, immediate reverse cleanup after the call, split/package/unity output,
warm-server determinism, strict GCC/Clang C11 at `-O0` and `-O2`, and
sanitizers without running unrelated constructor fixtures.

Use `test:constructor-early-exit` for the smaller root-guard lifetime rule. Its
positive fixture returns before construction on one path, constructs and uses a
nonescaping object on the surviving path, and proves that only the later return
owns cleanup. The same focused command recompiles the existing branch-local
negative so the broader unsafe case cannot become accepted accidentally.

`test/constructor_lowering/fixtures/minimal/Main.hx` is the small readable
example. The `early_exit` fixture proves function-lifetime automatic storage
after a validated root-level guard, exact per-exit HxcIR cleanup, runtime-free
structured C, Eval parity, reversed-input determinism, strict C11 at both
optimization levels, and sanitizer execution. The focused `record_parameter`
fixture proves a direct closed record
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
`direct_argument` proves `consume(left, new Resolver(), right)` for concrete
class and interface parameters plus a final instance method. Its generated C
keeps left-to-right evaluation, cleans a failed partial resolver together with
earlier arguments, and on success releases the right argument, resolver field,
and left argument immediately after the call in reverse creation order.
`factory_return` proves a validated `Null<Class>` factory, class-reference
pass-through, and a child returned from a temporary parent across Eval,
split/package/unity output, reversed discovery, warm compiler-server reuse,
strict C11, C++17 header consumption, O0/O2, allocation pressure, and
sanitizers. The revised
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
storing a bounded alias, unsafe borrow forwarding, constructor capture,
and recursive direct layout fail-closed. The promoted `owned_fallible` fixture
keeps a fallible child inline inside a collector-managed parent, proves
recursive cleanup of the child's Array field, and retains a self-reference
cycle through allocation pressure. The focused `--owned-fallible-only` lane
checks that graph across split/package/unity output, reversed discovery, warm
compiler-server reuse, O0/O2, and sanitizers without repeating unrelated
constructor fixtures. `--negative-only` checks only the still-unsupported
boundaries. Focused HxcIR fixtures prove that a declared automatic borrow alias
may be initialized and reloaded, while the same pointer still cannot initialize
an ordinary owning local or escape after that reload. The parallel interface
fixture proves the same rule for the small object-pointer/table-pointer pair
used by interface dispatch. This distinction matters because a copied pair is
an independent C value but not an independent owner of the object it names.
Because Eval is a dynamic platform whose uninitialized primitive fields are
`null`, the separate target-native default-field fixture proves the C target's
static-platform `0`/`false`/`0.0`/null defaults without pretending Eval is an
oracle for that target-specific representation fact.
The native harness compiles and runs strict C11 under identity-verified GCC and
Clang at `-O0` and `-O2`, verifies the intentional abort path, and compiles the
private header as C++17. The checked-in HxcIR/C/symbol snapshots contain no
checkout path or runtime dependency.
