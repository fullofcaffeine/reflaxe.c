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
remains `contract-seed-no-lowering`. Production lowering now consumes three
separate bounded surfaces. E2.T08 and its bounded storage extension admit
nonempty literal-backed `CArray<T, N>` plus positive compile-time-sized
`CArray.zero` automatic storage and local `Span<T>`/`ConstSpan<T>` borrowing,
indexing, and iteration. The exact-width storage extension also admits the
direct, non-failing subset of `c.IntConvert.exact` and
`c.IntConvert.modulo`. The first E6 vertical slice admits reached hand-authored
extern C declarations with exact names and headers for scalars, typedefs,
fieldless enums/constants, by-value structs, explicit binary32 `c.Float32`
values, and literal borrowed `CString` arguments. Other `c.*` operations remain
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

## Exact-width integer conversion boundary

Exact-width carriers intentionally do not inherit Haxe `Int` arithmetic or
implicit lossy conversions. Application code states its intended value rule
through a typed compiler contract:

```haxe
final red:c.UInt8 = c.IntConvert.modulo(sourceRed);
final redForMath:Int = c.IntConvert.exact(red);
```

The assignment, return, or argument context infers the target type. `exact`
requires every source value to be representable by that target. `modulo`
requires an unsigned target and retains the low target-width bits. Lowering
accepts only non-failing direct-C decisions between Haxe `Int`/`UInt` and the
exact-width `c.Int*`/`c.UInt*` family. It records the conversion in HxcIR and
emits a structural C cast without allocation, a helper, or `hxrt`.

An invalid mode, a signed reconstruction helper, a checked/failing conversion,
or a target-ABI integer reports source-positioned `HXC1001` and leaves no
artifact. Haxe `cast` is not an alternative conversion API: it is an unchecked
type assertion and remains forbidden in repository-owned Haxe. See
[UB-safe primitive arithmetic and typed integer conversions](arithmetic-semantics.md)
for the executable matrix and exact boundary.

## Direct import boundary

`CImportRegistry` consumes the same typed snapshot together with the real
typed-AST declaration graph. It prepares every reached import before symbol
finalization, then lowers exact names only through `CSymbolRegistry`. A valid
import has these properties:

- every reached type, constant, field, and function resolves one exact external
  C name. A header-owned `extern` declaration defaults to its identical Haxe
  type/field spelling; `@:c.name` is required only when the native spelling
  differs;
- an authoritative `@:c.include` supplies the definition; generated C never
  recreates the foreign struct, enum, or typedef;
- an imported enum name is the header's typedef use-site spelling, so anonymous
  `typedef enum { ... } Name` declarations remain usable without inventing a tag;
- a direct one-level typedef alias to an imported struct preserves its exact C
  typedef identity in the import plan while sharing the target struct's HxcIR
  value representation. This needs no wrapper or conversion; alias chains,
  cycles, and cross-family aliases remain fail-closed;
- functions are static extern, fixed-arity, non-callback C calls whose admitted
  parameter and result types are exact scalars or by-value imported values;
- imported struct fields are ordinary typed HxcIR places, so reads and writes
  stay structural rather than becoming C fragments;
- a `String` literal can be borrowed as `c.CString` only at a direct call site.
  The compiler validates its UTF-8 byte length, rejects embedded NUL, emits
  immutable translation-unit literal storage, and performs no allocation; and
- reached include, logical library, pkg-config, and framework facts are
  deduplicated with declaration provenance in the neutral build plan. Merely
  declaring an unused extern selects no fact and no runtime feature.

### Exact C `float` values

Haxe `Float` deliberately remains binary64 `double`, so a foreign C `float`
must use the distinct `c.Float32` carrier in constants, fields, parameters, and
results. Conversion is explicit at the Haxe/C seam:

```haxe
final nativeScale:c.Float32 = c.Float32.fromFloat(scale);
final scaleForHaxe:Float = nativeScale.toFloat();
```

Narrowing is round-to-nearest, ties-to-even; widening is exact. NaN stays NaN
without a payload guarantee, infinities and signed zero preserve their sign,
subnormals are supported, and finite overflow narrows to same-signed infinity.
No implicit `Float`/`c.Float32` coercion exists, so ordinary Haxe arithmetic
does not silently become single precision. Reachable Float32 projects emit
structural target-side binary32 assertions and select no helper or `hxrt`.
See [ADR 0008](adr/0008-primitive-representations-and-conversions.md) for the
full rounding and foreign floating-environment contract.

For example, the executable point-library fixture uses ordinary Haxe declarations:

```haxe
@:c.layout(c.Layout.Struct)
@:c.name("pointlib_point")
@:c.include("pointlib.h", c.IncludeKind.Local)
extern class Point {
  @:c.name("x") public var x:Coord;
  @:c.name("y") public var y:Coord;
}

@:c.include("pointlib.h", c.IncludeKind.Local)
@:c.link("pointlib")
extern class PointLib {
  @:c.name("pointlib_point_translate")
  public static function translate(point:Point, dx:Coord, dy:Coord):Point;
}
```

The same fixture also proves a transparent native alias:

```haxe
@:c.name("pointlib_point_alias")
@:c.include("pointlib.h", c.IncludeKind.Local)
extern typedef PointAlias = Point;
```

Calls may use `PointAlias` exactly where the C header does while field/place
semantics reuse `Point`'s imported layout. The compiler does not synthesize a C
typedef or an adapter because the authoritative header already owns both.
Raylib's `Camera` alias uses this same reusable path; see
[the raw core binding](raylib-raw-core.md).

The identity default is still an exact ABI fact, not a generated-name
heuristic. `CSymbolRegistry` validates it against C spelling, reserved names,
and namespace collisions and records it as the requested external name. A
default C calling convention on a header-owned fixed-arity function is likewise
implicit; non-default conventions remain explicit metadata. This is why a raw
binding can write `public static function BeginDrawing():Void` without repeating
either `@:c.name("BeginDrawing")` or `@:c.callingConvention(C)`.

Callbacks, variadics, native pointers, retained strings, opaque resource
ownership, unions, bit fields, packed layouts, source preprocessor definitions,
C++, and inferred or exported ABI remain outside this slice. They fail with source-positioned `HXC3000` and
leave no plausible artifact; adding an unsafe cast or raw C string is not an
escape hatch.

## C concepts expressed in Haxe

| C concept | Haxe-facing form | Why this form |
| --- | --- | --- |
| Functions, modules, enums, constants | Ordinary typed Haxe declarations | Haxe already models them and supplies IDE/type-checker support. |
| Exact integer widths and `size_t`-class types | `c.Int8` through `c.UInt64`, `c.Size`, `c.PtrDiff`, `c.IntPtr`, `c.UIntPtr` | They must not inherit Haxe `Int` semantics or an assumed host width. |
| Exact C binary32 value | `c.Float32`, with explicit `fromFloat`/`toFloat` | Haxe `Float` remains binary64; a lossy foreign narrowing must be visible and target-qualified. |
| Integer value conversion | `c.IntConvert.exact(value)`, `c.IntConvert.modulo(value)` | Conversion intent stays distinct from an unchecked type assertion; the inferred target is admitted only when the compiler proves the named direct semantics. |
| Pointers and qualifiers | `c.Ptr<T>`, `ConstPtr<T>`, `NullablePtr<T>`, `Ref<T>`, `ConstRef<T>`, `RestrictPtr<T>`, `VolatilePtr<T>` | Nullability, borrow shape, mutability, and aliasing obligations stay visible in types. |
| Function pointers, arrays, and views | `c.FunctionPtr<T>`, `CArray<T, N>`, `Span<T>`, `ConstSpan<T>`, `CString`, `StringView` | Application code does not reconstruct declarators or pointer/length pairs as strings. The admitted fixed-array slice preserves `N` for direct nonempty literals and bounded compiler-known `CArray.zero` storage, then lowers local span views without runtime objects; broader forms remain reserved. |
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
conflicts described above. Later compiler analyses still own broader qualifier
and ownership consistency, callback lifetime, exported-ABI leakage, variadics,
target capabilities, and non-include build-fact conflicts. The bounded
direct-import registry validates and reaches the exact supported subset; native
authority remains separate:

- Clang-derived facts define imported declarations and layouts;
- generated `_Static_assert` checks size, alignment, offset, and configuration;
- C and C++ consumers compile generated public headers independently; and
- ABI probes and manifests compare actual symbols and layouts.

The macro never writes files. `TypedCNameFinalizer` consumes its schema-2
snapshot without reparsing metadata, preserves exact names, assigns deterministic
defaults, and supplies finalized names to declaration planning. During M0, the
implementation-only
`reflaxe_c_contract_report` define prints its deterministic JSON payload for the
compile-backed snapshot test. The whole-program compiler now consumes the same
snapshot for the bounded reached direct-import slice and emits its C, build
plan, and runtime-free proof through Reflaxe output ownership. General typed-C
declaration emission and `hxc inspect macros` remain later E6/E7/E8 work.

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
- E6.T03-E6.T05 and E6.T09 now have the bounded hand-authored direct-import
  vertical slice described above. Full qualifiers, layouts, callbacks,
  variadics, deterministic Clang derivation, and broad build inputs remain with
  their original E6 owners.
- E7 owns export discovery, ABI-safe type validation, generated public headers,
  and independent consumers.
- E1.T08 owns the executable-only schema-1 neutral build plan and optional
  CMake/Meson seeds; E8.T04 and E8.T07 own the full toolchain plan and
  inspection product.
- E9 owns application DSL admission and examples; E10.T09 owns stable public ABI
  spellings.
