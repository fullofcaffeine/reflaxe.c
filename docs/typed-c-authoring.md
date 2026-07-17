# Typed C authoring contract

This document defines the first executable Haxe-facing contract for authoring C
declarations in `reflaxe.c` and its first target-owned dependency planner. It
refines [ADR 0002](adr/0002-haxe-first-typed-c-authoring.md) without claiming
arbitrary `c.*` lowering or production public-header emission.

## Current status

The M0 contract plus M1 declaration-planning slice do five things:

1. type-checks zero-runtime `c.*` contracts for exact-width integers, pointers,
   references, spans, arrays, C strings, ownership, allocators, volatility, and
   atomics;
2. validates canonical `@:c.*` declaration and build metadata after Haxe typing;
3. produces a deterministic structural `TypedCContractSnapshot` whose schema 2
   build facts retain sorted declaration provenance;
4. reports `HXC5002` at the originating Haxe span for the validation slice it
   understands; and
5. computes a typed, deterministic `CDeclarationPlan` for header/source
   placement, includes, portable guards, forward declarations, and complete-type
   ordering before any file is emitted.

The declaration-contract report itself does **not** lower types, emit C, prove
an ABI layout, expose raw C, or make `c.Unsafe` operational; its explicit status
remains `contract-seed-no-lowering`. Separately, E2.T08 admits one narrow local
operation slice: nonempty literal-backed `CArray<T, N>` plus `Span<T>` and
`ConstSpan<T>` borrowing, indexing, and iteration. Other `c.*` operations remain
fail-closed. `c.Syntax` and `c.Unsafe` are deliberately empty authority markers
until their owning safety and inspection work is complete.

Profile-aware validation now rejects `@:c.pack(...)` on a
`@:c.layout(c.Layout.Struct|Union)` declaration in `metal` with `HXC5002`.
Packing remains unavailable until a target-ABI probe and structural packed C
declaration can prove it. This does not affect E3.T01's ordinary private closed
anonymous records, which use strict C11 layout and no typed-C metadata.

## Declaration planning boundary

`CDeclarationPlanner` consumes the snapshot without consulting filesystem,
process, locale, runtime, or compiler-server state. It applies these rules:

- a complete-type edge orders declarations in one header or adds the target's
  generated header when the definitions are separated;
- a pointer-only edge adds a typed struct/union/opaque forward declaration and
  never adds an include merely for convenience;
- a headerless opaque declaration with an authored include is treated as an
  external authority, so that include is propagated to consumers instead of
  guessing the foreign tag/typedef shape;
- identical includes and source reasons are merged, with system includes before
  local includes and both groups sorted by UTF-8 bytes;
- public headers cannot acquire complete private definitions, while an opaque
  pointer may intentionally hide a private implementation; and
- public guards use an injective uppercase hexadecimal encoding of the UTF-8
  header path under `HXC_GENERATED_PATH_*_INCLUDED`.

Impossible complete-type cycles, complete source-only dependencies from a
header, missing finalized forward names, mixed header visibility, and conflicting
include kinds fail with actionable `HXC5002` errors. The plan retains every
include reason so later inspection can explain both authored build facts and
derived complete/external dependencies.

The checked-in `test/declaration_plan` adapter renders a narrow structural C AST
corpus from this plan. Its headers are compiled independently and together by
the native matrix. That is executable planning evidence, not the production
E1.T07 output writer, export ABI, or proof of arbitrary Haxe declaration
lowering. Planning selects no `hxrt` feature.

## C concepts expressed in Haxe

| C concept | Haxe-facing form | Why this form |
| --- | --- | --- |
| Functions, modules, enums, constants | Ordinary typed Haxe declarations | Haxe already models them and supplies IDE/type-checker support. |
| Exact integer widths and `size_t`-class types | `c.Int8` through `c.UInt64`, `c.Size`, `c.PtrDiff`, `c.IntPtr`, `c.UIntPtr` | They must not inherit Haxe `Int` semantics or an assumed host width. |
| Pointers and qualifiers | `c.Ptr<T>`, `ConstPtr<T>`, `NullablePtr<T>`, `Ref<T>`, `ConstRef<T>`, `RestrictPtr<T>`, `VolatilePtr<T>` | Nullability, borrow shape, mutability, and aliasing obligations stay visible in types. |
| Function pointers, arrays, and views | `c.FunctionPtr<T>`, `CArray<T, N>`, `Span<T>`, `ConstSpan<T>`, `CString`, `StringView` | Application code does not reconstruct declarators or pointer/length pairs as strings. E2.T08 preserves `N` as the typed identity of a directly assigned nonempty literal and lowers local span views without runtime objects; broader forms remain reserved. |
| Ownership and allocation | `c.Owned<T>`, `Borrowed<T>`, `Allocator`, `Arena`, `Result<T, E>` | Ownership and failure cannot disappear behind a convenient call. |
| Struct, union, enum, or opaque intent | ordinary declaration plus `@:c.layout(c.Layout.*)` | Layout is a declaration fact that Haxe syntax alone cannot state. |
| Header group and stable native name | `@:c.header("path.h", c.Header.Public\|Private)` and `@:c.name("symbol")` | The compiler can derive guards, forward declarations, dependencies, and ordering. |
| Packing, alignment, bit fields | `@:c.pack(n)`, `@:c.align(n)`, `@:c.bitField(n)` | Literal validation is source-local; native `_Static_assert` and ABI probes remain authoritative. |
| Linkage, visibility, convention, section | `@:c.linkage(c.Linkage.*)`, `@:c.visibility(c.Visibility.*)`, `@:c.callingConvention(c.CallingConvention.*)`, `@:c.section("...")` | These are explicit, inspectable target boundaries and can be rejected against capabilities. |
| External build inputs | `@:c.include`, `@:c.link`, `@:c.define`, `@:c.pkgConfig`, `@:c.framework` | Literal facts enter one neutral manifest; optional CMake/Meson files derive from its typed values and never concatenate shell or raw preprocessor code. |
| Compile-time invariant | `c.StaticAssert.require(literalBool, "reason")` | It gives an immediate Haxe diagnostic. Native size/alignment facts still require emitted C assertions. |

Empty `c.Lib`, `c.Build`, or `c.Export` marker objects were not added merely to
mirror nouns from the PRD. Build and export intent belongs on typed declarations
through validated metadata until a concrete use case demonstrates that a value
API is clearer.

## Canonical metadata

The M0 spellings are namespaced and case-sensitive:

```haxe
@:c.layout(c.Layout.Opaque)
@:c.name("widget_storage")
extern class WidgetStorage {}

@:c.layout(c.Layout.Struct)
@:c.header("widget.h", c.Header.Public)
@:c.name("widget_config")
@:c.align(8)
extern class WidgetConfig {
  public var capacity:c.Size;
  public var storage:c.NullablePtr<WidgetStorage>;
}

@:c.header("widget.h", c.Header.Public)
extern class WidgetApi {
  @:c.export
  @:c.name("widget_recommended_capacity")
  @:c.linkage(c.Linkage.External)
  @:c.callingConvention(c.CallingConvention.C)
  public static function recommendedCapacity(config:c.ConstRef<WidgetConfig>):c.Size;
}

@:c.layout(c.Layout.Opaque)
@:c.include("sqlite3.h", c.IncludeKind.System)
@:c.link("sqlite3")
@:c.define("SQLITE_THREADSAFE", 1)
@:c.pkgConfig("sqlite3")
extern class SQLite3 {}
```

Metadata enum parameters must use their canonical `c.*` value, not an
unconstrained string. Paths, names, integer values, and build values must be
compile-time literals. Unknown `@:c.*` metadata is an error so a typo cannot
silently change a header or ABI contract.

Header paths are normalized relative `.h` paths. Includes are normalized
relative paths. Link, package, and framework entries are logical names, not
flags or shell fragments. Sysroots, target triples, arbitrary compiler options,
and platform capability selection belong to the resolved build model rather
than source metadata.

Explicit C identifiers may not use C keywords, underscore-prefixed or
double-underscore implementation-reserved forms, or the `hxc_` / `hxrt_`
namespaces. Exact `@:c.name` values are never rewritten. Duplicate checks follow
C namespaces: tags are separate from ordinary symbols and members are separate
per aggregate. The [deterministic symbol registry](symbol-naming.md) supplies
documented `hxc_api_` public defaults when a name is omitted and records every
generated collision; the spelling is still pre-1.0 because E7 and E10.T09 own
public ABI stabilization and compatibility policy.

## Verification layers

The current Haxe macro validates only facts it can know soundly: metadata shape
and literals, normalized paths/names, declaration/layout compatibility,
power-of-two pack/alignment syntax, duplicate explicit symbols, opaque values
embedded by value, and impossible by-value declaration cycles. It also derives
typed declaration dependencies, including forward-declarable pointer edges.
Resolved-profile checks additionally reject the currently unsupported metal
packing combination before a declaration snapshot can be mistaken for layout
evidence.

The declaration planner now owns complete-versus-forward dependency structure,
header/source grouping, include provenance, and the complete-type/public-private
conflicts described above. Later compiler analyses still own qualifier and
ownership consistency, callback lifetime, exported-ABI leakage, variadics,
target capabilities, and non-include build-fact conflicts. Native authority
remains separate:

- Clang-derived facts define imported declarations and layouts;
- generated `_Static_assert` checks size, alignment, offset, and configuration;
- C and C++ consumers compile generated public headers independently; and
- ABI probes and manifests compare actual symbols and layouts.

The macro never writes files. `TypedCNameFinalizer` consumes its schema-2
snapshot without reparsing metadata, preserves exact names, assigns deterministic
defaults, and supplies finalized names to declaration planning. During M0, the
implementation-only
`reflaxe_c_contract_report` define prints its deterministic JSON payload for the
compile-backed snapshot test. The whole-program compiler will later consume the
same typed snapshot and emit inspected artifacts through Reflaxe output
ownership. `hxc inspect macros` is owned by E8 and is not implemented.

The fixture compiles equivalent roots in opposite reference orders and runs a
valid/invalid/valid sequence through one Haxe compiler server. The reports must
remain byte-identical and the failed request must not contaminate its successor.

## Admission record

Two compile-time mechanisms are admitted by this slice:

- `TypedCContractMacro` exists because ordinary Haxe cannot express header,
  layout, linkage, or build facts. Its input is typed declarations plus a
  constrained canonical metadata vocabulary; its output is a typed structural
  snapshot. Expansion is deterministic and its M0 effects are allocation
  `none`, ownership `declarative-only`, unsafe `none`, C-target-only portability,
  and an empty runtime-feature list.
- `c.StaticAssert.require` exists for a local, source-positioned compile-time
  invariant. It accepts a Bool literal and literal explanation, expands to no
  runtime operation, and selects no runtime feature.

No application DSL and no raw-C macro is admitted by this slice. A future DSL
must separately show why ordinary Haxe, this metadata vocabulary, and existing
typed abstractions are insufficient; add positive, negative, determinism,
inspection, and no-runtime evidence; and document ownership and unsafe effects.

## Downstream ownership

- E0.T04 integrates the complete compiler/macro and non-macro type-check gate;
  E0.T06 owns diagnostic catalog machinery.
- E1.T01 and E1.T03 provide structural C declarators plus typed
  include/forward/complete declaration planning; E1.T04 now supplies the
  deterministic symbol registry/default-name finalizer, and E1.T07 owns
  production multi-file emission of its manifest shape.
- E2.T08 owns the narrow fixed-array/span operational slice documented in
  [fixed arrays and span-based iteration](span-lowering.md).
- E3 owns aggregate representation, pointer/ownership operations, and unsafe
  lexical enforcement.
- E6 owns imported qualifiers, constants, layouts, functions, and exact external
  build facts derived from Clang.
- E7 owns export discovery, ABI-safe type validation, generated public headers,
  and independent consumers.
- E1.T08 owns the executable-only schema-1 neutral build plan and optional
  CMake/Meson seeds; E8.T04 and E8.T07 own the full toolchain plan and
  inspection product.
- E9 owns application DSL admission and examples; E10.T09 owns stable public ABI
  spellings.
