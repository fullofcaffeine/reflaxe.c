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

All supplied target roots pin:

```json
{
  "version": "4.3.7",
  "resolveLibs": "scoped"
}
```

The C scaffold follows that exact initial contract.

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
3. adds the target `_std` root after `std/` so it wins effective Haxe 4 precedence;
4. loads `-lib reflaxe`;
5. defines a development library version;
6. calls bootstrap and init macros.

`extraParams.hxml` avoids relative `-cp` entries because those resolve relative to the consumer project.

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

### Ruby lesson

The Ruby PRD explicitly rejects cargo-culting a third profile without a distinct tested contract. It defines `ruby_first` and `portable`, while insisting both emit idiomatic Ruby.

### C conclusion

C needs `portable` and `metal`. It does not need a separate `idiomatic` mode because idiomatic C is mandatory in both. Hosted/freestanding and runtime selection are orthogonal policies.

## Runtime analysis

The Rust and Go repositories contain concepts such as:

- `RuntimeRequirementAnalyzer`
- `HxrtFeatureAnalyzer`
- `NoHxrtEligibilityAnalyzer`
- feature reasons and reports
- selective runtime copying
- explicit no-runtime conflicts

C adopts the same pattern and strengthens it with allocator, GC, exception, reflection, thread, and platform feature groups.

## Boundary enforcement

The supplied targets distinguish:

- repository examples/snapshots that must not use raw target injection;
- optional strict user policy;
- typed target facade APIs;
- compiler-owned injection helpers.

C uses `BoundaryEnforcer`, `StrictModeEnforcer`, and a typed `c.Syntax`/`c.Unsafe` surface.

## Standard library

Observed patterns:

- `std/` for target-facing APIs;
- `std/<target>/_std` for upstream-colliding overrides;
- runtime and source-owned shim classification;
- compatibility/parity documents;
- provenance and third-party notices;
- upstream sweep plans.

C adds a machine-readable implementation ledger and per-API runtime/platform ownership.

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
