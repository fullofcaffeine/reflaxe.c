# ADR 0001: Direct C first and selective `hxrt`

- Status: Accepted
- Date: 2026-07-14
- Decision owners: project owner and compiler maintainers
- Related requirements: HXC-PROD-002, HXC-PROD-003, HXC-PROD-004,
  HXC-PROD-005

## Context

Haxe semantics sometimes need machinery that C does not provide directly, but
that does not justify routing ordinary values and operations through a universal
runtime. The Rust and Go sibling targets show two useful patterns: native-shaped
lowering may be selected whenever the source contract permits it, and every
remaining runtime requirement should have a stable, inspectable reason.

The C target needs an even stronger version of that rule because readable,
idiomatic, performant C and a genuinely runtime-free lane are core product
goals.

## Decision

### Direct lowering is the default strategy

The compiler resolves each operation and representation in this order:

1. emit an idiomatic C construct when it preserves the consumed Haxe or typed
   `c.*` source contract;
2. generate a program-local specialized helper when that keeps the behavior
   explicit without creating a shared runtime dependency;
3. request the smallest dependency-closed `hxrt` feature only for a remaining
   semantic gap;
4. fail with a source-positioned diagnostic when the resolved policy forbids
   that fallback.

Runtime planning happens after direct-representation and specialization
decisions. Merely mentioning a Haxe type is not by itself permission to select a
runtime feature. There is no unconditional baseline `hxrt` slice: a runtime-free
build emits, includes, compiles, and links no `hxrt` file or symbol.

### Profiles are presets, not separate compilers

Both profiles use the same lowering pipeline and both must emit idiomatic C.
Ordinary Haxe APIs retain Haxe semantics; typed `c.*` APIs, C externs, and C
metadata declare explicit C-native source boundaries.

If the user does not specify the orthogonal runtime options, the profile supplies
these defaults:

| Profile | Runtime policy | Runtime diagnostics | Intent |
| --- | --- | --- | --- |
| `portable` | `auto` | `summary` | Preserve Haxe behavior, while still choosing direct C first. |
| `metal` | `minimal` | `warn` | Permit only the narrow runtime allowlist and make every root requirement visible. |

Every valid profile/runtime combination remains selectable explicitly. In
particular, portable code may request `none`, and metal code may explicitly
request `auto` when it accepts the reported runtime costs. `metal` is not a
correctness-off mode, and it is not synonymous with a blanket ban on all helper
code.

Runtime policies mean:

- `auto`: select the exact dependency closure justified by reachable semantics;
- `minimal`: do the same, but reject features outside a versioned narrow
  allowlist, including broad dynamic dispatch, general reflection registration,
  tracing collection, and general exception machinery unless an accepted policy
  explicitly admits them;
- `none`: require a whole-program proof and reject every `hxrt` requirement.

`hxc_runtime_diagnostics=off|summary|warn` changes console reporting, never
semantics or packaging. `summary` emits one aggregate notice when `hxrt` is
selected. `warn` reports deduplicated root requirements at their source spans;
transitive dependency edges remain in the report instead of producing warning
spam. Policy violations are errors regardless of this diagnostic setting.

### Runtime selection is evidence

`hxc.runtime-plan.json` is emitted for every successful build, including a
runtime-free build. It records:

- the resolved profile, runtime policy, diagnostic policy, and provenance for
  each default or override;
- selected feature IDs and their deterministic dependency closure;
- stable semantic reason kinds, owning Haxe or `c.*` surface, and source spans;
- root requirements separately from transitive dependencies;
- direct/native representation decisions considered before fallback;
- emitted headers, sources, libraries, defines, and symbols;
- the positive runtime-free proof or all blockers.

Warnings and reports should suggest typed alternatives such as spans, explicit
results, value layouts, ownership-aware handles, or compile-time generation when
those alternatives preserve the intended source behavior. They must not suggest
silently changing semantics merely to remove `hxrt`.

## Consequences

- Runtime helpers are a reviewed semantic fallback, not the compiler's default
  vocabulary.
- Portable programs can be runtime-free when the compiler proves eligibility.
- Metal programs can deliberately opt into a reported runtime slice without
  forking the backend.
- Each lowering feature needs direct-C, runtime-plan, and no-runtime evidence.
- Runtime footprint regressions become observable in snapshots and budgets.
- The `minimal` allowlist and report schema are versioned product surfaces.

## Rejected alternatives

- A monolithic runtime or unconditional `core` slice: it makes runtime-free
  claims false and hides avoidable compiler work.
- A third `idiomatic` profile: idiomatic output is required in every profile.
- Making `metal` always equivalent to `none`: this prevents explicit, useful
  combinations and conflates source semantics with packaging policy.
- Warning on every transitive runtime dependency: it creates noise without
  identifying the source decision the user can change.
