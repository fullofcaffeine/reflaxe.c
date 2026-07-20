# Deterministic C symbol naming

`CSymbolRegistry` is the target-owned naming boundary between normalized Haxe
semantics and `CIdentifier`. It assigns names as one per-compilation batch and
produces the structural schema-2 payload that the production emitter writes as
`hxc.symbols.json`. Typed functions, locals, fields, type tags, helper names,
and header guards all pass through this boundary. The focused
`test/symbol_registry` report is naming evidence; generated application C is
covered by the lowering and Caxecraft suites.

## Identity and determinism

A `CSymbolRequest` is identified only by semantic coordinates:

- symbol role and qualified Haxe name;
- the real C namespace and its semantic scope;
- normalized overload signature and generic specialization arguments; and
- a source-stable ordinal for temporaries, closures, and closure environments.

A request can separately carry `readableName`: the source words that should be
visible to a C reader. This is deliberately not semantic identity. For example,
a closed record can keep its full structural digest in `semanticKey` while its C
tag uses `hxc_caxecraft_domain_BlockCoord`. Changing only the display words does
not change `stableKey()`, but it does change the naming fingerprint so two
components cannot silently disagree about how one semantic symbol should look.

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

Generated names use algorithm `hxc-c-symbol-v2`. Its rule is simple: start with
the most recognizable source spelling, then add machinery only when C requires
it.

- Translation-unit compiler-private names begin `hxc_`, public defaults begin
  `hxc_api_`, and generic external defaults begin `hxc_external_`.
- A generated local or structure/union member keeps the short source word after
  the compiler ownership prefix: Haxe `value` becomes `hxc_value`, and record
  field `x` becomes `hxc_x`. A generated public member uses the corresponding
  `hxc_api_` prefix until the authored export-ABI policy gives it an exact name.
- Named types and functions use their readable package/type/member path without
  encoded compiler-role words. Compiler-created concepts with no source name,
  such as a vtable or closure environment, retain a short role word.
- Anonymous values retain their source-stable `_n<ordinal>` when the ordinal is
  needed to tell them apart.

ASCII letters, digits, and ordinary underscore-separated words remain readable.
Other punctuation becomes a word separator, while each non-ASCII UTF-8 byte is
represented by a compact `_xHH` component. These display conversions are not
used to prove identity: if two distinct source spellings normalize to the same
C base, the batch collision resolver sees both semantic keys and safely
disambiguates them.

Overloads and specializations deliberately share the readable base. A real
collision group receives `_h` plus a SHA-256 prefix derived from each complete
canonical semantic key. Ordinary collision-free names receive no digest.
Finalization rechecks the resulting candidates across the whole effective C
namespace and expands colliding prefixes together until every name is unique,
so a hash-derived name cannot silently collide with another base or collision
group. Assignment is independent of which symbol was discovered first. Names
longer than 120 characters are shortened with the same stable hash marker. No
generated identifier begins with underscore or contains double underscore. The
manifest records every escape and collision decision.

The short prefix on locals and members is a correctness boundary, not decoration.
The C preprocessor replaces macros before the compiler applies function or
aggregate-member scopes. A header is therefore allowed to break a bare local
named `bool` or member named `NULL`/`NAN`, even though those names would not
collide during ordinary C scope lookup. Keeping `hxc_` preserves the recognizable
source word while isolating generated names from current and future header
macros. The focused native probe includes `<stdbool.h>`, `<stddef.h>`, and
`<math.h>` and compiles generated `hxc_bool`, `hxc_NULL`, and `hxc_NAN` names
under strict C11.

The generated public default is deterministic and inspectable, but public ABI
compatibility is not yet promised: E7 owns the final export model and E10.T09
owns pre-1.0 ABI stabilization. Use `@:c.name` when an authored stable ABI or a
foreign library actually requires a different exact spelling, not merely to
repeat the Haxe name.

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

For header-owned `extern` declarations, an omitted `@:c.name` already means the
validated Haxe declaration or field spelling. If the C and Haxe names are both
`position`, write `position` once; add metadata only for a real difference such
as Haxe `fieldOfView` mapping to C `fovy`. This keeps raw bindings compact while
making every annotation carry information.

`TypedCContractMacro` applies the same exact-name rules at the originating Haxe
span and keys duplicates by C namespace. `TypedCNameFinalizer` then consumes the
schema-2 `TypedCContractSnapshot` structurally, preserves authored names,
supplies defaults, and hands a copied snapshot to `CDeclarationPlanner`.
Declaration and field order with layout meaning is retained; the unordered
declaration set is canonicalized by module path.

## Manifest contract and effects

Each schema-2 symbol record contains its canonical semantic key, diagnostic
source symbol, separate readable-name components, role, C namespace/scope,
visibility, exact/generated origin, requested/base/final names,
overload/specialization/ordinal facts, escape reasons, and whether collision
resolution was required. A separate collision ledger groups the readable base
with both source and resolved names. Thus a maintainer can recover the full
machine identity without forcing that identity into every C declaration.

Naming is a pure compile-time operation. It allocates no generated-program
object, selects no `hxrt` feature, and emits no C by itself. E2.T02 consumes its
finalized names after validated HxcIR for the admitted body fixture, without
changing those properties. Verify the contract with:

```sh
npm run test:symbol-registry
npm run snapshots:check
```

The golden covers source-shaped tags, prefixed readable fields and locals,
standard-header macro resistance, reserved-name escapes, genuine
overload/specialization collisions, readable-name/semantic-key separation,
packages, modules, methods,
temporaries, overloads, specializations, closures/environments, tables,
descriptors, reflection entries, static initializers, exports, and the
runtime-private role. That synthetic role coverage does not request or link the
runtime; a real compilation registers such a symbol only after the runtime
feature planner proves it is required.
