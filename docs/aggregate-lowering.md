# Closed anonymous-record lowering

E3.T01 adds one bounded aggregate representation to the production compiler:
closed anonymous Haxe records whose fields are direct `Bool`, `Int`, `UInt`, or
`Float` values, or other admitted closed records. These values lower through
typed HxcIR to private named C structs. The slice is available in both
`portable` and `metal`, selects no `hxrt` feature, and does not establish a
public C ABI.

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
type, making every by-value copy explicit before structural C assignment.

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
npm run test:hxc-ir
npm run test:typed-c
npm run snapshots:check
```

The focused suite renders twice, reverses typed-function discovery, compares
portable and metal, snapshots HxcIR/C/symbol/layout records, compiles and runs
the generated runtime-free project, and verifies exact fail-closed identity,
mutation, `Void`, `Dynamic`, and metal packing edges. Required CI jobs run the
checked-in generated C and its C++17 layout companion separately under GCC/G++
and Clang/Clang++ at `-O0` and `-O2`.
