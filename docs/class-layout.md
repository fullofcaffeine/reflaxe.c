# Concrete class instance layouts

E3.T04 adds a bounded production representation for ordinary non-generic Haxe
classes. Reachable class declarations lower through schema-19 HxcIR to private
concrete C structs, while Haxe class values remain nullable references to that
storage. A class proven not to escape keeps this direct, runtime-free form. The
bounded `Array<Class>` graph path and retained-interface-field path instead
give the same payload an exact traced representation backed by the selective
collector. Neither form establishes a public C ABI.

This task defines storage and reference operations. E3.T05 separately adds a
bounded `new` path for unconditional, nonescaping local objects, including
constructors, `super`, field initialization, and cleanup. General allocation
and escaping ownership remain later work; see [bounded constructor
lowering](constructor-lowering.md).

## Nominal storage and inheritance

`CBodyClassRegistry` accepts reachable, concrete, non-extern, non-interface,
non-generic class declarations. Each class receives a collision-checked
semantic identity derived from its logical Haxe path. Discovery order, checkout
location, locale, profile, and runtime policy do not participate in that
identity.

An `IRTKClass` declaration records:

- an optional direct base-class instance;
- the class's own mutable storage fields in Haxe source order; and
- explicit header intent.

The corresponding concrete instance uses direct private storage, while a Haxe
class value is `IRTPointer(IRTInstance(...), true)`. A derived struct embeds its
complete base struct as the first real member, followed by its own fields. An
otherwise empty root class receives one compiler-owned `unsigned char` anchor
because strict C11 has no empty-struct production. An empty derived class needs
no anchor: its embedded base already provides complete storage.

The base layout path records `header=none`. E3.T06 may instead select one named
program-local virtual-table layout on the root of a hierarchy with a reachable
polymorphic call; derived classes inherit that pointer through their embedded
base and unrelated/final-only hierarchies remain header-free. No descriptor,
collector word, reflection record, or other unconditional object header is
added. E4.T05 now provides the separate immutable descriptor and runtime-owned
header contract for a later managed representation. HxcIR can request that
feature only after escape/lifetime planning proves it is required; merely
declaring or referencing a class does not make that request. The separate typed
`@:c.layout(Struct|Opaque)`
contracts remain the metal value/opaque route and do not turn an ordinary Haxe
class into exported ABI.

## Reference safety

Derived-to-base conversion is an explicit static `IRCRepresentation` step in
HxcIR. C emission follows the embedded-base path structurally:

```c
derived == NULL ? NULL : &derived->base.base
```

The exact member chain depends on the inheritance depth. No integer conversion,
pointer reinterpretation, or unchecked C cast is used, and null stays null.
Unrelated conversions and base-to-derived downcasts fail with source-positioned
`HXC1001` and leave no plausible output.

Field access also keeps the safety decision above C syntax. Before a nullable
class reference becomes an `IRPDereference` place, an `IRIONullCheck` for that
exact immutable value must run first on every control-flow route. The current
checked policy emits a fail-stop `abort()` edge in every build mode.

The body builder initially places a check beside every field or method use. A
named HxcIR pass then keeps the first check and removes a later one only when
the first check *dominates* it—meaning every route to the later use passes
through the earlier check. A check inside only one branch does not prove
anything after the branches rejoin. Reloading or reassigning a reference also
creates a new immutable HxcIR value, so it keeps a check of its own. Validation
recomputes the same dominance proof before C syntax is selected. Inherited
field access walks through the embedded base members to the declaring storage.

Reference equality and inequality use explicit
`haxe.class-reference.equal`/`not-equal` operations and compare normalized
object identity. They remain pointer comparisons, never structural field
comparison.

## When a class becomes traced

The compiler settles representation from the reachable program rather than
from a class declaration in isolation. If a concrete class is used as the
element of a reachable `Array<Class>`, that array can hold aliases and cycles,
so reference counting alone is insufficient. A class that retains an
interface value has the same lifetime pressure: the interface stores an object
pointer for later dispatch, and a caller-owned stack object could disappear
first. The array graph, a retained-interface owner, and the reachable concrete
implementations therefore use `IRRManaged("gc")`. A class that enters neither
graph remains ordinary C storage and keeps the collector out of the program.

A managed `new` allocates the exact class payload through `hxc_gc_allocate`.
Generated HxcIR publishes the returned pointer in an exact root slot before it
calls the constructor, so another allocation or collection cannot reclaim the
half-built object. The generated class descriptor traces only fields whose
settled representation is also collector-managed. Its optional finalizer
releases independently reference-counted fields, such as `Array<Int>`, without
trying to release collector-owned links. A generated `Array<Class>` descriptor
visits each live pointer slot and disposes only its backing element buffer when
the array payload itself is swept.

This is deliberately a bounded graph path, not a claim of general heap-class
support. A retained interface is admitted only as the first initialization of
the constructing object's own field. Managed class-virtual headers, inline
owned-class fields, generic classes, and other unproved escape shapes still
fail with a source-positioned diagnostic. They must gain their own
representation and lifetime evidence rather than inheriting this result by
accident.

## Layout and ABI boundary

Every generated class definition is followed in the private C implementation
by structural `_Static_assert` checks covering the first base/field offset,
field alignment, non-overlap, final extent, and the empty-root anchor. The
focused native fixture additionally uses an independent C provider and C++17
consumer to compare `sizeof`, alignment, offsets, base-prefix access, field
reads/writes, upcasts, null preservation, and identity at `-O0` and `-O2`.
Branch/join fixtures inspect both HxcIR and generated C guard counts, a null
receiver must still terminate with `SIGABRT`, and normal paths run under address
and undefined-behavior sanitizers when the installed compiler provides them.

`include/hxc/program.h` is the generated program's private compilation header,
not a public export. Production evidence requires the ABI report to retain
`analyzed-no-public-exports` with empty exported type and symbol sets. No class
tag, member name, size, alignment, offset, or reference spelling in this slice
is stable across compiler versions.

Ordinary reachable instance methods and minimal closed-world virtual dispatch
are admitted by E3.T06; see [closed-world virtual
dispatch](virtual-dispatch.md). E3.T07 owns the bounded call-only and retained
interface paths. The object descriptor/header contract comes from E4.T05, and
E4.T06 owns managed allocation, explicit roots, tracing, and collection;
`haxe_c-53k.2.1.2` first connected that backend to the bounded concrete
`Array<Class>` graph. E3.T05 constructors now select either direct stack
storage or the already-settled collector representation. Generic class
specialization, dynamic methods, reflection, dynamic casts/type tests, broader
escaping ownership, and public class ABI remain fail-closed.

## Evidence

Run:

```sh
npm run test:class-layout
npm run test:array-runtime
npm run test:virtual-dispatch
npm run test:hxc-ir
npm run test:typed-boundaries
npm run test:all-sources
npm run snapshots:check
```

The focused suite renders twice, reverses typed-module discovery, compares
portable and metal, snapshots HxcIR/private C/finalized symbols, and verifies
byte-identical production roots under automatic and explicit runtime-none
policy. Required CI lanes compile and run the checked-in generated C plus its
C++17 layout companion under identity-verified GCC/G++ and Clang/Clang++ at
both optimization levels. Negative fixtures cover interfaces, generic classes,
and downcasts without broadening those later-owned capabilities. Constructor
and escaping-allocation boundaries live in the dedicated E3.T05 suite.
The E3.T06 suite separately proves that a reachable hierarchy header is
root-only and that a final direct-call class keeps the header-free layout.
