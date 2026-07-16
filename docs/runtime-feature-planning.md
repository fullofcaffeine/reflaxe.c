# Runtime feature planning and selective packaging

The runtime planner implements the compiler-first policy from
[ADR 0001](adr/0001-direct-c-and-selective-runtime.md): preserve semantics with
direct idiomatic C first, then use a program-local specialization, and request
the narrowest dependency-closed `hxrt` slice only when neither compiler-owned
option is feasible. There is no unconditional `core` feature.

This is a bounded M0 capability. The graph, policy checks, reason propagation,
and exact packager are implemented. The currently admitted generated-Haxe
programs remain runtime-free, and every checked-in native runtime definition is
marked `native-seed-only`. No runtime-requiring Haxe construct can select or
link those provisional definitions yet.

## Typed graph contract

`RuntimeFeatureCatalog` supplies a fresh `RuntimeFeatureRegistry` for each
compilation. Every definition has a validated stable ID, availability,
environment set, minimal-policy eligibility, dependencies, owned headers and C
sources, symbols, libraries, and defines. The registry rejects duplicate IDs,
artifacts, or symbols; unavailable dependencies; invalid paths; and dependency
cycles before planning. Canonical UTF-8 ordering makes the catalog independent
of discovery and map iteration order.

The machine-readable review view is
[`runtime/hxrt/features.json`](../runtime/hxrt/features.json), validated against
[`runtime-features.schema.json`](specs/runtime-features.schema.json). Its current
provisional graph is:

```text
string -> alloc -> status -> runtime-abi
```

These components are split into independently owned `hxrt/*.h` and `.c` files.
The graph also reserves separate IDs for arrays, objects, tracing collection,
dynamic values, reflection, exceptions, threads, platform services, and other planned
features. Reservations fail closed and name the task that must implement them;
they are not empty features and cannot be selected.

The catalog and plan formats are internal schema-1 inspection contracts. Their
stabilization, compatibility rules, and public versioning remain owned by
E4.T11.

## Source reasons and closure

A semantic analyzer requests a feature with a `RuntimeRequirementReason` that
contains:

- a stable reason ID and feature ID;
- the semantic reason kind and consumed typed surface;
- a normalized repository-relative Haxe source span;
- an optional typed alternative that can avoid the fallback without changing
  semantics.

`RuntimeFeaturePlanner` retains those roots separately from dependency edges.
Every selected root and every transitive feature inherits at least one root
reason ID, so a dependency can never appear as unexplained runtime work. The
resolved plan records dependency-first feature order plus exact artifacts,
symbols, libraries, and defines. Input root order does not affect the result.

An empty request is accepted only with a positive compiler-owned no-runtime
proof. The admitted primitive compiler path uses this same planner after direct
representations, request-local helpers, and static initialization have been
decided. Its plan therefore contains no `hxrt` include, source, define, library,
or symbol.

Runtime-requirement inference and the complete blocker-producing no-runtime
eligibility analysis remain later compiler passes. This planner validates and
resolves their typed inputs; it does not infer requirements from a type name,
an allocation instruction, `Dynamic`, or an unsupported AST node.

## Policy and manual constraints

The planner applies the resolved profile, environment, runtime policy, and
diagnostic mode without changing their recorded provenance:

- `auto` admits the exact closure of implemented, purpose-appropriate features;
- `minimal` additionally rejects a definition outside the versioned narrow
  allowlist;
- `none` rejects every semantic root and requires the positive empty-plan proof.

Manual feature controls are constraints over compiler-inferred semantics, not
a way to smuggle code into the build. `require` may confirm only a feature
already selected by a semantic root, while `forbid` fails if reachable semantics
need that feature. Duplicate, conflicting, unavailable, environment-ineligible,
or policy-incompatible controls fail with `HXC2000`. The current typed override
model is an internal planning seam; no user-facing feature-override spelling is
published at M0.

Native-seed fixtures use the separate `native-seed-fixture` planning purpose.
The production `compiler-program` purpose rejects every provisional seed
definition even under `auto`, preventing independent C evidence from becoming
a generated-Haxe support claim.

## Compatibility boundary

| Axis | Current effect |
| --- | --- |
| Portable/metal | One planner serves both. Portable defaults to `auto + summary`; metal defaults to `minimal + warn`; explicit valid combinations remain available. |
| Runtime policy | `auto`, `minimal`, and `none` are enforced after direct C and program-local decisions, with provenance retained in every plan. |
| Environment | The provisional native seed is compile-checked only for hosted and freestanding C; WASI and Emscripten requests fail until dedicated evidence exists. |
| Generated C | Existing admitted Haxe programs remain byte-stable runtime-free C. Selective seed packages are independently authored native evidence, not generated Haxe output. |
| Public ABI | The split headers and `hxc_` symbols are provisional. E4.T02/E4.T03 harden allocator/string contracts, and E4.T11 owns ABI/manifest stabilization. |

## Exact packaging

`RuntimeFeaturePackager` accepts only a validated plan and a narrow typed
artifact source. It materializes exactly the selected artifact records as
`GeneratedFile` values in canonical order. Before its first read, it checks the
dependency order and every source/output/kind tuple against the validated
registry, so a fabricated plan cannot select another repository file. An empty
plan returns no files and does not even consult the artifact source. Future
compiler-selected runtime output must pass those values through normal Reflaxe
output ownership; the packager never writes or deletes output itself.

The canonical fixture proves an `alloc` request packages only
`runtime-abi + status + alloc`, while a `string` request adds only the string
header, source, and symbols. It compiles and runs both packages under strict
GCC and Clang lanes and inspects the alloc-only link for omitted string symbols.
It also covers cycle, unknown-dependency, policy, override, environment,
reserved-feature, and provisional-availability failures.

Run the focused evidence with:

```sh
npm run test:runtime-features
npm run snapshots:check
npm run test:native
```

This evidence proves deterministic planning and selective packaging. It does
not prove Haxe `String`, allocation, object graphs, exceptions, reflection, the
standard library, stable runtime ABI, or generated-Haxe runtime selection.
