# Typed-AST input boundary

The frontend adapter owns a deterministic view of the complete typed module set
that Haxe gives Reflaxe. It is the input to Haxe-to-HxcIR lowering, but it is not
itself a semantic IR and does not emit C.

## Lifecycle and ownership

Reflaxe calls `filterTypes` before its own callback filters and before
`onCompileStart`. `CReflaxeCompiler` therefore uses this lifecycle:

```text
filterTypes
  -> normalize this request's complete ModuleType array, field expressions,
     and entry point into the pending TypedProgramInput

onCompileStart
  -> create a fresh CompilationContext and empty CSymbolRegistry
  -> move pending capture into current-request storage
  -> clear pending storage

onCompileEnd
  -> clear current-request storage
  -> install the TypedProgramInput once in CompilationContext
  -> invoke CCompiler
```

Normalizing before Reflaxe callback filtering is important: typedefs and externs
remain available even when no class/enum callback would otherwise present them,
and field/entry expressions cannot be replaced by later framework preprocessing.
No later compiler stage may reconstruct the program from callback order.

Every request replaces all mutable adapter state, including the later symbol
request set. The compiler-server test runs
a rich program followed by a smaller program and requires the smaller report to
match its cold build byte for byte.

## Normalized model

`TypedProgramInput` retains raw `ModuleType`, `ClassField`, `EnumField`, and
`TypedExpr` objects for later lowering while exposing stable classifications:

- logical source modules and their declarations;
- primary versus secondary module ownership;
- class, interface, extern, enum, typedef, abstract, and abstract-implementation
  facts;
- fields, constructors, enum constructors, types, and metadata;
- class initializers, field-expression roots, and the application entry point.

The entry record eagerly retains the typed static target signature/body when
Haxe exposes it through `getMainExpr()`. This keeps the existing omitted-main-
module fallback inside the pre-filter capture boundary; `CCompiler` never asks
a mutable `ClassField` for a later body.

Semantically unordered module/declaration sets are sorted by logical
coordinates. Field groups, enum constructors, metadata, and expression roots
retain compiler-provided source order with explicit ordinals; determinism must
not erase initialization, enum-index, or annotation order. Inventory count
tables are sorted by kind. Source paths are derived from Haxe module paths
(`example.Widget` becomes
`example/Widget.hx`); raw host paths and compiler positions are never serialized
by the inventory report. E2.T02 body lowering converts the original compiler
positions to normalized, repository-relative HxcIR spans; the report continues
to omit raw positions.

Reflaxe adds the exact transient metadata
`@:build(reflaxe.ReflectCompiler.addToBuildCache())` after compiler-server cache
reuse. The normalized metadata view removes only that framework-owned hook so a
cold and cached request describe the same program. The raw compiler declaration
is retained unchanged, and all source-authored metadata remains visible.

### Exact record-field positions under the Haxe server

A Haxe object-shaped typedef such as this is a **named anonymous record**:

```haxe
typedef Result = {
  final value:Int;
}
```

“Anonymous” means the value is structurally an object with those fields rather
than a class instance; “named” means the typedef gives that shape the reusable
source name `Result`.

Haxe 5.0.0-preview.1 can reuse that typed shape in a compiler-server request
while changing the `ClassField.pos` visible to a macro. A cold request points
`value` at its declaration above. An unchanged warm request can instead point
it at `value: compute()` in an object literal. The C field layout is the same,
but a type error would highlight the wrong line and the HxcIR source span would
change.

`NamedRecordSourceProvenance` repairs this at the typed-input boundary:

1. A cold or rebuilt typedef is trusted only when every field position lies
   inside that typedef's own source range.
2. The compiler records only field names and byte offsets. It keys that plain
   data by a schema number, declaration path, the SHA-256 digest of the exact
   source bytes, and the SHA-256 digest of the complete Haxe define set.
3. `@:persistent` keeps this bounded plain-data cache across requests to the
   same Eval-hosted Haxe server. The cache holds at most 2,048 records and never
   retains a Haxe `Type`, `Position`, filename, or application value.
4. A warm request rebuilds request-local `Position` values using the current
   source file. A missing or mismatched record fails with `HXC9000` before
   HxcIR construction and tells the user to make one fresh-process build.

This is deliberately fail-closed: using the nearby typedef range or whichever
object literal Haxe exposed would produce plausible output with false
provenance. Exact source bytes and defines also make edits invalidate the
record, while reconstructing the filename per request permits identical source
in a second worktree without retaining the first worktree's host path.

`npm run test:typed-ast` proves cold, unchanged-warm, implementation edit,
public typedef edit, unsupported-type diagnostics, request restoration, server
restart, server-off, and second-worktree behavior. It compares HxcIR and every
normal generated file with a fresh-process oracle. Full `#line` and sidecar
source-map policy is not shipped by this boundary; it remains planned under
E8.T08. This plan preserves the exact coordinates that such mapping must later
consume.

## Inventory report

Defining `reflaxe_c_typed_ast_report` prints one path-stable JSON record prefixed
with `HXC_TYPED_AST_INVENTORY=` immediately before body lowering. Its status is
`normalized-typed-input-before-body-lowering`. It contains entry-point facts,
module/declaration/field records, owned expression-root records with ordinals,
and sorted counts for declaration, class, field, expression, type, and metadata
kinds. Its purpose is unsupported-node planning and regression evidence; it is
not the body-lowering result.

The report define is an implementation diagnostic, not application
configuration or a supported `hxc` flag. Likewise,
`reflaxe_c_test_reverse_typed_modules` is a test seam that reverses only the
captured input array before normalization. It must never affect normal builds or
be documented as a user feature.

Run the compile-backed suite with:

```sh
npm run test:typed-ast
```

Refresh the reviewed inventory only after inspecting an intentional frontend or
toolchain change:

```sh
npm run snapshots:update -- --suite typed-ast
```

The suite requires forward, reversed, repeated-cold, and compiler-server reports
to agree where their source programs agree. Its deliberately unsupported bodies
must now fail with exact source-anchored `HXC1001` and an empty output tree.
Passing this suite proves collection and normalization only; the separate
[body-lowering suite](body-lowering.md) proves the admitted TypedExpr-to-HxcIR/C
path and [function lowering](function-lowering.md) proves its narrow production
static-function consumer.
