# Bounded constructor lowering

E3.T05 admits constructors for concrete, non-generic Haxe classes when the
complete object lifetime is proven inside one generated C function. The
compiler lowers the real pinned-Haxe `TypedExpr` through schema-17 HxcIR and
structural C AST nodes. It does not allocate, select `hxrt`, use C++ constructor
syntax, or establish a public C ABI.

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

Copying the pair does **not** copy or own the object. The object can be a local
whose storage belongs to the caller, so the constructor parameter is a
call-bounded borrow: it may inspect the object or call through the interface,
but it may not store, return, throw, or capture the interface value. Constructor
preparation checks the typed Haxe body for that no-escape contract. HxcIR then
records the parameter as `ownership=borrowed-interface` and independently
rejects a later operation that would let the borrow escape. This keeps the
generated code allocation-free without turning a short-lived pointer into a
dangling one.

Enums, collections, managed values, and every other `IRTInstance` family remain
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
`initialized -> destroyed`. A failing constructor call executes the current
partial action first, then older initialized objects in reverse construction
order. Normal returns and constructor throws execute initialized actions in
reverse order. Direct scalar/class-reference storage currently needs no native
destructor statement, but the validated HxcIR order is still mandatory; later
owned fields can consume the same edges without recovering lifetime facts from
C lexical nesting.

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
npm run test:virtual-dispatch
npm run test:class-layout
npm run test:hxc-ir
npm run test:all-sources
npm run test:typed-boundaries
npm run snapshots:check
```

`test/constructor_lowering/fixtures/minimal/Main.hx` is the small readable
example. The focused `record_parameter` fixture proves a direct closed record
argument in split, package, and unity output, reversed discovery, warm compiler
server reuse, and strict native execution. `interface_parameter` proves the
same output and native matrix for a by-value interface pair whose constructor
performs real interface dispatch; `interface_parameter_escape` proves that the
pair cannot be retained after the call. `instance_parameter` proves that a
fieldless enum sharing the `IRTInstance` shape remains rejected. The positive
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
