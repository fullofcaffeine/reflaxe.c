# Deterministic generic specialization

E3.T03 adds a bounded production path for closed generic static functions and
the already admitted generic enum values. Reachability is discovered from the
real pinned-Haxe typed call graph, each closed instance is lowered through
validated HxcIR, and equivalent instances share one structural strict-C11
definition. This is program-local monomorphization, not a public generic ABI or
general support for generic classes and containers.

## Closed input boundary

The current specialization boundary accepts only concrete arguments whose C
representation is already proven:

- non-null `Bool`, `Int`, `UInt`, and `Float`; and
- concrete admitted Haxe enum instances whose own arguments meet this rule.

Haxe typedef aliases are expanded before identity is computed, so an alias of
`Int` shares the same instance as `Int`. `Dynamic`, unresolved type variables,
classes and references, anonymous records, function types, nullable values,
native pointers, and other open or representation-dependent arguments fail at
the source call with exact `HXC1001`. No boxed or descriptor-driven fallback is
selected implicitly, and rejection leaves no plausible output.

Type arguments are inferred from the written typed call arguments first and,
when needed, from the compiler-resolved callee function type. A shorter direct
call is accepted only when every omitted declaration argument is optional; the
function layer then supplies its typed default. Repeated occurrences of the
same type parameter must resolve to the same canonical type. Rest, indirect,
and unresolved virtual or interface omission retain fail-closed boundaries.

## Semantic identity and sharing

The authoritative specialization key is a versioned, length-prefixed UTF-8
encoding of the base function ID and normalized type-argument keys. Length
prefixes keep component boundaries unambiguous without relying on punctuation
escaping. Primitive keys preserve their semantic representation (`bool`,
`i32`, `u32`, or `f64`); enum keys include the nominal Haxe path and recursively
normalized argument list.

The nominal path is the typed `pack + name` identity supplied by Haxe. For a
public secondary type, the pinned compiler omits the source-module name and
rejects another public type with the same package and name; for a private
secondary type, Haxe inserts its hidden `_Module` package segment. The key must
therefore preserve the typed path exactly and must not append a second,
target-owned module identity.

SHA-256 supplies the compact instance ID and the specialization suffix passed
to `CSymbolRegistry`. It is never treated as the identity by itself: every
request-local registry retains the full key and rejects a digest that maps to a
different key. Discovery order, checkout path, locale, aliases, and compiler-
server reuse therefore do not affect instance ownership or C names.

## Reachability and recursion

`CStaticFunctionGraphCollector` uses a request-local worklist keyed by the full
specialized instance ID. A first call creates one closed input; later equivalent
calls merge source-rooted reachability reasons. The instance is registered
before its body is scanned, so direct and mutually recursive calls revisit the
same work item instead of expanding forever. Initializer bodies participate as
ordinary graph roots.

The compiler admits at most 64 generic function instances and 64 generic enum
instances per build. The 65th new function instance fails at the expanding
call, while an expanding enum type graph fails at its constructor source, both
with source-positioned `HXC1001`. Finite nested instances such as
`GenericBox<GenericBox<Int>>` remain distinct and terminate normally;
non-stationary recursive argument growth reaches the same hard type/depth
budgets instead of recursing forever. These are correctness limits for the
current bounded slice, not silent heuristics that switch the program to
universal boxing.

## Report and code-size budget

Every successful build containing at least one specialization emits the
content-addressed non-payload sidecar `hxc.specializations.json`. Its schema is
[`generic-specialization-report.schema.json`](specs/generic-specialization-report.schema.json).
The report records:

- the full semantic key, its checked digest, final C name, normalized arguments,
  and every sorted source reachability reason;
- whether each function or enum instance participates in a recursive cycle;
- isolated strict-C11 function-definition bytes, hashes, and HxcIR block and
  instruction counts;
- structural constructor/payload units for enum layouts;
- conservative dependency-closed enum-definition bytes and total estimated
  specialization C bytes; and
- the complete generated payload artifact count and byte total.

The current build limit is 524,288 estimated specialization C bytes. Finalized
structural function definitions are attributed individually. Enum layout bytes
are intentionally conservative because their emitted definition block can
contain shared dependencies and assertions; the report says so instead of
pretending to have an exact per-type partition. The emitter revalidates report
identity, counts,
keys, digests, ordering, source normalization, reason totals, code-size
arithmetic, and payload totals before it accepts the project. The focused suite
also extracts every specialized definition from the emitted C and independently
recomputes its exact UTF-8 byte length and SHA-256 digest.

The sidecar is omitted when no generic instance is reachable. It is itself
listed and hashed in `hxc.manifest.json`, but it is not counted as generated C
payload and cannot select a runtime feature.

## Runtime and ABI effects

Closed specialization adds `closed-generic-specializations` to the direct
compiler decisions in `hxc.runtime-plan.json`. The admitted program remains
runtime-free: it contains no `hxrt` include, source, define, library, feature,
or symbol under `auto`, `minimal`, or explicit `none`. This follows the required
order of direct C, then program-local specialization, then a reasoned runtime
strategy only for semantics that truly need runtime state.

Specialized names are private implementation details. Exported generic values
or functions still require E7's explicit layout, ownership, naming, calling-
convention, and compatibility contract. Generic classes, general arrays and
containers, reference arguments, descriptor-driven shared bodies, boxing,
reflection, dynamic dispatch, closures, and cross-program specialization
stability remain outside this slice.

## Evidence

Run:

```sh
npm run test:generic-specialization
npm run test:function-lowering
npm run test:enum-lowering
npm run test:project-emitter
npm run test:all-sources
npm run snapshots:check
```

The focused suite proves alias sharing, distinct `Bool`/`Int`/`UInt`/`Float`
instances, an ordered two-parameter key, nested generic calls and enum
arguments, a finite same-key recursive function instance, shared inner and
outer generic enum layouts, and a runtime-free executable. It compares repeated
isolated roots, reversed typed-module discovery, an alternate locale, and a warm
compiler server before and after a rejected request, plus portable, metal, and
explicit runtime-none projects. A same-root non-generic replacement proves that
Reflaxe ownership removes the conditional sidecar and direct decision. Negative
fixtures assert exact source-positioned `HXC1001` for `Dynamic`, an unbound
phantom parameter, non-stationary recursive function and type growth at their
hard bounds, and the code-size threshold with no artifacts. The generated
project compiles and runs as warning-clean strict C11 at `-O0` and `-O2` under
each available identity-matching GCC or Clang family; required CI provides both
families.

The code-size negative uses the internal fixture-only
`reflaxe_c_test_generic_code_size_limit` define to lower the production bound
without manufacturing a half-megabyte source file. It must trigger the same
post-emission `HXC1001` path with no output. The define is not application
configuration, cannot raise the production limit, and deliberately fails
internally if a fixture sets it without crossing the requested threshold.
