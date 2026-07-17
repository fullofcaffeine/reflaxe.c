# Runtime feature planning and selective packaging

The runtime planner implements the compiler-first policy from
[ADR 0001](adr/0001-direct-c-and-selective-runtime.md): preserve semantics with
direct idiomatic C first, then use a program-local specialization, and request
the narrowest dependency-closed `hxrt` slice only when neither compiler-owned
option is feasible. There is no unconditional `core` feature.

This is a bounded M0 capability. The graph, reachability-based requirement
analysis, policy checks, no-runtime eligibility proof, reason propagation, and
exact packager are implemented for the admitted generated-program slice.
Primitive and fixed-array/span programs remain runtime-free. E2.T07
additionally admits compiler-known String literals passed to hosted
`Sys.println` or default `trace`; that edge selects only the literal carrier and
minimal output closure. The allocator, full string-operation, and resizable-
array implementations remain `native-seed-only`. E2.T10 composes exactly that edge in
`examples/hello`; its plan propagates the one `Main.hx` reason through every
selected transitive feature and adds no broader slice.

## Typed graph contract

`RuntimeFeatureCatalog` supplies a fresh `RuntimeFeatureRegistry` for each
compilation. Every definition has a validated stable ID, availability,
environment set, minimal-policy eligibility, dependencies, owned headers and C
sources with reviewed SHA-256 digests, symbols, libraries, and defines. The
registry rejects duplicate IDs, artifacts, or symbols; unavailable dependencies;
invalid paths or hashes; and dependency cycles before planning. Canonical UTF-8
ordering makes the catalog independent of discovery and map iteration order.

The machine-readable review view is
[`runtime/hxrt/features.json`](../runtime/hxrt/features.json), validated against
[`runtime-features.schema.json`](specs/runtime-features.schema.json). Its current
graph is:

```text
runtime-base
├── status
├── string-literal
└── runtime-abi                   (native seed only)

io -> status + string-literal    (compiler selectable, hosted only)
alloc -> status                  (native seed only)
array -> alloc                   (native seed only)
string -> alloc + string-literal (native seed only)
status-name -> status            (native seed only)
```

These components are split into independently owned `hxrt/*.h` and `.c` files.
The header-only `string-literal` slice owns only the private byte pointer, byte
length, and trailing-NUL fact. It does not pull allocator or general string
symbols into a literal-output program.
The graph also reserves separate IDs for objects, tracing collection, dynamic
values, reflection, exceptions, threads, platform services, and other planned
features. Reservations fail closed and name the task that must implement them;
they are not empty features and cannot be selected.

The catalog is the machine-diffable internal schema-2
`hxc-runtime-feature-graph-v2` contract. It records internal ABI version 0.5.0,
same-major generated-code compatibility, the exact application-export exclusion
for runtime-owned types, every artifact digest, one digest over the sorted source
set, and the strict C11/C++17 header build baselines. The runtime plan remains
internal schema 2 (`hxc-runtime-plan-v2`), with a nested schema-1
`hxc-no-runtime-eligibility-v1` proof for empty plans.

Every nonempty closure includes `runtime-base`, so every selected public runtime
header sees `HXC_RUNTIME_ABI_MAJOR`. The generated private program header emits
one structural `_Static_assert` against the compiler's required major. Equal
major versions—including a different minor or patch—compile and link; a changed
major fails during native compilation with the assertion message. Runtime-free
output contains neither the version macro nor the assertion.

## Source reasons and closure

A semantic analyzer requests a feature with a `RuntimeRequirementReason` that
contains:

- a stable reason ID and feature ID;
- the exact reachable runtime operation ID;
- the semantic reason kind and consumed typed surface;
- a normalized repository-relative Haxe source span;
- an optional typed alternative that can avoid the fallback without changing
  semantics.

`RuntimeRequirementAnalyzer` first walks the complete reachable, validated
HxcIR. It recognizes explicit runtime calls, runtime implementations on
operations/allocation/lifetime work, runtime-managed representations, and
runtime cleanup actions. Every observed intent must match exactly one typed
source candidate, and every candidate must match reachable IR. Identical source
roots are deduplicated before stable reason IDs are assigned. A type or import
that was merely seen in typed input cannot select a helper because it creates no
reachable HxcIR runtime intent.

`RuntimeFeaturePlanner` retains the reconciled roots separately from dependency edges.
Every selected root and every transitive feature inherits at least one root
reason ID, so a dependency can never appear as unexplained runtime work. The
resolved plan records dependency-first feature order plus exact artifacts,
symbols, libraries, and defines. Input root order does not affect the result.

An empty request is accepted only with a positive compiler-owned no-runtime
proof. The admitted compiler path uses this same planner after direct
representations, request-local helpers, and static initialization have been
decided. The structured proof records reachable module/type-instance/function/
block/instruction/cleanup counts, zero runtime intents, the exact direct
decisions and program-local helper IDs, and empty feature/include/source/define/
library/symbol sets. Project emission and packaging validate the proof again
before accepting or reading an artifact.

Literal output is a nonempty request rooted at each admitted source call. It
selects `runtime-base`, `status`, `string-literal`, and `io` in dependency order,
packages one C source, and exposes only `hxc_io_println`. The exact plan records
the direct UTF-8 literal decision before this hosted side-effect fallback.

Unsupported source still fails during lowering rather than being mislabeled as a
runtime blocker. As later semantic lowerings admit new runtime intent, they must
add the matching typed candidate; an unexplained intent is `HXC9000`, never a
silent feature selection or a guessed fallback.

## Policy and manual constraints

The planner applies the resolved profile, environment, runtime policy, and
diagnostic mode without changing their recorded provenance:

- `auto` admits the exact closure of implemented, purpose-appropriate features;
- `minimal` additionally rejects a definition outside the versioned narrow
  allowlist;
- `none` rejects every semantic root and requires the positive empty-plan proof.
  One deterministic `HXC2000` lists every deduplicated blocker by stable reason
  ID, operation, kind, consumed surface, normalized source span, all root-to-leaf
  dependency chains, and a semantics-preserving alternative when one exists.

Manual feature controls are constraints over compiler-inferred semantics, not
a way to smuggle code into the build. `require` may confirm only a feature
already selected by a semantic root, while `forbid` fails if reachable semantics
need that feature. Duplicate, conflicting, unavailable, environment-ineligible,
or policy-incompatible controls fail with `HXC2000`. The current typed override
model is an internal planning seam; no user-facing feature-override spelling is
published at M0.

Native-seed fixtures use the separate `native-seed-fixture` planning purpose.
The production `compiler-program` purpose rejects `runtime-abi`, `status-name`,
`alloc`, full `string`, and `array` even under `auto`, preventing independent C evidence
from becoming a generated-Haxe support claim.

## Compatibility boundary

| Axis | Current effect |
| --- | --- |
| Portable/metal | One planner serves both. Portable defaults to `auto + summary`; metal defaults to `minimal + warn`; explicit valid combinations remain available. |
| Runtime policy | `auto`, `minimal`, and `none` are enforced after direct C and program-local decisions, with provenance retained in every plan. `none` either records the structured whole-program proof or reports every blocker before output/native linking. |
| Environment | Literal output is hosted-only and fails planning for freestanding, WASI, or Emscripten. The native allocator retains hosted execution and freestanding custom-allocator/no-libc-allocation evidence. |
| Generated C | Primitive-only admitted programs remain byte-stable and runtime-free. Literal output packages only the four-feature closure and one runtime C source through normal Reflaxe ownership, then checks ABI major 0 structurally. |
| Public ABI | Runtime 0.5.0 is a versioned internal same-major contract. The manifest marks all runtime-owned layouts forbidden in application exports; generated application exports remain unsupported and E7/E10.T09 own their future admission and stabilization. |

## Exact packaging

`RuntimeFeaturePackager` accepts only a validated plan and a narrow typed
artifact source. It materializes exactly the selected artifact records as
`GeneratedFile` values in canonical order. Before its first read, it checks the
dependency order and every source/output/kind tuple against the validated
registry, so a fabricated plan cannot select another repository file. An empty
plan returns no files and does not even consult the artifact source.
Compiler-selected runtime output passes those values through normal Reflaxe
output ownership; the packager never writes or deletes output itself.
For a nonempty plan, each read must also match its registered SHA-256 before the
first corresponding `GeneratedFile` is admitted. `hxc.manifest.json` then hashes
the packaged artifact and records the exact neutral source/include/build plan,
linking release output back to the schema-2 catalog without copying unselected
runtime metadata into the project.

The canonical fixture proves an `alloc` native request packages only
`runtime-base + status + alloc`; an `array` request adds only its header/source;
and a full `string` native request adds only
`string-literal` plus the full string header, source, and symbols. Its compiler
request packages exactly `runtime-base + status + string-literal + io`. It
compiles and runs all four packages under strict GCC and Clang lanes and
inspects the alloc-only link for omitted array/string symbols and the array link
for omitted string symbols. It also covers cycle,
unknown-dependency, policy, override, environment, reserved-feature, and
native-only availability failures.

The separate [allocator ownership contract](allocator-abi.md) defines the
E4.T02 zero-size, alignment, checked-size, out-parameter, failure-atomicity, and
identity rules. Required native lanes additionally run the alloc slice against
a static custom arena in `HXC_FREESTANDING` mode, reject undefined libc
allocation symbols, and compare C and C++ layout facts. This stronger native
evidence does not make `alloc` compiler-selectable.

The [UTF-8 scalar string contract](string-runtime.md) defines the E4.T03 valid
immutable representation, maximal-subpart lossy decoding, scalar-indexed
operations, builder failure atomicity, allocation accounting, and distinct
borrowed/owned CString lifetimes. Its exact feature closure is `runtime-base +
status + alloc + string-literal + string`; required native links reject object,
GC, reflection, and dynamic symbol families. This evidence does not make full
`string` compiler-selectable. E2.T07 uses only its independently owned literal
carrier and therefore does not prove general Haxe `String` lowering.

The [typed resizable-array contract](array-runtime.md) defines the E4.T04
contiguous unboxed owner, checked deterministic growth, exact-slot alias
handling, and optional copy/assign/destroy lifecycle strategy. Its exact closure
is `runtime-base + status + alloc + array`; required native links reject string,
object, GC, reflection, and dynamic symbol families. The feature remains
native-seed-only, and fixed C arrays/spans retain a separately proven empty
runtime plan.

The focused Haxe gate renders and packages twice before comparing the canonical
snapshots. Native CI then uses `--native-only` to validate the checked-in catalog
and plan against one another, reread exactly their selected `runtime/hxrt`
artifacts, and compile them with the required C compiler. This keeps native jobs
independent of Haxe installation while preserving the full typed-render proof in
the pinned-toolchain job.

Run the focused evidence with:

```sh
npm run test:runtime-features
npm run test:array-runtime
npm run test:string-output
npm run test:primitive-differential
npm run test:span-lowering
npm run test:hello
npm run snapshots:check
npm run test:native
```

This evidence proves deterministic reachability reconciliation and no-runtime
eligibility, selective packaging, the one generated-Haxe literal-output
selection, its bounded hello product composition, exact runtime source/build
provenance, and compatible-versus-incompatible internal ABI versions. The
separate E4.T02/E4.T03/E4.T04 fixtures prove their bounded native allocator,
string, and array contracts. None of this proves broad `String`/`Array`
lowering, general I/O, object
graphs, exceptions, reflection, broad standard-library support, a generated
public application ABI, or a supported release.
