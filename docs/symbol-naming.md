# Deterministic C symbol naming

`CSymbolRegistry` is the target-owned naming boundary between normalized Haxe
semantics and `CIdentifier`. It assigns names as one per-compilation batch and
produces the structural schema-1 payload that the production emitter will write
as `hxc.symbols.json`. The E2.T02 body slice now uses it for real typed functions
and lexical locals; production symbol-file/project emission remains a later
boundary. The focused `test/symbol_registry` report is naming evidence, not
generated application C.

## Identity and determinism

A `CSymbolRequest` is identified only by semantic coordinates:

- symbol role and qualified Haxe name;
- the real C namespace and its semantic scope;
- normalized overload signature and generic specialization arguments; and
- a source-stable ordinal for temporaries, closures, and closure environments.

Filesystem paths, object addresses, process counters, timestamps, locale, map
iteration, and discovery order are not inputs. Registration is therefore a
collect-then-finalize operation: the registry sorts the complete request set,
validates exact names, resolves generated-name groups, then seals. An identical
duplicate request is idempotent; conflicting facts for one semantic identity or
registration after sealing are `HXC9000` compiler invariant failures.

The registry models C's ordinary, tag, per-aggregate member, and per-function
label namespaces. Preprocessor names have an explicit separate category because
the later include/macro planner must reason about token replacement as well as C
identifier lookup. The same spelling is valid for a tag and a function, or for
members of two different structs. It is a collision only when both the spelling
and effective namespace/scope match.

## Generated defaults

Generated names use algorithm `hxc-c-symbol-v1`:

- compiler-private names begin `hxc_`;
- public defaults begin `hxc_api_`;
- defaults representing an imported external declaration begin
  `hxc_external_`;
- the prefix is followed by a role token and escaped qualified provenance;
- specialization arguments follow `_of_`; and
- anonymous ordinals use `_n<ordinal>`.

The public default is deterministic and inspectable, but an API author who wants
an idiomatic stable C spelling should use `@:c.name`. Public ABI compatibility
is not yet promised: E7 owns the final export model and E10.T09 owns pre-1.0 ABI
stabilization.

Qualified components are encoded as UTF-8 bytes. ASCII letters and digits are
retained except that `Z` becomes `zZ` and `z` becomes `zz`; underscore becomes
`zu`; every other byte becomes `zxHH`; components are joined with one
underscore. This keeps common Haxe paths readable while making underscores,
component boundaries, Unicode, and punctuation unambiguous. The compiler-owned
prefix isolates generated names from C keywords, implementation-reserved forms,
the `hxc_`/`hxrt_` namespaces, and standard-library future-name families. The
manifest records every applicable escape reason.

Overloads deliberately share the readable base. A collision group receives a
`_zh` plus a SHA-256 prefix derived from the complete canonical semantic key.
Finalization rechecks the resulting candidates across the whole effective C
namespace and expands colliding prefixes together until every name is unique,
so a hash-derived name cannot silently collide with another base or collision
group. Assignment is independent of which symbol was discovered first. Names
longer than 120 characters are shortened with the same stable hash marker. No
generated identifier begins with underscore or contains double underscore.

## Exact `@:c.name` values

An authored exact name is never sanitized, prefixed, truncated, or silently
disambiguated. It must be an ASCII C identifier and must not be:

- a C11 keyword;
- underscore-prefixed or contain double underscore (the conservative public
  C/C++ header rule); or
- in the compiler-owned `hxc_` or runtime-owned `hxrt_` namespace.

Invalid values fail with `HXC5002`. An exact-name collision also fails with
`HXC5002` and identifies every conflicting source symbol. Exact names may name a
real external library symbol intentionally; generated names remain isolated
from known standard-library reserved families. Later import/export and include
authority checks own whether an exact external spelling is valid for a specific
header and target ABI.

`TypedCContractMacro` applies the same exact-name rules at the originating Haxe
span and keys duplicates by C namespace. `TypedCNameFinalizer` then consumes the
schema-2 `TypedCContractSnapshot` structurally, preserves authored names,
supplies defaults, and hands a copied snapshot to `CDeclarationPlanner`.
Declaration and field order with layout meaning is retained; the unordered
declaration set is canonicalized by module path.

## Manifest contract and effects

Each schema-1 symbol record contains its canonical semantic key, readable source
symbol, role, C namespace/scope, visibility, exact/generated origin, requested
and finalized names, overload/specialization/ordinal facts, escape reasons, and
whether collision resolution was required. A separate collision ledger groups
the readable base with both source and resolved names.

Naming is a pure compile-time operation. It allocates no generated-program
object, selects no `hxrt` feature, and emits no C by itself. E2.T02 consumes its
finalized names after validated HxcIR for the admitted body fixture, without
changing those properties. Verify the contract with:

```sh
npm run test:symbol-registry
npm run snapshots:check
```

The golden covers packages, modules, types, fields, methods, locals,
temporaries, overloads, specializations, closures/environments, tables,
descriptors, reflection entries, static initializers, exports, and the
runtime-private role. That synthetic role coverage does not request or link the
runtime; a real compilation registers such a symbol only after the runtime
feature planner proves it is required.
