# Deterministic generic specialization

E3.T03 adds a bounded production path for closed generic functions and the
already admitted generic enum values. It covers static calls and instance calls
whose exact method body is known, such as a method on a `final` class.
Reachability is discovered from the real pinned-Haxe typed call graph, each
closed instance is lowered through validated HxcIR, and equivalent instances
share one structural strict-C11 definition. This is program-local
monomorphization, not a public generic ABI or general support for generic
classes and containers.

## Closed input boundary

The current specialization boundary accepts only concrete arguments whose C
representation is already proven:

- non-null `Bool`, `Int`, `UInt`, and `Float`;
- concrete admitted Haxe enum instances whose own arguments meet this rule; and
- closed anonymous records whose fields have an admitted direct representation.

While canonicalizing a closed record, the compiler also accepts an ordinary
non-`@:coreType` Haxe abstract when its underlying carrier already satisfies
the same boundary. For example, a field typed as `LogicalPath`, declared as
`abstract LogicalPath(String)`, keeps `LogicalPath` in the specialization key
and uses the admitted immutable String view as its storage carrier. Haxe has
already typed constructors, conversions, operators, and inline methods before
this pass; this rule preserves the resulting value representation rather than
re-running those behaviors. An unsupported carrier such as a class reference
remains an exact source-positioned failure, and a nominal name never makes it
legal.

Haxe typedef aliases are expanded before identity is computed, so an alias of
`Int` shares the same instance as `Int`. `Dynamic`, unresolved type variables,
classes and references, open records, function types, unsupported nullable
values, native pointers, and other open or representation-dependent arguments
fail at the source call with exact `HXC1001`. No boxed or descriptor-driven
fallback is selected implicitly, and rejection leaves no plausible output.

Type arguments are inferred from the written typed call arguments first and,
when needed, from the compiler-resolved callee function type. A shorter direct
call is accepted only when every omitted declaration argument is optional; the
function layer then supplies its typed default. Repeated occurrences of the
same type parameter must resolve to the same canonical type. Rest, indirect,
and unresolved virtual or interface omission retain fail-closed boundaries.

The resolver also preserves Haxe's ordinary non-null-to-nullable argument rule.
For example, a `Marker` value can be passed to a parameter declared as
`Null<Marker>`. The generic resolver compares `Marker` with the nullable
parameter's payload while HxcIR body lowering still emits the explicit
nullable injection. This division matters: the resolver recovers type
arguments from a call Haxe has already accepted; it does not erase the
representation step required by C.

An instance method is specialized only when dispatch is statically direct:
the compiler must know that the call cannot select an override at runtime.
Each specialization keeps the receiver parameter and uses the same full
semantic key, worklist, recursion budget, code-size budget, and collision
checks as a static function. Generic virtual and interface methods remain
fail-closed because specializing their dispatch-table slots needs a separate
ABI design; this slice does not guess one.

## Semantic identity and sharing

The authoritative specialization key is a versioned, length-prefixed UTF-8
encoding of the base function ID and normalized type-argument keys. Length
prefixes keep component boundaries unambiguous without relying on punctuation
escaping. Primitive keys preserve their semantic representation (`bool`,
`i32`, `u32`, or `f64`); enum keys include the nominal Haxe path and recursively
normalized argument list. A transparent record-field abstract key includes its
nominal Haxe path, closed abstract arguments, and normalized carrier key. Two
abstracts can therefore share the same C spelling without sharing a generic
specialization accidentally.

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
- exact reachable specialized-enum declaration/assertion bytes and total
  estimated specialization C bytes; and
- the complete generated payload artifact count and byte total.

The current build limit is 524,288 bytes, or 512 KiB, of estimated specialized
C **source text**. This is not a limit on the program's memory, final executable
size, or game content. It is a cheap, repeatable early warning for code
explosion: a recursive generic pattern could otherwise keep asking haxe.c to
create new concrete C functions or types until the generated project overwhelms
the C compiler. The native C compiler may later remove or combine code, so this
source-text measurement is deliberately a safety estimate rather than a
prediction of machine-code size.

Compiler theory and practice justify having a finite guard because
specialization can multiply or recursively discover concrete instances. They do
not prescribe this exact number. The 512 KiB threshold is a conservative haxe.c
engineering policy introduced with the bounded specialization slice; the
repository does not claim that it came from the C standard, a universal
compiler rule, or a benchmark-derived optimum. It complements the separate
64-function and 64-type limits by catching a smaller number of unusually large
specialized definitions. Future measured workloads may justify revisiting the
number, but a change must retain a deterministic fail-closed runaway guard.

### Comparable safeguards in other compilers

haxe.c's exact source-byte formula is target-owned, but limiting recursive or
expanding compile-time work is established compiler practice:

- [GCC's `-ftemplate-depth`](https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html#index-ftemplate-depth)
  bounds recursively nested C++ template instantiations so endless recursion
  cannot consume the compiler indefinitely.
- [Clang's implementation limits](https://clang.llvm.org/docs/UsersManual.html#controlling-implementation-limits)
  separately bound template depth and compile-time evaluation steps.
- [Rust's compile-time limits](https://doc.rust-lang.org/reference/attributes/limits.html)
  include a recursion limit and a type-length limit that counts substitutions
  used to construct a concrete type during monomorphization. The latter is
  currently enforced only by a nightly compiler option, so it is useful
  precedent for the cost dimension rather than an identical stable policy.
- [GHC's optimization controls](https://ghc.gitlab.haskell.org/ghc/doc/users_guide/using-optimisation.html#cmdoption-fspec-constr-count)
  cap specializations per function, apply a specialization size threshold, and
  use a work counter scaled by program size to stop a diverging optimizer.

These examples support haxe.c's layered approach: stable instance identity,
finite discovery, count limits, and a separate work/size guard. They do not
make generated C text the only valid cost model or justify 512 KiB
specifically. A future revision can use a more formatting-independent
structural cost—such as HxcIR instruction and type units—while retaining emitted
C bytes as observable evidence and preserving a hard termination boundary.
That calibration is tracked by Beads issue `haxe_c-5sd.11`.

### Why this is not a universal Reflaxe rule

The guard belongs to haxe.c's current lowering strategy, not to Reflaxe itself.
C11 has no language-level generics, so haxe.c creates concrete C definitions
for the closed type combinations it admits. The target must discover those
definitions, avoid creating the same one twice, and stop a recursively growing
set before it exhausts compiler resources.

Sibling targets face different pressure in the checked repositories:

- `haxe.rust` normally emits Rust generic parameters, bounds, and type
  arguments. Rust's compiler performs the later native monomorphization and
  owns its recursion/type-complexity limits. Reflaxe.Rust does perform bounded
  typed substitution for concrete inherited surfaces, but it does not maintain
  a haxe.c-style reachable-copy byte budget.
- `haxe.elixir.codex` and `haxe.ruby` target dynamically typed runtimes. Their
  current compilers do not generally emit a separate target function for every
  Haxe generic argument combination, so the same code-copy worklist does not
  arise.
- `haxe.go` emits concrete typed helpers for selected native `go.Chan`,
  `go.Slice`, `go.Map`, and `go.Result` call sites. The current implementation
  deduplicates those already-observed concrete types in small target-owned maps;
  it has no recursive general-generic discovery loop or equivalent emitted-byte
  budget. Broader specialization would need its own measured guard rather than
  copying haxe.c's number.
- Reflaxe.OCaml/hxhx mostly preserves target polymorphism or specializes
  bounded source declarations and selected hot paths. Its current compiler has
  no equivalent whole-program generated-copy budget.

This comparison is deliberately about the checked implementations, not a claim
that dynamic, Rust, Go, or OCaml targets can never need resource limits. Each
compiler should add the smallest guard justified by the work it actually owns.

Caxecraft exposed an error in the first version of this measurement. Its real
generic function and type counts stayed fixed, but adding ordinary game enums
made the reported estimate cross the limit. The report had counted the complete
program-wide enum block whenever any generic enum existed. Schema 2 now
attributes finalized function definitions individually and counts only the
forward declarations, complete definitions, and layout assertions for reachable
generic enum instances. Nested generic payloads are already members of that
closed specialization set and are counted once; unrelated ordinary enums are
not charged to the specialization budget. The emitter revalidates report
identity, counts, keys, digests, ordering, source normalization, reason totals,
code-size arithmetic, and payload totals before it accepts the project. The
focused suite also extracts every specialized definition from the emitted C and
independently recomputes its exact UTF-8 byte length and SHA-256 digest.

The sidecar is omitted when no generic instance is reachable. It is itself
listed and hashed in `hxc.manifest.json`, but it is not counted as generated C
payload and cannot select a runtime feature.

## Runtime and ABI effects

Closed specialization adds `closed-generic-specializations` to the direct
compiler decisions in `hxc.runtime-plan.json`. Specialization itself adds no
runtime machinery. The primitive/enum reference fixture therefore remains
runtime-free under `auto`, `minimal`, and explicit `none`. A specialized record
still composes the already-proven plan of its fields: the focused
`LogicalPath(String)` fixture selects only the existing static String headers,
not a generic box or descriptor. This follows the required order of direct C,
then program-local specialization, then a reasoned runtime strategy only for
semantics that truly need runtime state.

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
outer generic enum layouts, and a runtime-free executable. The same source
uses a final class to prove direct generic instance methods at several concrete
types, including a type inferred only from the expected return type. It also
passes a non-null record to a nullable method parameter, proving that
specialization and body conversion agree. A separate ordinary Haxe fixture
carries `LogicalPath(String)` in a closed record through a generic identity
call. It runs on Eval, compiles through validated HxcIR, repeats
byte-identically, emits readable `hxc_AssetRecord`/`hxc_assetPack` C in split,
package, and unity layouts, and executes as warning-clean strict C11. A paired
abstract-over-class-reference record fails at the field and leaves no
artifacts.

The suite also compares repeated isolated roots, reversed typed-module
discovery, an alternate locale, and a warm compiler server before and after a
rejected request, plus portable, metal, and explicit runtime-none projects. A
same-root non-generic replacement proves that Reflaxe ownership removes the
conditional sidecar and direct decision. Other negative fixtures assert exact
source-positioned `HXC1001` for `Dynamic`, an unbound phantom parameter,
non-stationary recursive function and type growth at their hard bounds, and the
code-size threshold with no artifacts. The generated reference project
compiles and runs as warning-clean strict C11 at `-O0` and `-O2` under each
available identity-matching GCC or Clang family; required CI provides both
families.

The code-size negative uses the internal fixture-only
`reflaxe_c_test_generic_code_size_limit` define to lower the production bound
without manufacturing a half-megabyte source file. It must trigger the same
post-emission `HXC1001` path with no output. The define is not application
configuration, cannot raise the production limit, and deliberately fails
internally if a fixture sets it without crossing the requested threshold.
