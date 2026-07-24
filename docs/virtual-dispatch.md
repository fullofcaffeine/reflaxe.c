# Closed-world virtual dispatch

E3.T06 admits ordinary instance methods and reachable class-hierarchy virtual
calls for the bounded concrete-class program already supported by E3.T04 and
E3.T05. The compiler discovers calls from pinned-Haxe typed expressions,
records the choice in schema-19 HxcIR, and emits program-local strict C11. The
slice works in `portable` and `metal`, selects no `hxrt` feature, and exposes no
public C ABI.

This is closed-world dispatch, not an unconditional object model. A class that
does not participate in a reachable polymorphic call keeps the header-free
layout from E3.T04. A final class called through its exact static type therefore
needs neither a virtual-table layout nor a table.

## Reachability and direct calls

`CBodyDispatchCatalog` visits only instance calls and constructed concrete
classes reached by the static-function graph. It preserves a normal direct C
call when the typed program already proves the target:

- `super` calls;
- private or `final` methods;
- inline methods;
- methods marked `@:nonVirtual`;
- calls whose receiver has a final class type; and
- methods declared by a final class.

Every other admitted ordinary overridable class method receives a hierarchy
slot. Slot IDs are rooted at the first declaration of the overridden method,
and layouts, slots, tables, and source call records use deterministic UTF-8
ordering. Reversing typed-module discovery, changing locale, changing the
resolved profile, or reusing a compiler server cannot change the selected
graph or emitted identifiers.

## Minimal layout and C emission

Schema-7 HxcIR adds an explicit program dispatch plan: hierarchy layouts,
representation-typed slots, concrete-class tables, and their implementation
entries. `IRCDVirtual` names the slot and receiver, while
`IRIOBindVirtualTable` records the table installed in newly constructed
storage. The validator proves that:

- one layout is attached to exactly one hierarchy root;
- layout slots and table entries have exact deterministic order;
- each table class descends from its layout root;
- every applicable slot has a reachable implementation with the exact slot
  representation; and
- each bind targets storage of the table's concrete class.

The C representation puts one pointer to the selected table layout at offset
zero of the hierarchy root. Derived classes continue to embed their complete
base object first, so they inherit that one pointer instead of receiving a
second header. Only reachable virtual slots appear in the table struct, and a
table is emitted only for a reachable constructed dynamic class that needs the
layout. Construction binds the table before user constructor code runs.

Instance methods are ordinary private C functions with an explicit typed
`self` parameter. A virtual slot uses the root method's receiver type. When an
override has a more-derived receiver, the compiler emits one typed program-
local adapter that converts the root pointer to the known complete object and
calls the implementation. Direct calls do not pass through these adapters.
Structural layout assertions cover the selected root header along with the
existing base and field layout checks.

## Bounded interface values

The current E3.T07 slice also admits a closed, non-generic Haxe interface when
the typed program already proves the implementing class. The generated
interface value is a compact pair:

- `object` is a pointer to the concrete instance; and
- `table` is a pointer to the program-local method table for that exact
  interface.

This is often called a *fat pointer*: “fat” only means the value carries a
second pointer with the dispatch information that a plain object pointer lacks.
It does not allocate and it does not add a general runtime object header.
HxcIR's `IRIOConstructInterface` builds the pair, while `IRCDInterface` names
the exact interface, slot, and receiver used by a call. The validator checks
that the interface instance, table, slot, object implementation, and function
signature agree before C syntax is selected.

A constructor may accept this interface pair by value when its typed body proves
that the pair remains a short-lived borrow for that call. HxcIR records
`ownership=borrowed-interface`; storing, returning, or forwarding it without
another checked borrow contract is rejected. See
[bounded constructor lowering](constructor-lowering.md#c-function-and-elision-model)
for the lifetime reason and generated-C example.

A constructor may also retain the pair in its own interface field. This is a
different ownership plan, not a longer borrow. The owner and every reachable
concrete implementation for that interface become collector-managed; the
owner's generated trace callback visits `field.object`, while `field.table`
continues to point at immutable program-local data. Interface dispatch accepts
both direct call-only implementations and `managed(gc)` retained
implementations because the method table and thunk signatures are identical.
The first implementation is conservative: once an interface field is retained,
all reachable concrete tables for that interface select managed storage.
Finer per-construction escape analysis can reduce allocations later without
changing the interface value or tracing contract.

## Override representation boundary

The pinned Haxe compiler may admit source-level covariant returns or
contravariant arguments. Those source relationships do not make incompatible C
function-pointer types interchangeable. Before C naming or emission, the
dispatch preparer compares every selected override argument and return using
the target's full semantic representation key. A mismatch reports
source-positioned `HXC1001` with the slot, implementation, position, expected
representation, and actual representation, then leaves no output tree.

This admitted slice also requires non-generic, non-overloaded, infallible
normal methods. It does not erase an unsupported signature into `void *`, a
varargs function, boxing, or a runtime descriptor.

## Explanatory report

When at least one instance call is reachable, production output includes
`hxc.dispatch.json`. The schema-1
`hxc-closed-world-instance-dispatch-v2` report is validated by
`docs/specs/dispatch-report.schema.json` and content-addressed in
`hxc.manifest.json` as `dispatch-report`. It records:

- every reachable call, its normalized source span, direct/virtual choice, and
  reason;
- finalized layout tags, slot members, and slot representations;
- selected concrete tables and implementation IDs;
- generated adapter names; and
- exact call/layout/slot/table/adapter counts plus an empty runtime-feature
  set.

The sidecar is conditional: a program with no reachable instance calls emits
no dispatch report and does not reserve an object header. The report explains
compiler decisions; it is not a stable public ABI map.

## Remaining boundaries

E3.T07 remains in progress: proven class-to-interface construction, inherited
interface slots, multiple interface views, interface calls, and call-bounded
constructor parameters are implemented; runtime-checked casts, dynamic type
tests, and general retained interface ownership remain fail-closed. Function
values and closures remain E3.T08. Generic classes or methods, dynamic instance
methods, escaping/heap objects, reflection, general exception-bearing virtual
methods, public class layouts, and stable exported method ABI remain
fail-closed. E3.T09 owns later devirtualization and escape analysis; E4 owns
general allocation, ownership, tracing, and descriptor policy.

## Evidence

Run:

```sh
npm run test:virtual-dispatch
npm run test:constructor-lowering
npm run test:class-layout
npm run test:hxc-ir
npm run test:typed-boundaries
npm run test:all-sources
npm run snapshots:check
```

The focused fixture proves three indirect calls across a three-class override
chain—including normalization of a statically derived receiver to its root
slot—while six final/private/metadata/`super` calls remain direct. It proves
that an unrelated final class receives no table, snapshots HxcIR, the report,
the generated header/source, and finalized symbols, and compares cold,
reversed-input, locale, profile, explicit-runtime-none, and warm-server builds.
Negative fixtures cover argument and return representation variance with exact
no-output diagnostics. Required native lanes compile and run the generated C
under identity-verified GCC and Clang at `-O0` and `-O2` and compile the private
header as C++17. The `.cpp` file is only an independent header consumer; it is
not generated output or C++ target evidence.
