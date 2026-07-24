# hxrt architecture and source guide

This document is the normative guide for deciding whether generated-program
semantics belong in direct C, a program-local specialization, or `hxrt`. It also
explains how to inspect a build's runtime plan and how every checked-in runtime
source is intended to be used.

The short rule is:

1. emit direct, idiomatic, structurally validated C when static facts make that
   correct;
2. emit a program-local, type/value-specialized helper when shared machinery is
   unnecessary;
3. select the narrowest dependency-closed `hxrt` feature only for semantics that
   genuinely need shared runtime-value, ownership, failure, platform, or ABI
   machinery;
4. fail with a source-positioned diagnostic when none of the admitted choices
   preserves Haxe semantics.

`hxrt` is therefore a selective semantic runtime, not a portability tax and not
an unconditional core. A runtime-free program contains no `hxrt` feature,
header, source, define, library, or symbol. This policy comes from
[ADR 0001](adr/0001-direct-c-and-selective-runtime.md); the exact graph and
packaging algorithm are described in
[runtime feature planning](runtime-feature-planning.md).

## Why hxrt exists

Haxe programs sometimes need behavior that C syntax alone cannot safely or
compactly express at each call site. Examples include ownership shared across
an ABI boundary, runtime-sized storage, fallible platform services, or a future
collector whose state must be coordinated across functions. A small shared C
slice can be the right implementation for those cases.

That does not make shared runtime code the default. Known representations,
constant work, bounded storage, static dispatch, devirtualization, lifetime
facts, and dead-feature elimination belong in the compiler. Moving those facts
into a generic helper would lose optimization opportunities, enlarge binaries,
and make `hxc_runtime=none` less useful. A repeated program-local helper may
eventually justify a feature, but only after measurements and semantic evidence
show that sharing is better than specialization.

The checked-in runtime is deliberately incomplete. Generated Haxe can select
the hosted literal-output closure plus bounded ordinary-Haxe Array,
`Map<Int, Bool>`, `Map<String, V>`, `haxe.io.Bytes`, and `String.charAt`
closures. Collections select allocator-backed storage transitively; StringMap and Bytes also select
the literal carrier used by their admitted String inputs. `String.charAt`
instead selects an allocation-free scalar-inspection slice. The compiler may
also select immutable object
descriptors and the precise collector when a managed-representation plan
explicitly needs them; ordinary direct classes remain header-free. Full strings, status-name, and
ABI-query slices remain native-seed-only. This is not a claim that general
allocation, arbitrary Map key/value families, `Array`, `Bytes`, escaping
classes, or general `String` lowering works.

## Selection pipeline

The runtime decision occurs after typed semantics and before project files are
written:

```text
TypedExpr reachable program
        |
        v
representation, escape/lifetime, specialization, and direct-C decisions
        |
        v
validated HxcIR with IRIStatic / IRIProgramLocal / IRIRuntime intent
        |
        v
RuntimeRequirementAnalyzer
  reconciles each reachable IRIRuntime(feature) with one typed source reason
        |
        v
root feature reasons + source spans
        |
        v
RuntimeFeaturePlanner
  checks purpose/profile/environment/policy and closes dependencies
        |
        v
hxc.runtime-plan.json
  roots, propagated reason IDs, edges, files, symbols, build facts, or proof
        |
        v
RuntimeFeaturePackager -> validated GeneratedFile values -> CProjectEmitter
```

The boundaries are intentional:

- Seeing a type or import during typed-AST collection selects nothing.
- `TypedExpr` lowering must first choose and record direct, program-local, or
  named runtime intent in HxcIR. It cannot infer a runtime from a C fragment.
- Every reachable runtime intent must match exactly one source-rooted candidate,
  and every candidate must still exist in reachable HxcIR. A mismatch is an
  internal compiler error, not permission to guess.
- The planner propagates each root reason through every dependency edge. A
  transitive feature never invents a second warning or appear without a reason.
- The packager rereads only the selected files, checks their registered hashes,
  converts them to normal `GeneratedFile` values, and lets Reflaxe's output
  ownership perform all writes and stale deletion.

HxcIR is useful here because runtime intent, failure edges, cleanup, ownership,
and evaluation order must survive before C syntax is chosen. It is not a
universal cross-target IR; see [the HxcIR design](hxc-ir.md) for that distinction.

## The feature catalog is the authority

[`runtime/hxrt/features.json`](../runtime/hxrt/features.json) is generated from
the typed `RuntimeFeatureCatalog`; it is not hand-edited. Schema 3 places each
feature's explanation beside its dependencies, artifacts, symbols, and source
hashes. Every feature record contains:

- a semantic contract and every admitted selection-root kind;
- direct-C and program-local alternatives that were considered;
- the reason a shared runtime slice is still justified;
- the exact dependency list, owned files, public runtime symbols, libraries,
  defines, environment set, and availability;
- a documentation reference and executable evidence paths.

The runtime-feature gate rejects a missing documentation record, missing
evidence file, stale documentation marker, unowned production header/source,
unregistered public `HXC_API` symbol, dependency drift, source-hash drift, and
catalog/schema drift. Update the typed catalog first, then regenerate with:

```sh
npm run snapshots:update -- --suite runtime-feature-graph
npm run test:runtime-features
```

Never edit `runtime/hxrt/features.json` or its hashes directly.

## Reading `hxc.runtime-plan.json`

Every successful generated program has a runtime plan, including a program that
uses no runtime. Read it in this order:

1. `requestedPolicy`, `resolvedPolicy`, and `policyProvenance` show which policy
   was requested and where it came from.
2. `status` is `analyzed-runtime-free` or `analyzed-runtime-features` for a
   compiler program. `analyzed-native-seed-features` belongs only to independent
   native fixtures.
3. `rootReasons` answers *why*. Each record names the feature, exact operation,
   semantic kind, typed surface, normalized source span, and any safe alternative.
4. `selectedFeatures` answers *what*. `root: true` identifies direct semantic
   requests; `root: false` identifies propagated dependencies. `reasonIds`
   links both back to `rootReasons`.
5. `dependencyEdges` shows why each transitive feature was added.
6. `artifactDetails`, `symbols`, `libraries`, and `defines` are the exact native
   packaging/build effects.
7. `directDecisions` and `programLocalHelpers` explain work intentionally kept
   outside `hxrt`.

For example, a literal `Sys.println` root selects `io`. The plan then shows
`status`, `string-literal`, and `runtime-base` as dependency-propagated features.
Removing a dependency from the link command would break the validated feature
contract; to remove it safely, eliminate or change the source root.

An empty plan is not an unexamined empty list. Its `noRuntimeProof` records the
reachable module/type/function/block/instruction/cleanup counts, zero surviving
runtime intents, direct decisions, program-local helper IDs, and explicit empty
feature/include/source/define/library/symbol sets. Project emission validates
that proof again before accepting the plan.

## Requesting and minimizing a runtime-free build

Use the normal production carrier and request the hard policy:

```sh
haxe -lib reflaxe.c -main Main \
  -D hxc_runtime=none \
  --custom-target c=build/c
```

`none` is a semantic constraint, not a linker switch. The compiler succeeds only
when whole-program analysis proves complete runtime absence. Otherwise HXC2000
lists every deduplicated blocker with its source span, operation, feature, every
root-to-leaf dependency chain, and a semantics-preserving alternative when one
is known. It never silently drops required behavior.

To reduce a nonempty plan safely:

1. Start with `rootReasons`, not the transitive feature list.
2. Locate the source span and read its `operationId`, `surface`, and `alternative`.
3. Ask whether representation, lifetime, value, dispatch, or capacity is now
   statically knowable. If so, implement or use the direct compiler lowering.
4. If the operation remains runtime-dependent but is closed over one type/value
   family, consider a bounded program-local helper.
5. Recompile and confirm that the root disappeared, its dependency-only features
   disappeared when no other root needs them, and native behavior still matches
   the Haxe oracle.

Do not delete packaged C, add raw `__c__`, hand-edit the plan, or use a manual
forbid as a substitute for semantics. Manual constraints can confirm inferred
work or reject it; they cannot invent a missing implementation or make required
runtime behavior disappear.

`auto` admits the exact implemented closure. `minimal` additionally requires
every selected feature to be on the reviewed narrow allowlist. The portable
profile defaults to `auto + summary`; metal defaults to `minimal + warn`.
Explicit policy remains orthogonal to the profile.

## Cross-cutting runtime boundaries

### Ownership, allocation, and lifetime

Runtime owners are private, move-only-by-convention C records. Their allocator
identity and context lifetime travel with their storage. Empty storage is
canonical, size arithmetic is checked, and fallible mutation publishes outputs
only after success. Borrowed string and array pointers never extend owner
lifetime and may be invalidated by mutation. These layouts are forbidden in
application exports; a future stable C API must define its own explicit handles,
allocators, nullability, and failure rules.

Direct stack values, fixed arrays, and nonescaping spans do not use `alloc`.
The compiler must complete escape and lifetime analysis before requesting any
allocation feature.

### Failure and cleanup

Current C runtime entry points return the closed `hxc_status` vocabulary when
they can fail. Outputs are unchanged on failure unless a function's documented
contract says otherwise. Destructors do not fail. Generated literal output
checks `hxc_io_println` and uses the compiler's admitted fail-stop policy.

Future exceptions cannot be hidden in an ordinary helper call. HxcIR preserves
failure and cleanup edges until a validated result/status or contained-unwinding
strategy is selected, as required by
[ADR 0006](adr/0006-explicit-failure-edges-and-contained-unwinding.md).

### ABI and versioning

The runtime ABI is internal and versioned, currently 0.9.0. Generated
runtime-using C emits a structural C11 `_Static_assert` for the required major.
Minor and patch changes within the same major are compatible by current policy;
a major mismatch fails native compilation. Runtime-free output contains no
runtime version macro or assertion.

This is not a stable application ABI. `hxc_runtime.h` and the feature headers
must not be installed or advertised as a supported user ABI, and runtime-owned
structs must not cross public application exports.

### Threads and collection

No checked-in feature provides threads, atomics, or thread-local error state.
The selected `gc` slice implements the precise non-moving single-thread
collector from [ADR 0005](adr/0005-precise-nonmoving-collector.md), including
explicit root chains and exact pins instead of arbitrary foreign/interior
pointer rooting. It can trace several registered execution-context chains, but
it supplies no synchronization; E5.T11 owns real thread coordination and the
stop-the-world handshake. Current records contain no hidden global mutable
runtime state.

### Environments and external dependencies

The generated `io` root is hosted-only. Its source has a defensive freestanding
branch that returns `HXC_STATUS_IO_ERROR`, but the planner rejects selecting it
for a freestanding compiler program. The native allocator seed supports a
caller-supplied allocator in freestanding evidence; its hosted default uses the
standard C allocator. Current feature records add no third-party library.
Compiler toolchains, the host C library, and test oracles are external evidence,
not packaged runtime artifacts.

## Registered feature contracts

The prose below orients readers; the generated feature catalog owns the exact
paths, hashes, dependencies, and symbol lists.

<!-- hxrt-feature:runtime-base -->
### `runtime-base`

Dependency-only header foundation for fixed-width types, ABI version macros,
visibility, and C/C++ alignment spelling. It has no function, allocation,
failure, lifetime, or thread behavior. Every selected runtime header depends on
it directly or transitively; runtime-free programs omit it.

<!-- hxrt-feature:runtime-abi -->
### `runtime-abi`

Native-seed-only query for the linked internal runtime version. Generated C uses
the compile-time major assertion instead, so this feature is evidence machinery
and never a generated semantic root.

<!-- hxrt-feature:status -->
### `status`

Dependency-only closed status definitions for fallible runtime C boundaries.
The header stores no last-error state, allocates nothing, and has no source file.

<!-- hxrt-feature:status-name -->
### `status-name`

Native-seed-only symbolic name lookup used by smoke diagnostics. Generated code
branches on typed statuses directly and does not select this convenience helper.

<!-- hxrt-feature:alloc -->
### `alloc`

Dependency-only checked allocator and move-only allocation-owner contract. It
supports hosted and caller-supplied allocator evidence, aligned storage,
failure-atomic resize, and checked size arithmetic. The bounded Array slice may
select it transitively; arbitrary generated allocation remains unsupported. See
[allocator ownership](allocator-abi.md).

<!-- hxrt-feature:array -->
### `array`

Compiler-selectable resizable unboxed array storage built on `alloc`. The first
ordinary-Haxe slice adds shared identity, local retain/release ownership,
literals, length, checked indexing, push, and source-order iteration for
acyclic direct elements. Generated Bytes elements and closed records containing
Bytes use typed program-local copy/assign/destroy callbacks; they remain
unboxed and do not select reflection or a collector. The native layer
additionally proves alias-safe insert/resize paths that generated Haxe does not
yet expose. Fixed arrays and spans stay direct and runtime-free. See
[array runtime](array-runtime.md).

<!-- hxrt-feature:int-map -->
### `int-map`

Compiler-selectable shared storage for the first ordinary-Haxe
`Map<Int, Bool>` specialization. Haxe presents that source type as
`haxe.ds.IntMap<Bool>` in the typed program. Haxe.c recognizes both identities
as one closed family before the generic `haxe.IMap` interface can erase the
exact key and value types.

The generated representation is a private `struct hxc_int_bool_map_ref *`.
Keys remain signed 32-bit Haxe `Int` values, Bool values remain native C
`bool`, and a separate occupied flag distinguishes “stored false” from
“missing key.” Assigning the map to a new local retains the same mutable table,
so changes through either alias are visible through the other. Construction,
`set(Int, Bool)`, and `exists(Int)` are the complete current method set.
`get`, removal, iteration, and other value types still stop with a
source-positioned IntMap diagnostic; the compiler does not guess nullable,
iterator, or ownership semantics for them.

The table uses open addressing: it hashes a key to a slot and checks later
slots after a collision. Capacity is always a power of two and the table keeps
an empty slot, which guarantees that lookup terminates. Growth allocates and
rehashes replacement storage before publishing it. If allocation fails, every
existing key and every alias still observes the old valid table.

This runtime slice is selected only for a mutable, run-time-sized map whose
shared identity is observable. A compiler-known immutable lookup can remain
direct constant C, while a proven small key range may later use a
program-local bitset or table. The independent C fixture forces allocation
failure at the private ABI; the ordinary-Haxe fixture separately compares Eval
with generated strict C, so the runtime and compiler are not merely checking
matching assumptions.

<!-- hxrt-feature:string-map -->
### `string-map`

Compiler-selectable shared storage for admitted ordinary-Haxe
`Map<String, V>` specializations. `V` is not erased: the compiler records the
exact key and value types in HxcIR and emits a typed
`struct hxc_string_map_ref *`. The runtime stores each value directly at its
proven `sizeof(V)` and `_Alignof(V)`; it is not a `Dynamic` map and does not box
every value behind a separately allocated pointer.

The currently generated value families are `Bool`, Haxe `Int`, payload-free
Haxe enums, and finite closed records. The first three have no owned children,
so the compiler uses the original size-and-alignment constructor and the
runtime copies their bytes directly. Their types are still exact: `Int` is the
validated signed `int32_t` mapping, and each fieldless Haxe enum remains its own
nominal native C enum rather than becoming a generic integer.

A record may contain other admitted direct values, including nested Arrays,
Bytes, tagged optionals, and finite enums, as long as none of them needs
collector tracing. If the record owns a reference-counted child, the compiler
generates one type-specific copy/assign/destroy callback trio and creates the
map with `hxc_string_map_ref_create_with_ops`. These callbacks retain a new
owner before publishing it, roll back earlier retains if a later retain fails,
and release owned fields in reverse order. The runtime knows only when to call
the policy; the program-local generated functions know the exact record type.

Keeping `hxc_string_map_ref_create(allocator, size, alignment, out_map)` is an
intentional compatibility decision. Previously generated trivial maps continue
to compile against the same-major runtime ABI. The additive
`create_with_ops` entry point is selected only when the stored value really
needs lifetime work.

Keys are compared by canonical UTF-8 contents and copied into table-owned
storage, so a temporary String view cannot leave a dangling pointer.
Initializing another local from a map retains the same mutable table; replacing
an already-owning local is not admitted yet. An explicit
`Null<Map<String, V>>` uses the same pointer carrier: `NULL` is absence, map
identity equality compares pointers, and retain/release treat `NULL` as a
successful no-op so ordinary cleanup needs no special branch. Operations that
need a table still reject `NULL`. `get` returns a tagged nullable value so an
absent key is distinct from every valid stored value, including `false`. A
present managed record result owns its copied nested values until the generated
optional cleanup releases them. Empty keys are valid String values and are
stored without inventing a sentinel key.

Growth and insertion are checked and failure-atomic: an allocation or value-copy
failure does not publish a partial entry, and a failed replacement preserves the
old value. Rehashing relocates the table's existing bytes without logically
copying or destroying their owners; it is the same ownership move a
handwritten C table performs when replacing its slot block.

Tagged payload enums remain unsupported as top-level map values because their
active union member needs a typed ownership policy; Float and unrelated
reference families remain outside this intentionally bounded specialization.

The Haxe fixture proves language semantics through generated C. The separate
handwritten-C native fixture injects allocator and callback failures directly,
so code generation and hxrt cannot accidentally validate the same bug
together. Other key/value specializations, iteration, collector-traced values,
and owner-replacing map assignment remain explicitly unsupported until they
receive complete typed lifetime contracts.

<!-- hxrt-feature:bytes -->
### `bytes`

Compiler-selectable fixed-length binary storage for ordinary
`haxe.io.Bytes`. It preserves shared mutable identity, checks every position
and range, and releases storage after the final owner. `sub` creates an
independent copy, while overlapping `blit` behaves as if the source bytes were
saved before the destination changes. The slice depends on `alloc` for
run-time-sized ownership and on `string-literal` only for admitted
`Bytes.ofString` literals. See [Bytes runtime](bytes-runtime.md).

<!-- hxrt-feature:string-literal -->
### `string-literal`

Dependency-only, allocation-free carrier for compiler-owned valid UTF-8 literal
bytes, byte length, and trailing-NUL fact. Embedded NUL remains ordinary String
content. It is intentionally separate from the full string feature.

<!-- hxrt-feature:string-scalar -->
### `string-scalar`

Compiler-selectable, allocation-free inspection of immutable valid-UTF-8
String views. The first ordinary-Haxe root is `String.charAt(index)`. It counts
Unicode scalar values rather than UTF-8 bytes, returns a borrowed one-scalar
view when the index exists, and returns the empty String for a negative or
out-of-range index. “Borrowed” means the result points into the receiver's
immutable bytes and therefore shares their lifetime; it owns nothing to free.

The slice also owns checked validation, scalar length/access, borrowed slicing,
comparison, and hashing used by the broader native String seed. A private
header-only decoder keeps those operations on one UTF-8 implementation without
creating another link-time feature. Selecting `charAt` packages
`string_scalar.c`, `string_scalar.h`, `string_decode.h`, `string_literal.h`,
`status.h`, and `base.h`; it packages neither `allocator.c` nor `string.c`.
Other ordinary Haxe String methods still fail at the String lowering boundary.
See [string runtime](string-runtime.md) and
[ADR 0004](adr/0004-utf8-scalar-string-contract.md).

<!-- hxrt-feature:string -->
### `string`

Native-seed-only owned UTF-8 construction, builders, lossy decoding, and
explicit CString conversion above `string-scalar`. Literal emission and
ordinary `charAt` do not select it. See [string runtime](string-runtime.md) and
[ADR 0004](adr/0004-utf8-scalar-string-contract.md).

<!-- hxrt-feature:io -->
### `io`

One current generated-Haxe semantic root family. Reachable
`sys-println-literal` and `trace-literal` HxcIR operations select this hosted
exact-length write-and-flush service. It depends on `status` and
`string-literal`, exposes only `hxc_io_println`, and pulls in neither allocation
nor full strings.

<!-- hxrt-feature:object -->
### `object`

Compiler-selectable, collector-neutral payload descriptors. A descriptor
records exact `sizeof`/alignment expressions plus optional typed trace and
finalizer callbacks; a small validated runtime-owned header points to it.
Direct and nonescaping class storage does not select this feature, and the
slice contains no allocator, mark bit, root scanner, reflection table, or
collector. See [object and type descriptors](object-descriptors.md).

<!-- hxrt-feature:gc -->
### `gc`

Compiler-selectable precise, non-moving mark-and-sweep collection. It depends
on `alloc` and `object`, follows only exact descriptor fields and registered
stack/global/execution-context roots, provides explicit foreign pins, and
reports allocation/collection/pause counters. It does not scan arbitrary C
memory or add headers to direct classes. See [precise non-moving
collection](gc-runtime.md).

## Checked-in source classification

### Feature-owned production slices

Files below `runtime/hxrt/include/hxrt/` and `runtime/hxrt/src/` are the exact
feature-owned source inventory. They are production-quality C for their bounded
contracts, but availability still controls whether generated Haxe may select
them.

| File | Purpose and selection |
| --- | --- |
| `include/hxrt/base.h` | Dependency-only shared C types, visibility, alignment, and internal ABI macros. |
| `include/hxrt/abi.h`, `src/abi.c` | Native-seed ABI version query. No ownership or failure state. |
| `include/hxrt/status.h` | Dependency-only status vocabulary. No source or mutable state. |
| `include/hxrt/status_name.h`, `src/status.c` | Native-seed status-name evidence helper. |
| `include/hxrt/allocator.h`, `src/allocator.c` | Dependency-only allocator callbacks, owner lifecycle, checked arithmetic, and aligned hosted implementation; selected transitively by managed collections. |
| `include/hxrt/array.h`, `src/array.c` | Compiler-selectable resizable typed storage, shared Array identity, and element lifecycle. |
| `include/hxrt/int_map.h`, `src/int_map.c` | Compiler-selectable Int-keyed shared `Map<Int, Bool>` storage with exact unboxed keys, values, and membership. |
| `include/hxrt/string_map.h`, `src/string_map.c` | Compiler-selectable String-keyed shared map storage with copied keys and exact unboxed values. |
| `include/hxrt/bytes.h`, `src/bytes.c` | Compiler-selectable fixed-length mutable byte storage, shared identity, checked ranges, and overlap-safe copying. |
| `include/hxrt/gc.h`, `src/gc.c` | Compiler-selectable precise non-moving collector, exact roots/pins, pressure policy, and observable reports. |
| `include/hxrt/object.h`, `src/object.c` | Compiler-selectable immutable managed-payload descriptors and exact trace/finalizer dispatch; no allocation or collection. |
| `include/hxrt/string_literal.h` | Compiler-selectable dependency-only direct literal layout. |
| `include/hxrt/string_scalar.h`, `include/hxrt/string_decode.h`, `src/string_scalar.c` | Compiler-selectable allocation-free UTF-8 scalar inspection, including ordinary Haxe `String.charAt`; the decoder header is an internal implementation shared with the broader String seed. |
| `include/hxrt/string.h`, `src/string.c` | Native-seed owned UTF-8 construction, builders, lossy decoding, and CString machinery. |
| `include/hxrt/io.h`, `src/io.c` | Hosted compiler-selectable literal output and explicit status. |

Each file begins with a local source contract naming its feature, selection
mode, callers, dependencies, and important ownership/failure/thread/ABI effects.
Inline comments explain implementation choices that are easy to accidentally
make undefined, non-atomic, alias-unsafe, or semantically broader.

### Provisional umbrella

`runtime/hxrt/include/hxc_runtime.h` is a native-smoke umbrella over all seed
headers plus legacy inline Int32 helpers. It is intentionally absent from the
feature artifact graph, is never copied by selective compiler packaging, and is
not a stable public header. New generated code must include exact feature
headers or use direct/program-local compiler output; it must not select this
umbrella as a hidden core.

### Test fixtures

Everything under `runtime/hxrt/test/` is independent native input, never
generated C and never a packaged runtime source. In particular,
`public_header_cpp.cpp` is C++ because it proves that the provisional C header
surface is consumable under C++17 and that `extern "C"`, layout, and alignment
contracts survive a C++ compiler. It does not mean the compiler emits C++ or
that a stable public C++ API exists.

The differential and feature-graph files under `test/` are executable evidence,
not runtime implementation. They may request `native-seed-fixture` planning for
unconnected slices or compile ordinary Haxe through the production planner for
the explicitly compiler-selectable closures.

### Generated and external files

When a plan is nonempty, copies appear under the generated project's
`runtime/include/` and `runtime/src/` paths. Those copies are normal Reflaxe-
owned generated artifacts and must not be edited. The source of truth remains
the checked-in feature-owned file whose digest is recorded in the catalog.

System headers, libc implementations, GCC/Clang, Haxe or Eval oracles, Raylib,
and future platform SDKs are external dependencies or test authorities. They do
not become `hxrt` features merely because a fixture links them.

## Adding or changing a runtime feature

A runtime change is complete only when all of the following move together:

- the semantic gap is shown to resist direct C and reasonable bounded
  program-local specialization;
- HxcIR carries a named runtime intent and explicit failure/cleanup effects;
- the typed catalog defines stable roots, dependencies, exact files, symbols,
  environments, policy eligibility, rationale, documentation, and evidence;
- every new header/source has a file-level source contract and comments for
  non-obvious lifetime, aliasing, overflow, failure-atomicity, or platform code;
- the source-positioned requirement analyzer and plan snapshot prove root and
  dependency provenance;
- native and differential tests cover semantics, warnings, failure paths,
  allocations, code size, and performance where the feature is hot;
- runtime-free neighboring cases still prove complete `hxrt` absence;
- the manifest/schema snapshots, focused documents, capability claims, and ABI
  version are reviewed intentionally.

Reserved IDs are not empty implementations. Until their owner task supplies all
of this evidence, requesting them must continue to fail closed.
