# Haxe enum lowering

E3.T02 adds a bounded production path for Haxe algebraic enums. Concrete enum
values lower from pinned-Haxe `TypedExpr` through schema-4 HxcIR and the HxcIR
validator before structural strict C11 is selected. The emitted definitions are
private compiler implementation details in both `portable` and `metal`; this
work does not establish a public C ABI or support broader generic classes and
references.

## Representation

An enum whose constructors all have no payload uses a native C enum when the
whole concrete declaration is available. Source constructor order determines
explicit integer discriminants. This is a value representation optimization,
not an exported ABI promise.

An enum with any payload uses four structural layers:

- one C enum for readable discriminants;
- one C struct for each constructor that carries payload;
- one union containing those constructor payload structs; and
- one outer struct containing the discriminant and payload union.

Every tag, member, payload field, function, local, and temporary is requested
through the shared `CSymbolRegistry`. Constructor operands are evaluated into
ordered HxcIR values before a named `IRIOConstructTag`; C then uses designated
initializers, so it does not depend on C initializer evaluation order.

The focused slice accepts payloads composed of admitted primitive values and
other admitted enum instances. `Option<Int>` and `Option<Bool>` therefore
receive distinct deterministic concrete layouts and symbols. E3.T03 now gives
these closed instances the same normalized argument vocabulary, full-key digest
collision check, count budget, source-reason ledger, and specialization report
used by generic static functions. Finite nesting such as
`Option<Option<Int>>` produces dependency-ordered distinct layouts, while
non-stationary type growth stops at the hard depth/type budgets. The layout
remains private and program-local;
see [deterministic generic specialization](generic-specialization.md).

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

## Recursive values and lifetime boundary

The enum registry analyzes concrete instance dependencies before finalizing
storage. A recursive payload edge becomes an explicit non-null pointer to the
same concrete instance, making the C struct finite. Construction first copies
the recursive child into a stable automatic local and then stores its address;
projection loads through the corresponding typed dereference place.

That backing storage is valid only within the constructing function. Recursive
enum parameters and returns therefore fail with source-positioned `HXC1001`
until escape, ownership, allocation, and lifetime analysis can choose a sound
representation. Non-stationary recursive generic arguments also fail closed.
The compiler never hides recursion behind an unconditional runtime allocation.

## Runtime, project, and ABI effects

Bounded enum values add the direct compiler decision
`bounded-haxe-enum-values` to the runtime plan. A successful project records all
concrete instances in the runtime-free reachability proof and contains no
`hxrt` header, source, define, library, or symbol. Portable, metal, automatic,
minimal, and explicit `hxc_runtime=none` policy resolution do not change the
generated enum C.

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

Reference payloads such as `String`, aggregate payloads, enum identity/equality,
reflection, arbitrary patterns, escaping recursive values, and public export
remain unsupported. The first unsupported typed node emits source-positioned
`HXC1001` and leaves no plausible output.

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
layout records, and finalized symbols. It compiles and runs generated projects
under automatic, explicit runtime-none, and metal policies. The required native
matrix uses GCC/G++ and Clang/Clang++ at `-O0` and `-O2`, while negative
fixtures cover recursive escape, reference and aggregate payloads, source
non-exhaustiveness, and validator-only malformed tag operations.
