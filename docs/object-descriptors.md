# Object and type descriptors

<!-- hxrt-feature:object -->

An object descriptor is a small, immutable record that tells the managed-memory
runtime how large one object payload is, how it is aligned, which outgoing
managed references it contains, and whether it has an optional cleanup
function. “Payload” means the generated C struct that stores the Haxe fields;
the collector's own bookkeeping is separate runtime storage.

This is infrastructure for escaping Haxe objects, not a universal object
wrapper. A class proven to stay inside one C function still uses direct
automatic storage. A runtime-free or metal build does not package `object.h`,
`object.c`, a descriptor, or an object header merely because a class exists.

## Why this layer exists

Haxe class values preserve identity. Two references can point at the same
mutable instance, and ordinary Haxe graphs can contain cycles. Once an instance
escapes its original stack lifetime, generated C needs stable storage and the
future precise collector needs an exact way to find references reachable from
that storage.

The descriptor is the narrow shared answer. It keeps these facts in one place:

- `object_size` and `object_alignment` come from `sizeof(T)` and
  `HXC_ALIGNOF(T)` in generated C, leaving C as the final layout authority;
- `trace` visits only live managed references known from the Haxe type and
  representation plans;
- `finalize` is optional and does not create a Haxe promise of prompt or
  deterministic finalization; and
- `abi_version` and validated flags make incompatible internal layouts fail
  closed.

It intentionally contains no type name, field-name table, method lookup,
dynamic constructor, or arbitrary metadata. Those are reflection features and
remain separately selected. It also contains no mark bit or allocation-list
link. Those are private choices of the collector backend.

## Header and payload relationship

`hxc_object_header` points at one validated descriptor. It is a runtime-owned
prefix associated with a managed allocation, not a field silently inserted into
every generated class:

```text
runtime-owned allocation
  object/collector header -> immutable descriptor
  generated class payload -> ordinary typed C fields
```

This separation matters for readable C, foreign-function interface (FFI)
stability, and the hard runtime-free contract. A selected collector may extend
its own private allocation record without changing the payload layout or an
exported application type. Runtime structs remain forbidden in the public C
application binary interface (ABI).

## Exact tracing

The trace callback receives the payload and a visitor function. Generated trace
code calls that visitor once for each non-null managed base reference currently
stored in the object. A closed enum traces only the payload selected by its
active tag; a tagged optional traces only when present; a record delegates to
its managed fields; and an Array delegates through its typed element plan.

The callback does not scan arbitrary bytes and does not treat every pointer as
a root. An interior pointer remains a borrow that needs a live typed base, while
a pointer held by foreign C needs an explicit pinned handle. These rules come
from [ADR 0005](adr/0005-precise-nonmoving-collector.md).

## Current boundary and next owner

E4.T05 owns the validated descriptor/header contract and the compiler's
selective descriptor plans. E4.T06 supplies managed allocation, explicit root
and pin APIs, safepoints, marking, sweeping, cycles, statistics, and at-most-once
finalizer invocation. `haxe_c-53k.2.1.2` now connects those facilities to the
bounded concrete `Array<Class>` graph. It emits one exact class descriptor and
one exact array descriptor, allocates through the collector, roots a fresh
object before its constructor runs, and preserves reference identity through
array growth and indexed replacement. Direct classes and ordinary primitive
arrays keep their previous collector-free representations.

The integration is general to the admitted type shape, not special-cased to a
Caxecraft name. Broader managed hierarchies, interfaces, generics, and inline
owned-class fields remain separate work and fail closed until their tracing and
lifetime rules are proven.

Run the focused contract and packaging evidence with:

```sh
npm run test:runtime-features
npm run test:array-runtime
```

The suite checks deterministic selection and packaging, malformed descriptors,
leaf and traced payloads, finalizer dispatch, strict C11 compilation under the
available GCC and Clang families, and exact absence from a runtime-free plan.
