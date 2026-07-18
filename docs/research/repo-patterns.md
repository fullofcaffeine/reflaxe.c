# Extracted repository patterns

This document records the concrete patterns extracted from the supplied Repomix snapshots. It is descriptive evidence for the scaffold; `docs/PRD.md` is the normative product contract.

## Inputs inspected

- Haxe-to-Rust target snapshot
- Haxe-to-Elixir target snapshot
- Haxe-to-OCaml/HxHx snapshot
- Haxe-to-Ruby target snapshot
- Haxe-to-Go target snapshot (additional family evidence)
- Reflaxe framework snapshot
- Haxe compiler snapshot, especially the C++ generator and `std/cpp`

## Family-level conventions

### Repository root

Repeated files and directories include:

```text
.haxerc
.gitattributes
.gitignore
.gitleaks.toml
.github/workflows/
.beads/
AGENTS.md
CHANGELOG.md
README.md
SECURITY.md
extraParams.hxml
haxe_libraries/
haxelib.json
package.json
src/
std/
runtime/
examples/
test/
scripts/
docs/
```

Not every target has every directory, but this is the stable family shape.

### Haxe pin

The supplied sibling target roots pin:

```json
{
  "version": "4.3.7",
  "resolveLibs": "scoped"
}
```

The C scaffold initially followed that contract. Its Haxe 4 experiment exposed
a target-family difference: Cross installs unremovable UTF-16 platform facts,
while C's accepted String contract is scalar-indexed UTF-8. The production C
scaffold therefore pins the official `5.0.0-preview.1` release and uses its
custom-target platform hook. The divergence is narrow, exact, and
compile-tested rather than a general Haxe 5 compatibility claim.

For the M0 implementation, Lix is pinned to npm artifact `17.0.2` and Reflaxe
is vendored from commit `73a983112e039daad46b37912ab238df6bf0cf53`.
This follows the mature sibling-target source-checkout shape while adding an
independent content checksum and a temporary installed-package probe. The
vendored framework is compiler-only; it has no relationship to target runtime
feature selection.

### Typed frontend, target AST, and semantic IR layering

The inspected targets all begin from Haxe's typed compiler tree. `TypedExpr` is
therefore already the family-level high-level intermediate representation; none
of the sibling architectures supports a claim that Haxe source must first be
copied into a universal Reflaxe IR.

The current sibling checkouts use these broad shapes:

| Target | Observed pipeline | Boundary observation |
| --- | --- | --- |
| Go | `TypedExpr -> typed Go AST -> Go AST passes/import analysis -> printer` | The documented Go IR primarily owns structural target syntax and deliberately leaves portable/native admission to semantic policy. |
| Rust | `TypedExpr -> RustFile/RustExpr -> representation analyses and Rust AST transforms -> printer` | Rust's target AST can directly express more ownership, enum, result, and reference structure than C. |
| Ruby | `TypedExpr -> RubyFile/RubyExpr -> printer` | The fail-closed typed-expression switch and focused block/keyword analyzers are strong; the target AST remains small, so ordinary control and access forms still cross `RubyRawExpr`/`RubyRawStatement` boundaries inside a very large compiler. A richer Ruby AST and focused semantic plans are a clearer next step than a general CFG. |
| Elixir | `TypedExpr -> ElixirAST -> ordered pass registry -> printer` | A focused `LoopIR` is introduced only for loop analysis/emission strategy, and function-result invariants are checked between passes. Large builder/transformer stages and early `ERaw`/printed-AST boundaries remain candidates for focused plans and richer nodes. |
| OCaml | `TypedExpr -> OcamlExpr -> OCaml printer` | `OcamlExpr` is a structural target AST. The separate `hxhx` path has compiler-owned typed program models, while its current `GenIrProgram` is explicitly an alias rather than a normalized cross-backend IR. |

The reusable family principle is not “every target needs HxcIR.” It is:

1. retain Haxe typing and source meaning at the `TypedExpr` boundary;
2. introduce target-owned structural AST nodes wherever later passes must
   inspect target syntax;
3. add a smaller semantic plan or sub-IR only when a demonstrated Haxe-to-target
   gap needs an invariant before syntax is chosen; and
4. fail explicitly instead of hiding an unsupported path in raw target text.

C has stronger pressure for a separate semantic lowering layer because strict
C11 combines weaker operand-order guarantees and undefined primitive behavior
with no built-in ownership, cleanup, exception, object, string, or collection
model. HxcIR is the current C-target response to that gap, not a proposal to
retrofit sibling compilers.

A future shared Reflaxe semantic layer remains plausible, but the strongest
sibling precedent is evidence-first extraction. At least two backends should
first demonstrate the same semantic transformation and differential behavior;
the shared model must describe observable Haxe meaning rather than target
syntax or runtime symbols; and every backend must remain free to choose its own
representation. The `hxhx` `GenIrProgram` documentation already states a
compatible rule: its alias should become a normalized IR only after two or more
backends prove the repeated transformation.

Implementation language is orthogonal to this layering. The `haxe.ocaml`
Reflaxe compiler is Haxe code that emits OCaml, and its use of `OcamlExpr` shows
that Haxe enums and pattern matching are sufficient for a typed target AST. A
hypothetical compiler implemented in OCaml could express lower plans with OCaml
variants or transformed typed trees, but the Haxe-to-target semantic gap—not
the host language—determines whether those plans are needed. See
[the HxcIR rationale](../hxc-ir.md#why-a-second-ir-when-haxe-already-has-one)
for the normative C-project boundary and admission guardrails.

### Reflaxe target metadata

Target manifests use:

```json
"reflaxe": {
  "name": "Target",
  "abbv": "xx",
  "stdPaths": [
    "std",
    "std/target/_std"
  ]
}
```

For C:

```json
"reflaxe": {
  "name": "C",
  "abbv": "c",
  "stdPaths": ["std", "std/c/_std"]
}
```

### Source-checkout library HXML

The self-referential target HXML consistently:

1. adds `src/`;
2. adds `std/`;
3. adds the target `_std` root after `std/` so it wins effective Haxe classpath precedence;
4. loads `-lib reflaxe`;
5. defines a development library version;
6. calls bootstrap and init macros.

`extraParams.hxml` avoids relative `-cp` entries because those resolve relative to the consumer project.

The base `reflaxe.hxml` must not retain upstream Reflaxe's automatic
`ReflectCompiler.Start()` macro. Target initialization owns that call, otherwise
the two-stage target sequence can double-start the framework.

## Two-stage ignition

### `CompilerBootstrap`

Concrete responsibilities observed:

- locate the library root from `Context.resolvePath`;
- add vendored Reflaxe only when not already resolved;
- detect packaged/flattened Reflaxe source layouts;
- emit a typed fatal diagnostic when the framework is unavailable;
- avoid trying to repair standard-library override precedence too late.

### `CompilerInit`

Concrete responsibilities observed:

- detect target build through `target.name` and/or stable output defines;
- guard against duplicate registration;
- resolve profile/build context;
- enable target-specific standard-library capability defines;
- install repository and user boundary macros;
- call `ReflectCompiler.Start()`;
- call `ReflectCompiler.AddCompiler(...)` once.

The original pinned C experiment exposed a family-level trap beneath that common shape.
On Haxe 4.3.7, an output-define Reflaxe compiler with no native Haxe target uses
the compiler's `Cross` platform configuration. `Cross` installs
`target.name=cross`, `target.utf16`/`utf16`, `target.sys`, and static facts
before initialization macros. Sibling targets may compensate with extensive
`_std` overrides, but that does not remove the global conditionals seen by
portable application source. C therefore retained this case as a negative
instead of copying output-define activation as proof of correct platform
semantics.

The exact Haxe 4 source authority used for that negative is
`src/context/common.ml` (`default_config` and `init_platform`) plus
`std/haxe/macro/Compiler.hx` and `Context.hx`: the first installs the Cross
facts, the second exposes `define` but no removal operation, and the latter
documents `getDefines()` as a non-mutable copy.

The accepted production resolution pins Haxe `5.0.0-preview.1` at revision
`2c1e544e0a2c7524ef4c8e103f1b0580362ea538`. Its custom-target protocol invokes
`c.Init.init()` before platform finalization, allowing
`Compiler.setPlatformConfiguration(...)` to establish static, scalar-Unicode,
environment-derived C facts. `CompilerInit` then derives the Reflaxe
`c_output` transport from that custom-target output. Re-run both the old Cross
audit and the Haxe 5 custom-target/API audit on every Haxe pin change.

The C target adopts:

```text
outputDirDefineName = c_output
fileOutputType = Manual
fileOutputExtension = .c
targetCodeInjectionName = __c__
ignoreBodilessFunctions = false
ignoreExterns = true
trackUsedTypes = true
trackClassHierarchy = true
```

## Per-build context

The supplied Go and Rust compilers demonstrate why build policy and analysis data belong in a `CompilationContext`:

- profile and target module names;
- runtime feature inference and reasons;
- native/metal boundary scans;
- lowering decision ledgers;
- selected pass names and reasons;
- optimizer counters;
- reflection plans;
- no-runtime eligibility.

The C context extends this with layout, ownership, ABI, include, target-triple, and undefined-behavior-related data.

## AST and pass architecture

Repeated target modules include:

```text
ast/<Target>AST.hx
ast/<Target>ASTPrinter.hx
ast/<Target>ASTTransformer.hx
naming/<Target>Naming.hx
passes/<Target>Pass.hx
passes/PassRunner.hx
passes/NormalizePass.hx
```

The Elixir target's very large transformer set demonstrates both the power and the maintenance cost of repairing target syntax after ad hoc lowering. For C, semantic sequencing and lifetime decisions should happen in a small normalized IR before C AST emission, reducing late printer repair passes.

## Profiles

### Rust and Go lesson

Portable and metal/native contracts are real when a target has ownership, representation, runtime, or ecosystem boundaries.

The current Rust design makes `idiomatic` a quality bar rather than a profile,
uses typed native surfaces to decide representation eligibility, records stable
semantic runtime-requirement reasons before emission, and applies both a
source/typed-AST no-runtime eligibility check and a final generated-code guard.
Its typed-DSL admission policy prefers ordinary Haxe, typed metadata/macros, and
typed facades before a constrained DSL or raw target code.

The current Go design separates its compatibility profile preset from native
authority, specialization, and fallback axes. Typed APIs/modules establish the
source boundary; selective runtime packaging does not change semantics. This is
a useful warning against making profile names carry every independent policy.

### Ruby lesson

The Ruby PRD explicitly rejects cargo-culting a third profile without a distinct tested contract. It defines `ruby_first` and `portable`, while insisting both emit idiomatic Ruby.

### C conclusion

C needs `portable` and `metal`. It does not need a separate `idiomatic` mode because idiomatic C is mandatory in both. Hosted/freestanding and runtime selection are orthogonal policies. Profiles may supply reviewable defaults: portable uses exact automatic slicing with an aggregate summary, while metal uses the narrow allowlist and source warnings. Typed `c.*` consumption remains an explicit source fact rather than a semantic change inferred only from a profile name.

## Runtime analysis

The Rust and Go repositories contain concepts such as:

- `RuntimeRequirementAnalyzer`
- `HxrtFeatureAnalyzer`
- `NoHxrtEligibilityAnalyzer`
- feature reasons and reports
- selective runtime copying
- explicit no-runtime conflicts

C adopts the same pattern and strengthens it with allocator, GC, exception, reflection, thread, and platform feature groups.

Unlike the current Go selective planner's compatibility baseline, C must not add
an unconditional runtime `core`: direct C and program-local specialization run
first, and a successful no-runtime proof means zero `hxrt` build artifacts or
symbols. Root semantic reasons are reported separately from transitive feature
edges so warnings stay actionable.

## Boundary enforcement

The supplied targets distinguish:

- repository examples/snapshots that must not use raw target injection;
- optional strict user policy;
- typed target facade APIs;
- compiler-owned injection helpers.

C uses `BoundaryEnforcer`, `StrictModeEnforcer`, and a typed `c.Syntax`/`c.Unsafe` surface.

## Haxe-first C authoring

The Rust typed-DSL rules generalize well to C. The target should prefer, in
order, ordinary Haxe declarations, typed `c.*` abstractions, validated
metadata/macros, a narrow typed DSL for a genuine syntax gap, and raw C only at
an explicit unsafe authority boundary.

For C this includes authoring as well as consuming headers: typed Haxe facts can
describe header groups, prototypes, structs/unions/enums, qualifiers, incomplete
types, linkage, calling conventions, visibility, layout, constants/assertions,
and build inputs. Haxe macros can reject invalid declaration graphs, ownership,
or layout contracts at the original source position. They complement rather
than replace C/C++ compilation, Clang-derived ABI facts, `_Static_assert`, and
compiled probes.

A DSL is not accepted merely for convenience. It needs typed inputs/outputs,
negative diagnostics, deterministic inspectable expansion, explicit
allocation/ownership/unsafe/runtime effects, and a concrete safety,
portability, or ergonomics benefit proportional to its surface area.

The M0 C seed applies two concrete sibling lessons without copying their target
semantics: Rust-style native façade types are compile-time/codegen contracts
rather than wrapper allocations, and Rust/Go-style metadata registries are
rebuilt from the current typed module set and sorted before reporting. C extends
that pattern with structural declaration dependencies, literal build facts,
opaque/by-value cycle checks, and an explicitly empty runtime-effect list. See
`docs/typed-c-authoring.md`; actual C layouts still require native authority.

## Standard library

Observed patterns:

- `std/` for target-facing APIs;
- `std/<target>/_std` for upstream-colliding overrides;
- runtime and source-owned shim classification;
- compatibility/parity documents;
- provenance and third-party notices;
- upstream sweep plans.

C adds a machine-readable implementation ledger and per-API runtime/platform ownership.

## Unicode target semantics

Haxe 4.3.7 deliberately permits target-specific string storage and non-BMP
indexing, while its Unicode iterator APIs normalize iteration to code points.
The Eval target is UTF-8 and a local `--interp` probe observed an astral
character length/code of `1`/`128512`, composed/decomposed lengths of `1`/`2`,
and an embedded-NUL length of `3`.

The Rust sibling independently codifies scalar-indexed portable strings and
warns target overrides not to copy UTF-16 surrogate walking or expose UTF-8 byte
offsets. That is the strongest family precedent for a native UTF-8 target: use
one scalar-indexed Haxe model, keep byte operations explicitly typed, and avoid
a second representation for `UnicodeString`. ADR 0004 adopts that contract for
C and adds deterministic malformed-input and `CString` boundary rules.

The structural M0 probe now confirms that the real Haxe 5 `CustomTarget(c)`
selects the upstream `String.hx`, `StringTools.hx`, and `UnicodeString.hx`
scalar branches with no UTF-16 helper/adaptor fields. It also snapshots the
target-owned platform configuration for hosted and freestanding environments.
Eval remains an independent oracle and future CLI bootstrap host, not a
user-program carrier. The Haxe 4 Cross branch remains rejected because its
UTF-16 flags cannot be removed through its public macro API.

## Output management

Reflaxe `OutputManager`:

- supports manual, single-file, per-module, and per-class modes;
- writes only changed files;
- tracks generated files in `_GeneratedFiles.json`;
- safely deletes stale generated files;
- supports arbitrary extra files.

C uses manual output but retains this ownership system for headers, sources, build files, runtime slices, and reports.

## Testing

The supplied targets repeatedly use:

- deterministic snapshots;
- fixture selectors;
- expected generated files;
- runtime execution;
- negative diagnostics;
- update/bless commands;
- timeouts and failed-state recording;
- profile and strict-boundary tests;
- standard-library sweeps.

C adds mandatory native compilation, strict warning gates, C/C++ header tests, sanitizer suites, ABI probes, and cross-compiler runs.

## hxcpp/Haxe C++ generator lessons

The attached Haxe compiler's C++ generator demonstrates:

- per-module header/source generation;
- deterministic cached writes;
- include guard generation;
- source/header dependency tracking;
- reserved-name remapping;
- generated registration and boot order;
- runtime-owned type separation;
- metadata-driven includes, native names, ABI/layout flags, and build code;
- a generated build manifest (`Build.xml`);
- resource embedding.

The C target should retain those project-organization lessons but replace C++ object/runtime assumptions with C structs, function tables, explicit initialization, and a tool-neutral manifest.

## C/C++ boundary conclusion

Generated C is easy to consume from C++ when headers use `extern "C"` guards. The inverse is not automatic: C++ member functions, overloads, templates, exceptions, and object-layout conversions require wrappers compiled as C++.

The product therefore promises:

- direct C consumption;
- first-class C ABI export;
- C++-friendly headers;
- deterministic C++ shim generation;
- no claim of direct arbitrary C++ ABI consumption.

## Beads

Supplied repositories contain Beads state and agent instructions. Current Beads uses a Dolt-backed issue graph and commands such as:

```text
bd init
bd ready --json
bd update <id> --claim
bd create ... --parent <epic>
bd dep add <child> <blocker>
bd close <id>
bd prime
bd dolt push
bd dolt pull
```

The C scaffold includes a deterministic plan file and bootstrap script. It deliberately does not fabricate database internals.
