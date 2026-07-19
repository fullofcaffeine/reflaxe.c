# Bounded constructor lowering

E3.T05 admits constructors for concrete, non-generic Haxe classes when the
complete object lifetime is proven inside one generated C function. The
compiler lowers the real pinned-Haxe `TypedExpr` through schema-10 HxcIR and
structural C AST nodes. It does not allocate, select `hxrt`, use C++ constructor
syntax, or establish a public C ABI.

This is deliberately a useful but narrow construction model. A direct local
initializer such as `var item = new Item(7)` receives automatic C storage when
it is in the function's unconditional entry block and the reference cannot
escape that function. Returning it, aliasing it, passing it to another Haxe
function, assigning it into storage, constructing it conditionally, or storing
`this` into a field fails with source-positioned `HXC1001`. General escaping
objects remain owned by the allocator, lifetime, and tracing work in E4.

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

- an escaping, aliased, assigned, returned, or passed stack reference;
- conditional or otherwise non-entry local construction;
- constructor dependency cycles, with the canonical nominal cycle path;
- extern or `@:c.layout` native construction, because imported construction
  and destruction policy is not inferred from a Haxe declaration;
- generic class construction without a closed class specialization; and
- broader exceptions, interface/dynamic/generic dispatch, allocation,
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
example. The positive semantic corpus adds inheritance, default fields,
side-effecting arguments and initializers, a throwing base constructor, an
inner temporary, and empty-constructor elision. It compares Eval with repeated,
reversed-input, portable, metal, and explicit runtime-none production builds.
Because Eval is a dynamic platform whose uninitialized primitive fields are
`null`, the separate target-native default-field fixture proves the C target's
static-platform `0`/`false`/`0.0`/null defaults without pretending Eval is an
oracle for that target-specific representation fact.
The native harness compiles and runs strict C11 under identity-verified GCC and
Clang at `-O0` and `-O2`, verifies the intentional abort path, and compiles the
private header as C++17. The checked-in HxcIR/C/symbol snapshots contain no
checkout path or runtime dependency.
