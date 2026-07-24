# Closed anonymous-record lowering

E3.T01 adds one bounded aggregate representation to the production compiler:
closed anonymous Haxe records whose fields are direct `Bool`, `Int`, `UInt`, or
`Float` values, compile-time abstracts over those values, or other admitted
closed records. The same direct-record slice now includes `Null<Record>` when
the record has a complete direct lifecycle plan, including records that own
admitted Arrays, Bytes, optionals, or enums. A record may
also contain an already validated header-owned
by-value C struct, including a Haxe abstract over a transparent C typedef. This
includes a closed `enum abstract ... (Int)` without making the Haxe program
replace its meaningful type with a raw numeric tag. An ordinary Haxe enum whose
constructors carry no data may also be stored directly: it remains a nominal
enum in Haxe and becomes a native C enum field. A bounded payload enum is also
direct when every payload is an unmanaged value with a complete finite layout.
It becomes a tagged C struct containing a discriminant and a union, and that
complete value is stored in the enclosing record. These values lower through
typed HxcIR to private named C structs. Unmanaged records select no `hxrt`
feature. A record that owns an admitted Array or managed enum composes those
fields' narrow lifecycle operations and selects only their dependency-closed
runtime features. Both forms are available in `portable` and `metal`; neither
establishes a public C ABI.

## Structural identity and layout

`CBodyAggregateRegistry` expands typedefs and accepts only closed anonymous
types with known value fields. It canonicalizes fields by the UTF-8 bytes of
their semantic names, then hashes the complete recursive field shape. Two Haxe
aliases with the same structural shape therefore request the same HxcIR
instance and finalized C tag even if their source field declarations were
discovered in a different order. Nested definitions are emitted
dependency-first.

Canonical layout order does not change Haxe evaluation order. Object-literal
field expressions become ordered HxcIR instructions in source order; the later
`IRIOConstructAggregate` names each result by canonical field ID. C emission
uses a typed compound literal with designated members, so it never relies on C
initializer evaluation order or positional coincidence.

A non-core Haxe abstract contributes its proven underlying representation to
the record plan. For example, an `enum abstract MiningOutcome(Int)` field is
represented as one `Int` member in C. For a nominal immutable String view such
as `LogicalPath(String)`, the plan retains the nominal source identity beside
the shared `IRTString` carrier; generic specialization uses both facts in its
key before aggregate lowering selects the C field. The Haxe type still
restricts source values and supports named decisions, while C stores the
already-typed carrier without a wrapper object. Core abstracts and unsupported
underlying representations still fail closed.

An ordinary enum contributes its complete nominal instance key to the record
shape. Two enums can spell the same constructors and payloads and still be
different Haxe types, so the compiler must not merge records that contain
them. A fieldless enum becomes a native C enum. A finite payload enum whose
fields copy as plain values becomes a tagged struct and union. The generated C
defines either complete enum representation before the record that stores it
by value in split, package, and unity layouts.

A record may now own admitted managed fields. For example,
`All(children:Array<Predicate>)` owns an Array, while
`Not(child:Predicate)` owns a recursive enum tree. The aggregate plan generates
one record-level retain/destroy pair. It visits fields in canonical order,
rolls back already-retained fields if a later retain fails, and destroys fields
in reverse order. Enum fields delegate to a tag-aware helper, so only the active
union arm is inspected. Recursive enum links use unique tree ownership and
deep copy; cycle-capable graphs still require the future tracing collector.

An admitted closed record can also be the value in an ordinary
`Map<String, Record>`. The map stores the record directly rather than boxing it.
For an unmanaged record, its proven size and alignment are enough. For a record
that owns Arrays, Bytes, managed optionals, or other admitted direct children,
the StringMap plan generates a complete copy/assign/destroy callback trio from
this same aggregate lifecycle plan. Lookup returns an owning tagged optional;
replacement retains the new record before releasing the old one; removal and
final map cleanup destroy every live record exactly once. Collector-traced
graphs remain rejected before map storage can erase their lifetime needs. The
focused generated-Haxe and independent native evidence lives in
[`test/differential/string-map`](../test/differential/string-map/).

That same StringMap evidence covers three callback-free direct values: `Bool`,
Haxe `Int`, and a payload-free Haxe enum. The compiler keeps each value's exact
HxcIR type, then gives the runtime its proven C size and alignment. `Int`
becomes signed `int32_t`; a payload-free enum becomes its nominal native C enum.
They are not erased to one generic integer type. Tagged enums remain rejected
because copying their active union payload may require ownership or tracing
work that a byte copy cannot provide.

Haxe evaluates record fields from left to right, but one field may contain an
`if`, `switch`, or another expression that creates separate control-flow paths.
An HxcIR value belongs to the path where it was computed, so the compiler saves
an earlier completed field in a typed automatic local before entering that
later flow. It reloads the value after the paths join and only then constructs
the record. The generated C remains direct and allocation-free; the local is
the same ordinary stack-shaped temporary a C programmer would use to preserve
evaluation order. Imported `c.StructInit` values follow the same rule.

An imported field takes its identity from the existing typed C-import plan.
The compiler does not copy or guess the foreign layout: the authoritative
header still defines that struct, while the private generated record contains
one ordinary value of the imported type. This is useful for application-owned
resource bundles such as `{texture: Texture2D, ready: Bool}` without forcing
positional parameters or raw C into otherwise ordinary Haxe.

Each generated private struct is followed in the C implementation by
structural `_Static_assert` checks for its first offset, non-overlapping field
order, required field alignment, and final extent. The focused native probe
also has an independent C producer and C++17 consumer compare exact `sizeof`,
`alignof`, and every `offsetof` value at `-O0` and `-O2`. These checks prove the
private generated layout for the tested compiler invocation; they do not make
the shape an exported or stable ABI.

## Value, address, and projection semantics

Closed records use direct value representation. Function parameters, returns,
calls, local initialization, and local loads retain the record's HxcIR instance
type. For unmanaged records, the later C can use a structural assignment. For
managed records, HxcIR records explicit retain/release work: fresh values may
transfer ownership, borrowed copies retain, and lexical cleanup destroys the
owned value exactly once.

Field access keeps value and place operations distinct:

- projecting from a parameter or immutable temporary uses
  `IRIOProject(instance, field)`;
- reading a field of an addressable compiler-owned local uses
  `IRPField(local, field)` directly. The local is already stable storage, so
  taking `&local.field` and immediately dereferencing it would preserve no
  extra fact; and
- construction, projection, field places, address results, loads, calls, and
  returns are all type-checked by `HxcIRValidator`.

The direct field place is still an explicit HxcIR read. A later, separately
verified value-coalescing step may print that read at its single use only when
there is no intervening read, write, call, failure, or lifetime change. Reads
through imported pointers and other foreign or potentially volatile places
remain materialized.

This contract does not infer aliasing or identity from C syntax. Field mutation
on an aggregate value, record equality/identity, open or extended structures,
methods, `Void` fields, recursive by-value shapes, `Dynamic`, reflection, and
class-instance objects remain source-positioned `HXC1001` failures with no
output. Haxe is free to scalar-replace a record before the target boundary;
when an aggregate actually reaches the target, the rules above apply.

## Nullable value types: a presence flag, not a pointer

Haxe can express an optional value record naturally:

```haxe
typedef Point = {
	final x:Int;
	final y:Int;
}

static function maybePoint(found:Bool):Null<Point>
	return found ? {x: 3, y: 4} : null;
```

A C pointer would be a tempting translation, but it would introduce questions
that do not exist in this Haxe value: who allocated the record, who frees it,
and how long does the pointed-to storage live? Instead, haxe.c keeps the value
direct and allocation-free:

```c
struct hxc_optional_Point {
  bool hxc_has_value;
  struct hxc_Point hxc_value;
};
```

`hxc_has_value` answers whether the Haxe value is `null`. `hxc_value` contains
the record only when that flag is true. C zero-initializes the unused payload
of an absent compound literal, so the compiler never reads an uninitialized
record and does not need a generic runtime helper.

The semantic steps remain visible before C is chosen:

1. `null` becomes a tagged-null constant.
2. A concrete record entering `Null<Record>` becomes
   `IRCNullableInject`—in plain terms, “set the flag and store this record.”
3. `value == null` or `value != null` tests the presence flag directly.
4. A field read first emits `IRIONullCheck`, then
   `IRCNullableUnwrap`, and only then projects the field.

The validator requires that the null check run on every control-flow route to
the unwrap. This is called a *dominating check*: “dominating” simply means the
program cannot reach the read without passing the check first. The C emitter is
therefore formatting a proven operation, not guessing that `.hxc_value` is
safe.

The direct optional slice accepts primitive Bool/Int/UInt/Float values,
closed-record and closed-enum payloads, and literal-backed String views,
including nominal abstracts such as `ScenarioId(String)`. A `Null<Enum>` uses
the same presence-plus-payload shape.
Before a `switch` or payload pattern reads the enum, HxcIR checks the presence
flag and unwraps the enum value; it never asks generated C to inspect the
inactive payload of `null`.
Literal bytes live for the whole program, so copying that view needs no
retain/release work. A managed record keeps the same presence-plus-payload
shape, but its generated optional helper first checks `hxc_has_value` and only
then delegates to the record's retain/destroy helper. A managed enum follows
the same rule through its tag-aware retain/destroy helper, so only the active
case is visited. An absent optional never
touches its inactive payload bytes. A present constructor moves a fresh record
or enum owner or retains a borrowed value, and a copied optional retains only its
present payload. Runtime-created owned Strings and other payload families still
need their own admitted representation and fail closed until it exists.
`Null<Array<T>>` and nullable owned classes do not need a tagged optional
because their existing C pointer carrier already represents null exactly.
Optional-to-optional value equality is not implied by the presence
representation. Null tests are admitted; content equality belongs to the
payload's own lowering rule.

## Typed-C and ABI boundary

Anonymous-record lowering is independent from `@:c.layout`. The typed-C macro
still produces a declaration contract only; it does not turn a Haxe declaration
into a production public layout. In the `metal` profile, combining
`@:c.layout(c.Layout.Struct)` with `@:c.pack(...)` now fails with `HXC5002`
until target-ABI packing probes and a structural packed-declaration form exist.
Plain private anonymous records use ordinary strict C11 layout in both profiles.

Public exports must eventually use explicit named ABI structs and the E7
export/layout analysis. No generated record tag, member name, size, alignment,
or offset in this slice is stable across compiler versions.

## Evidence

Run:

```sh
npm run test:aggregate-lowering
npm run test:c-import
npm run test:hxc-ir
npm run test:typed-c
npm run snapshots:check
```

The focused suite renders twice, reverses typed-function discovery, compares
portable and metal, snapshots HxcIR/C/symbol/layout records, compiles and runs
the generated runtime-free split, package, and unity projects, and verifies a
bounded payload-enum record through construction, by-value copy, exhaustive
matching, and checked payload projection. The enum-lowering suite owns the
managed-record fixture because it exercises the enum and record lifecycle as
one feature: construction, copies, calls, returns, Array elements, rollback,
cleanup, cold/warm-server determinism, all layouts, and sanitizers. It also
covers exact absent/present managed optional behavior, Eval parity, strict
native C in unity/split/package layouts, normal/reversed and cold/warm-server
byte determinism, AddressSanitizer/UndefinedBehaviorSanitizer, and fail-closed
runtime-none policy. A strict C++17 consumer also includes the real generated
header and checks the presence/payload layout at `-O0` and `-O2`. The narrow command is
`python3 test/aggregate_lowering/run.py --managed-optional-only`. Identity,
mutation, `Void`, `Dynamic`, and metal packing
remain negative edges. The Haxe `--interp` invocation executes the same fixture
as the Eval oracle before the native projects run. Required CI jobs run the checked-in generated C and its C++17
layout companion separately under GCC/G++ and Clang/Clang++ at `-O0` and
`-O2`.
