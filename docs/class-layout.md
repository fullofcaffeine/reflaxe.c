# Concrete class instance layouts

E3.T04 adds a bounded production representation for ordinary non-generic Haxe
classes. Reachable class declarations lower through schema-5 HxcIR to private
concrete C structs, while Haxe class values remain nullable references to that
storage. The slice is available in both `portable` and `metal`, selects no
`hxrt` feature, and does not establish a public C ABI.

This task defines storage and reference operations only. Haxe-side allocation,
constructors, `super` calls, and field-initialization order remain E3.T05 work.
The positive Haxe fixture can therefore pass, compare, upcast, and access class
references, but it does not pretend that `new` is already supported.

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

The admitted portable and metal paths record `header=none`. They do not add a
descriptor, vtable, collector word, reflection record, or other unconditional
object header. HxcIR can name a specific runtime feature if a later semantic
pass proves that metadata is required; merely declaring or referencing a class
does not make that request. The separate typed `@:c.layout(Struct|Opaque)`
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
class reference becomes an `IRPDereference` place, the same block must contain
an `IRIONullCheck` for that value and its resolved profile/build policy. The
current checked policy emits a fail-stop `abort()` edge in every build mode;
optimization may remove it only after a future proof makes that safe. Inherited
field access walks through the embedded base members to the declaring storage.

Reference equality and inequality use explicit
`haxe.class-reference.equal`/`not-equal` operations and compare normalized
object identity. They remain pointer comparisons, never structural field
comparison. Allocation, ownership, lifetime, and collection are deliberately
absent from this slice.

## Layout and ABI boundary

Every generated class definition is followed in the private C implementation
by structural `_Static_assert` checks covering the first base/field offset,
field alignment, non-overlap, final extent, and the empty-root anchor. The
focused native fixture additionally uses an independent C provider and C++17
consumer to compare `sizeof`, alignment, offsets, base-prefix access, field
reads/writes, upcasts, null preservation, and identity at `-O0` and `-O2`.

`include/hxc/program.h` is the generated program's private compilation header,
not a public export. Production evidence requires the ABI report to retain
`analyzed-no-public-exports` with empty exported type and symbol sets. No class
tag, member name, size, alignment, offset, or reference spelling in this slice
is stable across compiler versions.

Instance methods and constructors remain unsupported; virtual dispatch is
E3.T06, interfaces are E3.T07, and the object descriptor/header plus tracing
policy belongs to E4.T05. Generic class specialization, reflection, dynamic
casts/type tests, allocation, escaping ownership, and public class ABI also
remain fail-closed.

## Evidence

Run:

```sh
npm run test:class-layout
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
allocation, and downcasts without broadening those later-owned capabilities.
