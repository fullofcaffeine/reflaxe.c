# Typed-AST input boundary

The frontend adapter owns a deterministic view of the complete typed module set
that Haxe gives Reflaxe. This is the input to future Haxe-to-HxcIR lowering; it
is not itself a semantic IR and does not emit C.

## Lifecycle and ownership

Reflaxe calls `filterTypes` before its own callback filters and before
`onCompileStart`. `CReflaxeCompiler` therefore uses this lifecycle:

```text
filterTypes
  -> replace pending capture with this request's complete ModuleType array

onCompileStart
  -> create a fresh CompilationContext
  -> move pending capture into current-request storage
  -> clear pending storage

onCompileEnd
  -> clear current-request storage
  -> normalize modules plus getMainModule()/getMainExpr()
  -> install the TypedProgramInput once in CompilationContext
  -> invoke CCompiler
```

Capturing before Reflaxe callback filtering is important: typedefs and externs
remain available even when no class/enum callback would otherwise present them.
No later compiler stage may reconstruct the program from callback order.

Every request replaces all mutable adapter state. The compiler-server test runs
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

Semantically unordered module/declaration sets are sorted by logical
coordinates. Field groups, enum constructors, metadata, and expression roots
retain compiler-provided source order with explicit ordinals; determinism must
not erase initialization, enum-index, or annotation order. Inventory count
tables are sorted by kind. Source paths are derived from Haxe module paths
(`example.Widget` becomes
`example/Widget.hx`); raw host paths and compiler positions are never serialized
by the inventory report. Later source-span normalization remains part of actual
HxcIR lowering.

Reflaxe adds the exact transient metadata
`@:build(reflaxe.ReflectCompiler.addToBuildCache())` after compiler-server cache
reuse. The normalized metadata view removes only that framework-owned hook so a
cold and cached request describe the same program. The raw compiler declaration
is retained unchanged, and all source-authored metadata remains visible.

## Inventory report

Defining `reflaxe_c_typed_ast_report` prints one path-stable JSON record prefixed
with `HXC_TYPED_AST_INVENTORY=` immediately before the current `HXC1000`
boundary. It contains entry-point facts, module/declaration/field records,
owned expression-root records with ordinals, and sorted counts for declaration,
class, field, expression, type, and metadata kinds. Its purpose is
unsupported-node planning and regression evidence.

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
to agree where their source programs agree. It also requires source-anchored
`HXC1000` and an empty output tree. Passing it proves collection and
normalization only; typed Haxe still does not reach HxcIR or C.
