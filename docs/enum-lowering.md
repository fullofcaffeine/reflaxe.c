# Haxe enum lowering

E3.T02 adds a bounded production path for Haxe algebraic enums. Concrete enum
values lower from pinned-Haxe `TypedExpr` through schema-17 HxcIR and the HxcIR
validator before structural strict C11 is selected. The emitted definitions are
private compiler implementation details in both `portable` and `metal`; this
work does not establish a public C ABI or support broader generic classes and
references.

## Representation

An enum whose constructors all have no payload uses a native C enum when the
whole concrete declaration is available. Source constructor order determines
explicit integer discriminants. This is a value representation optimization,
not an exported ABI promise.

A closed anonymous record may store that native enum directly. The record's
structural key includes the enum's nominal identity, not merely its constructor
spelling, and C project planning emits the complete enum definition before the
record in split, package, and unity layouts. Payload enums may also be embedded
in closed records. If the active payload owns managed data, the record composes
the enum's lifecycle helper rather than pretending a C struct assignment is a
complete copy.

Ordinary `==` and `!=` between two values of the same fieldless enum compare
those discriminants directly. HxcIR names this bounded operation as enum-tag
equality, validates that both operands are the same payload-free direct enum,
and emits one C `==` or `!=`. Operand evaluation still follows Haxe's
left-to-right order. Payload-enum equality remains fail-closed because comparing
the generated outer C structs would be invalid C and would not define equality
for their nested values.

An enum with any payload uses four structural layers:

- one C enum for readable discriminants;
- one C struct for each constructor that carries payload;
- one union containing those constructor payload structs; and
- one outer struct containing the discriminant and payload union.

Every tag, member, payload field, function, local, and temporary is requested
through the shared `CSymbolRegistry`. Constructor operands are evaluated into
ordered HxcIR values before a named `IRIOConstructTag`; C then uses designated
initializers, so it does not depend on C initializer evaluation order.

The focused slice accepts payloads composed of admitted primitive values,
literal-backed String views, other admitted enum instances, private managed
Array references, managed private class references, and closed records whose
admitted fields have a complete lifecycle or tracing plan. This applies both to
a concrete constructor such as
`WrappedRule(rule:Rule)` and to a closed generic instance such as
`Option<Rule>`: the record remains an unboxed C struct inside the active union
member. `Option<Int>` and `Option<Bool>` therefore receive distinct
deterministic concrete layouts and symbols. E3.T03 now gives
these closed instances the same normalized argument vocabulary, full-key digest
collision check, count budget, source-reason ledger, and specialization report
used by generic static functions. Finite nesting such as
`Option<Option<Int>>` produces dependency-ordered distinct layouts, while
non-stationary type growth stops at the hard depth/type budgets. The layout
remains private and program-local;
see [deterministic generic specialization](generic-specialization.md).

## Managed payload ownership

A private tagged enum may carry an admitted `Array<T>` or closed record with
managed fields. Construction either moves a fresh owner into the active payload
or retains an existing alias.
Copying the enum calls one generated retain helper; cleanup calls the matching
destroy helper. Both helpers switch on the tag before touching the union, so a
constructor without managed data performs no ownership operation and inactive
union bytes are never interpreted as a live value. For a managed record
payload, the active enum arm delegates to the record's existing retain/destroy
helpers. The enum does not duplicate or reinterpret the record's field policy.

The same helper is used when an enum is stored inside an Array element. This
keeps the value unboxed and supports nested shapes such as Caxecraft's
`FlowAction.Schedule(Array<FlowArgument>)` and
`ChooseSeeded(Array<{ actions:Array<FlowAction> }>)` through the existing typed
Array callbacks. Function parameters borrow enum values. A fresh constructor
returned from a function transfers its owner to the caller; a named owning
local can transfer its cleanup on return. Pattern-projected Array values borrow
from the enum for the duration of the arm.

Array-backed paths select the existing `array` runtime slice. Recursive enum
links also select the allocator slice described below. These helpers do not
establish a public enum ABI, general garbage collection, or arbitrary cyclic
collection support.

### Managed class references are traced, not retained

A class reference inside an enum follows the tracing collector's rules rather
than the retain/destroy rules above. Copying the enum copies one stable pointer;
it does not copy the class object and does not increment a reference count.
HxcIR records a typed path from each immutable enum, record, or optional value
to every embedded collector pointer. Generated function root frames publish
that pointer only while the matching enum tag and optional-presence guards are
true, so inactive union storage is never read.

An `Array<EnumWithClass>` remains a shared Array, but its object descriptor
walks each live element, switches on the tag, and visits the active class
pointer. The same finite traversal composes through closed records and tagged
optionals. This keeps the generated C representation unboxed and readable while
still making collection exact. A mixed value may both trace a class pointer and
run typed copy/destroy callbacks for an independently reference-counted Array or
Bytes field; tracing and ownership are deliberately separate plans.

Recursive enums that also reach collector references remain fail-closed. Their
owned child chain can be arbitrarily deep, so a finite list of stack projection
paths would be unsound. Supporting that combination needs one explicit
recursive trace owner rather than pretending the first few links are enough.

## Matching and checked projection

Haxe enum switches lower to `IRTTagSwitch`, whose ordered cases name Haxe
constructors and typed block edges. `HxcIRValidator` requires each constructor
exactly once for an exhaustive switch and rejects an exhaustive switch with a
default edge. C emission uses a structural `switch` over the finalized
discriminant member, with one case per HxcIR edge; no arm can fall through into
another.

`TEnumParameter` lowers to `IRIOProjectTag`. The instruction records the
expected constructor, payload index, exact result type, and resolved
profile/build policy. The current admitted path emits a tag comparison before
every payload union read and calls the registered C `abort` symbol on mismatch.
This preserves the required safe/debug behavior and does not select `hxrt`.
The checked policy remains explicit in HxcIR for later optimization only when a
future proof can demonstrate that removing a redundant check preserves
semantics.

Haxe itself rejects a non-exhaustive source pattern before target lowering. The
HxcIR negative corpus independently constructs a non-exhaustive tag switch to
prove that malformed internal IR is rejected as `HXC9000` rather than silently
gaining a C default.

## Recursive values and their ownership boundary

The enum registry analyzes concrete instance dependencies before finalizing
storage. A recursive payload edge becomes an explicit non-null pointer to one
allocator-backed child, making the C struct finite. The pointer is not shared:
each parent uniquely owns its child. Copying a recursive enum deep-copies that
tree; destroying it walks the active tag, destroys nested managed payloads,
then frees the child. Calls borrow parameters, while returns and fresh
construction transfer or retain ownership explicitly.

This rule supports finite acyclic trees. A value graph with back-edges or shared
recursive nodes would need cycle-aware identity, so it still fails closed until
the tracing collector owns that representation. Non-stationary recursive
generic arguments also fail closed under the existing specialization budgets.

## Runtime, project, and ABI effects

Bounded enum values add the direct compiler decision
`bounded-haxe-enum-values` to the runtime plan. Unmanaged enum programs remain
runtime-free. A recursive owned enum selects `alloc`; an Array payload selects
`array` and its dependency-closed allocator/status/base features. The runtime
plan records each root operation and source reason, so the compiler never adds
a broad unconditional runtime core. Portable and metal preserve the same
private enum representation.

A reachable generic enum also adds `closed-generic-specializations` and is
listed in `hxc.specializations.json` with its full key, normalized arguments,
merged source reasons, recursion state, and conservative structural/code-size
cost. Non-generic enum programs do not emit that sidecar.

The private implementation follows every enum definition with structural
`_Static_assert` checks for tag values, member offsets, alignment, non-overlap,
and final extent. Independent C and C++17 consumers also compare construction,
copy behavior, size, alignment, offsets, and recursive pointer layout. These
checks validate the generated private representation for this compiler
invocation; public exported enums still require the E7 ABI contract for layout,
ownership, calling convention, and versioning.

Owned/dynamic String payloads, payload-enum equality, reflection, arbitrary
patterns, recursive enums containing collector references, cycle-capable
recursive graphs, and public export remain unsupported.
The first unsupported typed node emits source-positioned `HXC1001` and leaves
no plausible output.

## Evidence

Run:

```sh
npm run test:enum-lowering
npm run test:generic-specialization
npm run test:hxc-ir
npm run test:all-sources
npm run snapshots:check
```

The focused runner renders twice, reverses typed-function discovery, compares
portable and metal, and snapshots HxcIR, the private header/source, concrete
layout records, and finalized symbols. Its managed fixture compiles normal and
reversed discovery plus cold, first-server, and warm-server requests; compares
their unity output byte for byte; emits split and package layouts; and executes
all three layouts. That fixture covers direct and generic managed-record enum
payloads, copying, pattern projection, function return, and Array element
callbacks. A second managed-class fixture covers construction, copy,
assignment, parameters and returns, nullable class payloads, checked
projection, records and Arrays containing the enum, pressure-triggered tracing,
and class finalization. It also runs the generated managed programs with
AddressSanitizer and UndefinedBehaviorSanitizer. The required native matrix
uses GCC/G++ and Clang/Clang++ at `-O0` and `-O2`, while negative fixtures cover
unsupported references, source non-exhaustiveness, recursive collector payloads,
and validator-only malformed tag/root operations.
