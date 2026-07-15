# PRD: `reflaxe.c` / `hxc` — Haxe to idiomatic C

**Status:** Complete product and implementation specification over an existing M0 scaffold\
**Audience:** Project owner, Codex, compiler contributors, runtime contributors, interoperability contributors\
**Target repository:** `reflaxe.c`\
**User-facing compiler/CLI name:** `hxc`\
**Baseline Haxe compiler:** exact 5.0.0-preview.1 custom-target carrier\
**Baseline Reflaxe framework:** 4.0.0-beta-compatible API surface\
**Default C dialect:** strict hosted C11\
**Document date:** 2026-07-15\
**Current implementation state:** An M0 scaffold is already present; it is intentionally fail-closed and is not yet a functioning Haxe-to-C compiler.\
**Canonical product intent:** this file (`docs/PRD.md`)\
**Canonical reproducible execution seed:** `docs/specs/beads-plan.json`\
**Live execution state after import:** Beads (`bd`)\
**Scaffold inventory and gap analysis:** Section 32\
**Machine-translatable requirements catalog:** Section 33 and `docs/specs/requirements.json`\
**Machine-readable scaffold inventory:** `docs/specs/bootstrap-inventory.json`\
**Complete human-readable Beads seed backlog:** Section 30\
**Beads translation contract:** Section 36

This document is normative unless an accepted architecture decision record (ADR) supersedes a section. The words **MUST**, **SHOULD**, and **MAY** indicate requirement strength.

---


## 0. How Codex must use this PRD

This is a **brownfield bootstrap**, not a greenfield project brief. The archive already contains a deliberate M0 skeleton: target activation macros, profile/runtime policy types, a Reflaxe adapter, a fail-closed whole-program compiler entry point, a structural C AST and printer seed, a small runtime ABI seed, target-facing `c.*` abstractions, example seeds, and a deterministic Beads plan. Codex MUST inspect and extend those assets rather than generating an unrelated target from scratch.

The published Git checkout remains a partial extraction of that archive.
`rg --files` is authoritative for what is actually present. The typed `c.*`
contract seed, macro validator, minimal compiler adapter/context, generated-file
boundary, and fail-closed `CCompiler` are now present and compile-backed; broad
Haxe-to-C lowering, the complete runtime, CLI implementation, and many listed
assets are not. A named archive asset is a design constraint, not evidence that
the corresponding implementation exists or works in this checkout.

### 0.1 Required read order

Before changing code, Codex MUST read:

1. `CODEX_HANDOFF.md` for the archive contract, when present in the checkout;
2. `AGENTS.md` for non-negotiable implementation behavior;
3. this PRD completely, including Sections 31–40;
4. `docs/architecture.md` for compiler-layer invariants;
5. `docs/research/repo-patterns.md` for patterns extracted from the supplied target repositories and Haxe/hxcpp;
6. `docs/BEADS_PLAN.md` and `docs/specs/beads-plan.json` for the proposed execution graph;
7. the existing source and runtime files before proposing replacements.

The Repomix snapshots in `reference/repomix/` are evidence and precedent. They are not code to copy wholesale.

### 0.2 Status vocabulary

This PRD uses the following implementation-status terms:

- **Present:** the file or behavior exists and has been inspected in this scaffold.
- **Seeded:** an intentionally partial representation exists and should be extended.
- **Planned:** specified here but not implemented.
- **Provisional:** implemented or specified only to establish a direction and still requires an architecture decision record (ADR).
- **Deferred:** outside the current milestone but still in the product roadmap.
- **Rejected:** intentionally excluded because it conflicts with the product contract.

A present file is not evidence that its full capability is implemented. For example, the compile-verified declarator corpus in `CASTPrinter.hx` does not mean expression precedence or Haxe lowering semantics are complete.

### 0.3 Normative priority

When sources conflict, use this order:

1. an accepted ADR that explicitly supersedes a PRD decision;
2. this PRD;
3. architecture and configuration documents;
4. Beads issue acceptance criteria;
5. existing scaffold code;
6. examples and comments.

Existing code that contradicts a higher-level contract is a bug or provisional seed, not an implicit product decision.

### 0.4 PRD versus Beads

This PRD is the durable source of **why, scope, semantics, and acceptance policy**. Beads is the live source of **what is currently ready, claimed, blocked, discovered, or complete**.

Codex MUST NOT turn this document into a second task tracker. It should materialize the checked-in Beads graph, preserve stable plan keys, and create newly discovered work in Beads with `discovered-from` relationships. Section 36 defines the conversion and update process.

### 0.5 Existing scaffold preservation rule

Codex MUST NOT:

- run a Reflaxe “new target” generator over the repository root;
- replace the project with a generic C transpiler template;
- bypass `CompilerBootstrap`, `CompilerInit`, `CompilationContext`, `CReflaxeCompiler`, or the structural C AST without an accepted ADR;
- report success by keeping or weakening the scaffold's deliberate `HXC1000` failure;
- make examples compile through unrestricted raw C injection;
- import hxcpp's C++ runtime/object model as the default C representation;
- delete target abstractions merely because their lowering is not implemented yet.

The first implementation work is an audit and extension of the scaffold, not a reset.

### 0.6 Decision handling

Where this PRD records a recommended but unresolved decision, Codex should:

1. create or update the corresponding P0/P1 Beads decision issue;
2. write an ADR containing options, constraints, compatibility impact, and a recommended default;
3. proceed with reversible interface work when possible;
4. avoid hardening a public ABI or semantic promise until the decision is accepted.

Questions that do not block reversible implementation should not stop all work.

Accepted records currently ratify direct/selective runtime planning (ADR 0001),
Haxe-first typed C authoring (ADR 0002), the C11 target/platform baseline (ADR
0007), UTF-8 scalar strings (ADR 0004), the precise non-moving collector (ADR
0005), and contained exception unwinding (ADR 0006). Later implementation must
change those contracts through a superseding ADR, not by drifting code or
examples.

---

## 1. Executive summary

`reflaxe.c` is a Reflaxe-based Haxe target that emits readable, idiomatic, build-system-friendly C. Its command-line experience is named `hxc`.

The product thesis is:

> **Haxe's missing C target: typed, macro-powered C with a first-class C application binary interface (ABI).**

The compiler serves four related audiences:

1. **Haxe developers** gain direct access to the C ecosystem, embedded platforms, native libraries, operating-system APIs, WebAssembly toolchains, and stable foreign-function interfaces (FFIs).
2. **C developers** gain a safer source language with algebraic data types, generics, null safety tools, structural abstractions, a module system, and compile-time metaprogramming, while retaining transparent C output and explicit control over layout, ownership, allocation, and ABI.
3. **C++ developers** gain generated C libraries and headers that are straightforward to consume from C++, plus optional generated C++ convenience wrappers. Arbitrary C++ APIs still require a C shim; C cannot directly model classes, templates, overloads, exceptions, or C++ ABI details.
4. **TypeScript developers** get a familiar typed syntax and gradual path into native or WebAssembly programming through Haxe, without pretending that Haxe and TypeScript are identical languages.

The compiler has one lowering pipeline and two public semantic contracts:

- **`portable`** is the default. It prioritizes Haxe semantics, the complete Haxe standard library, and source portability to other Haxe targets. It still emits direct, idiomatic C whenever doing so preserves behavior.
- **`metal`** is the explicit C-first contract. It exposes pointers, layouts, ownership, allocators, calling conventions, and ABI boundaries. It rejects hidden runtime behavior unless the user opts into it.

Runtime selection is orthogonal to the profile:

- **`auto`** selects only required runtime slices.
- **`minimal`** permits a documented small runtime but forbids broad fallback features.
- **`none`** requires proof that the program can compile without `hxrt`; otherwise compilation fails with actionable diagnostics.

Profiles supply defaults without removing that orthogonality: `portable` resolves
to `auto` with an aggregate runtime summary, while `metal` resolves to `minimal`
and warns at each root runtime requirement. Any valid combination can be selected
explicitly, including runtime-free portable code and an explicitly runtime-using
metal build.

This is deliberately not “nice C versus correct C.” Every profile MUST produce idiomatic C. The difference is which source-level contract wins when Haxe semantics and direct C representation conflict.

The implementation strategy is:

```text
Haxe typed AST
  -> normalized Haxe semantic IR
  -> explicit-order / explicit-lifetime IR
  -> C AST with correct declarators
  -> ordered target passes
  -> deterministic C printer
  -> source/header/runtime/build/ABI artifacts
```

The compiler MUST lower to hand-written-looking C where semantics match. Runtime helpers are a selective implementation technique, not the default representation of every value.

C-native source surfaces follow the same philosophy. Haxe declarations, typed
`c.*` abstractions, metadata, and compile-time macros SHOULD express headers,
layouts, linkage, ownership, and build facts before a narrow DSL or raw C is
considered. The Haxe compiler is intentionally used for earlier, source-positioned
verification, while C compilers, Clang-derived facts, `_Static_assert`, and ABI
probes retain final native authority. See ADR 0001 and ADR 0002.

---

## 2. Evidence and patterns extracted from the supplied repositories

The attached repositories establish a recognizable target-family architecture. `reflaxe.c` SHOULD look and operate like a sibling, not an unrelated compiler project.

### 2.1 Shared repository pattern

The Rust, Elixir, OCaml, Ruby, and Go targets consistently contain most of the following:

- `.haxerc` pinning one reviewed Haxe version with scoped library resolution.
- `haxelib.json` with a `reflaxe` section declaring target name, abbreviation, and standard-library roots.
- `haxe_libraries/reflaxe.hxml` for a vendored or scoped Reflaxe dependency.
- `haxe_libraries/reflaxe.<target>.hxml` for source-checkout development.
- a current-working-directory-agnostic `extraParams.hxml`.
- `CompilerBootstrap.hx` and `CompilerInit.hx` as a two-stage target ignition sequence.
- a per-build `CompilationContext` instead of process-global mutable state.
- a target AST, deterministic printer, naming module, compiler contexts, analyzers, passes, and output emitter.
- target-facing `std/` APIs plus target-specific `_std` overrides.
- a runtime that is copied or generated only when required.
- examples, snapshots, runtime tests, negative tests, scripts, release metadata, security policy, and agent instructions.
- Beads project state and workflow guidance.

The C checkout now pins the official Haxe `5.0.0-preview.1` release at revision
`2c1e544e0a2c7524ef4c8e103f1b0580362ea538`, Lix `17.0.2`, and Reflaxe commit
`73a983112e039daad46b37912ab238df6bf0cf53`. The Haxe Linux, macOS, and Windows
release artifacts are checksum-locked. Reflaxe compiler sources are vendored
and content-hashed for source and staged-package reproducibility; this
compiler-only dependency is not part of `hxrt` or generated programs. Exact
metadata and the update procedure live in `docs/specs/toolchain-lock.json` and
`docs/toolchain.md`.

### 2.2 Two-stage compiler ignition

The supplied targets separate two concerns:

1. **`CompilerBootstrap.Start()`** runs early enough to resolve vendored Reflaxe sources and classpath-sensitive standard-library overrides.
2. **`CompilerInit.Start()`** detects the target, resolves policy, enables target defines, installs boundary macros, starts Reflaxe, and calls `ReflectCompiler.AddCompiler(...)` exactly once.

`reflaxe.c` MUST preserve this split. Standard-library precedence cannot be repaired reliably by a macro that runs after colliding modules have already been typed.

The original Haxe 4.3.7 experiment proved that phase separation cannot repair
compiler platform facts. Its default Reflaxe `Cross` carrier installs
`target.name=cross`, `target.utf16`/`utf16`, and generic platform capabilities
before initialization macros, while the public macro API has no undefine
operation. Changing `target.name` alone is forbidden and legacy Cross remains
an executable `HXC0003` negative.

The accepted production resolution is Haxe 5's custom-target hook. Direct builds
use `--custom-target c=<output>`; Haxe invokes `c.Init.init()` before platform
finalization, and the target installs a static, scalar-Unicode configuration
whose `sys`, thread, and atomic facts come only from the resolved C environment.
`CompilerInit` derives Reflaxe's internal `c_output` transport from that output,
then registers the compiler exactly once. The lifecycle fixture retains this
real carrier and skips only the deliberately unimplemented lowering boundary.
Eval is a differential oracle, non-C isolation target, and future `hxc` host—
never the production carrier for a user program.

### 2.3 Reflaxe registration contract

The repeated registration fields are:

- `outputDirDefineName`
- `fileOutputType`
- `fileOutputExtension`
- `targetCodeInjectionName`
- `expressionPreprocessors`
- `ignoreBodilessFunctions`
- `ignoreExterns`
- `trackUsedTypes` where dependency-aware output is required
- `trackClassHierarchy` where dispatch or object layout requires it
- reserved target identifiers and target metadata validation

For C, the initial registration contract is:

```haxe
ReflectCompiler.AddCompiler(new CReflaxeCompiler(), {
  outputDirDefineName: "c_output",
  fileOutputType: Manual,
  fileOutputExtension: ".c",
  targetCodeInjectionName: "__c__",
  expressionPreprocessors: [],
  ignoreBodilessFunctions: false,
  ignoreExterns: true,
  trackUsedTypes: true,
  trackClassHierarchy: true,
  deleteOldOutput: true,
  reservedVarNames: CNaming.reservedWords()
});
```

`Manual` output is intentional because one Haxe compilation produces C sources, headers, runtime slices, build files, manifests, symbol maps, and ABI reports. All files still MUST be written through Reflaxe's `OutputManager` so unchanged files are not rewritten and stale generated files are removed safely.

### 2.4 AST-first compilation

The strongest supplied targets avoid concatenating strings while walking Haxe expressions. They lower into a target AST or intermediate representation, run ordered transformations, then print deterministically.

C makes this discipline mandatory because:

- declarations and declarators are structurally different;
- pointer-to-function, array-of-pointer, and pointer-to-array syntax is easy to print incorrectly;
- precedence and parenthesization are correctness concerns;
- C leaves evaluation order unspecified in many expressions;
- signed overflow, aliasing, alignment, lifetime, and union access can invoke undefined behavior;
- headers and sources must share canonical declarations;
- ABI layout must be audited independently of pretty printing.

### 2.5 Profile contracts, not style toggles

Rust and Go demonstrate portable/native or portable/metal contracts. Ruby demonstrates an equally important counter-pattern: do not create a third profile merely because another target has one.

For C, two contracts are justified because pointer ownership, layout, allocation, and ABI exposure create a real semantic boundary. A separate “idiomatic” profile is not justified: idiomatic C is required in both contracts.

### 2.6 Selective runtime planning

The Rust and Go repositories model runtime requirements explicitly and can report why a feature pulled in a runtime slice. `reflaxe.c` MUST do the same.

Every helper must have:

- a stable feature identifier;
- a reason record tied to a type/expression/source position;
- dependencies on other runtime features;
- generated-source and linker requirements;
- a `none`-mode eligibility rule;
- tests proving that unused runtime features are absent.

### 2.7 Purity and boundary enforcement

The supplied targets keep their own examples and snapshots honest by forbidding raw `__target__` escape hatches. Some also offer strict user mode.

`reflaxe.c` MUST provide:

- a repository `BoundaryEnforcer` that rejects `__c__` in examples, templates, tests, and standard-library surfaces except explicitly approved compiler fixtures;
- a user `StrictModeEnforcer` enabled by `-D reflaxe_c_strict`;
- typed target facades under `std/c/` for legitimate low-level operations;
- a narrow, explicitly unsafe escape hatch for cases that cannot yet be modeled.

A missing feature should become a reusable compiler or library capability, not a copied raw C string.

### 2.8 Standard-library ownership and provenance

The supplied targets distinguish compiler-owned direct lowerings, source-owned target overrides, runtime helpers, and platform adapters. `reflaxe.c` needs a machine-readable standard-library ledger with, for each public Haxe API:

- implementation owner;
- semantic status;
- portable/metal availability;
- runtime feature requirements;
- platform restrictions;
- differential-test status;
- source provenance and license.

### 2.9 Snapshot and semantic test harness

The mature repositories use deterministic fixtures, generated-output snapshots, target compilation, runtime execution, negative diagnostics, update/bless workflows, selection, timeouts, and failed-test state.

For C, generated-text snapshots are necessary but insufficient. Every positive fixture MUST also compile with at least Clang and GCC under strict warning settings, and the relevant suites MUST run under sanitizers.

### 2.10 hxcpp patterns worth retaining

The built-in Haxe C++ generator and hxcpp runtime provide useful architectural lessons without dictating C output:

- split declarations into headers and definitions into source files;
- generate include guards deterministically;
- maintain a dependency graph for includes and type initialization;
- distinguish runtime-owned types from generated user types;
- mangle reserved identifiers consistently;
- define deterministic registration, boot, static initialization, and program-entry order;
- copy or emit resources reproducibly;
- separate generated code from runtime support;
- provide metadata-driven native includes, names, layouts, calling conventions, and build integration;
- emit a build manifest rather than making users reverse-engineer link requirements.

What MUST NOT be copied blindly:

- C++ object model assumptions;
- C++ templates, exceptions, overloads, references, constructors/destructors, or name mangling;
- hxcpp's runtime representation when a simpler C representation works;
- a build system coupled only to one runtime implementation.

### 2.11 Target-family conventions adopted by this PRD

`reflaxe.c` adopts the following family conventions:

- Exact Haxe 5.0.0-preview.1 custom-target pin for the first production
  compatibility contract; do not imply a broad Haxe 5 version range.
- vendored/scoped Reflaxe development configuration.
- `CompilerBootstrap` + `CompilerInit`.
- `CompilationContext` reset for every build.
- target AST and deterministic printer.
- ordered pass registry.
- profiles as semantic contracts.
- strict raw-injection boundaries.
- selective runtime analyzer and reports.
- target `_std` overrides with provenance.
- deterministic snapshots and target-native compilation.
- Beads as the execution graph, not a stale Markdown checklist.

---

## 3. Haxe macros versus Ruby metaprogramming

Haxe macros and Ruby metaprogramming are comparably powerful for several outcomes, but they operate at different phases and have different failure modes.

### 3.1 Ruby metaprogramming

Ruby metaprogramming is principally runtime and open-world. It can:

- reopen classes;
- create methods dynamically;
- intercept missing method calls;
- evaluate generated code;
- react to runtime values, load order, environment, and application state.

This makes Ruby extraordinarily flexible. It also means a program's effective shape can depend on runtime execution.

### 3.2 Haxe macros

Haxe macros execute during compilation. Depending on macro kind, they can:

- transform expressions;
- inspect parsed and typed syntax;
- inspect generic and resolved type information;
- add, remove, or rewrite class fields;
- build types;
- enforce policies with source-positioned errors;
- generate additional source, headers, manifests, schema files, or bindings;
- erase a high-level DSL into zero-cost target operations.

Haxe macros are stricter because phase separation and the compiler's type model constrain what they can observe and mutate. They cannot directly react to arbitrary runtime values. In return, they can make invalid states fail before C is generated and can produce deterministic artifacts.

### 3.3 Product implication for `hxc`

For DSLs, boilerplate elimination, schema generation, bindings, ABI wrappers, serialization, command routing, and compile-time validation, Haxe macros are at least as capable as the Ruby techniques commonly used to create Rails-like ergonomics. They are not a drop-in replacement for runtime `method_missing` or open classes.

The desired `hxc` style is therefore:

> Ruby/Rails-level source ergonomics where conventions help, but compile those conventions into explicit, statically auditable C rather than carrying a reflective framework into runtime.

This is especially valuable in C because macro-generated typed code can preserve a zero-cost or minimal-cost implementation while hiding repetitive pointer, cleanup, binding, and registration mechanics.

---

## 4. Product problem

Haxe has mature native routes, especially C++ through hxcpp, but it lacks a first-class target whose product contract is human-readable C, direct C ABI participation, explicit layout and ownership control, and easy consumption of C headers.

Existing options force one or more compromises:

- write C manually and lose Haxe's type system and macros;
- target C++ and accept C++ syntax, ABI, build conventions, runtime assumptions, and object model;
- use a higher-level target and introduce a separate FFI boundary before reaching C libraries;
- generate C through an unrelated backend not integrated with Haxe's standard library and target conventions;
- expose a native library through ad hoc wrappers without ownership, error, or symbol-version policy.

`hxc` addresses the gap by making C both a generated implementation language and a first-class interoperability contract.

---

## 5. Product goals

### G1. Idiomatic, inspectable C

Generated code SHOULD look like code an experienced C programmer could review, debug, profile, and patch mentally:

- conventional headers and source files;
- normal structs, enums, functions, loops, `switch`, and explicit temporaries;
- no universal boxed value when a concrete representation is known;
- direct calls when dispatch is statically known;
- localized runtime calls with descriptive names;
- stable formatting and deterministic symbol naming;
- comments or source maps that connect generated constructs to Haxe source.

### G2. Correct Haxe semantics

The `portable` contract MUST support the complete non-target-specific Haxe language and standard library, subject only to explicitly documented platform limitations. Semantic shortcuts are not acceptable merely to make output shorter.

### G3. Near-metal C programming

The `metal` contract MUST let users express:

- pointers and const pointers;
- structs, unions, enums, bitfields, packing, alignment, and flexible array members where supported;
- fixed-width integers and platform-sized integers;
- stack, static, arena, custom-allocator, and manually owned values;
- callbacks and context pointers;
- volatile and atomic access;
- calling conventions, symbol names, visibility, and sections;
- freestanding or embedded builds;
- precise C headers and ABI exports.

### G4. First-class C library consumption

Binding an existing C library MUST be deterministic, repeatable, reviewable, and dramatically easier than hand-writing extern boilerplate.

### G5. First-class library production

A Haxe module MUST be exportable as a stable C library with generated public headers, ownership and error contracts, symbol manifests, build metadata, and cross-language examples.

### G6. Minimal selective runtime

Direct lowering is preferred. A runtime helper is introduced only for a proven semantic gap, and the compiler MUST report why it is present.

### G7. Full standard-library support

The end-state is full Haxe standard-library support in the `portable` contract, backed by an auditable parity ledger and upstream conformance sweep.

### G8. Source portability

Code written only against portable Haxe and portable `hxc` libraries SHOULD compile on the user's other Haxe targets. C-only APIs MUST live under explicit modules/metadata and be easy to isolate.

### G9. Excellent diagnostics and automation

Unsupported features, ABI hazards, ownership ambiguity, undefined-behavior risks, and runtime-policy conflicts MUST fail early with specific diagnostics and suggested remedies.

### G10. Agent-friendly implementation

The repository MUST support long-horizon implementation by Codex and human contributors through Beads, small acceptance-testable tasks, deterministic scripts, architecture records, and machine-readable reports.

---

## 6. Non-goals

The following are not initial goals:

- Replacing hxcpp for every Haxe application or claiming C is universally superior to C++.
- Directly consuming arbitrary C++ headers as if they were C headers.
- Preserving Haxe reflection with zero metadata or zero runtime cost.
- Guaranteeing zero allocation for arbitrary portable Haxe programs.
- Making generated C identical to a particular human author's style.
- Supporting every compiler extension in the first release.
- Treating raw C injection as the normal interop mechanism.
- Making C preprocessor macros the primary high-level programming model.
- Implementing a new general-purpose C build system.
- Promising source-level compatibility with target-specific APIs from hxcpp, HashLink, JavaScript, or other backends.
- Self-hosting the Haxe compiler in C as a prerequisite for 1.0.

---

## 7. Positioning and audiences

### 7.1 For Haxe developers

`hxc` is the path to:

- C libraries with generated typed bindings;
- embedded and freestanding environments;
- operating-system and vendor SDKs;
- small native executables and shared libraries;
- WebAssembly through C toolchains;
- inspectable native code;
- a stable C ABI for use from many languages.

### 7.2 For C developers

`hxc` is “better typed C” only when the claim is made precisely:

- Haxe provides a richer static type system, modules, generics, algebraic enums, abstracts, null-safety tooling, and macros;
- `metal` keeps C's layout, ownership, and ABI visible;
- generated output remains C and is inspectable;
- unsafe operations are explicit rather than impossible;
- users can choose runtime-free subsets.

It is not a claim that Haxe erases every C concern. Layout, aliasing, ownership, concurrency, allocation, and ABI design remain real and must be modeled.

### 7.3 For C++ developers

C++ can include well-formed C headers and link to a C ABI naturally. Generated public headers MUST include `__cplusplus` guards. Optional C++ wrappers MAY add namespaces, RAII (resource acquisition is initialization) ownership, overloads, and lightweight classes on top of the stable C ABI.

C cannot directly call arbitrary C++ member functions, templates, overloads, or exception-throwing APIs. `hxc bindgen --language c++` therefore means “generate or consume a deterministic `extern "C"` shim,” not “pretend C++ is C.”

### 7.4 For TypeScript developers

Haxe syntax, classes, structural types, generics, and tooling can provide a familiar entry point. The bridge story SHOULD focus on:

- portable domain code;
- native libraries with generated TypeScript-facing FFI packages;
- WebAssembly builds with generated JavaScript/TypeScript glue;
- not on claiming source compatibility with TypeScript.

### 7.5 Honest comparison with hxcpp

`hxc` SHOULD be sold on differentiated strengths:

| Concern | `hxc` / C | hxcpp / C++ |
|---|---|---|
| Generated language | C | C++ |
| Primary ABI story | Stable C ABI | C++ runtime/object model, with CFFI options |
| C header consumption | Direct | Usually direct through C++ compatibility |
| Arbitrary C++ libraries | Requires C shim | Native fit |
| Embedded/freestanding reach | Product priority | Possible, runtime/toolchain dependent |
| Human-readable low-level output | Product priority | Generated C++ plus hxcpp runtime |
| Runtime-free subset | Explicit contract | Not the primary contract |
| Classes/templates/RAII | Lowered to C mechanisms | Native C++ mechanisms |
| Ecosystem maturity | New project | Mature and battle-tested |

The README MUST not demean hxcpp. hxcpp is evidence that Haxe can be a serious systems-language frontend. `hxc` offers a different substrate and ABI contract.

---

## 8. Product principles

1. **Correctness before cleverness.** Never rely on unspecified evaluation order or undefined behavior to save a temporary.
2. **Direct lowering before runtime fallback.** Use C's native construct when it represents the Haxe behavior exactly.
3. **Fail closed.** An unsupported construct produces a compiler diagnostic, not plausible but incorrect C.
4. **Profiles are contracts.** They are not formatter presets.
5. **Runtime is explainable.** Every included helper has a reason report.
6. **ABI is a product surface.** Public headers, ownership, errors, versions, and visibility are tested artifacts.
7. **Interop facts are parsed, not guessed.** AI can improve names and documentation; Clang-derived facts remain authoritative.
8. **Generated code is user-facing.** Stable names, comments, formatting, and debuggability matter.
9. **Portable source remains portable.** C-specific power is isolated in explicit modules or metadata.
10. **No raw-injection normalization.** Repeated raw code is a missing abstraction.
11. **Per-build state only.** Compiler daemons and cached builds must not leak state.
12. **Determinism is a feature.** Identical input and toolchain policy produce byte-identical generated artifacts.
13. **Haxe first, C explicit.** Use ordinary Haxe, typed `c.*` APIs, and validated metadata before inventing a DSL; use raw C only at a named unsafe boundary.
14. **Compile-time checks are layered.** Haxe macros catch source-contract mistakes early, while C compilers, Clang, static assertions, and ABI probes remain authoritative for native facts.
15. **Every abstraction earns its surface area.** A macro, metadata spelling, or DSL needs a documented correctness, portability, or ergonomics benefit and inspectable cost.

---

## 9. Public contracts and configuration

### 9.1 Profile define

```text
-D reflaxe_c_profile=portable|metal
```

Aliases MAY be accepted during development:

```text
-D reflaxe_c_portable
-D reflaxe_c_metal
```

Conflicting selections MUST be a fatal diagnostic. The canonical report always records `portable` or `metal`.

### 9.2 `portable` contract

`portable` is the default and MUST:

- preserve Haxe language and standard-library semantics;
- permit selective runtime features only after direct C and program-local specialization have been considered;
- prefer concrete C types and direct control flow;
- preserve source evaluation order explicitly;
- support garbage-collected object graphs where required;
- support exceptions, reflection, dynamic values, closures, and classes through direct lowering plus runtime slices;
- keep portable code compilable on other Haxe targets unless it imports `c.*` or uses C-specific metadata;
- reject ABI exposure that has no stable C representation unless an adapter is provided.

`portable` is not a runtime-heavy lane. A portable whole program that needs no
`hxrt` feature MUST emit no `hxrt` artifact, and the runtime plan records the
positive proof. Its default runtime policy is `auto`, with one aggregate runtime
summary when a feature is selected.

### 9.3 `metal` contract

`metal` MUST:

- make allocation and ownership behavior explicit;
- expose target-native C abstractions;
- allow no-runtime builds;
- reject hidden boxing, tracing allocation, reflection registration, or exception machinery unless opted in;
- allow exact layouts, names, visibility, sections, calling conventions, and native headers;
- permit direct use of C libraries and platform APIs;
- provide diagnostics when a Haxe construct would require a forbidden fallback;
- remain type-safe where safety is expressible and mark unsafe operations explicitly.

`metal` does not mean “all Haxe features disappear.” It means the compiler cannot silently add broad semantic machinery.

Typed `c.*` APIs, C externs, and validated C metadata define explicit native
source boundaries; selecting `metal` alone does not turn arbitrary portable Haxe
APIs into different APIs. The default metal runtime policy is `minimal`, and
root runtime requirements are warnings. Users who require a mechanically proven
zero-runtime artifact select `none`; users may explicitly select `auto` when they
accept its fully reported fallback costs.

### 9.4 Runtime policy

```text
-D hxc_runtime=auto|minimal|none
-D hxc_runtime_diagnostics=off|summary|warn
```

- `auto`: infer the exact dependency closure needed after direct C lowering and
  program-local specialization.
- `minimal`: apply the same inference, but allow only a versioned narrow feature
  allowlist; fail if broad machinery such as tracing collection, reflection
  registration, general dynamic dispatch, or general exceptions is needed.
- `none`: require a proven runtime-free program and emit/include/compile/link no
  `hxrt` artifact or symbol.

The runtime-diagnostic setting affects console presentation, never semantics or
packaging:

- `off`: keep evidence in the runtime plan without a console notice;
- `summary`: print one aggregate notice when `hxrt` is selected;
- `warn`: report deduplicated root semantic requirements at their source spans
  and keep transitive dependency edges in the plan rather than emitting warning
  spam.

Absent explicit overrides, the profile preset resolves `portable` to
`auto + summary` and `metal` to `minimal + warn`. Resolution provenance is
recorded. Policy conflicts remain errors regardless of diagnostic presentation.

Runtime selection MUST follow this precedence:

1. direct idiomatic C with the required semantics;
2. a compiler-generated, program-local specialization or helper;
3. the narrowest dependency-closed `hxrt` feature set;
4. a source-positioned policy diagnostic when fallback is forbidden.

The compiler MUST emit `hxc.runtime-plan.json` for every successful build,
including runtime-free builds. It includes feature IDs, stable root reason kinds,
source spans and consumed APIs, transitive dependencies, direct/native
representation decisions, copied/generated files, symbols, libraries, defines,
policy provenance, and the positive no-runtime proof or all blockers.

### 9.5 Environment preset

Hosted versus freestanding is an environment, not a semantic profile:

```text
-D hxc_environment=hosted|freestanding|wasi|emscripten
```

Initial support:

- `hosted`: required for the first usable releases and the full applicable
  portable standard-library promise on accepted Tier 1 platforms.
- `freestanding`: a capability-limited metal release lane; version 1.0 requires
  at least one declared ARM Cortex-M GCC profile with emulator or hardware
  execution evidence, not a hosted-standard-library claim.
- `wasi`: Tier 2 after hosted file/process abstractions are modular.
- `emscripten`: Tier 2 for browser/Node WebAssembly integrations.

### 9.6 C dialect and extensions

```text
-D hxc_c_standard=c11|c17|c23
-D hxc_c_extensions=none|gnu|msvc
```

Default: `c11` + `none`.

`c11` is the normative generated-source and public-header floor. `c17` compiles
the same contract without changing representation or ABI. `c23` is experimental
and MAY improve internal spelling only after compiler-matrix evidence; generated
public headers remain C11-compatible. Extensions MUST be recorded in the build
manifest. Portable standard-library code MUST not depend on nonstandard
extensions unless guarded by a platform adapter. See ADR 0007.

### 9.7 Optimization and debug policy

Compiler semantic output MUST not depend on optimization level. Build configuration MAY select:

```text
-D hxc_build=debug|release|minsizerel
-D hxc_line_directives
-D hxc_emit_comments
-D hxc_emit_source_map
```

Line directives and comments must not perturb symbol identity or ABI hashes.

---

## 10. Compiler architecture

### 10.1 Top-level pipeline

```text
CompilerBootstrap
  -> CompilerInit / policy resolution
  -> Reflaxe typed module collection
  -> source boundary and metadata validation
  -> Haxe semantic normalization
  -> type/layout/escape/runtime analyses
  -> HxcIR lowering
  -> C AST lowering
  -> C pass pipeline
  -> deterministic project emission
  -> optional C toolchain build
  -> reports and ABI verification
```

### 10.2 Why two intermediate layers

A direct Haxe AST to C AST mapping is tempting but insufficient for difficult semantics. `HxcIR` is a small normalized representation that makes the following explicit before C syntax is chosen:

- expression evaluation sequence;
- temporary storage;
- value category and addressability;
- ownership and lifetime intent;
- null representation;
- call dispatch kind;
- exception edges;
- cleanup edges;
- boxing/unboxing;
- allocation sites;
- source position;
- runtime feature requests.

The C AST then models syntax and declarations accurately. This division prevents C printer concerns from contaminating semantic analyses while avoiding an excessively general compiler framework.

### 10.3 Reflaxe adapter

`CReflaxeCompiler` extends `GenericCompiler` and is responsible only for:

- collecting selected Haxe classes, enums, typedefs, abstracts, and module types;
- resetting per-build state;
- invoking the C compiler pipeline at compile end;
- writing all generated artifacts through `OutputManager`;
- invoking an optional post-generation build after output completes.

The substantial compiler lives in testable non-Reflaxe components under `src/reflaxe/c/compiler`, `ast`, `analyze`, `passes`, and `emit`.

### 10.4 Compilation context

`CompilationContext` MUST be constructed once per compilation and contain:

- resolved profile, runtime policy, environment, C dialect, extension policy, build mode;
- target triple and pointer width when known;
- output mode and project naming;
- deterministic symbol table and collision ledger;
- type usage and hierarchy indices;
- layout cache;
- monomorphization registry;
- runtime feature plan and reason ledger;
- include/import registry;
- native library/link requirement registry;
- static initialization graph;
- export and binding registries;
- diagnostics and warning policy;
- pass selection and pass telemetry;
- source map entries;
- ABI surface model;
- standard-library ownership ledger view.

No mutable compiler state may survive into the next build.

### 10.5 HxcIR minimum model

The first stable HxcIR SHOULD contain:

- modules;
- types and type instances;
- global/static storage;
- functions with basic blocks;
- explicit local variables and temporaries;
- explicit sequencing;
- structured control flow plus normalized branch targets;
- calls classified as direct, virtual, interface, closure, native, runtime, or intrinsic;
- load/store/address operations;
- aggregate construction and projection;
- tagged-union construction and matching;
- allocation/deallocation/retain/trace intents;
- cleanup regions;
- throw/catch/result edges;
- source spans.

It should not attempt to become LLVM IR (intermediate representation). It exists only to make Haxe-to-C semantic decisions explicit and testable.

### 10.6 C AST requirements

The C AST MUST model:

- translation units;
- preprocessing directives as structured nodes where practical;
- includes and include categories;
- declarations, definitions, storage classes, qualifiers, attributes, and visibility;
- base type specifiers separately from declarators;
- pointers, arrays, functions, parenthesized declarators, and abstract declarators;
- structs, unions, enums, typedefs, static assertions, and forward declarations;
- initializers, designated initializers, and compound literals when enabled;
- statements, labels, loops, switch cases, and declarations in blocks;
- expressions with complete precedence and associativity metadata;
- comments and source mapping anchors;
- C11 atomics and `_Static_assert`;
- extension nodes that are gated by dialect policy.

Raw string nodes MAY exist only for validated compiler-owned intrinsics and build metadata, never as the normal lowering path.

### 10.7 Pass registry

Passes MUST be explicit, named, ordered, and independently testable. The initial registry is:

1. `SemanticValidationPass`
2. `ExplicitEvaluationOrderPass`
3. `ControlFlowNormalizationPass`
4. `GenericSpecializationPass`
5. `RepresentationSelectionPass`
6. `EscapeAndClosurePass`
7. `OwnershipAndCleanupPass`
8. `ExceptionStrategyPass`
9. `DispatchDevirtualizationPass`
10. `RuntimeFeatureFinalizationPass`
11. `CDeclarationPlanningPass`
12. `CHeaderDependencyPass`
13. `CStatementCleanupPass`
14. `CNameFinalizationPass`
15. `DeterminismValidationPass`

A pass report MUST record selected passes, reasons, timings, and important transformation counters.

### 10.8 Deterministic naming

Generated names MUST be stable across machines and independent of hash-map iteration. The scheme MUST cover:

- packages, modules, classes, enums, fields, methods, locals, temporaries;
- generic specializations;
- closures and closure environments;
- vtables and interface tables;
- type descriptors and reflection entries;
- static initialization functions;
- exported ABI symbols;
- runtime-private symbols.

Rules:

- user-visible exports use explicit `@:c.name` or a documented stable default;
- internal names use `hxc_` plus escaped qualified names;
- C keywords, implementation-reserved identifiers, and library-reserved names are escaped;
- identifiers beginning with double underscore or underscore plus uppercase are never generated;
- collisions are resolved deterministically and recorded in `hxc.symbols.json`;
- anonymous constructs use source-stable ordinals, not memory addresses.

### 10.9 Output layout

Default project output:

```text
out/
  include/
    hxc/<project>.h
    hxc/<module>.h
    hxc/runtime.h              # only when public runtime types are needed
  src/
    <module>.c
    hxc_boot.c
    hxc_exports.c              # only when exports exist
    hxc_reflection.c           # only when reflection exists
  runtime/
    include/hxc/*.h
    src/*.c
  cmake/CMakeLists.txt
  meson.build
  hxc.manifest.json
  hxc.runtime-plan.json
  hxc.abi.json
  hxc.symbols.json
  hxc.stdlib-report.json
  compile_commands.json        # after a supported build backend configures
  _GeneratedFiles.json         # Reflaxe ownership metadata
```

Single-file amalgamation MAY be supported later as an emission mode, but it must be generated from the same AST and manifest, not a separate compiler.

### 10.10 Header dependency strategy

The emitter MUST minimize includes and avoid cycles using:

- forward declarations for opaque or pointer-only references;
- stable public/private header separation;
- explicit complete-type dependency analysis;
- umbrella public headers;
- runtime headers split by feature;
- deterministic include sorting;
- generated include guards or `#pragma once` only under an explicit policy.

Public headers default to portable include guards.

---

## 11. C semantic baseline and undefined-behavior policy

### 11.1 Default baseline

C11 is the accepted normative baseline because it offers fixed-width types,
atomics, `_Static_assert`, alignment support, and broad toolchain reach without
requiring recent C23 support. Compiler-owned output, selected runtime slices,
fixtures, and public headers default to strict ISO C11 without extensions.

C17 is a compatibility selection over the same source contract. C23 MAY unlock
cleaner internal syntax after capability evidence, but generated semantics MUST
remain equivalent, public headers remain C11-compatible, and the selection must
not silently change ABI. Optional facilities such as threads remain
capability-gated rather than inferred from the language version. See ADR 0007.

### 11.2 Compiler tiers

Version 1.0 release-blocking tiers are tuples, not bare operating-system names:

- **Tier 1 hosted Linux/glibc:** `x86_64` and `aarch64`, GCC and Clang.
- **Tier 1 hosted macOS:** `arm64` and `x86_64`, Apple Clang.
- **Tier 1 hosted Windows/UCRT:** `x86_64`, `clang-cl`; public consumers use
  the MSVC ABI.
- **Tier 1 freestanding metal:** at least one release-declared ARM Cortex-M
  `arm-none-eabi` capability profile using GNU Arm Embedded GCC, limited to the
  documented `metal` plus `none`/allowed `minimal` surface.
- **Tier 2:** native MSVC `cl`, Linux/musl and additional architectures, WASI,
  Emscripten, and additional Clang freestanding profiles while their evidence
  remains non-release-blocking.
- **Tier 3:** embedded vendor compilers through documented capability profiles.

Hosted Tier 1 requires native compile/link/run and C/C++ consumer evidence;
eligible lanes also run sanitizers. Freestanding Tier 1 requires compile, link,
map/symbol inspection, and emulator or hardware smoke. Compile-only and
cross-compile evidence are recorded separately and cannot silently satisfy a
native-run obligation. See ADR 0007.

Exact version floors belong in a machine-readable support matrix and are updated
separately from this PRD. Unsupported combinations fail planning with their
missing capability rather than inheriting support from an OS label.

### 11.3 Warning policy

Generated code MUST compile under the project's strict warning baseline without warnings. The baseline SHOULD include equivalent checks for:

- all common warnings;
- extra warnings;
- shadowing;
- conversions and sign conversions in compiler-owned code;
- missing prototypes;
- strict prototypes;
- declaration-after-statement policy when targeting older compilers;
- undefined macros;
- format strings;
- implicit fallthrough;
- cast alignment and qualifier loss where supported.

Warnings from third-party headers are isolated with system-include policy rather than globally disabled.

This native warning baseline is separate from compiler policy diagnostics.
`hxc_runtime_diagnostics=warn` reports root semantic decisions that selected
`hxrt` even when the resulting C is warning-clean. Each warning MUST name the
feature, the source construct or API that required it, and a semantics-preserving
alternative when one exists. Transitive feature dependencies appear in
`hxc.runtime-plan.json` and do not each create another warning.

### 11.4 Undefined behavior discipline

The compiler MUST not intentionally depend on C undefined behavior. In particular:

- signed overflow is avoided through defined unsigned operations or checked helpers;
- shifts validate or normalize counts according to the Haxe contract;
- division edge cases are handled explicitly;
- evaluation order is serialized with temporaries when side effects could interact;
- aliasing is modeled conservatively;
- bit reinterpretation uses `memcpy` or a proven target intrinsic, not invalid pointer punning;
- alignment requirements are tracked;
- lifetime of stack values and closure environments is proven;
- null pointer arithmetic is forbidden;
- out-of-bounds access is either checked or explicitly unsafe under the metal contract;
- union access follows the selected representation and compiler policy;
- uninitialized reads are impossible in generated paths;
- data races are not introduced by code generation.

Every exception requires an ADR and a sanitizer/compiler test.

---

## 12. Language lowering requirements

This section defines the intended representation, not a license to skip
conformance tests. Each rule MUST be backed by differential fixtures against
the relevant Haxe reference targets and the exact pinned C custom-target
platform contract.

### 12.1 Primitive values

| Haxe surface | Default C representation | Notes |
|---|---|---|
| `Bool` | `bool` | Include `<stdbool.h>` under C11. ABI exports may use fixed integer forms when cross-language stability requires it. |
| `Int` | `int32_t` | Arithmetic helpers avoid C signed-overflow undefined behavior. |
| `UInt` | `uint32_t` | Preserve bitwise semantics explicitly. |
| `Float` | `double` | NaN, infinity, comparison, and conversion behavior require conformance tests. |
| `Void` | `void` | Expression-valued Haxe constructs are normalized before C emission. |
| `Null<T>` | representation-selected | Pointer null for reference-like values; tagged optional for non-nullable scalars; optimized away when proven non-null. |

The compiler MUST define and test conversion, comparison, division, modulo, shift, and overflow behavior rather than inheriting whatever a C compiler happens to do.

### 12.2 Evaluation order

Haxe expressions have source-level sequencing expectations that C does not always guarantee. Any expression with multiple side effects MUST be lowered into ordered statements and temporaries.

Example intent:

```haxe
arr[nextIndex()] += produce();
```

must not become a compact C expression that can call `nextIndex()` or `produce()` in an unintended order. The HxcIR first records each operation explicitly, then C emission uses address and value temporaries.

### 12.3 Strings

The portable string representation is an explicit runtime type, not a raw `char *`:

```c
typedef struct hxc_string {
  const unsigned char *data;
  size_t byte_length;
  /* representation flags / ownership / optional cached indexing metadata */
} hxc_string;
```

ADR 0004 accepts immutable, valid UTF-8 storage and Eval-like Unicode-scalar
indexing. `target.unicode` is enabled and `target.utf16` is not. `String.length`,
character access, slicing/search indices, and iterators count Unicode scalar
values, never UTF-8 bytes or UTF-16 surrogate halves. No normalization occurs.
Ordinary decoding replaces each maximal subpart of an ill-formed UTF-8 sequence
with U+FFFD;
checked target APIs return an explicit validation result, and lossless arbitrary
data remains `Bytes`.

This semantic decision survived the M0 carrier discovery. Haxe 4.3.7 `Cross`
selects the wrong UTF-16 branches and remains rejected rather than treated as
implementation evidence. The exact Haxe 5 custom-target production snapshot now
proves matching structural `String`, `StringTools`, and `UnicodeString`
branches; implementation work may rely on that contract but must still supply
behavioral and generated-C conformance tests.

Requirements:

- string literals are deduplicated where safe;
- embedded NUL bytes are supported;
- the private representation may cache scalar length or indexing metadata, but
  no allocation/index table is unconditional;
- C string conversion is explicit and lifetime-aware;
- `c.CString` is a separate null-terminated byte-string abstraction;
- safe `CString` conversion rejects embedded NUL and distinguishes borrowed
  trailing-NUL storage from allocator-owned conversion;
- substring, character access, iteration, comparison, hashing, parsing, and Unicode behavior have differential tests;
- metal code may use byte spans or C strings without pulling in portable string machinery.

The internal string layout is not a stable public C ABI. Exported interfaces use
explicit UTF-8 views or documented owned buffers.

### 12.4 Arrays and vectors

Concrete arrays SHOULD specialize to typed storage:

```c
typedef struct hxc_array_i32 {
  int32_t *data;
  size_t length;
  size_t capacity;
} hxc_array_i32;
```

Requirements:

- monomorphized element types where practical;
- generic boxed fallback only when representation cannot be specialized;
- explicit bounds policy;
- capacity growth overflow checks;
- element tracing/destruction metadata only when needed;
- slices/views represented separately from owning arrays;
- metal APIs expose `c.Span<T>` and allocator-aware vectors.

### 12.5 Anonymous structures

Closed anonymous structures SHOULD become named generated structs with deterministic field order and names. Open or reflective structures may require descriptors or dynamic maps.

Structural compatibility does not imply ABI compatibility. Public exports must use explicit named ABI structs.

### 12.6 Haxe enums

Haxe algebraic enums SHOULD become tagged unions:

```c
typedef enum hxc_result_tag {
  HXC_RESULT_OK,
  HXC_RESULT_ERROR
} hxc_result_tag;

typedef struct hxc_result {
  hxc_result_tag tag;
  union {
    value_type ok;
    error_type error;
  } payload;
} hxc_result;
```

Requirements:

- deterministic tag values;
- constructor payload layout;
- exhaustive switch lowering;
- direct stack representation when size and lifetime permit;
- boxed representation only for recursive, dynamic, or ABI reasons;
- reflection descriptors only when used;
- explicit ABI annotations before exposing enum layout publicly.

### 12.7 Classes and inheritance

Portable classes use explicit C object-model components:

- object struct;
- base-prefix layout where valid;
- type descriptor;
- vtable only when virtual dispatch is used;
- interface tables only for implemented/used interfaces;
- direct static call when target is known;
- devirtualization when hierarchy analysis proves a leaf target;
- opaque handles at stable ABI boundaries by default.

A likely layout is:

```c
typedef struct hxc_object_header {
  const hxc_type_desc *type;
} hxc_object_header;

typedef struct app_widget {
  hxc_object_header header;
  int32_t count;
} app_widget;
```

The exact header is runtime-policy-dependent. Runtime-free metal structs do not receive an object header unless requested.

Constructors and cleanup are ordinary generated functions. The compiler must not imitate C++ syntax.

### 12.8 Interfaces

Interfaces MAY lower through:

- direct calls when concrete type is known;
- per-interface itables (interface tables) containing function pointers;
- fat interface values containing object pointer plus itable pointer;
- monomorphized/static specialization in metal code.

Dispatch choice is a compiler reportable decision.

### 12.9 Generics

Default strategy:

1. monomorphize reachable concrete generic instantiations;
2. merge representation-identical specializations where proven safe;
3. use descriptor-driven generic bodies only when specialization would be unbounded or dynamic;
4. reject ABI exposure of compiler-private specialization names unless explicitly stabilized.

The compiler MUST control code-size explosion with reports and thresholds, never by silently boxing all generics.

### 12.10 Functions and closures

Non-capturing functions SHOULD lower to direct C function pointers.

Capturing closures use a pair:

```c
typedef struct hxc_closure {
  hxc_value (*invoke)(void *env, const hxc_value *args, size_t argc);
  void *env;
} hxc_closure;
```

Concrete typed closures SHOULD use specialized function signatures rather than universal `hxc_value` when possible.

Escape analysis determines whether environments are:

- elided;
- stack allocated;
- arena allocated;
- garbage-collected;
- manually owned under metal policy.

Callback interop SHOULD expose a function pointer plus `void *context` pattern.

### 12.11 Dynamic values

`Dynamic` and reflective unknown values use a tagged `hxc_value` only when required. It MUST not become the universal internal representation.

The value system must support the exact reachable categories, with optional feature slices for:

- scalar boxing;
- strings;
- arrays;
- objects;
- enums;
- functions;
- null;
- type values.

Operations on `Dynamic` require explicit runtime-feature reasons.

### 12.12 Reflection and runtime type information

Reflection is demand-driven:

- type descriptors are emitted only for used reflective operations or exported dynamic APIs;
- field name tables are emitted only when field enumeration/name lookup is used;
- constructors are registered only when dynamic construction is used;
- metadata is emitted only when requested;
- unused reflection machinery must disappear under dead-code elimination and runtime planning.

`hxc inspect reflection` MUST explain emitted descriptors and source reasons.

### 12.13 Exceptions

ADR 0006 accepts two implementations selected per function/region after HxcIR
has made exceptional successors and cleanup regions explicit:

1. **Result/status lowering** where a closed call graph and control-flow proof
   permit explicit propagation without semantic drift. This is the preferred
   runtime-free and metal strategy.
2. **Runtime exception frames** for general throw/catch/finally behavior. The
   selected strict-C11 mechanism is an encapsulated thread-local frame/cleanup
   chain using `setjmp`/`longjmp`.

Requirements:

- cleanup/finally semantics are preserved;
- no C++ exceptions;
- exceptions never cross a public C ABI;
- exported functions translate exceptions to status/error values;
- callbacks catch and translate before returning into foreign C, and a transfer
  never crosses a foreign frame or thread;
- metal defaults to explicit `c.Result<T,E>` or status returns;
- runtime `longjmp` usage is isolated and tested against automatic-local,
  `volatile`, active-frame, cleanup, payload-rooting, and lifetime rules;
- partially initialized values and `finally` actions clean up exactly once;
- exception strategy appears in compile reports.

### 12.14 Control flow

- Haxe expression-valued branches are assigned into temporaries.
- `switch` uses C `switch` only when representation and equality rules match; otherwise emit ordered conditionals or tag switches.
- loops use structured C where possible.
- labeled cleanup blocks MAY be used to centralize destruction and error paths.
- `break`, `continue`, and returns preserve nested Haxe behavior.
- no GNU statement expressions in strict mode.

### 12.15 Static initialization

The compiler builds a deterministic dependency graph and emits:

- module registration;
- type registration when needed;
- static field initialization;
- user entry point;
- library initialization and shutdown APIs when building a library.

Cycles MUST be detected and handled according to Haxe semantics or rejected with an exact cycle diagnostic. C's unspecified cross-translation-unit initialization is never relied upon.

### 12.16 Native integer and pointer abstractions

Target APIs provide distinct types for:

- `Int8`, `UInt8`, `Int16`, `UInt16`, `Int32`, `UInt32`, `Int64`, `UInt64`;
- `Size`, `PtrDiff`, `IntPtr`, `UIntPtr`;
- `Ptr<T>`, `ConstPtr<T>`, nullable pointer, function pointer;
- volatile and atomic wrappers;
- typed spans and C arrays.

Haxe `Int` remains semantically separate from native `int`.

### 12.17 Concurrency and atomics

Hosted portable support eventually includes Haxe threading APIs where the platform supports them. The C runtime supplies a platform abstraction over threads, mutexes, condition variables, thread-local storage, and atomics.

Metal code can use typed C11 atomic abstractions directly. Memory order must be explicit and type checked.

### 12.18 Platform APIs

`sys.*` support is split into capability modules:

- files and directories;
- process and environment;
- time and clocks;
- sockets and DNS;
- threads;
- standard input/output;
- dynamic libraries.

Each module declares hosted/freestanding/WASI/Emscripten support and has explicit fallback or unsupported diagnostics.

---

## 13. Memory management and runtime architecture

### 13.1 Runtime design

The runtime is named `hxrt` internally and uses `hxc_` public/internal symbol prefixes. It is a feature graph, not a monolith.

`hxrt` is selected only after representation, direct-lowering, escape, lifetime,
and specialization analyses have exhausted valid runtime-free C forms. The
planner MUST NOT add a universal `core` feature merely because a compilation
occurred. `core` is an ordinary dependency of features that actually need it;
an eligible build has no runtime headers, files, link inputs, defines, or
symbols.

Candidate feature groups:

- `core` — assertions, panic hooks, source positions;
- `alloc` — allocator interface and checked allocation;
- `string`;
- `array`;
- `object`;
- `gc`;
- `closure`;
- `dynamic`;
- `reflection`;
- `exception`;
- `thread`;
- `io`;
- `filesystem`;
- `process`;
- `socket`;
- `regex`;
- `date_time`;
- `unicode`;
- `export_error`.

Features declare dependencies in a machine-readable registry.

Each root request also declares a stable semantic reason kind, owning typed
surface, source span, and the policy that admitted it. Dependency-derived
features record their parent edge rather than pretending to be independent
source requirements. The deterministic plan distinguishes:

- direct C/native representation decisions;
- program-local specialized helpers;
- root runtime requirements;
- transitive runtime features;
- manual requests and their validation provenance;
- runtime-free proof blockers.

Source reasons include, where applicable, object identity/reference mutation,
cyclic managed graphs, shared closure cells, dynamic operations, reachable
reflection, general exception behavior, portable string/array semantics, and
real platform abstractions. Free-form “runtime needed” explanations are not
sufficient acceptance evidence.

### 13.2 Allocator interface

All compiler-owned allocation goes through an allocator contract:

```c
typedef struct hxc_allocator {
  void *ctx;
  void *(*alloc)(void *ctx, size_t size, size_t alignment);
  void *(*realloc)(void *ctx, void *ptr, size_t old_size, size_t new_size, size_t alignment);
  void (*free)(void *ctx, void *ptr, size_t size, size_t alignment);
} hxc_allocator;
```

The final signature may adapt for C portability, but requirements are:

- system allocator default for hosted builds;
- custom allocator injection;
- arena adapters;
- deterministic out-of-memory policy;
- overflow-safe size calculation;
- ABI-safe allocator handles when exposed.

### 13.3 Portable object graph memory

Full Haxe semantics permit cyclic object graphs, so reference counting alone is not a complete portable solution.

ADR 0005 accepts this baseline when tracing collection is actually selected:

- a target-owned, stop-the-world, precise, non-moving mark-and-sweep collector
  driven by generated type descriptors or specialized trace functions;
- explicit global tables and compiler-emitted stack/thread root chains rather
  than conservative scanning of arbitrary C memory;
- optional conservative collector adapter for platforms where desired;
- optional arena or manual representations for proven regions;
- no collector in runtime-free metal programs.

A non-moving collector simplifies C pointer stability and FFI, at the cost of
fragmentation, stop-the-world pauses, and descriptor/root work. Interior or
foreign pointers do not implicitly keep managed storage alive; they require a
typed root/pinned handle or remain borrows within a proven owner lifetime. The
backend is pluggable behind a versioned internal interface, but generated public
types and C ABIs never expose backend-private layout. Collector stress,
fragmentation, pause, and allocation benchmarks remain mandatory before beta.

### 13.4 Escape and region optimization

Even in portable mode, the compiler SHOULD avoid GC allocation through:

- scalar replacement;
- stack allocation of non-escaping aggregates;
- stack closure environments;
- arena regions for bounded temporary graphs;
- static allocation for immutable literals;
- move-like transfer into arrays/objects where semantics permit.

Optimizations must not alter observable identity, reflection, finalization, or exception cleanup.

### 13.5 Metal ownership vocabulary

Target APIs and metadata SHOULD support:

- `Owned<T>` — exactly one destruction responsibility;
- `Borrowed<T>` — non-owning lifetime-bound reference;
- `Shared<T>` — explicit shared ownership implementation;
- `ArenaRef<T>` — lifetime bound to an arena;
- `StaticRef<T>` — static lifetime;
- raw `Ptr<T>` — unsafe, no inferred ownership.

Haxe cannot express Rust's full borrow checker through ordinary typing. Macros and analyses can still catch common misuse and generate scope cleanup, but diagnostics must not overclaim formal guarantees.

### 13.6 Cleanup model

HxcIR carries cleanup regions. C lowering may use:

- direct lexical cleanup;
- a single `cleanup:` label per function;
- nested cleanup labels for partial initialization;
- generated destroy functions;
- allocator-specific cleanup;
- exception-frame cleanup hooks.

Public C APIs always document who frees returned data and which function performs the free.

---

## 14. Haxe standard-library strategy

### 14.1 Product requirement

The full cross-target Haxe standard library is a 1.0 requirement for the `portable` contract, excluding APIs explicitly documented as target-specific to another backend.

“Full” means:

- public types type-check;
- behavior is covered by upstream or equivalent tests;
- platform gaps are documented and diagnosed;
- no broad placeholder implementation silently returns incorrect data.

### 14.2 Ownership categories

Every API is assigned one of:

1. **Direct compiler lowering** — e.g. primitive arithmetic or array length.
2. **Thin C/OS wrapper** — e.g. selected math or file operations.
3. **Target source override** — Haxe implementation under `std/c/_std`.
4. **Runtime slice** — semantics require reusable C support.
5. **Platform adapter** — hosted, POSIX-like, Windows, WASI, embedded.
6. **Unsupported target-specific API** — explicit diagnostic.

### 14.3 Ledger

`docs/specs/stdlib-ledger.json` is the source of truth. `docs/specs/stdlib-ledger.csv` is a review-friendly generated view. Entries use these fields:

```json
{
  "type": "haxe.io.Bytes",
  "member": "getInt32",
  "owner": "runtime",
  "feature": "bytes",
  "profiles": ["portable", "metal"],
  "environments": ["hosted", "wasi", "emscripten"],
  "status": "planned|partial|conformant",
  "ownerBeads": "E5.T03",
  "tests": ["unitstd/..."],
  "notes": "..."
}
```

CI fails when an implemented override lacks a ledger entry or a conformant entry loses its tests.

### 14.4 Implementation order

1. primitives, `Std`, `Math`, basic `String`;
2. arrays, iterators, maps, lists, bytes, buffers;
3. exceptions, type/reflection essentials;
4. serialization, JSON, regex;
5. file and process APIs;
6. networking;
7. threads and synchronization;
8. date/time and platform details;
9. complete upstream sweep and gap closure.

### 14.5 Differential oracle

For behavior not fully specified by documentation, tests run the same portable Haxe source against selected reference targets and compare normalized results. Differences are recorded in ADRs; the C target must not accidentally inherit C behavior.

---

## 15. C interop and deterministic binding generation

### 15.1 Interop philosophy

C interop should feel like part of the compiler, not a separate handcrafted art. The workflow is:

```text
header + compile flags + target triple
  -> Clang semantic AST
  -> normalized ABI model
  -> deterministic Haxe externs/abstracts
  -> optional safe wrapper layer
  -> compile/link manifest
  -> generated binding tests
```

### 15.2 CLI

```text
hxc bindgen path/to/header.h \
  --module vendor.sqlite \
  --target <triple> \
  --include-dir ... \
  --define ... \
  --compile-commands compile_commands.json \
  --pkg-config sqlite3 \
  --output bindings/sqlite
```

### 15.3 Parser authority

The binding generator MUST use Clang's parsed type/layout information through LibTooling, libclang, or a versioned AST-dump adapter. Regex parsing is forbidden for ABI facts.

The parser inputs and version are hashed into `hxc.bindings.lock.json`.

### 15.4 Required C coverage

The generator must model:

- typedef chains;
- incomplete and opaque structs;
- complete structs and unions;
- enums and explicit values;
- arrays and flexible array members;
- pointers and const/volatile/restrict qualifiers;
- function prototypes and function pointers;
- callbacks with context pointers;
- variadic functions;
- calling conventions;
- packing, alignment, and attributes;
- bitfields;
- macro constants where Clang can evaluate them;
- inline functions and function-like macros through optional generated C wrappers;
- conditional compilation under the exact provided flags;
- platform-sized and fixed-width types;
- ownership/nullability annotations from known conventions or user mapping files.

Unsupported constructs fail with a precise list and source locations.

### 15.5 Generated Haxe layers

A binding package SHOULD contain:

1. **Raw extern layer** matching ABI names and types closely.
2. **Typed abstract layer** for handles, flags, spans, result codes, ownership, and nullability.
3. **Optional ergonomic wrapper layer** generated from explicit mapping configuration.
4. **Native shim C source** only where required for macros, inlines, or C++ adaptation.
5. **Binding manifest and tests**.

Raw generated files are deterministic and can be regenerated. User-written ergonomic wrappers live separately and are never overwritten.

### 15.6 Mapping configuration

A checked-in mapping file can specify:

- renamed Haxe modules/types/methods;
- handle ownership and destructor;
- borrowed versus owned return values;
- nullability;
- error-code conversion;
- string encoding;
- array length parameter relationships;
- callback lifetime;
- ignored declarations;
- wrapper generation.

The generator validates mappings against the parsed ABI model.

### 15.7 AI-assisted binding workflow

AI MAY:

- suggest idiomatic Haxe names;
- infer likely ownership from documentation and naming conventions;
- draft wrapper documentation and examples;
- propose mapping entries;
- identify suspicious signatures for review.

AI MUST NOT be the source of truth for sizes, alignments, field offsets, calling conventions, symbol names, constants, or parsed types. Suggested ownership is marked unverified until accepted by a human or deterministic annotation source.

### 15.8 C++ library consumption

`hxc` supports C++ through one of:

- an existing official C API;
- a user-supplied C shim;
- a generated C++ shim compiled by a C++ compiler and exposing `extern "C"` functions.

The shim generator must address:

- constructors/destructors through create/destroy functions;
- methods through handle-taking functions;
- overloads through distinct exported names;
- templates through explicit configured instantiations;
- exceptions through status/error translation;
- strings/containers through explicit ABI adapters;
- object lifetime through opaque handles.

It is a separate language adapter over a C ABI, not direct C++ code generation by the main backend.

---

## 16. Producing C libraries and shared objects

### 16.1 Export annotations

The M0 declaration vocabulary supplies the first five namespaced facts below;
E7 must admit and implement the ownership/error/version facts before accepting
those final proposed spellings:

```haxe
@:c.export
@:c.name("todo_store_open")
@:c.header("todo_store.h", c.Header.Public)
@:c.visibility(c.Visibility.Default)
@:c.callingConvention(c.CallingConvention.C)
@:c.owned                  // proposed E7 contract
@:c.borrowed               // proposed E7 contract
@:c.nullable               // proposed E7 contract
@:c.error("status")        // proposed E7 contract
@:c.since("1.0")           // proposed E7 contract
```

Metadata is validated centrally through the shared typed C contract model. E7
must extend its allowlist, diagnostics, snapshots, and admission record rather
than accepting unvalidated annotations locally.

### 16.2 ABI-safe public types

Allowed directly:

- fixed-width integers;
- `size_t`-modeled sizes where target-specific ABI is accepted;
- floating-point scalars with declared ABI assumptions;
- plain ABI structs/unions/enums with frozen layout policy;
- pointers to opaque handles;
- byte/string views with explicit length;
- callback pointer plus context pointer;
- status codes.

Not allowed without adapters:

- compiler-private class structs;
- garbage-collected raw object pointers with undocumented roots;
- Haxe `Dynamic`;
- portable Haxe arrays or strings without an ABI wrapper;
- exceptions crossing the boundary;
- unstable generic specialization types.

### 16.3 Generated artifacts

`hxc export` MUST produce:

- public C header;
- implementation wrapper source;
- symbol list;
- ABI JSON model including sizes/alignments/offsets when target-known;
- ABI hash;
- version script/export map/DEF file as platform appropriate;
- CMake package metadata;
- pkg-config metadata where appropriate;
- Meson dependency metadata where appropriate;
- static and shared library build targets;
- C consumer smoke test;
- C++ consumer smoke test;
- optional Python/Ruby/TypeScript binding seed generated from the C ABI.

### 16.4 Header C++ compatibility

Every public header uses:

```c
#ifdef __cplusplus
extern "C" {
#endif

/* declarations */

#ifdef __cplusplus
}
#endif
```

Optional generated `.hpp` wrappers are additive and never replace the C header.

### 16.5 Ownership contract

Every pointer-like parameter and result has one of:

- borrowed for call duration;
- borrowed with documented owner lifetime;
- transferred into callee;
- newly owned by caller;
- shared handle with retain/release;
- nullable or non-null.

Generated docs, header annotations, and ABI JSON agree. Missing ownership on an exported pointer is an error in strict ABI mode.

### 16.6 Error contract

Default exported function shape:

```c
hxc_status todo_store_open(
  hxc_string_view path,
  todo_store **out_store,
  hxc_error *out_error
);
```

Rules:

- no exception crosses ABI;
- status values are stable and versioned;
- optional structured error includes code, message view/owned message policy, and source category;
- thread-local “last error” is not the sole mechanism;
- partial outputs have documented state;
- cleanup functions are always generated for owned opaque handles.

### 16.7 Shared object targets

The build layer supports:

- Linux/Unix `.so`;
- macOS `.dylib`;
- Windows `.dll` plus import library;
- static `.a`/`.lib`;
- object library and amalgamated-source modes later.

Platform naming, visibility, and rpath behavior live in build adapters, not C semantics.

### 16.8 ABI compatibility testing

CI SHOULD include:

- header compilation as C and C++;
- exported symbol diff against a baseline;
- layout diff against a baseline for frozen structs;
- consumer binaries built separately from the library;
- old-consumer/new-library compatibility tests after the first stable ABI;
- ABI hash review on every release PR.

---

## 17. Haxe target abstractions, extensions, and macros

### 17.1 Core `c.*` API

M0-reserved modules:

```text
c.Syntax
c.Ptr<T>
c.ConstPtr<T>
c.NullablePtr<T>
c.Ref<T>
c.ConstRef<T>
c.RestrictPtr<T>
c.VolatilePtr<T>
c.FunctionPtr<T>
c.CString
c.StringView
c.Span<T>
c.ConstSpan<T>
c.CArray<T, N>
c.Int8
c.UInt8
c.Int16
c.UInt16
c.Int32
c.UInt32
c.Int64
c.UInt64
c.Size
c.PtrDiff
c.IntPtr
c.UIntPtr
c.Volatile<T>
c.Atomic<T>
c.Result<T, E>
c.Owned<T>
c.Borrowed<T>
c.Allocator
c.Arena
c.Header
c.IncludeKind
c.Layout
c.StaticAssert
c.Linkage
c.CallingConvention
c.Visibility
c.MemoryOrder
c.Unsafe
```

These are target-facing types, compile-time contracts, and compiler intrinsics,
not necessarily runtime objects. A `c.*` import is an explicit C-native source
boundary; it must not silently infect unrelated portable modules or require
`hxrt` merely because the abstraction exists.

The M0 implementations are intentionally method-free compiler contracts except
for the compile-time-only `c.StaticAssert.require`. They have no wrapper
allocation or runtime representation and no lowering yet. Empty `c.Lib`,
`c.Build`, or `c.Export` marker types are not admitted: build and export intent
uses validated declaration metadata until a concrete typed value API earns its
surface. `CArray<T, N>` currently reserves `N` as a type-level length witness;
the exact witness-generation ergonomics remain E3 work.

### 17.2 Typed declarations, headers, structs, and unions

C developers MUST be able to author C-facing declarations and header structure
from typed Haxe. ADR 0002 ratifies the namespaced M0 metadata vocabulary below;
public generated-ABI stability still belongs to E7 and E10. The surface MUST
cover:

- public/private header grouping and stable C names;
- imported extern declarations and exported Haxe declarations;
- prototypes, enums, structs, unions, opaque/incomplete handles, arrays,
  function pointers, bitfields, packing, and alignment;
- forward declarations and complete-type dependency checking;
- `const`, `volatile`, `restrict`, atomic, mutability, and ownership facts;
- internal/external/static/inline linkage, visibility, calling conventions,
  sections, and target capability gates;
- typed compile-time constants and assertions.

Representative intent:

```haxe
@:c.layout(c.Layout.Opaque)
@:c.name("widget_storage")
extern class WidgetStorage {}

@:c.layout(c.Layout.Struct)
@:c.header("widget.h", c.Header.Public)
@:c.name("widget_config")
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
```

The declaration planner derives prototypes, forward declarations, local/system
includes, public/private visibility, and dependency order from typed facts. Users
do not manually author include guards or repeat dependency ordering in strings.
External headers remain explicit metadata because they are build inputs, not
facts the Haxe type graph can invent.

Ratified M0 metadata examples:

```haxe
@:c.layout(c.Layout.Struct)
@:c.name("sqlite3_vfs")
extern class SqliteVfs { ... }

@:c.layout(c.Layout.Union)
extern class ValueUnion { ... }

@:c.pack(1)
@:c.align(8)
@:c.bitField(3)
```

Macros and compiler analyses validate invalid combinations, incomplete by-value
types, declaration cycles, duplicate/reserved symbols, target-dependent layout,
qualifier/ownership conflicts, and unsafe callback or pointer lifetimes at Haxe
source positions. Generated C also emits `_Static_assert` where a native compiler
must verify size, alignment, offset, or configuration facts. Clang and compiled
ABI probes remain authoritative for imported header layouts.

### 17.3 Native includes and link metadata

Typed APIs replace raw preprocessor strings:

```haxe
@:c.include("sqlite3.h", c.IncludeKind.System)
@:c.link("sqlite3")
@:c.define("SQLITE_THREADSAFE", 1)
@:c.pkgConfig("sqlite3")
extern class SQLite3 { ... }
```

The compiler aggregates and deduplicates these into the manifest and build files.

The M0 collector already normalizes these literal facts into a deterministic
typed snapshot and rejects malformed paths, names, values, duplicate explicit
symbols, opaque by-value use, and impossible by-value cycles with `HXC5002`.
It emits no files or C and explicitly reports an empty runtime-feature set. See
`docs/typed-c-authoring.md` for the precise implemented/provisional boundary and
macro admission record.

Haxe conditional compilation SHOULD resolve configuration before C emission
when possible. Function-like C macros SHOULD become typed inline functions or
generated wrappers when ABI-compatible. Raw preprocessor branches and macro
bodies are reserved for external/platform contracts that genuinely require
them; the C preprocessor is not the high-level authoring model.

### 17.4 Unsafe boundary

Unsafe operations require an explicit lexical marker or API, for example:

```haxe
c.Unsafe.run(() -> {
  ptr.writeUnchecked(0, value);
});
```

The exact syntax may change, but requirements are:

- unsafe use is searchable;
- strict mode can forbid it;
- reports count unsafe sites;
- raw `__c__` remains a lower-level escape hatch and is not needed for ordinary pointer or ABI work.

### 17.5 Extension methods

Static extensions SHOULD make low-level APIs concise without hiding ownership:

```haxe
using c.PtrTools;
using c.SpanTools;

var bytes:ConstSpan<UInt8> = ...;
var header = bytes.readStruct<Header>(0);
```

Compile-time macros validate size, alignment, bounds policy, and layout eligibility.

### 17.6 Compile-time schema and DSL generation

Haxe macros SHOULD power high-level ergonomic libraries that erase into ordinary C. Candidate first-party examples:

- command-line interface parser from typed command declarations;
- SQLite row/schema mapping;
- binary protocol structs;
- serialization/deserialization;
- finite-state machines;
- plugin registries;
- C export wrappers;
- header binding configuration.

Generated runtime reflection is not required when compile-time information suffices.

The compiler and standard library SHOULD deliberately use Haxe's type checker
and macro system for checks C often reports later or less locally: format/command
schemas, literal ranges, buffer-size relationships, enum exhaustiveness, layout
eligibility, ownership/nullability completeness, callback retention, atomic
ordering, header visibility leaks, and conflicting build facts. These checks
complement native compilation; they do not pretend Haxe can prove arbitrary C
ABI or undefined-behavior facts on its own.

### 17.7 Macro and DSL admission/testing

A first-party macro, metadata spelling, or DSL MUST have a documented reason to
exist. Prefer, in order: ordinary Haxe; typed `c.*` abstractions; validated
metadata/macros; a narrow typed DSL where Haxe has no clear native form; and only
then an explicit raw C boundary.

An admitted macro or DSL requires:

- typed inputs and outputs, or a constrained parser when text is the external
  format being modeled;
- a concrete correctness, portability, or ergonomics gain that justifies its
  maintenance and language-surface cost;
- source-positioned actionable diagnostics;
- deterministic, inspectable expansion and generated-declaration artifacts;
- explicit allocation, ownership, unsafe, portability, and runtime effects;
- no bypass around profile, runtime, environment, ABI, or raw-boundary policy;
- no stringly typed C mini-language disguised as a convenience API.

Every macro library needs:

- positive generated-source tests;
- negative diagnostic tests with source positions;
- determinism tests;
- cross-target typing tests when the source API is intended to be portable;
- no-runtime evidence where advertised.

If an implementation uses raw C internally, that remains a documented unsafe
authority boundary and application code receives a typed facade. Repeated raw
use is a missing abstraction to design, not a pattern to normalize.

---

## 18. `hxc` command-line product

`hxc` is an optional product-level orchestration layer, not a replacement Haxe
compiler and not a requirement imposed by Reflaxe. Direct Haxe/HXML invocation
MUST remain supported and authoritative. The CLI exists because C workflows
span native compiler/linker selection, sysroots and SDKs, build manifests,
Clang-backed binding generation, public-ABI export, runtime inspection, and ABI
verification without one universal ecosystem driver. It MUST expose the exact
commands and manifests it coordinates and MUST NOT become an opaque custom
build system.

The CLI has a two-stage bootstrap contract:

1. The development and recovery entry point runs the shared target-neutral Haxe
   CLI core on Haxe's built-in Eval target through `haxe --run Run` or
   `haxelib run reflaxe.c`. This path requires Haxe but no separate Neko,
   HashLink, or prebuilt `hxc` runtime.
2. After `reflaxe.c` supports the `sys`, string, configuration, process, error,
   and cleanup surface used by the CLI, the same core is compiled through
   `reflaxe.c` and a native C compiler into the packaged `hxc` executable.

The native artifact is a dogfood and release gate, not a circular bootstrap
dependency. A clean checkout MUST be able to build it using direct Haxe/HXML
invocation plus the selected C toolchain without an existing native `hxc`.
Eval MUST remain tested as a bootstrap/recovery path and as a differential
oracle for native CLI behavior. Command parsing, configuration, build planning,
diagnostics, and JSON output MUST be shared rather than reimplemented per host.

### 18.1 Commands

```text
hxc new <name> [--kind app|library|embedded]
hxc build [project.hxml] [--profile portable|metal] [--runtime auto|minimal|none]
          [--runtime-diagnostics off|summary|warn]
hxc run [project.hxml] [-- args...]
hxc test [selector]
hxc clean
hxc doctor
hxc inspect manifest|runtime|abi|symbols|stdlib|lowering|declarations|macros
hxc bindgen <header> [options]
hxc export [project.hxml] [options]
hxc fmt-generated
hxc version
```

The bootstrap can expose these through `haxelib run reflaxe.c`; packaged launchers MAY install the shorter `hxc` command.

### 18.2 Configuration

HXML remains the authoritative Haxe compiler input. An optional `hxc.json` MAY hold structured native build/export/bindgen settings, but it must not duplicate ordinary Haxe arguments invisibly.

All resolved configuration is emitted into `hxc.manifest.json`.

### 18.3 Build backends

The compiler emits tool-neutral source and link requirements, then adapters generate:

- CMake;
- Meson;
- direct compiler command/Ninja for minimal projects.

Users can also consume generated sources from an existing build system using the manifest. No custom build system is mandatory.

### 18.4 Doctor

`hxc doctor` checks:

- Haxe version and scoped libraries;
- Reflaxe availability;
- C compiler and dialect support;
- CMake/Meson/Ninja availability as selected;
- target triple and sysroot;
- pkg-config;
- Clang tooling for bindgen;
- sanitizer capability;
- Beads for contributors;
- output directory writability;
- incompatible environment defines.

Output supports human text and JSON.

### 18.5 Inspectability

`hxc inspect` is a core feature, not a debugging afterthought. It explains:

- why a runtime feature was included;
- why a call was virtual or direct;
- why a value boxed or allocated;
- why a generic specialization exists;
- why a header is included;
- which source declaration produced a C symbol;
- which standard-library implementation owns an API;
- which ABI fields changed.

---

## 19. Examples and showcase applications

### 19.1 Required examples

1. `hello` — smallest portable application, no unnecessary runtime.
2. `fundamentals` — values, arrays, enums, classes, closures, exceptions, generics.
3. `metal_structs` — exact layout, pointers, spans, custom allocator, no runtime.
4. `sqlite_bindgen` — deterministic binding generation and ergonomic wrapper.
5. `shared_library` — Haxe implementation consumed by C and C++.
6. `embedded_ring_buffer` — freestanding-friendly data structure and custom allocator.
7. `wasm_bridge` — later milestone, generated TypeScript declarations and glue.
8. `todo_cli` — the flagship application.

### 19.2 Todo CLI product concept

The todo application is a C-native command-line program with SQLite persistence. It demonstrates the “Rails/Ruby essence” without carrying Rails into C:

- convention over configuration;
- declarative typed schema;
- generated repository boilerplate;
- typed commands and arguments;
- validation declared near data;
- migrations generated deterministically;
- concise business logic;
- explicit and inspectable C output;
- no runtime reflection when compile-time generation is enough.

Illustrative Haxe source:

```haxe
@:table("todos")
class Todo {
  @:primaryKey @:autoIncrement
  public var id:Int;

  @:column("text") @:notNull
  public var title:String;

  @:default(false)
  public var done:Bool;
}

@:commands
class TodoCommands {
  @:command("add")
  static function add(@:arg title:String):Result<Void, AppError> {
    return Todos.insert({title: title, done: false});
  }

  @:command("list")
  static function list(@:flag("--all") all:Bool = false):Result<Void, AppError> {
    for (todo in Todos.query(all ? Any : Pending)) {
      Sys.println('${todo.id}: ${todo.title}');
    }
    return Ok(null);
  }
}
```

Macros generate:

- SQL schema/migrations;
- prepared statement wrappers;
- typed row decoding;
- repository methods;
- CLI parser/help;
- error propagation glue;
- cleanup paths;
- optional completion scripts.

The generated C should resemble a small, disciplined hand-written SQLite CLI rather than a dynamic ORM runtime.

### 19.3 Portable core split

The example SHOULD separate:

- `todo_core`: portable domain types and behavior that compile on other attached Haxe targets;
- `todo_sqlite_c`: C-specific persistence adapter;
- `todo_cli_c`: C command-line entry point.

This proves the portability promise rather than merely describing it.

---

## 20. Diagnostics, debugging, and source mapping

### 20.1 Diagnostic IDs

Every compiler diagnostic has a stable ID, severity, source span, profile, and remediation. Categories include:

- unsupported semantic construct;
- runtime feature selected after direct lowering was ineligible;
- runtime-policy violation;
- metal fallback violation;
- unsafe operation;
- invalid typed C declaration, header, layout, or macro contract;
- ABI-invalid export;
- ownership ambiguity;
- layout mismatch;
- binding parser limitation;
- platform capability gap;
- non-determinism;
- internal compiler error.

### 20.2 Generated comments and line mapping

Debug builds SHOULD support:

- `#line` directives where toolchains handle them reliably;
- a sidecar source map from C ranges/symbols to Haxe source;
- concise generated comments for module/type/function origin;
- stable temporary names derived from source concepts when practical.

Release mode may omit comments while preserving sidecar maps.

### 20.3 Panic and assertion hooks

Runtime failures route through configurable hooks containing:

- stable error code;
- message;
- Haxe source position when available;
- generated C location;
- thread/context information where supported.

Embedded builds can replace the hook without pulling in standard I/O.

---

## 21. Testing and quality strategy

### 21.1 Test layers

1. **Pure unit tests** for naming, AST, printer, layout, and passes.
2. **Golden printer tests** for difficult C declarators and precedence.
3. **Haxe-to-C snapshots** for representative source constructs.
4. **C compile tests** with strict warnings.
5. **Runtime execution tests** with normalized stdout/stderr/exit code.
6. **Negative compiler tests** asserting diagnostic IDs and spans.
7. **Differential semantic tests** against reference Haxe targets.
8. **Standard-library sweep** using upstream tests.
9. **Sanitizer tests** for address, undefined behavior, leak, and thread issues where supported.
10. **Fuzz tests** for bindgen inputs, C AST printer round trips, mangling, and selected runtime parsers.
11. **ABI tests** for headers, symbols, layout, and old consumers.
12. **Cross-compiler tests** across supported toolchains and optimization levels.
13. **Determinism tests** building twice in separate absolute paths.
14. **Performance and code-size benchmarks**.

### 21.2 Snapshot contract

Each snapshot fixture contains:

```text
test/snapshot/<family>/<case>/
  src/Main.hx
  build.hxml
  expected/
    include/...
    src/...
    manifest fragments
  expected.stdout
  expected.exit
  case.json
```

`case.json` declares profiles, runtime policies, toolchains, environment, sanitizer eligibility, and expected diagnostics.

### 21.3 Snapshot update policy

Snapshots update only through an explicit command such as:

```text
hxc test --update <selector>
```

The command prints a semantic diff summary. CI never updates snapshots.

### 21.4 Compiler compilation gate

Generated C is compiled with:

- no warnings under the strict baseline;
- debug and release optimization;
- the compiler family required by each ADR 0007 Tier 1 tuple, including both
  GCC and Clang on Linux;
- C and C++ header consumers for exported APIs;
- sanitizer builds for eligible suites.

### 21.5 Differential testing

The same Haxe test program runs on:

- `hxc`;
- Haxe eval/interp where applicable;
- one or more mature targets appropriate to the feature.

Results are normalized for target-specific paths, line endings, floating formatting, and nondeterministic map order only where the Haxe contract allows it.

### 21.6 Undefined-behavior gate

No release candidate passes while sanitizer failures, uninitialized reads, strict-aliasing warnings, or compiler miscompilation reproducers remain open at release-blocking priority.

### 21.7 Performance gates

Benchmarks include:

- startup time;
- executable and library size;
- allocation count and bytes;
- string/array operations;
- virtual and interface dispatch;
- enum matching;
- closure call overhead;
- FFI call overhead;
- SQLite todo workload;
- compile time and generated C compile time.

Performance claims in the README require reproducible benchmark definitions and comparison context.

---

## 22. Security and supply-chain requirements

- Bindgen never executes header-provided code; it invokes a pinned/configured parser in syntax-only mode.
- External commands are argument arrays, not shell-concatenated strings.
- Output paths reject traversal and unsafe overlap with source roots.
- Generated-file deletion is restricted to files recorded by Reflaxe ownership metadata.
- Build manifests record compiler, target triple, flags, libraries, and runtime features.
- Generated bindings record header hashes and parser version.
- Release artifacts include checksums and provenance.
- Repository-owned code is licensed under `GPL-3.0-only`; copied or adapted
  third-party code retains compatible notices, SPDX/provenance, and license
  review evidence.
- Third-party runtime code has license and provenance records.
- `docs/specs/third-party-provenance.json` is the fail-closed distribution
  inventory. External development tools are distinguished from redistributed
  source/binaries; reserved vendor/runtime roots reject unowned files; and every
  target `_std` file has content-hashed per-file provenance.
- Release assembly runs
  `scripts/ci/check_license_policy.py --package-root <assembled-tree>` so the
  canonical license, third-party notice, provenance manifest, and retained
  upstream licenses match the reviewed repository copies.
- Generated C/header, template, and selectively emitted/linked runtime license
  treatment remains owned by decision `haxe_c-od2.5`; no release workflow may
  infer an exception or grant solely from the repository license.
- Fuzzing covers untrusted binding configuration and manifest parsing.
- Exported ABI functions validate lengths, overflows, and nullability at trust boundaries according to policy.
- Unsafe operations are counted and reviewable.

---

## 23. Repository layout

```text
reflaxe.c/
  .beads/                         # created and owned by `bd init`
  .github/workflows/
  .haxerc
  AGENTS.md
  LICENSE
  LICENSES/
  THIRD_PARTY_NOTICES.md
  CHANGELOG.md
  README.md
  SECURITY.md
  haxelib.json
  package.json
  extraParams.hxml
  haxe_libraries/
    reflaxe.hxml
    reflaxe.c.hxml
  src/
    Run.hx
    reflaxe/c/
      BuildDetection.hx
      CompilerBootstrap.hx
      CompilerInit.hx
      CompilationContext.hx
      CProfile.hx
      CRuntimePolicy.hx
      ProfileResolver.hx
      CReflaxeCompiler.hx
      CCompiler.hx
      CDiagnostic.hx
      analyze/
      ast/
      compiler/
      emit/
      macros/
      naming/
      passes/
  std/
    c/
      Syntax.hx
      Ptr.hx
      ConstPtr.hx
      Span.hx
      CString.hx
      Result.hx
      Owned.hx
      Borrowed.hx
      Allocator.hx
      abi/
      memory/
    c/_std/
  runtime/hxrt/
    include/
    src/
    features.json
  docs/
    PRD.md
    architecture.md
    BEADS_PLAN.md
    research/repo-patterns.md
    specs/
    adr/
  examples/
  templates/
  test/
  tools/
  scripts/
    beads/bootstrap.py
    ci/
```

---

## 24. Beads project-management contract

### 24.1 Source of truth

Beads is the execution graph. This PRD explains intent; it is not a live task-status list.

The repository includes a deterministic bootstrap plan. Contributors use this safe initialization and materialization sequence:

```sh
bd version
[ -d .beads ] || bd init --quiet
bd setup codex
python3 scripts/beads/validate_plan.py --json
python3 scripts/beads/bootstrap.py --json
python3 scripts/beads/bootstrap.py --apply
bd prime
bd ready --json
bd update <id> --claim --json
# implement, test, document, and collect evidence
bd close <id> --reason "Requirements HXC-... satisfied; validation: <commands>; artifacts: <paths>" --json
```

The bootstrap is idempotent by matching stable plan keys stored in issue metadata or, as a fallback, exact prefixed titles. It MUST be previewed and validated before apply.

Never run `bd init --force` over existing project state. Do not hand-author Beads database internals. JSONL is an export/interchange artifact when enabled, not a replacement for canonical Beads state. Git and Beads remote synchronization are separate, permissioned operations; run `bd dolt push` or equivalent only when a remote is configured and repository policy explicitly authorizes it.

### 24.2 Epic structure

- **E0 — Repository and governance foundation**
- **E1 — Reflaxe compiler core and C AST**
- **E2 — Primitive semantics, control flow, and UB discipline**
- **E3 — Aggregates, generics, classes, and dispatch**
- **E4 — Runtime, memory, dynamic values, and exceptions**
- **E5 — Haxe standard-library parity**
- **E6 — C interop and deterministic bindgen**
- **E7 — Exported C ABI and shared libraries**
- **E8 — CLI, build adapters, diagnostics, and inspection**
- **E9 — Examples, todo CLI, documentation, and adoption**
- **E10 — Quality, performance, security, and releases**

The checked-in `docs/specs/beads-plan.json` contains the detailed tasks, dependencies, priorities, descriptions, and acceptance criteria.

### 24.3 Task sizing

A normal task should be completable in one focused change and have observable acceptance criteria. Large discoveries become child issues linked with `discovered-from` rather than silently expanding scope.

### 24.4 Agent workflow

Every Codex session:

1. runs `bd prime` when available;
2. queries `bd ready --json`;
3. claims exactly one primary task;
4. reads parent epic and dependencies;
5. adds tests before or with implementation;
6. records design changes in an ADR when they alter a public contract;
7. creates discovered issues rather than burying TODOs;
8. closes only after the task's commands pass;
9. commits generated Beads export/state according to repository policy;
10. pushes/synchronizes Beads state when configured.

---

## 25. Milestones and acceptance gates

### M0 — Bootstrap repository

Deliverables:

- family-consistent repository structure;
- Haxe/Reflaxe pinning and target metadata;
- bootstrap/init macros;
- profile/runtime policy resolution;
- minimal C AST and printer skeleton;
- runtime ABI seed compiling as C11;
- `hxc doctor` and help skeleton;
- PRD, architecture, research, ADR seeds;
- Beads bootstrap plan;
- CI structure and runtime smoke tests.

Acceptance:

- JSON files parse;
- runtime seed compiles with available GCC and Clang under strict flags;
- generated public runtime header compiles as both C and C++;
- Beads plan can dry-run without `bd` and apply when `bd` is installed;
- unsupported compiler paths fail explicitly rather than emit incorrect programs.

### M1 — C AST, printer, project emitter

Deliverables:

- complete declarator model;
- expression precedence model;
- headers/sources/includes;
- deterministic naming;
- manifest and generated-file ownership;
- CMake and Meson seed output.

Acceptance:

- exhaustive declarator golden tests;
- AST printer output compiles under GCC/Clang;
- two builds in different absolute paths are byte-identical;
- stale generated files are safely removed.

### M2 — Primitive semantics and structured control flow

Deliverables:

- primitive values and conversions;
- locals, functions, calls;
- explicit evaluation order;
- `if`, loops, switch, return/break/continue;
- strings literals and minimal trace/I/O;
- arrays without general object runtime where possible.

Acceptance:

- core fixtures compile and execute;
- differential semantic suite passes;
- sanitizer suite passes;
- `hello` contains no unused broad runtime feature.

### M3 — Aggregates, enums, generics, classes, closures

Deliverables:

- anonymous/named structs;
- tagged Haxe enums;
- monomorphization;
- class layout and constructors;
- virtual/interface dispatch;
- closures and escape analysis.

Acceptance:

- direct calls devirtualize in reports;
- recursive enums and generic containers work;
- nonescaping closures allocate no heap memory;
- C output remains warning-clean.

### M4 — Portable runtime and memory

Deliverables:

- allocator contract;
- selected GC implementation;
- object descriptors;
- dynamic values;
- reflection slices;
- exception/catch/finally implementation;
- cleanup regions.

Acceptance:

- cyclic graphs collect correctly;
- exception cleanup tests pass;
- runtime plan includes only reachable features;
- `-D hxc_runtime=none` succeeds for eligible fixtures and gives reasoned errors for ineligible fixtures.

### M5 — Core and extended standard library

Deliverables:

- standard-library ledger;
- core collections, strings, bytes, JSON, regex;
- sys file/process/time/socket/thread adapters;
- upstream test integration.

Acceptance:

- agreed upstream test threshold reached with no silent skips;
- every gap is explicit and tracked;
- portable todo domain compiles on C and selected sibling targets.

### M6 — C bindgen

Deliverables:

- Clang-backed ABI model;
- deterministic raw extern generation;
- mapping configuration;
- safe wrapper generation;
- pkg-config and compile-database support;
- layout assertions and binding locks.

Acceptance:

- SQLite and zlib-class libraries bind from headers reproducibly;
- regenerated output is clean;
- ABI facts match compiled C probes;
- unsupported constructs produce precise diagnostics.

### M7 — Export and shared-library workflow

Deliverables:

- export metadata;
- ABI model/hash;
- public headers and wrappers;
- symbol visibility/version files;
- static/shared builds;
- C and C++ consumers.

Acceptance:

- consumers build independently;
- exceptions and ownership are translated correctly;
- headers compile under C and C++;
- ABI diff gate detects intentional and accidental changes.

### M8 — CLI and build product

Deliverables:

- complete command set;
- new-project templates;
- doctor and inspect reports;
- build adapters;
- source maps and diagnostics catalog.

Acceptance:

- clean user journey from install to `hxc new`, `hxc run`;
- JSON output supports automation;
- existing build systems can consume the manifest without generated CMake/Meson.

### M9 — Flagship examples and adoption

Deliverables:

- todo CLI with SQLite/macros;
- metal/embedded example;
- shared library examples;
- C++ shim example;
- WebAssembly bridge proof where scheduled;
- comparison and migration docs.

Acceptance:

- examples use no unapproved `__c__`;
- todo generated C is reviewed against a hand-written baseline;
- docs state limitations honestly;
- benchmark results are reproducible.

### M10 — Beta and 1.0 hardening

Deliverables:

- compatibility matrix;
- full release CI;
- security review;
- fuzzing baseline;
- performance budgets;
- standard-library parity target;
- stable ABI policy;
- package/release automation.

Acceptance:

- no release-blocking correctness, UB, security, or ABI issues;
- all 1.0 gates have archived evidence;
- immutable release artifacts and provenance are published;
- post-1.0 compatibility policy is documented.

---

## 26. Success metrics

### Correctness

- percentage of upstream Haxe tests passing;
- number of known semantic divergences;
- sanitizer-clean fixture count;
- compiler crash rate on corpus.

### Code quality

- generated warning count: zero under supported baseline;
- deterministic-build pass rate;
- runtime feature precision;
- generated lines/runtime calls per benchmark relative to a reviewed hand-written C baseline.

### Interop

- time/steps to generate and use a binding;
- number of supported C declaration categories;
- ABI probe match rate;
- number of languages consuming exported example libraries.

### Performance

- runtime and code size across representative workloads;
- allocation reductions from direct lowering;
- FFI overhead;
- compiler and C build time.

### Adoption

- successful project templates;
- external library bindings;
- issue-to-merged-task cycle time in Beads;
- documentation completion and user-reported friction.

No metric justifies semantic drift that violates the active profile contract.

---

## 27. Risks and mitigations

### R1. C is less semantically close to Haxe than it first appears

Classes, GC, closures, exceptions, dynamic values, reflection, and Unicode require real design.

**Mitigation:** explicit HxcIR, selective runtime, semantic ledger, differential tests, and no promise that every construct becomes one obvious C construct.

### R2. “Idiomatic C” becomes subjective

**Mitigation:** define measurable conventions, publish generated examples, use experienced C review, and prioritize correctness/readability over stylistic micro-optimization.

### R3. Runtime becomes hxcpp rewritten in C

**Mitigation:** feature graph, no-runtime analyzer, concrete specialization, runtime reason reports, and benchmarks that track helper use.

### R4. Metal and portable paths fork into two compilers

**Mitigation:** one IR and pass pipeline; profiles select policies and diagnostics, not separate lowering implementations.

### R5. Undefined behavior hides in optimized builds

**Mitigation:** explicit evaluation/lifetime IR, strict flags, multiple optimization levels, sanitizers, fuzzing, and compiler differential builds.

### R6. Standard-library scope delays usefulness

**Mitigation:** vertical milestones that deliver useful apps and interop early, while tracking complete parity as a 1.0 gate.

### R7. Bindgen output is technically correct but unusable

**Mitigation:** raw/ergonomic layer separation, explicit mappings, generated tests, and flagship bindings.

### R8. C++ marketing overpromises

**Mitigation:** README states the shim requirement prominently and ships a concrete C++ adapter example.

### R9. ABI stability freezes poor early choices

**Mitigation:** ABI is experimental before 1.0, opaque handles by default, versioned export metadata, ABI diff tooling, and explicit freeze milestones.

### R10. Macro ergonomics hide allocation or ownership

**Mitigation:** expansion reports, generated C review, ownership annotations, no-runtime tests, and source APIs that expose important lifetime choices.

### R11. Beads plan drifts from architecture

**Mitigation:** stable plan keys, parent epics, acceptance criteria, ADR links, and a CI check that all active milestone tasks refer to valid PRD sections.

---

## 28. Owner decisions and remaining implementation confirmations

Accepted decisions below are normative through their ADR. Items without an ADR
remain reversible implementation defaults owned by the named Beads work; they
are not permission to contradict an accepted record.

1. **Project/license (owner-confirmed 2026-07-15):** repository named
   `reflaxe.c`; optional orchestration CLI named `hxc`; repository-owned code
   licensed `GPL-3.0-only`. The current upstream inventory and fail-closed
   notice/package checks live in `THIRD_PARTY_NOTICES.md`,
   `docs/specs/third-party-provenance.json`, and
   `scripts/ci/check_license_policy.py`. Generated-output and runtime
   redistribution treatment remains decision `haxe_c-od2.5`.
2. **Profiles (accepted):** `portable` default plus `metal`; no third
   “idiomatic” profile. Both use the direct-C-first pipeline in ADR 0001.
3. **Runtime policy (accepted):** `auto|minimal|none` remains orthogonal to
   profile; absent overrides, portable resolves to `auto + summary` and metal
   to `minimal + warn`. Direct C and program-local specialization always
   precede `hxrt`; `none` proves complete runtime absence. See ADR 0001.
4. **C dialect and target identity (accepted):** ADR 0007 defines strict ISO
   C11/no extensions, C17 compatibility, experimental ABI-neutral C23 syntax,
   Haxe 5 `--custom-target c=<output>` activation, `c` identity, derived
   `c_output` transport, and capability-owned platform flags.
5. **Runtime memory (accepted):** ADR 0005 defines a selectively linked precise
   non-moving mark-and-sweep baseline with exact roots and pluggable internal
   backends; eligible code remains collector- and runtime-free.
6. **String storage (accepted):** ADR 0004 defines immutable valid UTF-8,
   Unicode-scalar indexing, deterministic malformed-input replacement, and
   separate `Bytes`, `CString`, and exported-view contracts.
7. **Build integration:** emit CMake and Meson plus a tool-neutral manifest; do
   not require a custom build system.
8. **Bindgen:** Clang-derived facts; AI suggestions are review-only.
9. **C++:** consume through existing/generated C shims; emit C++-friendly
   headers and optional wrappers. ADR 0003 remains owned by E6.T10.
10. **Exceptions and ABI failure (accepted):** ADR 0006 uses proven explicit
    result lowering or a contained strict-C11 unwind slice internally. Opaque
    handles, explicit ownership, and status/errors cross native boundaries; a
    Haxe or C++ exception/non-local transfer never does.
11. **Haxe version (accepted):** exact official `5.0.0-preview.1` custom-target
    contract at revision `2c1e544e0a2c7524ef4c8e103f1b0580362ea538`;
    broaden only with explicit API audit, artifact hashes, ADR review, and full
    compile-backed evidence. Legacy Haxe 4 Cross is a negative, not a fallback.
12. **Version 1.0 platforms (accepted):** ADR 0007 defines the release-blocking
    Linux, macOS, Windows, and capability-limited ARM Cortex-M lanes. Exact
    version floors and promotion evidence remain with E10.
13. **Typed C authoring (accepted):** Haxe declarations plus typed `c.*` APIs
    and validated metadata/macros model headers and C-native facts; narrow DSLs
    require an admission rationale and raw C remains an explicit unsafe escape.
    See ADR 0002.

---

## 29. Codex implementation contract

Codex receives this repository and should:

1. run the structure/runtime checks;
2. initialize and apply the Beads plan;
3. inspect `bd ready --json` and claim the first unblocked task;
4. retain the two-stage bootstrap pattern;
5. keep `extraParams.hxml` current-working-directory agnostic;
6. keep all mutable compiler state in `CompilationContext`;
7. add typed IR/AST nodes rather than concatenate raw C;
8. write generated artifacts only through Reflaxe output management;
9. fail unsupported semantics with stable diagnostics;
10. add positive, negative, C-compile, and runtime tests with each feature;
11. preserve explicit evaluation order and UB policy;
12. record material architecture changes as ADRs;
13. create discovered Beads issues instead of unowned TODO comments;
14. never make examples pass by inserting raw `__c__`;
15. keep README claims aligned with evidence and current limitations.

### 29.1 Recommended first execution sequence

1. Complete `CType`/declarator AST and golden printer tests.
2. Complete `CExpr` precedence and literal escaping tests.
3. Implement project emitter with headers/sources/manifest.
4. Implement `Main.main`, primitive constants, blocks, locals, return, and `trace`/stdout intrinsic.
5. Compile and execute `examples/hello` under GCC and Clang.
6. Add explicit evaluation-order lowering and adversarial tests.
7. Add primitive type/conversion semantic matrix.
8. Add runtime feature analyzer/report and prove hello's minimal plan.
9. Add arrays and tagged enums.
10. Begin the standard-library ledger before broad API implementation.

### 29.2 Definition of done for any lowering feature

A lowering feature is not done until:

- Haxe source fixture exists;
- HxcIR/AST unit or snapshot coverage exists;
- generated C compiles warning-free;
- runtime behavior matches the oracle;
- negative invalid cases have stable diagnostics;
- profile/runtime-policy differences are tested;
- runtime-plan impact is asserted;
- documentation and ledger entries are updated;
- Beads acceptance criteria are met and the issue is closed.

---

## 30. Complete Beads seed backlog

This section makes the PRD self-contained when it is handed to Codex without the rest of the archive. It mirrors `docs/specs/beads-plan.json`, which remains the preferred machine-readable import source. The current seed contains **11 epics, 122 tasks, and 274 explicit blocking edges**. Stable keys MUST be preserved when materialized into Beads.

The wording is brownfield-aware: where a seed file already exists, “implement” means inspect, test, complete, and document it rather than create a duplicate subsystem. A task closes only when all acceptance criteria pass.

### 30.1 E0 — Repository and governance foundation

**Milestone:** M0\
**Priority:** P0\
**Outcome:** Ratify contracts, make the scaffold build reproducibly, and establish the project rules Codex must follow.

#### E0.T01 — Ratify product contracts and open ADR decisions

- **Priority / effort / type:** P0 / small / task
- **Blocked by:** none
- **PRD references:** §8, §9, §28, ADR 0001, 0002, 0004–0007
- **Requirement IDs:** `HXC-PROD-002`, `HXC-PROD-004`, `HXC-PROD-005`, `HXC-SCAF-006`
- **Outcome:** Resolve the owner decisions that affect public semantics before implementation hardens accidental choices.
- **Description:** Resolve the owner decisions that affect public semantics before implementation hardens accidental choices. PRD: §8, §9, §28
- **Acceptance criteria:**
  - Record decisions for license, C baseline, string encoding, portable collector, exception ABI, and 1.0 platforms.
  - Update affected ADRs and PRD sections without contradictory language.
  - Create Beads discoveries for deferred experiments; do not hide undecided semantics in code.
- **Labels:** hxc-plan, epic-e0, effort-small, architecture, decision

#### E0.T02 — Choose license and complete third-party notices

- **Priority / effort / type:** P0 / small / task
- **Blocked by:** E0.T01
- **PRD references:** §22, §28
- **Requirement IDs:** `HXC-SCAF-001`, `HXC-SCAF-006`, `HXC-QA-011`, `HXC-QA-013`
- **Outcome:** Replace the scaffold license placeholder with an approved repository license and auditable notices.
- **Description:** Replace the scaffold license placeholder with an approved repository license and auditable notices. PRD: §22, §28
- **Acceptance criteria:**
  - Canonical license text is checked in.
  - Reflaxe, Haxe-derived standard-library work, Clang tooling, and runtime dependencies have attribution/provenance entries.
  - CI rejects missing license/notice files in release artifacts.
- **Labels:** hxc-plan, epic-e0, effort-small, governance, legal

#### E0.T03 — Pin Haxe, Reflaxe, and dependency resolution

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E0.T01
- **PRD references:** §2.2, §2.3, §25 M0, ADR 0007
- **Requirement IDs:** `HXC-SCAF-002`, `HXC-SCAF-006`, `HXC-SCAF-007`
- **Outcome:** Create a reproducible source-checkout and package build using the exact accepted Haxe 5 custom-target carrier and a known Reflaxe revision/version.
- **Description:** Create a reproducible source-checkout and package build using Haxe 5.0.0-preview.1, its locked platform artifacts, and a known Reflaxe revision/version. PRD: §2.2, §2.3, §25 M0
- **Acceptance criteria:**
  - Fresh checkout resolves dependencies without relying on the caller's current working directory.
  - CompilerBootstrap runs before CompilerInit and only for C builds.
  - The pin and update procedure are documented and CI-verifiable.
- **Labels:** hxc-plan, epic-e0, effort-medium, tooling, reflaxe

#### E0.T04 — Make the bootstrap Haxe sources compile

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E0.T03
- **PRD references:** §10.3, §25 M0, ADR 0007
- **Requirement IDs:** `HXC-SCAF-003`, `HXC-SCAF-004`, `HXC-SCAF-007`, `HXC-MAC-009`
- **Outcome:** Type-check the compiler adapter, CLI, C AST seed, macros, and c.* surface under the pinned toolchain.
- **Description:** Type-check the compiler adapter, CLI, C AST seed, macros, and c.* surface under the pinned toolchain. PRD: §10.3, §25 M0
- **Acceptance criteria:**
  - A dedicated HXML compiles all target-owned Haxe modules.
  - Macro and non-macro branches type-check where applicable.
  - Unsupported compiler execution ends with HXC1000 and cannot emit plausible program C.
- **Labels:** hxc-plan, epic-e0, effort-medium, compiler, bootstrap

#### E0.T05 — Establish CI structure and runtime smoke matrix

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E0.T04
- **PRD references:** §21, §25 M0
- **Requirement IDs:** `HXC-SCAF-007`, `HXC-QA-001`, `HXC-QA-002`, `HXC-QA-003`
- **Outcome:** Run structural, JSON, Haxe, C11 runtime, C++ header, and native fixture checks on every change.
- **Description:** Run structural, JSON, Haxe, C11 runtime, C++ header, and native fixture checks on every change. PRD: §21, §25 M0
- **Acceptance criteria:**
  - GCC and Clang compile runtime/fixtures with warnings as errors.
  - A C++ compiler consumes public C headers.
  - CI output identifies skipped optional tools and never reports an unrun required gate as passed.
- **Labels:** hxc-plan, epic-e0, effort-medium, ci, quality

#### E0.T06 — Create stable diagnostic catalog and policy

- **Priority / effort / type:** P1 / small / task
- **Blocked by:** E0.T01
- **PRD references:** §20.1
- **Requirement IDs:** `HXC-SCAF-004`, `HXC-COMP-014`, `HXC-CLI-010`
- **Outcome:** Reserve diagnostic ranges and document wording, severity, source position, remediation, and stability rules.
- **Description:** Reserve diagnostic ranges and document wording, severity, source position, remediation, and stability rules. PRD: §20.1
- **Acceptance criteria:**
  - All current diagnostics appear in a machine-readable catalog.
  - Tests assert IDs rather than fragile full prose where appropriate.
  - Internal compiler failures are distinct from unsupported-source diagnostics.
- **Labels:** hxc-plan, epic-e0, effort-small, diagnostics

#### E0.T07 — Finalize contribution, security, and release governance

- **Priority / effort / type:** P1 / small / task
- **Blocked by:** E0.T02
- **PRD references:** §22, §29
- **Requirement IDs:** `HXC-QA-001`, `HXC-QA-011`, `HXC-QA-013`, `HXC-QA-014`
- **Outcome:** Give human and agent contributors a single operational contract for changes, disclosures, generated files, and releases.
- **Description:** Give human and agent contributors a single operational contract for changes, disclosures, generated files, and releases. PRD: §22, §29
- **Acceptance criteria:**
  - AGENTS and CONTRIBUTING agree on Beads workflow and definition of done.
  - SECURITY names a private reporting path and supported versions policy.
  - Generated files, vendoring, release signing, and provenance responsibilities are explicit.
- **Labels:** hxc-plan, epic-e0, effort-small, governance, security

#### E0.T08 — Validate and materialize the Beads bootstrap graph

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E0.T01
- **PRD references:** §24
- **Requirement IDs:** `HXC-SCAF-007`, `HXC-SCAF-008`, `HXC-QA-014`
- **Outcome:** Prove the checked-in plan is acyclic, idempotently materializable, and useful to Codex.
- **Description:** Prove the checked-in plan is acyclic, idempotently materializable, and useful to Codex. PRD: §24
- **Acceptance criteria:**
  - Dry-run works without bd installed.
  - Apply creates epics, children, labels, descriptions, acceptance criteria, and blocking edges.
  - A second apply creates no duplicate issues, including after issues have been closed.
  - bd ready exposes parallel starter work consistent with the graph.
- **Labels:** hxc-plan, epic-e0, effort-medium, beads, planning

#### E0.T09 — Define fixture taxonomy and snapshot update policy

- **Priority / effort / type:** P1 / small / task
- **Blocked by:** E0.T04
- **PRD references:** §21.1, §21.2, §21.3
- **Requirement IDs:** `HXC-COMP-014`, `HXC-QA-001`, `HXC-QA-007`
- **Outcome:** Standardize positive, negative, AST, C snapshot, runtime, differential, ABI, and performance fixtures.
- **Description:** Standardize positive, negative, AST, C snapshot, runtime, differential, ABI, and performance fixtures. PRD: §21.1, §21.2, §21.3
- **Acceptance criteria:**
  - Each fixture type has a directory, runner contract, and expected outputs.
  - Snapshot regeneration requires explicit command and shows semantic diffs.
  - Examples are not silently used as tests without declared assertions.
- **Labels:** hxc-plan, epic-e0, effort-small, testing

#### E0.T10 — Publish bootstrap capability and limitation manifest

- **Priority / effort / type:** P1 / small / task
- **Blocked by:** E0.T04, E0.T06
- **PRD references:** §18.5, §25 M0
- **Requirement IDs:** `HXC-SCAF-001`, `HXC-SCAF-004`, `HXC-DOC-007`
- **Outcome:** Expose what the scaffold implements, reserves, rejects, and has not validated in machine-readable form.
- **Description:** Expose what the scaffold implements, reserves, rejects, and has not validated in machine-readable form. PRD: §18.5, §25 M0
- **Acceptance criteria:**
  - Manifest distinguishes implemented, scaffold-only, experimental, and unsupported capabilities.
  - README and hxc doctor derive or agree with the manifest.
  - No full-stdlib, production, or performance claim appears before its gate passes.
- **Labels:** hxc-plan, epic-e0, effort-small, product, manifest

### 30.2 E1 — Reflaxe compiler core and C AST

**Milestone:** M1\
**Priority:** P0\
**Outcome:** Build the deterministic AST/IR/output foundation before broad language lowering.

#### E1.T01 — Complete the C11 AST declarator and type model

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E0.T04
- **PRD references:** §10.6, §25 M1
- **Requirement IDs:** `HXC-COMP-002`, `HXC-COMP-004`, `HXC-COMP-008`
- **Outcome:** Represent all C declarator shapes structurally, including nested pointer/array/function declarations, qualifiers, attributes, and incomplete types.
- **Description:** Represent all C declarator shapes structurally, including nested pointer/array/function declarations, qualifiers, attributes, and incomplete types. PRD: §10.6, §25 M1
- **Acceptance criteria:**
  - Golden tests cover precedence-sensitive declarations in parameters, typedefs, fields, and function pointers.
  - No normal compiler path stores user-derived C syntax as an opaque string.
  - AST can represent C11 constructs needed by M2-M7 without printer-specific hacks.
- **Labels:** hxc-plan, epic-e1, effort-large, ast, c11

#### E1.T02 — Finish expression and statement printer precedence

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E1.T01
- **PRD references:** §10.6, §25 M1
- **Requirement IDs:** `HXC-SCAF-005`, `HXC-COMP-005`, `HXC-COMP-007`
- **Outcome:** Print readable C with the minimum safe parentheses and deterministic formatting.
- **Description:** Print readable C with the minimum safe parentheses and deterministic formatting. PRD: §10.6, §25 M1
- **Acceptance criteria:**
  - Every unary/binary/conditional/call/member/cast precedence pair has tests.
  - Printed fixtures compile under GCC and Clang strict C11.
  - Printer escaping handles identifiers, strings, characters, comments, and line directives safely.
- **Labels:** hxc-plan, epic-e1, effort-large, printer, ast

#### E1.T03 — Implement include and declaration dependency planning

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E1.T01
- **PRD references:** §10.10
- **Requirement IDs:** `HXC-COMP-001`, `HXC-COMP-007`, `HXC-COMP-008`
- **Outcome:** Compute forward declarations, complete-type requirements, header/source placement, and deterministic include order.
- **Description:** Compute forward declarations, complete-type requirements, header/source placement, and deterministic include order. PRD: §10.10
- **Acceptance criteria:**
  - Pointer-only cycles use forward declarations.
  - By-value cycles produce an actionable diagnostic.
  - System/local includes are deduplicated and stable.
  - Headers are independently compilable.
- **Labels:** hxc-plan, epic-e1, effort-medium, emitter, dependencies

#### E1.T04 — Implement deterministic symbol registry and naming

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E1.T01, E0.T06
- **PRD references:** §10.8
- **Requirement IDs:** `HXC-COMP-001`, `HXC-COMP-006`, `HXC-COMP-007`
- **Outcome:** Map Haxe modules/types/fields/locals/specializations to collision-free C identifiers with readable provenance.
- **Description:** Map Haxe modules/types/fields/locals/specializations to collision-free C identifiers with readable provenance. PRD: §10.8
- **Acceptance criteria:**
  - C keywords and implementation-reserved identifiers are escaped.
  - Overloads/generic specializations remain stable across filesystem paths and discovery order.
  - Collision diagnostics identify both source symbols.
  - Public `@:c.name` values are validated rather than rewritten silently.
- **Labels:** hxc-plan, epic-e1, effort-medium, naming, abi

#### E1.T05 — Expand HxcIR semantic core

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E1.T01
- **PRD references:** §10.2, §10.5
- **Requirement IDs:** `HXC-COMP-001`, `HXC-COMP-002`, `HXC-COMP-003`
- **Outcome:** Represent explicit evaluation order, places/values, cleanup scopes, calls, dispatch, conversion, failure edges, and initialization independently of C syntax.
- **Description:** Represent explicit evaluation order, places/values, cleanup scopes, calls, dispatch, conversion, failure edges, and initialization independently of C syntax. PRD: §10.2, §10.5
- **Acceptance criteria:**
  - IR invariants are documented and validated.
  - IR dump is deterministic and source-position aware.
  - Side effects and cleanup order can be tested before C emission.
  - Unsupported typed AST nodes become explicit diagnostics, not opaque values.
- **Labels:** hxc-plan, epic-e1, effort-large, ir, semantics

#### E1.T06 — Build typed-AST collection and normalization adapter

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E1.T05, E0.T09
- **PRD references:** §10.3, §10.4
- **Requirement IDs:** `HXC-SCAF-002`, `HXC-SCAF-003`, `HXC-COMP-001`
- **Outcome:** Collect all relevant Reflaxe module types into per-build normalized input while preserving source metadata and ownership.
- **Description:** Collect all relevant Reflaxe module types into per-build normalized input while preserving source metadata and ownership. PRD: §10.3, §10.4
- **Acceptance criteria:**
  - Compiler-server builds reset all mutable state.
  - Externs, abstracts, typedefs, metadata, entry point, and module ownership are classified.
  - Input order changes do not change normalized output.
  - A typed AST inventory report is available for unsupported-node planning.
- **Labels:** hxc-plan, epic-e1, effort-large, reflaxe, frontend

#### E1.T07 — Implement deterministic multi-file project emitter

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E1.T02, E1.T03, E1.T04, E1.T06
- **PRD references:** §10.9, §25 M1
- **Requirement IDs:** `HXC-COMP-009`, `HXC-COMP-011`, `HXC-COMP-012`
- **Outcome:** Emit owned headers, sources, runtime-plan, compiler manifest, ABI placeholders, and build metadata through Reflaxe OutputManager.
- **Description:** Emit owned headers, sources, runtime-plan, compiler manifest, ABI placeholders, and build metadata through Reflaxe OutputManager. PRD: §10.9, §25 M1
- **Acceptance criteria:**
  - Generated-file ownership safely deletes stale files and never touches user-owned files.
  - Writes are content-addressed or skipped when unchanged.
  - Two builds in different absolute directories are byte-identical after declared path normalization.
  - Scaffold marker path remains fail-closed until real lowering replaces it.
- **Labels:** hxc-plan, epic-e1, effort-large, emitter, determinism

#### E1.T08 — Emit CMake, Meson, and raw manifest build seeds

- **Priority / effort / type:** P1 / medium / task
- **Blocked by:** E1.T07
- **PRD references:** §18.3, §25 M1
- **Requirement IDs:** `HXC-SCAF-005`, `HXC-COMP-001`, `HXC-COMP-010`
- **Outcome:** Generate optional build-system adapters from one neutral compile/link plan.
- **Description:** Generate optional build-system adapters from one neutral compile/link plan. PRD: §18.3, §25 M1
- **Acceptance criteria:**
  - CMake and Meson consume the same source/include/define/library model.
  - Users can ignore generated adapters and read the JSON manifest.
  - Paths and flags are quoted safely.
  - Generated projects build the AST/printer fixtures.
- **Labels:** hxc-plan, epic-e1, effort-medium, build, cmake, meson

#### E1.T09 — Add determinism and stale-output adversarial tests

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E1.T07
- **PRD references:** §21.2, §25 M1
- **Requirement IDs:** `HXC-COMP-009`, `HXC-COMP-012`, `HXC-QA-007`
- **Outcome:** Exercise changed module order, absolute paths, removed modules, renamed symbols, line endings, and cached compiler-server runs.
- **Description:** Exercise changed module order, absolute paths, removed modules, renamed symbols, line endings, and cached compiler-server runs. PRD: §21.2, §25 M1
- **Acceptance criteria:**
  - Byte-for-byte checks pass across isolated roots.
  - Removed output is deleted only when listed as compiler-owned.
  - No timestamp, random ID, map iteration, or host path leaks into normal artifacts.
  - Failure reports identify first differing artifact and offset.
- **Labels:** hxc-plan, epic-e1, effort-medium, testing, determinism

#### E1.T10 — Create AST fixture compiler and round-trip corpus

- **Priority / effort / type:** P1 / medium / task
- **Blocked by:** E1.T02, E1.T07
- **PRD references:** §21.1
- **Requirement IDs:** `HXC-COMP-004`, `HXC-COMP-005`, `HXC-QA-001`
- **Outcome:** Allow tests to construct C AST directly, emit translation units, and compile/link/run them without Haxe language lowering.
- **Description:** Allow tests to construct C AST directly, emit translation units, and compile/link/run them without Haxe language lowering. PRD: §21.1
- **Acceptance criteria:**
  - Corpus covers declarations, precedence, initializers, control flow, attributes, and header/source splits.
  - Both GCC and Clang are supported.
  - Compiler invocations and outputs are recorded for reproducibility.
  - The harness is reusable by later lowering tasks.
- **Labels:** hxc-plan, epic-e1, effort-medium, testing, ast

### 30.3 E2 — Primitive semantics, control flow, and UB discipline

**Milestone:** M2\
**Priority:** P0\
**Outcome:** Deliver the first correct executable Haxe-to-C vertical slice with explicit evaluation order and no C undefined behavior.

#### E2.T01 — Map primitive Haxe types and conversions

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E1.T05, E1.T06
- **PRD references:** §12.1
- **Requirement IDs:** `HXC-SEM-001`, `HXC-SEM-003`, `HXC-SEM-005`
- **Outcome:** Define exact C representations and conversion rules for Void, Bool, Int, Float, nullability, and target ABI integers.
- **Description:** Define exact C representations and conversion rules for Void, Bool, Int, Float, nullability, and target ABI integers. PRD: §12.1
- **Acceptance criteria:**
  - Representation table is an ADR-backed machine-readable contract.
  - Narrowing, widening, NaN, infinity, signed/unsigned, and nullable cases have tests.
  - Portable and metal mappings differ only where their source contracts require it.
  - No host-dependent `long` assumption leaks into ABI types.
- **Labels:** hxc-plan, epic-e2, effort-large, lowering, types

#### E2.T02 — Lower constants, locals, blocks, and return

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E2.T01, E1.T02
- **PRD references:** §12.1, §12.14
- **Requirement IDs:** `HXC-SCAF-004`, `HXC-COMP-002`, `HXC-SEM-022`
- **Outcome:** Produce the first real HxcIR/C lowering for literal expressions and structured function bodies.
- **Description:** Produce the first real HxcIR/C lowering for literal expressions and structured function bodies. PRD: §12.1, §12.14
- **Acceptance criteria:**
  - Source positions survive into diagnostics and optional line mapping.
  - Local names are deterministic and shadowing-safe.
  - Void/value returns are validated.
  - Unsupported expressions fail at their exact source location.
- **Labels:** hxc-plan, epic-e2, effort-medium, lowering, core

#### E2.T03 — Lower static functions, arguments, and direct calls

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E2.T02, E1.T04
- **PRD references:** §12.10
- **Requirement IDs:** `HXC-SEM-001`, `HXC-SEM-004`, `HXC-SEM-011`
- **Outcome:** Emit prototypes/definitions and direct calls with explicit parameter/result representations.
- **Description:** Emit prototypes/definitions and direct calls with explicit parameter/result representations. PRD: §12.10
- **Acceptance criteria:**
  - Entry-point signature is correct for executable artifacts.
  - Default/optional/rest arguments either lower correctly or produce scoped diagnostics.
  - Argument conversions and evaluation are represented in HxcIR.
  - Recursion and mutual recursion compile through header planning.
- **Labels:** hxc-plan, epic-e2, effort-large, lowering, functions

#### E2.T04 — Serialize Haxe evaluation order explicitly

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E2.T03, E1.T05
- **PRD references:** §12.2
- **Requirement IDs:** `HXC-PROD-002`, `HXC-COMP-003`, `HXC-SEM-004`
- **Outcome:** Introduce temporaries and sequencing so generated behavior never depends on C's unspecified operand/argument order.
- **Description:** Introduce temporaries and sequencing so generated behavior never depends on C's unspecified operand/argument order. PRD: §12.2
- **Acceptance criteria:**
  - Adversarial side-effect fixtures cover calls, assignments, fields, indexing, short circuit, ternary, and increments.
  - IR shows the chosen order explicitly.
  - Optimization passes may remove temporaries only with a proof/test.
  - Differential execution matches a reference Haxe target.
- **Labels:** hxc-plan, epic-e2, effort-large, semantics, evaluation-order

#### E2.T05 — Implement UB-safe integer and floating operations

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E2.T01, E2.T04
- **PRD references:** §11.4, §12.1
- **Requirement IDs:** `HXC-SEM-002`, `HXC-SEM-003`, `HXC-QA-004`
- **Outcome:** Preserve Haxe arithmetic semantics without signed overflow, invalid shifts, division overflow, or unchecked conversion undefined behavior.
- **Description:** Preserve Haxe arithmetic semantics without signed overflow, invalid shifts, division overflow, or unchecked conversion undefined behavior. PRD: §11.4, §12.1
- **Acceptance criteria:**
  - INT_MIN/-1, divide-by-zero, negative shifts, overflow, modulo, and bit operations have boundary tests.
  - Generated code passes UBSan where supported.
  - Runtime helpers are used only where direct C cannot express the contract safely.
  - Fast paths remain inspectable under optimization.
- **Labels:** hxc-plan, epic-e2, effort-large, semantics, ub

#### E2.T06 — Lower if, short-circuit, loops, switch, and jumps

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E2.T04
- **PRD references:** §12.14
- **Requirement IDs:** `HXC-SCAF-004`, `HXC-SEM-022`, `HXC-SEM-023`
- **Outcome:** Implement structured control flow while preserving expression values, break/continue targets, and cleanup obligations.
- **Description:** Implement structured control flow while preserving expression values, break/continue targets, and cleanup obligations. PRD: §12.14
- **Acceptance criteria:**
  - Nested loops/switches and labeled internal jumps are tested.
  - Short-circuit operands execute exactly as Haxe specifies.
  - No user-observable fallthrough is introduced.
  - Unreachable or unsupported control flow has stable diagnostics.
- **Labels:** hxc-plan, epic-e2, effort-large, lowering, control-flow

#### E2.T07 — Lower string literals and minimal trace/Sys.println

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E2.T03, E4.T03
- **PRD references:** §12.3, §14.4
- **Requirement IDs:** `HXC-PROD-002`, `HXC-SEM-006`, `HXC-EX-001`
- **Outcome:** Support immutable literals and the minimal hosted output path needed by hello without pulling the full object runtime.
- **Description:** Support immutable literals and the minimal hosted output path needed by hello without pulling the full object runtime. PRD: §12.3, §14.4
- **Acceptance criteria:**
  - Literal bytes/length and chosen encoding match the string ADR.
  - NUL bytes are represented explicitly.
  - Sys.println handles errors according to the hosted policy.
  - Runtime plan records only required string/I/O slices.
- **Labels:** hxc-plan, epic-e2, effort-medium, lowering, strings, io

#### E2.T08 — Lower fixed arrays and span-based iteration

- **Priority / effort / type:** P1 / medium / task
- **Blocked by:** E2.T04, E2.T05
- **PRD references:** §12.4, §17.1
- **Requirement IDs:** `HXC-SEM-005`, `HXC-SEM-007`, `HXC-MAC-001`
- **Outcome:** Support compile-time fixed arrays and c.Span/c.ConstSpan without the general Haxe Array runtime.
- **Description:** Support compile-time fixed arrays and c.Span/c.ConstSpan without the general Haxe Array runtime. PRD: §12.4, §17.1
- **Acceptance criteria:**
  - Bounds behavior is explicit by profile/build mode.
  - Pointer arithmetic is element-scaled and overflow-aware.
  - For loops over spans compile to ordinary indexed/pointer loops.
  - Metal no-runtime fixture links no hxrt symbol.
- **Labels:** hxc-plan, epic-e2, effort-medium, lowering, arrays, metal

#### E2.T09 — Implement deterministic static initialization phase

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E2.T03, E2.T06
- **PRD references:** §12.15
- **Requirement IDs:** `HXC-COMP-003`, `HXC-SEM-004`, `HXC-SEM-021`
- **Outcome:** Order module/class initialization explicitly and reject unresolvable cycles.
- **Description:** Order module/class initialization explicitly and reject unresolvable cycles. PRD: §12.15
- **Acceptance criteria:**
  - Initialization graph and reasons are inspectable.
  - Each initializer runs once before first required use according to the selected strategy.
  - Cycles have a deterministic diagnostic path.
  - No compiler discovery order affects output or behavior.
- **Labels:** hxc-plan, epic-e2, effort-large, semantics, initialization

#### E2.T10 — Complete and run the hello vertical slice

- **Priority / effort / type:** P0 / medium / task
- **Blocked by:** E2.T03, E2.T05, E2.T06, E2.T07, E1.T07
- **PRD references:** §19.1, §25 M2
- **Requirement IDs:** `HXC-PROD-001`, `HXC-EX-001`, `HXC-QA-002`, `HXC-QA-005`
- **Outcome:** Compile examples/hello from Haxe through generated C to a native executable.
- **Description:** Compile examples/hello from Haxe through generated C to a native executable. PRD: §19.1, §25 M2
- **Acceptance criteria:**
  - Exact stdout is `Hello from hxc` plus one newline.
  - GCC and Clang builds are warning-clean.
  - Output is deterministic and readable against the documented baseline.
  - Runtime-plan contains a source reason for every selected feature and no broad object/GC/reflection slice.
- **Labels:** hxc-plan, epic-e2, effort-medium, vertical-slice, example

#### E2.T11 — Create primitive differential and sanitizer suite

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E2.T10, E0.T09
- **PRD references:** §21.5, §21.6
- **Requirement IDs:** `HXC-PROD-002`, `HXC-QA-004`, `HXC-QA-005`
- **Outcome:** Run generated C against a selected Haxe reference target over edge cases and randomized primitive programs.
- **Description:** Run generated C against a selected Haxe reference target over edge cases and randomized primitive programs. PRD: §21.5, §21.6
- **Acceptance criteria:**
  - Seeded randomized cases are reproducible.
  - Semantic mismatches minimize to stored fixtures.
  - ASan/UBSan are clean on supported hosts.
  - Known intentional divergences require explicit ledger entries and tests.
- **Labels:** hxc-plan, epic-e2, effort-large, testing, differential, sanitizer

### 30.4 E3 — Aggregates, generics, classes, and dispatch

**Milestone:** M3\
**Priority:** P1\
**Outcome:** Lower Haxe's structured type system into readable C layouts and calls without defaulting to a universal box.

#### E3.T01 — Lower anonymous structures and value structs

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E2.T11, E1.T03
- **PRD references:** §12.5, §17.2
- **Requirement IDs:** `HXC-SEM-009`, `HXC-SEM-014`, `HXC-MAC-002`
- **Outcome:** Represent structurally known records as named/deduplicated C structs with clear value/reference behavior.
- **Description:** Represent structurally known records as named/deduplicated C structs with clear value/reference behavior. PRD: §12.5, §17.2
- **Acceptance criteria:**
  - Field order/layout policy is deterministic.
  - Anonymous structural equality does not accidentally become C nominal incompatibility.
  - By-value copies and address-taking are explicit.
  - Metal `@:c.layout(c.Layout.Struct)` validation rejects unsupported layout combinations.
- **Labels:** hxc-plan, epic-e3, effort-large, lowering, structs

#### E3.T02 — Lower Haxe enums as tagged unions

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E3.T01
- **PRD references:** §12.6
- **Requirement IDs:** `HXC-SEM-010`, `HXC-SEM-016`, `HXC-SEM-024`, `HXC-MAC-002`
- **Outcome:** Emit readable discriminants and payload unions, including recursive and generic cases.
- **Description:** Emit readable discriminants and payload unions, including recursive and generic cases. PRD: §12.6
- **Acceptance criteria:**
  - Nullary constructors may optimize to enums when ABI-safe.
  - Payload access is tag-checked in safe/debug configurations.
  - Pattern matches are exhaustive and compile to structured switch code.
  - Recursive representation avoids illegal infinitely sized C types.
- **Labels:** hxc-plan, epic-e3, effort-large, lowering, enums

#### E3.T03 — Implement deterministic generic monomorphization

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E2.T11, E1.T04
- **PRD references:** §12.9
- **Requirement IDs:** `HXC-SEM-007`, `HXC-SEM-015`, `HXC-SEM-016`, `HXC-MAC-005`
- **Outcome:** Specialize reachable generic code/types by normalized type arguments and share equivalent instances.
- **Description:** Specialize reachable generic code/types by normalized type arguments and share equivalent instances. PRD: §12.9
- **Acceptance criteria:**
  - Specialization keys are stable and collision-free.
  - Recursive generic discovery terminates.
  - Code-size and specialization reports are emitted.
  - Unsupported dynamic/open generic cases have precise diagnostics or explicit runtime strategy.
- **Labels:** hxc-plan, epic-e3, effort-large, lowering, generics

#### E3.T04 — Define and emit class instance layouts

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E3.T01, E3.T03
- **PRD references:** §12.7
- **Requirement IDs:** `HXC-PROD-002`, `HXC-SEM-013`, `HXC-SEM-014`
- **Outcome:** Lower fields, inheritance prefix/layout, object headers, and null/reference representation according to profile/runtime needs.
- **Description:** Lower fields, inheritance prefix/layout, object headers, and null/reference representation according to profile/runtime needs. PRD: §12.7
- **Acceptance criteria:**
  - Layout has compile-time assertions where ABI relevant.
  - Base/derived conversions are safe and inspectable.
  - Portable metadata is selective; metal value/opaque representations remain available.
  - Private implementation layout never leaks into exported ABI by default.
- **Labels:** hxc-plan, epic-e3, effort-large, lowering, classes

#### E3.T05 — Lower constructors, super calls, and field initialization

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E3.T04, E2.T09
- **PRD references:** §12.7, §12.15
- **Requirement IDs:** `HXC-SEM-007`, `HXC-SEM-013`, `HXC-SEM-021`
- **Outcome:** Preserve Haxe construction order, default field initialization, and failure cleanup.
- **Description:** Preserve Haxe construction order, default field initialization, and failure cleanup. PRD: §12.7, §12.15
- **Acceptance criteria:**
  - Super/field/body ordering matches Haxe semantics.
  - Partially initialized objects are cleaned safely on failure.
  - Empty/trivial constructors optimize away where proven.
  - Constructor cycles and unsupported native layouts diagnose clearly.
- **Labels:** hxc-plan, epic-e3, effort-large, lowering, constructors

#### E3.T06 — Implement virtual method dispatch and override validation

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E3.T04, E3.T05
- **PRD references:** §12.7
- **Requirement IDs:** `HXC-SEM-013`, `HXC-SEM-014`, `HXC-SEM-015`
- **Outcome:** Emit minimal vtables only for reachable polymorphic calls and preserve override signatures.
- **Description:** Emit minimal vtables only for reachable polymorphic calls and preserve override signatures. PRD: §12.7
- **Acceptance criteria:**
  - Nonvirtual/final/direct calls remain direct.
  - Vtable slots are deterministic across modules.
  - Override variance/representation mismatches are rejected.
  - Dispatch reports explain each indirect call.
- **Labels:** hxc-plan, epic-e3, effort-large, lowering, dispatch

#### E3.T07 — Implement interface tables and casts

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E3.T06
- **PRD references:** §12.8
- **Requirement IDs:** `HXC-SEM-013`, `HXC-SEM-015`, `HXC-SEM-023`
- **Outcome:** Represent reachable interfaces with compact itables or equivalent descriptors and safe cast checks.
- **Description:** Represent reachable interfaces with compact itables or equivalent descriptors and safe cast checks. PRD: §12.8
- **Acceptance criteria:**
  - Multiple interfaces and inherited interfaces work.
  - Failed casts follow Haxe semantics.
  - Unused interfaces emit no tables.
  - Interface method calls remain ABI-internal unless explicitly exported.
- **Labels:** hxc-plan, epic-e3, effort-large, lowering, interfaces

#### E3.T08 — Lower function values and closures

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E3.T03, E4.T02
- **PRD references:** §12.10
- **Requirement IDs:** `HXC-SEM-012`, `HXC-SEM-016`, `HXC-SEM-023`
- **Outcome:** Represent function values as typed function/context pairs and allocate environments only when required.
- **Description:** Represent function values as typed function/context pairs and allocate environments only when required. PRD: §12.10
- **Acceptance criteria:**
  - Nonescaping captures can live on stack/region.
  - Escaping closures retain captured values safely.
  - Recursive closures work.
  - Calling convention and cleanup are represented explicitly.
- **Labels:** hxc-plan, epic-e3, effort-large, lowering, closures

#### E3.T09 — Add devirtualization and escape-analysis passes

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E3.T06, E3.T07, E3.T08
- **PRD references:** §13.4, §21.7
- **Requirement IDs:** `HXC-SEM-013`, `HXC-SEM-015`, `HXC-SEM-016`, `HXC-RT-007`, `HXC-MAC-006`
- **Outcome:** Prove and apply direct-call, stack-allocation, scalar-replacement, and dead-metadata optimizations without changing semantics.
- **Description:** Prove and apply direct-call, stack-allocation, scalar-replacement, and dead-metadata optimizations without changing semantics. PRD: §13.4, §21.7
- **Acceptance criteria:**
  - Every optimization has a reportable proof/reason and opt-out debug mode.
  - Differential suite runs with passes on/off.
  - Nonescaping closure/object benchmark performs zero heap allocations.
  - Optimization never changes public ABI artifacts.
- **Labels:** hxc-plan, epic-e3, effort-large, optimization, analysis

#### E3.T10 — Build aggregate/generic/class semantic corpus

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E3.T02, E3.T03, E3.T05, E3.T07, E3.T08
- **PRD references:** §21
- **Requirement IDs:** `HXC-SEM-021`, `HXC-QA-001`, `HXC-QA-005`
- **Outcome:** Create positive, negative, recursive, cross-module, and differential fixtures for M3 semantics.
- **Description:** Create positive, negative, recursive, cross-module, and differential fixtures for M3 semantics. PRD: §21
- **Acceptance criteria:**
  - Recursive enums, generic containers, inheritance, interfaces, closures, and anonymous records are covered.
  - GCC/Clang warning and sanitizer gates pass.
  - Generated C snapshots are reviewed for readability.
  - M3 capability manifest is updated from evidence.
- **Labels:** hxc-plan, epic-e3, effort-large, testing, m3

### 30.5 E4 — Runtime, memory, dynamic values, and exceptions

**Milestone:** M4\
**Priority:** P1\
**Outcome:** Implement selective runtime capabilities and full portable object-graph semantics while preserving a proven runtime-free lane.

#### E4.T01 — Implement runtime feature graph and selective packaging

- **Priority / effort / type:** P1 / medium / task
- **Blocked by:** E1.T07, E0.T10
- **PRD references:** §13.1
- **Requirement IDs:** `HXC-RT-001`, `HXC-RT-002`, `HXC-RT-004`, `HXC-RT-008`
- **Outcome:** Turn runtime feature IDs/dependencies/reasons into deterministic source/header/link selection.
- **Description:** Turn runtime feature IDs/dependencies/reasons into deterministic source/header/link selection. PRD: §13.1
- **Acceptance criteria:**
  - Feature closure is deterministic and cycle-checked.
  - Every selected feature has at least one source reason.
  - Unselected runtime files/symbols are absent from build plan.
  - Manual overrides are validated against profile/runtime policy.
- **Labels:** hxc-plan, epic-e4, effort-medium, runtime, planner

#### E4.T02 — Harden allocator ABI and custom allocator integration

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E4.T01, E0.T05
- **PRD references:** §13.2
- **Requirement IDs:** `HXC-SEM-019`, `HXC-RT-005`, `HXC-RT-011`
- **Outcome:** Finalize aligned allocation/reallocation/release contracts for hosted, freestanding, and exported ownership.
- **Description:** Finalize aligned allocation/reallocation/release contracts for hosted, freestanding, and exported ownership. PRD: §13.2
- **Acceptance criteria:**
  - Overflow/alignment/zero-size behavior is specified and tested.
  - Custom allocator fixture works without libc allocation.
  - Allocator identity follows allocations that cross boundaries.
  - C and C++ ABI/layout tests pass on supported platforms.
- **Labels:** hxc-plan, epic-e4, effort-large, runtime, allocator

#### E4.T03 — Implement string runtime and encoding contract

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E4.T02, E0.T01
- **PRD references:** §12.3, §13, ADR 0004
- **Requirement IDs:** `HXC-PROD-005`, `HXC-SEM-006`, `HXC-RT-008`
- **Outcome:** Provide immutable/mutable string primitives, UTF policy, hashing, comparison, slicing, conversion, and C boundary views.
- **Description:** Provide immutable/mutable string primitives, UTF policy, hashing, comparison, slicing, conversion, and C boundary views. PRD: §12.3, §13, ADR 0004
- **Acceptance criteria:**
  - Embedded NUL and invalid encoding policy are tested.
  - Borrowed CString lifetime and owned conversion are explicit.
  - Common literal/concat/compare paths are direct and allocation-aware.
  - String feature can link without object/GC/reflection when possible.
- **Labels:** hxc-plan, epic-e4, effort-large, runtime, string

#### E4.T04 — Implement Haxe Array runtime slice

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E4.T02, E2.T01
- **PRD references:** §12.4
- **Requirement IDs:** `HXC-PROD-002`, `HXC-SEM-007`, `HXC-RT-008`
- **Outcome:** Support resizable typed arrays with correct length/capacity, bounds, mutation, iteration, and element retention.
- **Description:** Support resizable typed arrays with correct length/capacity, bounds, mutation, iteration, and element retention. PRD: §12.4
- **Acceptance criteria:**
  - Primitive and reference element arrays are covered.
  - Growth overflow and allocation failure are safe.
  - Array operations preserve Haxe semantics under aliasing.
  - Metal fixed arrays/spans do not depend on this slice.
- **Labels:** hxc-plan, epic-e4, effort-large, runtime, array

#### E4.T05 — Implement object/type descriptor core

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E3.T04, E4.T02
- **PRD references:** §13.1, §12.12
- **Requirement IDs:** `HXC-SEM-013`, `HXC-RT-006`, `HXC-RT-008`
- **Outcome:** Define minimal portable object headers/descriptors needed by collection, dispatch, reflection, and cleanup.
- **Description:** Define minimal portable object headers/descriptors needed by collection, dispatch, reflection, and cleanup. PRD: §13.1, §12.12
- **Acceptance criteria:**
  - Descriptors are immutable/deterministic and only emitted when reachable.
  - Object size/alignment/trace/finalization information is validated.
  - Metal no-runtime types have no hidden header.
  - Descriptor ABI is internal and versioned.
- **Labels:** hxc-plan, epic-e4, effort-large, runtime, object

#### E4.T06 — Implement selected portable collector

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E4.T05, E0.T01
- **PRD references:** §13.3, ADR 0005
- **Requirement IDs:** `HXC-SEM-025`, `HXC-RT-003`, `HXC-RT-006`
- **Outcome:** Implement the ratified collector with precise roots/fields, cycles, allocation pressure, and deterministic integration points.
- **Description:** Implement the ratified collector with precise roots/fields, cycles, allocation pressure, and deterministic integration points. PRD: §13.3, ADR 0005
- **Acceptance criteria:**
  - Cyclic graphs are reclaimed.
  - Interior/foreign pointers follow documented restrictions.
  - Stack/global/thread roots are correct on supported platforms.
  - Collector stress suite is sanitizer-clean and reports pauses/allocations.
- **Labels:** hxc-plan, epic-e4, effort-xlarge, runtime, gc

#### E4.T07 — Implement dynamic value representation and operations

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E4.T05, E4.T06, E3.T03
- **PRD references:** §12.11
- **Requirement IDs:** `HXC-SEM-017`, `HXC-SEM-023`, `HXC-RT-008`
- **Outcome:** Box only source sites that require Dynamic/heterogeneous representation and implement conversions, fields, calls, equality, and errors.
- **Description:** Box only source sites that require Dynamic/heterogeneous representation and implement conversions, fields, calls, equality, and errors. PRD: §12.11
- **Acceptance criteria:**
  - Typed code paths do not route through Dynamic.
  - Tag/payload representation is UB-safe.
  - Boxing reasons appear in inspection reports.
  - Differential fixtures cover primitives, objects, enums, functions, and null.
- **Labels:** hxc-plan, epic-e4, effort-xlarge, runtime, dynamic

#### E4.T08 — Implement selective reflection and RTTI slices

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E4.T07, E3.T07
- **PRD references:** §12.12
- **Requirement IDs:** `HXC-PROD-002`, `HXC-SEM-018`, `HXC-RT-008`
- **Outcome:** Support Type/Reflect/runtime type information only for operations and types proven reachable.
- **Description:** Support Type/Reflect/runtime type information only for operations and types proven reachable. PRD: §12.12
- **Acceptance criteria:**
  - Feature-level and type-level reflection reachability are inspectable.
  - Metadata not used by the program is absent.
  - Field lookup/call/create semantics match Haxe for supported cases.
  - No-runtime builds receive reasoned errors for reflective operations.
- **Labels:** hxc-plan, epic-e4, effort-large, runtime, reflection

#### E4.T09 — Implement exceptions, catch, finally, and cleanup regions

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E4.T02, E2.T06, E3.T05
- **PRD references:** §12.13, §13.6, ADR 0006
- **Requirement IDs:** `HXC-SEM-019`, `HXC-RT-009`, `HXC-RT-010`
- **Outcome:** Preserve throw/catch/finally behavior with explicit cleanup and no exception crossing a public C ABI.
- **Description:** Preserve throw/catch/finally behavior with explicit cleanup and no exception crossing a public C ABI. PRD: §12.13, §13.6
- **Acceptance criteria:**
  - Nested try/catch/finally and rethrow order matches Haxe.
  - Resources and partially initialized values clean up exactly once.
  - Result lowering is used where statically/legal and reported.
  - General runtime exception mechanism is portable across supported compilers or scoped by platform.
- **Labels:** hxc-plan, epic-e4, effort-xlarge, runtime, exceptions

#### E4.T10 — Implement no-runtime eligibility proof and diagnostics

- **Priority / effort / type:** P0 / large / task
- **Blocked by:** E2.T11, E4.T01
- **PRD references:** §9.4, §13.5
- **Requirement IDs:** `HXC-RT-002`, `HXC-RT-003`, `HXC-EX-002`
- **Outcome:** Decide whether a whole program can honor hxc_runtime=none and explain every blocker with source provenance.
- **Description:** Decide whether a whole program can honor hxc_runtime=none and explain every blocker with source provenance. PRD: §9.4, §13.5
- **Acceptance criteria:**
  - Eligible primitive/span fixtures link with no hxrt symbols or files.
  - Ineligible programs fail before native link with sorted blockers.
  - No helper is selected merely because a type was seen but not used.
  - Proof is emitted in runtime-plan JSON.
- **Labels:** hxc-plan, epic-e4, effort-large, analysis, no-runtime

#### E4.T11 — Version runtime ABI and feature manifests

- **Priority / effort / type:** P1 / medium / task
- **Blocked by:** E4.T01, E4.T02
- **PRD references:** §13.1, §16.8
- **Requirement IDs:** `HXC-RT-004`, `HXC-RT-010`, `HXC-RT-012`
- **Outcome:** Define internal/public runtime ABI compatibility, feature schema versions, and generated-code checks.
- **Description:** Define internal/public runtime ABI compatibility, feature schema versions, and generated-code checks. PRD: §13.1, §16.8
- **Acceptance criteria:**
  - Generated code rejects incompatible runtime major versions.
  - Feature manifests are machine-diffable.
  - Public exported APIs do not accidentally expose unstable runtime structs.
  - Release artifacts include runtime source/build provenance.
- **Labels:** hxc-plan, epic-e4, effort-medium, runtime, abi

#### E4.T12 — Create runtime stress, failure, and sanitizer suite

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E4.T03, E4.T04, E4.T06, E4.T09
- **PRD references:** §21.6
- **Requirement IDs:** `HXC-RT-008`, `HXC-RT-009`, `HXC-QA-004`
- **Outcome:** Exercise allocation failure, cycles, deep graphs, strings/arrays, exception cleanup, threads where applicable, and corrupted inputs.
- **Description:** Exercise allocation failure, cycles, deep graphs, strings/arrays, exception cleanup, threads where applicable, and corrupted inputs. PRD: §21.6
- **Acceptance criteria:**
  - ASan/UBSan/LSan and platform equivalents are clean.
  - Deterministic fault injection reaches every allocation site class.
  - Stress seeds and limits are reproducible.
  - Runtime test failures identify selected feature/version/configuration.
- **Labels:** hxc-plan, epic-e4, effort-large, testing, runtime, sanitizer

### 30.6 E5 — Haxe standard-library parity

**Milestone:** M5\
**Priority:** P1\
**Outcome:** Systematically support the Haxe standard library with direct lowering, target-owned adapters, and narrow runtime slices.

#### E5.T01 — Generate and maintain the stdlib ownership/parity ledger

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E0.T09, E4.T01
- **PRD references:** §14.2, §14.3
- **Requirement IDs:** `HXC-PROD-006`, `HXC-SEM-025`, `HXC-STD-001`
- **Outcome:** Inventory Haxe std modules/APIs and assign direct lowering, target override, runtime helper, native adapter, unsupported, and test provenance.
- **Description:** Inventory Haxe std modules/APIs and assign direct lowering, target override, runtime helper, native adapter, unsupported, and test provenance. PRD: §14.2, §14.3
- **Acceptance criteria:**
  - Ledger is generated/checkable against the pinned Haxe std tree.
  - Every unsupported/partial API has a linked Beads issue and diagnostic policy.
  - Rows include profile/environment/runtime/test ownership.
  - CI prevents unreviewed ledger drift on Haxe upgrades.
- **Labels:** hxc-plan, epic-e5, effort-large, stdlib, ledger

#### E5.T02 — Implement String and Unicode stdlib parity

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E4.T03, E5.T01
- **PRD references:** §14.4
- **Requirement IDs:** `HXC-SEM-006`, `HXC-STD-002`, `HXC-STD-007`
- **Outcome:** Support String, StringBuf, Unicode/code-point operations, and encoding conversions to the agreed compatibility level.
- **Description:** Support String, StringBuf, Unicode/code-point operations, and encoding conversions to the agreed compatibility level. PRD: §14.4
- **Acceptance criteria:**
  - Pinned upstream tests for the scope pass or have explicit ledger gaps.
  - ASCII fast paths and Unicode correctness are both tested.
  - Invalid sequences and indexing semantics are documented.
  - C boundary conversions expose ownership and failure.
- **Labels:** hxc-plan, epic-e5, effort-xlarge, stdlib, string, unicode

#### E5.T03 — Implement core collection parity

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E4.T04, E3.T03, E5.T01
- **PRD references:** §14.4
- **Requirement IDs:** `HXC-STD-002`, `HXC-STD-003`, `HXC-STD-007`
- **Outcome:** Support Array, Vector, List, StringMap, IntMap, ObjectMap, EnumValueMap, and iterators with typed representations.
- **Description:** Support Array, Vector, List, StringMap, IntMap, ObjectMap, EnumValueMap, and iterators with typed representations. PRD: §14.4
- **Acceptance criteria:**
  - Collection semantics and mutation during iteration match Haxe where specified.
  - Generic specialization avoids universal boxing when types are known.
  - Hash/equality behavior is explicit for dynamic/object keys.
  - Upstream and adversarial tests pass.
- **Labels:** hxc-plan, epic-e5, effort-xlarge, stdlib, collections

#### E5.T04 — Implement Bytes, Buffer, and core I/O parity

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E4.T04, E5.T01
- **PRD references:** §14.4
- **Requirement IDs:** `HXC-SEM-006`, `HXC-SEM-008`, `HXC-STD-004`
- **Outcome:** Support haxe.io.Bytes/BytesBuffer/Input/Output and endian/encoding operations with safe bounds and partial I/O.
- **Description:** Support haxe.io.Bytes/BytesBuffer/Input/Output and endian/encoding operations with safe bounds and partial I/O. PRD: §14.4
- **Acceptance criteria:**
  - Binary round-trips and boundary failures match Haxe.
  - Large lengths cannot overflow native allocation or pointer arithmetic.
  - Hosted and custom stream adapters are separable.
  - No hidden text conversion occurs in byte APIs.
- **Labels:** hxc-plan, epic-e5, effort-xlarge, stdlib, bytes, io

#### E5.T05 — Implement Std, Math, Type, and Reflect parity

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E4.T07, E4.T08, E5.T01
- **PRD references:** §14
- **Requirement IDs:** `HXC-SEM-018`, `HXC-SEM-023`, `HXC-STD-002`
- **Outcome:** Cover core conversion, math, type inspection/creation, and reflection surfaces with selective runtime use.
- **Description:** Cover core conversion, math, type inspection/creation, and reflection surfaces with selective runtime use. PRD: §14
- **Acceptance criteria:**
  - Math edge cases follow Haxe target contract.
  - Std.string/parse/casts have differential tests.
  - Type/Reflect only retain necessary descriptors.
  - No-runtime diagnostics identify reflective dependency chains.
- **Labels:** hxc-plan, epic-e5, effort-xlarge, stdlib, core, reflection

#### E5.T06 — Implement JSON and serialization support

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E5.T02, E5.T03, E4.T07
- **PRD references:** §14.4
- **Requirement IDs:** `HXC-STD-004`, `HXC-STD-008`, `HXC-STD-009`
- **Outcome:** Support haxe.Json parse/stringify and required serialization primitives with bounded resource behavior.
- **Description:** Support haxe.Json parse/stringify and required serialization primitives with bounded resource behavior. PRD: §14.4
- **Acceptance criteria:**
  - Round-trip and malformed-input corpus passes.
  - Depth/size limits and error positions are explicit.
  - Typed fast paths do not require reflection where generated serializers suffice.
  - Fuzzing hooks are available.
- **Labels:** hxc-plan, epic-e5, effort-large, stdlib, json

#### E5.T07 — Implement regular-expression support

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E5.T02, E5.T04
- **PRD references:** §14.4
- **Requirement IDs:** `HXC-SEM-025`, `HXC-STD-006`, `HXC-STD-007`
- **Outcome:** Choose/integrate a regex backend or scoped implementation that meets Haxe EReg semantics on supported platforms.
- **Description:** Choose/integrate a regex backend or scoped implementation that meets Haxe EReg semantics on supported platforms. PRD: §14.4
- **Acceptance criteria:**
  - Backend/license/feature mapping is documented.
  - EReg matching, groups, replace, split, and global state tests pass.
  - Unsupported syntax is detected deterministically.
  - Runtime feature and native dependency are explicit.
- **Labels:** hxc-plan, epic-e5, effort-large, stdlib, regex

#### E5.T08 — Implement Date and time adapters

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E5.T01, E5.T02
- **PRD references:** §14.4
- **Requirement IDs:** `HXC-SEM-025`, `HXC-STD-006`, `HXC-STD-010`
- **Outcome:** Support Date/timezone/timestamp operations through portable and platform adapters.
- **Description:** Support Date/timezone/timestamp operations through portable and platform adapters. PRD: §14.4
- **Acceptance criteria:**
  - UTC/local/DST boundary corpus is deterministic with controlled timezone data.
  - Precision/range conversions are overflow-safe.
  - Freestanding/WASI limitations are explicit.
  - Clock APIs distinguish wall and monotonic time.
- **Labels:** hxc-plan, epic-e5, effort-large, stdlib, time

#### E5.T09 — Implement filesystem, environment, and process adapters

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E5.T04, E5.T01
- **PRD references:** §14.4, §12.18
- **Requirement IDs:** `HXC-SEM-025`, `HXC-STD-004`, `HXC-STD-005`
- **Outcome:** Support sys.FileSystem, sys.io.File, environment, command/process, paths, and errors on hosted targets.
- **Description:** Support sys.FileSystem, sys.io.File, environment, command/process, paths, and errors on hosted targets. PRD: §14.4, §12.18
- **Acceptance criteria:**
  - POSIX and Windows path/error behavior has platform tests.
  - Argument/environment escaping does not invoke a shell unless requested.
  - WASI/freestanding capability errors are explicit.
  - Resources close on success and failure.
- **Labels:** hxc-plan, epic-e5, effort-xlarge, stdlib, sys

#### E5.T10 — Implement socket/network adapters

- **Priority / effort / type:** P2 / xlarge / task
- **Blocked by:** E5.T04, E5.T09
- **PRD references:** §14.4
- **Requirement IDs:** `HXC-PROD-002`, `HXC-STD-005`, `HXC-STD-008`
- **Outcome:** Support the scoped sys.net surface with portable address/error/blocking semantics.
- **Description:** Support the scoped sys.net surface with portable address/error/blocking semantics. PRD: §14.4
- **Acceptance criteria:**
  - TCP/UDP/local resolution fixtures run on supported CI.
  - Timeout/nonblocking behavior is explicit.
  - Windows socket initialization/cleanup is correct.
  - No network tests rely on external internet services.
- **Labels:** hxc-plan, epic-e5, effort-xlarge, stdlib, network

#### E5.T11 — Implement threads, synchronization, TLS, and atomics

- **Priority / effort / type:** P2 / xlarge / task
- **Blocked by:** E4.T06, E4.T12, E5.T01
- **PRD references:** §12.17, §14.4
- **Requirement IDs:** `HXC-SEM-025`, `HXC-STD-009`, `HXC-STD-010`
- **Outcome:** Map Haxe thread primitives to C11/platform threads with collector/root and memory-model integration.
- **Description:** Map Haxe thread primitives to C11/platform threads with collector/root and memory-model integration. PRD: §12.17, §14.4
- **Acceptance criteria:**
  - Mutex/condition/semaphore/thread-local/atomic semantics are tested.
  - Collector sees thread roots safely.
  - Data-race tests use TSan where available.
  - Single-thread/freestanding builds do not link thread support.
- **Labels:** hxc-plan, epic-e5, effort-xlarge, stdlib, threads, atomics

#### E5.T12 — Integrate upstream Haxe std tests and parity dashboard

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E5.T02, E5.T03, E5.T04, E5.T05, E5.T06, E5.T09
- **PRD references:** §14.5, §21.5
- **Requirement IDs:** `HXC-PROD-006`, `HXC-STD-001`, `HXC-QA-005`
- **Outcome:** Run the pinned upstream suite with explicit selection, results, gaps, and differential comparisons.
- **Description:** Run the pinned upstream suite with explicit selection, results, gaps, and differential comparisons. PRD: §14.5, §21.5
- **Acceptance criteria:**
  - No silent skip is possible.
  - Dashboard reports pass/fail/unsupported by module, profile, environment, and platform.
  - Every failure links to a Beads issue or approved divergence.
  - Release gates consume archived machine-readable results.
- **Labels:** hxc-plan, epic-e5, effort-large, testing, stdlib, dashboard

### 30.7 E6 — C interop and deterministic bindgen

**Milestone:** M6\
**Priority:** P1\
**Outcome:** Make existing C libraries easy and safe to consume from exact headers, flags, and ABI facts.

#### E6.T01 — Design and implement Clang-backed bindgen driver

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E1.T07, E8.T01
- **PRD references:** §15.1, §15.3
- **Requirement IDs:** `HXC-FFI-001`, `HXC-FFI-002`, `HXC-FFI-003`
- **Outcome:** Invoke Clang with exact target/defines/includes/flags and capture a versioned semantic declaration model.
- **Description:** Invoke Clang with exact target/defines/includes/flags and capture a versioned semantic declaration model. PRD: §15.1, §15.3
- **Acceptance criteria:**
  - Invocation is reproducible and inspectable.
  - Header parse diagnostics preserve source files/lines.
  - Clang version/target/flags enter the binding lock.
  - No regex parser is authoritative for ABI facts.
- **Labels:** hxc-plan, epic-e6, effort-xlarge, bindgen, clang

#### E6.T02 — Model preprocessor configuration and header entry sets

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E6.T01
- **PRD references:** §15.2, §15.3
- **Requirement IDs:** `HXC-FFI-002`, `HXC-FFI-003`, `HXC-MAC-003`
- **Outcome:** Represent translation-unit inputs, defines, include ordering, target triple, language mode, sysroot, and conditional declarations.
- **Description:** Represent translation-unit inputs, defines, include ordering, target triple, language mode, sysroot, and conditional declarations. PRD: §15.2, §15.3
- **Acceptance criteria:**
  - Equivalent invocations normalize to stable configuration hashes.
  - Conflicting defines/includes are diagnosed.
  - Generated wrappers include only declarations reachable from configured roots.
  - Configuration provenance appears in generated comments/lock.
- **Labels:** hxc-plan, epic-e6, effort-large, bindgen, preprocessor

#### E6.T03 — Map C scalars, enums, typedefs, qualifiers, and constants

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E6.T01, E2.T01
- **PRD references:** §15.4, §15.5
- **Requirement IDs:** `HXC-FFI-005`, `HXC-FFI-006`, `HXC-EX-003`
- **Outcome:** Generate exact raw Haxe extern representations for primitive ABI declarations.
- **Description:** Generate exact raw Haxe extern representations for primitive ABI declarations. PRD: §15.4, §15.5
- **Acceptance criteria:**
  - Integer width/signedness follows target ABI, not spelling assumptions.
  - Anonymous/named enums and macro constants have deterministic names.
  - Const/volatile/restrict and typedef identity are preserved where meaningful.
  - Compiled probes validate size/range/value mappings.
- **Labels:** hxc-plan, epic-e6, effort-large, bindgen, types

#### E6.T04 — Map structs, unions, anonymous members, bitfields, and packing

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E6.T03, E1.T01
- **PRD references:** §15.4
- **Requirement IDs:** `HXC-FFI-005`, `HXC-FFI-006`, `HXC-MAC-002`
- **Outcome:** Represent complete/incomplete aggregates and verify layout through Clang facts and generated C assertions/probes.
- **Description:** Represent complete/incomplete aggregates and verify layout through Clang facts and generated C assertions/probes. PRD: §15.4
- **Acceptance criteria:**
  - Size/alignment/offset/bitfield fixtures pass across supported targets.
  - Flexible arrays and packed/aligned declarations are explicit.
  - Unsupported nonportable layout produces a precise diagnostic.
  - Opaque handles remain opaque by default.
- **Labels:** hxc-plan, epic-e6, effort-xlarge, bindgen, layout

#### E6.T05 — Map functions, function pointers, callbacks, and variadics

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E6.T03, E6.T04, E3.T08
- **PRD references:** §15.4
- **Requirement IDs:** `HXC-FFI-001`, `HXC-FFI-007`, `HXC-FFI-008`
- **Outcome:** Generate callable externs with correct calling convention, nullability hooks, callback lifetime, and scoped variadic support.
- **Description:** Generate callable externs with correct calling convention, nullability hooks, callback lifetime, and scoped variadic support. PRD: §15.4
- **Acceptance criteria:**
  - cdecl/stdcall/platform conventions are tested where available.
  - Callback trampolines retain/release environments safely.
  - Variadics require configured typed wrappers unless a safe direct form exists.
  - errno/out-parameter/error conventions remain available to wrapper mapping.
- **Labels:** hxc-plan, epic-e6, effort-xlarge, bindgen, functions, callbacks

#### E6.T06 — Emit deterministic raw extern modules

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E6.T02, E6.T03, E6.T04, E6.T05
- **PRD references:** §15.5
- **Requirement IDs:** `HXC-FFI-001`, `HXC-FFI-004`, `HXC-FFI-012`
- **Outcome:** Write minimally transformed, mechanically reviewable Haxe externs plus native build metadata.
- **Description:** Write minimally transformed, mechanically reviewable Haxe externs plus native build metadata. PRD: §15.5
- **Acceptance criteria:**
  - Regeneration from identical input is byte-identical.
  - User-authored wrappers live separately and are never overwritten.
  - Names have stable collision handling and source comments.
  - Generated extern package type-checks under Haxe and the target.
- **Labels:** hxc-plan, epic-e6, effort-large, bindgen, emitter

#### E6.T07 — Implement declarative safe-wrapper mapping

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E6.T06, E4.T03, E4.T04
- **PRD references:** §15.6
- **Requirement IDs:** `HXC-FFI-004`, `HXC-FFI-009`, `HXC-FFI-012`
- **Outcome:** Generate optional idiomatic wrappers from explicit ownership, nullability, length, error, string, handle, and lifetime mappings.
- **Description:** Generate optional idiomatic wrappers from explicit ownership, nullability, length, error, string, handle, and lifetime mappings. PRD: §15.6
- **Acceptance criteria:**
  - Mapping file is schema-validated and versioned.
  - Unsafe raw layer remains accessible but clearly separated.
  - Ownership transfer and destructor selection are checked.
  - Wrapper tests include allocation failure and null/error paths.
- **Labels:** hxc-plan, epic-e6, effort-xlarge, bindgen, wrappers, ownership

#### E6.T08 — Generate binding locks, ABI probes, and drift reports

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E6.T06, E6.T07
- **PRD references:** §15.5, §16.8
- **Requirement IDs:** `HXC-FFI-003`, `HXC-FFI-011`, `HXC-QA-009`
- **Outcome:** Record inputs and ABI facts so dependency/header changes are detected before runtime corruption.
- **Description:** Record inputs and ABI facts so dependency/header changes are detected before runtime corruption. PRD: §15.5, §16.8
- **Acceptance criteria:**
  - Lock includes header digests, flags, target, Clang version, declaration/layout hashes, and mappings.
  - Compiled probes compare size/alignment/offset/calling assumptions.
  - Drift report categorizes source-only, wrapper, and ABI-breaking changes.
  - CI can require explicit lock update.
- **Labels:** hxc-plan, epic-e6, effort-large, bindgen, abi, lock

#### E6.T09 — Support pkg-config and compile_commands inputs

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E6.T02, E8.T04
- **PRD references:** §15.2
- **Requirement IDs:** `HXC-FFI-010`, `HXC-MAC-003`, `HXC-CLI-005`
- **Outcome:** Resolve exact include/define/link context from common native build metadata without hiding the final command.
- **Description:** Resolve exact include/define/link context from common native build metadata without hiding the final command. PRD: §15.2
- **Acceptance criteria:**
  - pkg-config static/dynamic modes are distinguished.
  - compile_commands selection is deterministic for multiple entries.
  - Resolved flags are normalized and inspectable.
  - Environment-dependent paths are recorded with reproducibility warnings.
- **Labels:** hxc-plan, epic-e6, effort-medium, bindgen, build-integration

#### E6.T10 — Generate supported C++ extern-C shims

- **Priority / effort / type:** P2 / xlarge / task
- **Blocked by:** E6.T05, E6.T07
- **PRD references:** §15.8, ADR 0003
- **Requirement IDs:** `HXC-FFI-013`, `HXC-FFI-014`, `HXC-EX-005`
- **Outcome:** Wrap configured C++ constructors/methods/destructors into opaque C handles and exception-safe C functions.
- **Description:** Wrap configured C++ constructors/methods/destructors into opaque C handles and exception-safe C functions. PRD: §15.8, ADR 0003
- **Acceptance criteria:**
  - No C++ class/template/STL/exceptions appear in generated public C header.
  - All C++ exceptions are caught and mapped to configured errors.
  - Ownership and destructor paths are tested.
  - Unsupported overload/template/lifetime cases are diagnosed rather than guessed.
- **Labels:** hxc-plan, epic-e6, effort-xlarge, bindgen, cpp, shim

#### E6.T11 — Add advisory AI mapping workflow without ABI authority

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E6.T07, E6.T08
- **PRD references:** §15.7
- **Requirement IDs:** `HXC-FFI-011`, `HXC-FFI-012`, `HXC-CLI-005`
- **Outcome:** Allow an LLM to propose names/docs/ownership mappings as a reviewable patch while deterministic parsed facts remain authoritative.
- **Description:** Allow an LLM to propose names/docs/ownership mappings as a reviewable patch while deterministic parsed facts remain authoritative. PRD: §15.7
- **Acceptance criteria:**
  - AI is off by default and never runs during reproducible build/regeneration.
  - Prompt/context/output provenance is recorded when used.
  - Suggestions cannot alter sizes, calling conventions, layouts, or declarations without deterministic validation.
  - Human review diff clearly separates suggestions from facts.
- **Labels:** hxc-plan, epic-e6, effort-medium, bindgen, ai, advisory

#### E6.T12 — Prove bindgen with SQLite and zlib-class fixtures

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E6.T08, E6.T09
- **PRD references:** §15, §25 M6
- **Requirement IDs:** `HXC-PROD-008`, `HXC-FFI-001`, `HXC-MAC-002`, `HXC-EX-003`
- **Outcome:** Regenerate and use representative stable C libraries covering opaque handles, callbacks, buffers, errors, enums, and build metadata.
- **Description:** Regenerate and use representative stable C libraries covering opaque handles, callbacks, buffers, errors, enums, and build metadata. PRD: §15, §25 M6
- **Acceptance criteria:**
  - Clean regeneration produces no diff.
  - Haxe examples compile/link/run against system or pinned fixture libraries.
  - Layout/ABI probes pass.
  - Unsupported platform differences are explicit in locks and CI matrix.
- **Labels:** hxc-plan, epic-e6, effort-large, bindgen, examples

### 30.8 E7 — Exported C ABI and shared libraries

**Milestone:** M7\
**Priority:** P1\
**Outcome:** Make Haxe-authored code a first-class producer of stable C headers, static libraries, and shared objects.

#### E7.T01 — Discover and validate exported declarations

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E3.T10, E0.T06
- **PRD references:** §16.1
- **Requirement IDs:** `HXC-RT-010`, `HXC-ABI-001`, `HXC-CLI-006`
- **Outcome:** Collect `@:c.export` and related public ABI metadata and build a deterministic export model independent of internal symbol names.
- **Description:** Collect `@:c.export` and related public ABI metadata and build a deterministic export model independent of internal symbol names. PRD: §16.1
- **Acceptance criteria:**
  - Duplicate/invalid names and visibility conflicts are source-positioned.
  - Only explicitly exported declarations enter public ABI.
  - Generic/instance exports require concrete wrapper shape.
  - Export inventory is inspectable before C generation.
- **Labels:** hxc-plan, epic-e7, effort-large, export, metadata

#### E7.T02 — Implement ABI-safe public type checker

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E7.T01, E6.T04
- **PRD references:** §16.2
- **Requirement IDs:** `HXC-ABI-004`, `HXC-ABI-005`, `HXC-ABI-007`
- **Outcome:** Allow fixed scalars, declared value structs/enums, pointers/spans, callbacks, opaque handles, and explicit result/error shapes while rejecting internal Haxe/runtime layouts.
- **Description:** Allow fixed scalars, declared value structs/enums, pointers/spans, callbacks, opaque handles, and explicit result/error shapes while rejecting internal Haxe/runtime layouts. PRD: §16.2
- **Acceptance criteria:**
  - Every public pointer has ownership and nullability.
  - By-value structs have stable layout assertions.
  - Strings/arrays/classes require explicit boundary mapping.
  - Diagnostics suggest a safe wrapper shape.
- **Labels:** hxc-plan, epic-e7, effort-xlarge, export, abi, types

#### E7.T03 — Emit public C headers and implementation wrappers

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E7.T02, E1.T03, E1.T04
- **PRD references:** §16.3, §16.4
- **Requirement IDs:** `HXC-ABI-001`, `HXC-ABI-002`, `HXC-ABI-003`
- **Outcome:** Generate standalone, namespaced, documented C headers and wrappers that translate to internal calling conventions.
- **Description:** Generate standalone, namespaced, documented C headers and wrappers that translate to internal calling conventions. PRD: §16.3, §16.4
- **Acceptance criteria:**
  - Header compiles as C11 and C++17 with extern-C guards.
  - Include guards, visibility, calling conventions, and integer types are portable.
  - No private/internal runtime symbol leaks.
  - C and C++ consumers need only installed public artifacts.
- **Labels:** hxc-plan, epic-e7, effort-xlarge, export, header

#### E7.T04 — Implement ownership, nullability, and error boundary adapters

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E7.T02, E4.T09
- **PRD references:** §16.5, §16.6
- **Requirement IDs:** `HXC-SEM-020`, `HXC-ABI-006`, `HXC-ABI-007`
- **Outcome:** Translate Haxe values/failures into explicit C contracts and generated destroy/free APIs.
- **Description:** Translate Haxe values/failures into explicit C contracts and generated destroy/free APIs. PRD: §16.5, §16.6
- **Acceptance criteria:**
  - No Haxe exception crosses the C ABI.
  - Owned outputs have a matching release function/allocator contract.
  - Borrowed lifetimes are documented and testable.
  - Out-parameter/status/result conventions are configurable but deterministic.
- **Labels:** hxc-plan, epic-e7, effort-xlarge, export, ownership, errors

#### E7.T05 — Emit visibility, symbol version, and export-map files

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E7.T03
- **PRD references:** §16.7
- **Requirement IDs:** `HXC-RT-010`, `HXC-ABI-009`, `HXC-ABI-010`
- **Outcome:** Hide internal symbols and export only the public ABI on ELF, Mach-O, and PE/COFF targets.
- **Description:** Hide internal symbols and export only the public ABI on ELF, Mach-O, and PE/COFF targets. PRD: §16.7
- **Acceptance criteria:**
  - nm/dumpbin-equivalent tests compare actual exports to manifest.
  - Version scripts/export lists/.def files are deterministic.
  - Static and shared builds use consistent public names.
  - Unsupported toolchain behavior is detected by doctor/build.
- **Labels:** hxc-plan, epic-e7, effort-large, export, visibility, platform

#### E7.T06 — Build static and shared library artifacts

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E7.T03, E7.T04, E7.T05, E8.T04
- **PRD references:** §16.7
- **Requirement IDs:** `HXC-ABI-009`, `HXC-ABI-010`, `HXC-CLI-006`
- **Outcome:** Produce archive/shared artifacts, import libraries where needed, and installation metadata from the neutral build plan.
- **Description:** Produce archive/shared artifacts, import libraries where needed, and installation metadata from the neutral build plan. PRD: §16.7
- **Acceptance criteria:**
  - Linux .so, macOS .dylib, and Windows .dll paths are covered according to the supported matrix.
  - PIC/import/export flags are correct.
  - CMake/Meson/pkg-config installation artifacts work.
  - Reproducible build metadata records toolchain and inputs.
- **Labels:** hxc-plan, epic-e7, effort-xlarge, export, shared-library, build

#### E7.T07 — Generate ABI manifest, hash, and compatibility diff

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E7.T02, E7.T03
- **PRD references:** §16.8
- **Requirement IDs:** `HXC-ABI-008`, `HXC-ABI-011`, `HXC-ABI-012`, `HXC-DOC-006`
- **Outcome:** Describe public symbols/types/layout/ownership/calling convention/version in a stable schema and classify changes.
- **Description:** Describe public symbols/types/layout/ownership/calling convention/version in a stable schema and classify changes. PRD: §16.8
- **Acceptance criteria:**
  - Manifest hash is independent of documentation/formatting-only changes.
  - Diff detects removed/changed symbols, layout, enum values, ownership, nullability, and calling convention.
  - Intentional breaks require version policy action.
  - CI stores before/after reports.
- **Labels:** hxc-plan, epic-e7, effort-large, export, abi, compatibility

#### E7.T08 — Run independent C and C++ consumer matrix

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E7.T06, E7.T07
- **PRD references:** §16.3, §25 M7
- **Requirement IDs:** `HXC-ABI-002`, `HXC-ABI-003`, `HXC-EX-004`, `HXC-QA-009`
- **Outcome:** Build/install/consume exported libraries outside the generator tree using multiple C/C++ compilers.
- **Description:** Build/install/consume exported libraries outside the generator tree using multiple C/C++ compilers. PRD: §16.3, §25 M7
- **Acceptance criteria:**
  - Consumers include only public headers and link installed artifacts.
  - C++ sees no name mangling issue.
  - Owned/error paths and repeated load/unload are tested.
  - Consumer failures cannot pass due to in-tree private include paths.
- **Labels:** hxc-plan, epic-e7, effort-large, export, consumer, testing

#### E7.T09 — Generate optional header-only C++ RAII wrappers

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E7.T03, E7.T04
- **PRD references:** §16.4
- **Requirement IDs:** `HXC-PROD-007`, `HXC-ABI-003`, `HXC-ABI-005`
- **Outcome:** Provide opt-in C++ convenience types over the stable C ABI without making C++ ABI part of the product contract.
- **Description:** Provide opt-in C++ convenience types over the stable C ABI without making C++ ABI part of the product contract. PRD: §16.4
- **Acceptance criteria:**
  - Wrapper is header-only or otherwise explicitly versioned.
  - Move/copy/destruction semantics match C ownership.
  - Exceptions are opt-in and never cross C functions.
  - Raw C API remains complete and primary.
- **Labels:** hxc-plan, epic-e7, effort-medium, export, cpp, raii

#### E7.T10 — Add foreign-language ABI smoke consumers

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E7.T08
- **PRD references:** §16, §19
- **Requirement IDs:** `HXC-EX-004`, `HXC-EX-009`, `HXC-QA-009`
- **Outcome:** Demonstrate that ordinary C ABI artifacts load from at least Python ctypes and one additional FFI ecosystem without language-specific generator magic.
- **Description:** Demonstrate that ordinary C ABI artifacts load from at least Python ctypes and one additional FFI ecosystem without language-specific generator magic. PRD: §16, §19
- **Acceptance criteria:**
  - Consumers run against installed shared library.
  - String/buffer/handle/error ownership is exercised.
  - Examples document platform loader paths safely.
  - These are ABI proofs, not promises of full high-level bindings.
- **Labels:** hxc-plan, epic-e7, effort-medium, export, ffi, examples

### 30.9 E8 — CLI, build adapters, diagnostics, and inspection

**Milestone:** M8\
**Priority:** P1\
**Outcome:** Turn compiler components into the coherent hxc product and an automation-friendly build surface.

#### E8.T01 — Implement hxc.json parser, schema validation, and precedence

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E0.T04, E0.T06
- **PRD references:** §18.2, docs/configuration.md
- **Requirement IDs:** `HXC-CLI-005`, `HXC-CLI-009`, `HXC-CLI-010`
- **Outcome:** Load strict configuration, overlays, CLI overrides, and direct defines into one provenance-aware build model.
- **Description:** Load strict configuration, overlays, CLI overrides, and direct defines into one provenance-aware build model. PRD: §18.2, docs/configuration.md
- **Acceptance criteria:**
  - Unknown keys and invalid combinations fail before compilation.
  - Path resolution is independent of current working directory.
  - Effective config and source of every value are inspectable.
  - Schema and parser tests stay synchronized.
- **Labels:** hxc-plan, epic-e8, effort-large, cli, configuration

#### E8.T02 — Build hxc command/router and JSON output conventions

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E8.T01
- **PRD references:** §18.1
- **Requirement IDs:** `HXC-CLI-004`, `HXC-CLI-005`, `HXC-CLI-010`
- **Outcome:** Implement stable command parsing, exit codes, logging, machine-readable responses, and help/version behavior.
- **Description:** Implement stable command parsing, exit codes, logging, machine-readable responses, and help/version behavior. PRD: §18.1
- **Acceptance criteria:**
  - Commands share error/diagnostic formatting.
  - --json emits only documented JSON on stdout.
  - Human logs go to stderr when JSON mode is active.
  - Unknown/experimental flags are not silently ignored.
- **Labels:** hxc-plan, epic-e8, effort-large, cli, ux

#### E8.T03 — Implement hxc new project templates

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E8.T02, E0.T02
- **PRD references:** §18.1, §25 M8
- **Requirement IDs:** `HXC-CLI-002`, `HXC-CLI-008`, `HXC-CLI-010`
- **Outcome:** Instantiate CLI, C ABI library, and freestanding templates with deterministic substitutions and no overwrite surprises.
- **Description:** Instantiate CLI, C ABI library, and freestanding templates with deterministic substitutions and no overwrite surprises. PRD: §18.1, §25 M8
- **Acceptance criteria:**
  - Template output validates against schema and type-checks.
  - Existing files require explicit force/merge behavior.
  - License/project name/module validation is safe.
  - Generated README states current target limitations honestly.
- **Labels:** hxc-plan, epic-e8, effort-medium, cli, templates

#### E8.T04 — Create neutral native build plan and toolchain adapters

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E1.T07, E8.T01
- **PRD references:** §18.3
- **Requirement IDs:** `HXC-COMP-010`, `HXC-MAC-003`, `HXC-CLI-003`
- **Outcome:** Translate generated sources, runtime slices, native sources, includes, defines, libraries, artifact type, and target into exact compile/link steps.
- **Description:** Translate generated sources, runtime slices, native sources, includes, defines, libraries, artifact type, and target into exact compile/link steps. PRD: §18.3
- **Acceptance criteria:**
  - Plan is serializable and inspectable.
  - GCC/Clang/MSVC-family adapters quote arguments without shell concatenation.
  - Cross-compile/sysroot/target settings are explicit.
  - Incremental dependency keys include relevant compiler inputs.
- **Labels:** hxc-plan, epic-e8, effort-xlarge, build, toolchain

#### E8.T05 — Implement build, run, test, and clean commands

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E8.T02, E8.T04, E2.T10
- **PRD references:** §18.1
- **Requirement IDs:** `HXC-CLI-003`, `HXC-CLI-004`, `HXC-CLI-008`
- **Outcome:** Provide the normal user loop while preserving separate codegen/native phases and exact command visibility.
- **Description:** Provide the normal user loop while preserving separate codegen/native phases and exact command visibility. PRD: §18.1
- **Acceptance criteria:**
  - build can stop after codegen/objects/library/executable.
  - run forwards arguments and exit code safely.
  - test integrates repository/user fixtures without hiding commands.
  - clean removes only owned outputs/caches.
- **Labels:** hxc-plan, epic-e8, effort-xlarge, cli, build

#### E8.T06 — Complete hxc doctor capability diagnostics

- **Priority / effort / type:** P1 / medium / task
- **Blocked by:** E8.T02, E8.T04, E6.T01
- **PRD references:** §18.4
- **Requirement IDs:** `HXC-CLI-001`, `HXC-CLI-005`, `HXC-CLI-010`
- **Outcome:** Check Haxe/Reflaxe/C/C++/Clang bindgen/build tools/platform features and explain missing optional versus required capabilities.
- **Description:** Check Haxe/Reflaxe/C/C++/Clang bindgen/build tools/platform features and explain missing optional versus required capabilities. PRD: §18.4
- **Acceptance criteria:**
  - Human and JSON modes are stable.
  - Doctor performs compile/link probes where version strings are insufficient.
  - Results include remediation and selected config/target context.
  - No network access is required for normal checks.
- **Labels:** hxc-plan, epic-e8, effort-medium, cli, doctor

#### E8.T07 — Implement hxc inspect reports

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E8.T02, E1.T07, E4.T01
- **PRD references:** §18.5
- **Requirement IDs:** `HXC-COMP-015`, `HXC-MAC-010`, `HXC-CLI-007`
- **Outcome:** Expose config, typed inventory, HxcIR, C AST, lowering decisions, runtime plan, symbols, includes, build commands, macros, ABI, and size/allocation summaries.
- **Description:** Expose config, typed inventory, HxcIR, C AST, lowering decisions, runtime plan, symbols, includes, build commands, macros, ABI, and size/allocation summaries. PRD: §18.5
- **Acceptance criteria:**
  - Reports have human and versioned JSON forms.
  - Source-to-decision links are stable.
  - Sensitive environment values/paths can be redacted.
  - Inspection does not change generated semantics.
- **Labels:** hxc-plan, epic-e8, effort-large, cli, inspect

#### E8.T08 — Implement source maps and native diagnostic remapping

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E1.T02, E2.T02, E8.T04
- **PRD references:** §20.2
- **Requirement IDs:** `HXC-COMP-013`, `HXC-COMP-014`, `HXC-CLI-008`
- **Outcome:** Map C compiler/linker errors and optional debug locations back to Haxe source and generated artifacts.
- **Description:** Map C compiler/linker errors and optional debug locations back to Haxe source and generated artifacts. PRD: §20.2
- **Acceptance criteria:**
  - #line mode and sidecar map mode are supported/compared.
  - Generated helper errors identify originating Haxe feature where possible.
  - Raw native diagnostics remain available.
  - Paths are normalized and do not break reproducibility.
- **Labels:** hxc-plan, epic-e8, effort-large, diagnostics, sourcemap

#### E8.T09 — Publish versioned JSON diagnostics and event stream

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E0.T06, E8.T02
- **PRD references:** §20.1
- **Requirement IDs:** `HXC-COMP-014`, `HXC-CLI-004`, `HXC-CLI-010`
- **Outcome:** Give IDEs/agents structured compiler, native toolchain, bindgen, ABI, and test diagnostics.
- **Description:** Give IDEs/agents structured compiler, native toolchain, bindgen, ABI, and test diagnostics. PRD: §20.1
- **Acceptance criteria:**
  - Schema includes ID, severity, phase, positions, notes, related locations, remediation, and command context.
  - Multi-line/native errors are losslessly attached.
  - Schema compatibility policy is documented.
  - CLI integration tests assert stdout/stderr discipline.
- **Labels:** hxc-plan, epic-e8, effort-medium, diagnostics, json

#### E8.T10 — Package and install reflaxe.c and hxc reproducibly

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E8.T05, E0.T02
- **PRD references:** §25 M8, §22
- **Requirement IDs:** `HXC-CLI-002`, `HXC-CLI-003`, `HXC-QA-013`
- **Outcome:** Create haxelib/source archives and platform-appropriate CLI installation without relying on developer paths.
- **Description:** Create haxelib/source archives and platform-appropriate CLI installation without relying on developer paths. PRD: §25 M8, §22
- **Acceptance criteria:**
  - Package contains required std/runtime/templates/schemas/docs and excludes caches/secrets.
  - Fresh installation builds hello.
  - Checksums/SBOM/provenance are generated.
  - Version comes from immutable release metadata.
- **Labels:** hxc-plan, epic-e8, effort-large, release, packaging

#### E8.T11 — Emit compile_commands and IDE/build-system integration

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E8.T04, E8.T07
- **PRD references:** §18.3
- **Requirement IDs:** `HXC-COMP-010`, `HXC-MAC-010`, `HXC-CLI-003`
- **Outcome:** Make generated/native C easy to inspect with clangd and consume from existing build systems.
- **Description:** Make generated/native C easy to inspect with clangd and consume from existing build systems. PRD: §18.3
- **Acceptance criteria:**
  - compile_commands contains exact per-file invocations.
  - Manifest documents generated/public/private include roots.
  - No generated CMake/Meson is required for integration.
  - IDE paths remain useful in out-of-tree builds.
- **Labels:** hxc-plan, epic-e8, effort-medium, tooling, ide

### 30.10 E9 — Examples, todo CLI, documentation, and adoption

**Milestone:** M9\
**Priority:** P2\
**Outcome:** Prove the product through honest, polished examples and a macro-powered SQLite todo CLI.

#### E9.T01 — Polish hello as the canonical first-run example

- **Priority / effort / type:** P2 / small / task
- **Blocked by:** E2.T10, E8.T05
- **PRD references:** §19.1
- **Requirement IDs:** `HXC-EX-001`, `HXC-DOC-001`, `HXC-DOC-005`
- **Outcome:** Turn hello into a documented install/build/run/inspect walkthrough with expected generated C and runtime reasoning.
- **Description:** Turn hello into a documented install/build/run/inspect walkthrough with expected generated C and runtime reasoning. PRD: §19.1
- **Acceptance criteria:**
  - Instructions work from fresh package install.
  - Generated output excerpts match current compiler.
  - Both portable and no-runtime-eligible variants are explained where accurate.
  - No raw __c__ appears.
- **Labels:** hxc-plan, epic-e9, effort-small, examples, hello

#### E9.T02 — Deliver metal no-runtime and embedded examples

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E2.T08, E4.T10, E8.T04
- **PRD references:** §19.1
- **Requirement IDs:** `HXC-PROD-004`, `HXC-PROD-012`, `HXC-EX-002`, `HXC-EX-007`
- **Outcome:** Show explicit spans/pointers/layout/volatile/sections/custom startup with zero hidden runtime for hosted and freestanding targets.
- **Description:** Show explicit spans/pointers/layout/volatile/sections/custom startup with zero hidden runtime for hosted and freestanding targets. PRD: §19.1
- **Acceptance criteria:**
  - Binary/symbol inspection proves no hxrt/libc dependency where claimed.
  - Unsafe regions are scoped and documented.
  - At least one cross-toolchain build is reproducible.
  - Example uses typed SDK/native facades rather than raw injection.
- **Labels:** hxc-plan, epic-e9, effort-large, examples, metal, embedded

#### E9.T03 — Publish C bindgen walkthroughs

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E6.T12, E8.T05
- **PRD references:** §19.1
- **Requirement IDs:** `HXC-EX-003`, `HXC-EX-006`, `HXC-DOC-006`
- **Outcome:** Document raw extern generation, safe mapping, lock/probe review, rebuild, and upgrade drift for representative C libraries.
- **Description:** Document raw extern generation, safe mapping, lock/probe review, rebuild, and upgrade drift for representative C libraries. PRD: §19.1
- **Acceptance criteria:**
  - All checked-in generated bindings regenerate cleanly.
  - Ownership/error choices are explained.
  - Exact Clang/native commands are inspectable.
  - AI assistance, if shown, is clearly optional/advisory.
- **Labels:** hxc-plan, epic-e9, effort-medium, examples, bindgen

#### E9.T04 — Publish exported shared-library and FFI examples

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E7.T10, E8.T05
- **PRD references:** §19.1
- **Requirement IDs:** `HXC-EX-004`, `HXC-EX-009`, `HXC-DOC-006`
- **Outcome:** Show Haxe-authored C ABI library creation, installation, ABI diff, and consumption from C/C++/other FFI.
- **Description:** Show Haxe-authored C ABI library creation, installation, ABI diff, and consumption from C/C++/other FFI. PRD: §19.1
- **Acceptance criteria:**
  - Consumers build outside generator tree.
  - Owned handle/string/error paths are included.
  - Platform shared-library differences are documented.
  - ABI manifest/versioning workflow is demonstrated.
- **Labels:** hxc-plan, epic-e9, effort-large, examples, export, ffi

#### E9.T05 — Publish honest C++ shim workflow and hxcpp boundary guide

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E6.T10, E7.T09
- **PRD references:** §7.3, §7.5, §15.8
- **Requirement IDs:** `HXC-PROD-007`, `HXC-EX-005`, `HXC-DOC-002`, `HXC-DOC-003`
- **Outcome:** Explain when to choose hxc, hxcpp, an existing C API, or a generated/manual C++ shim.
- **Description:** Explain when to choose hxc, hxcpp, an existing C API, or a generated/manual C++ shim. PRD: §7.3, §7.5, §15.8
- **Acceptance criteria:**
  - Guide states that arbitrary C++ cannot be consumed directly from C.
  - Exceptions, templates, STL, ownership, and ABI stability are covered.
  - Example shim builds and catches all exceptions at C boundary.
  - No universal superiority claim is made.
- **Labels:** hxc-plan, epic-e9, effort-medium, docs, cpp, hxcpp

#### E9.T06 — Implement todo schema and migration macros

- **Priority / effort / type:** P2 / xlarge / task
- **Blocked by:** E5.T06, E6.T12, E8.T07
- **PRD references:** §17.6, §19.2
- **Requirement IDs:** `HXC-MAC-007`, `HXC-MAC-008`, `HXC-EX-006`
- **Outcome:** Generate typed table/column metadata, migrations, schema hash, row codecs, and inspectable artifacts from Haxe declarations.
- **Description:** Generate typed table/column metadata, migrations, schema hash, row codecs, and inspectable artifacts from Haxe declarations. PRD: §17.6, §19.2
- **Acceptance criteria:**
  - Duplicate/unsupported/destructive changes have compile-time diagnostics.
  - Generated SQL/code is deterministic and inspectable.
  - No runtime reflection is required for known models.
  - Macro cache inputs and source positions are correct.
- **Labels:** hxc-plan, epic-e9, effort-xlarge, todo, macros, sqlite

#### E9.T07 — Implement todo SQLite repository and transaction layer

- **Priority / effort / type:** P2 / xlarge / task
- **Blocked by:** E9.T06, E6.T12, E5.T09
- **PRD references:** §19.2
- **Requirement IDs:** `HXC-FFI-009`, `HXC-MAC-008`, `HXC-EX-006`
- **Outcome:** Use generated SQLite bindings/codecs/statements with explicit errors, resource ownership, transactions, and migrations.
- **Description:** Use generated SQLite bindings/codecs/statements with explicit errors, resource ownership, transactions, and migrations. PRD: §19.2
- **Acceptance criteria:**
  - CRUD, migration, rollback, busy/error, and corrupt-input tests pass.
  - Statements/database close on every path.
  - SQL injection is impossible through value parameters.
  - Generated/native allocation behavior is inspectable.
- **Labels:** hxc-plan, epic-e9, effort-xlarge, todo, sqlite, repository

#### E9.T08 — Implement macro-generated todo CLI parser/help/completion

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E9.T06, E8.T02
- **PRD references:** §17.6, §19.2
- **Requirement IDs:** `HXC-MAC-004`, `HXC-MAC-007`, `HXC-MAC-008`, `HXC-MAC-009`, `HXC-EX-006`
- **Outcome:** Derive commands/options/validation/help/completion from typed declarations with Rails-like conventions and explicit generated code.
- **Description:** Derive commands/options/validation/help/completion from typed declarations with Rails-like conventions and explicit generated code. PRD: §17.6, §19.2
- **Acceptance criteria:**
  - add/list/complete/remove/help workflows and error exit codes are tested.
  - Help is deterministic and readable.
  - No runtime method_missing/reflection dispatcher is shipped.
  - Generated parser is visible through hxc inspect macros.
- **Labels:** hxc-plan, epic-e9, effort-large, todo, cli, macros

#### E9.T09 — Prove todo domain portability across sibling targets

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E9.T07, E5.T12
- **PRD references:** §5 G8, §19.3
- **Requirement IDs:** `HXC-PROD-009`, `HXC-EX-006`, `HXC-QA-010`
- **Outcome:** Keep domain/service logic free of C-only APIs and compile/run it under selected supplied sibling compilers.
- **Description:** Keep domain/service logic free of C-only APIs and compile/run it under selected supplied sibling compilers. PRD: §5 G8, §19.3
- **Acceptance criteria:**
  - Portable source boundary is mechanically checked.
  - Target-specific repository/CLI adapters are isolated.
  - Shared domain tests pass on C plus selected Rust/Elixir/OCaml/Ruby targets as practical.
  - Any source divergence is documented and minimized.
- **Labels:** hxc-plan, epic-e9, effort-large, todo, portability

#### E9.T10 — Benchmark generated C against hand-written baselines

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E9.T07, E3.T09, E10.T05
- **PRD references:** §19.2, §21.7
- **Requirement IDs:** `HXC-PROD-003`, `HXC-PROD-011`, `HXC-EX-010`, `HXC-QA-008`, `HXC-QA-012`
- **Outcome:** Compare compile time, binary size, allocations, throughput, latency, and readability for todo and focused kernels.
- **Description:** Compare compile time, binary size, allocations, throughput, latency, and readability for todo and focused kernels. PRD: §19.2, §21.7
- **Acceptance criteria:**
  - Harness pins inputs/toolchains/flags and reports variance.
  - Hand-written baseline is fair and reviewed.
  - Regressions create Beads issues rather than marketing reinterpretation.
  - Results distinguish portable and metal profiles.
- **Labels:** hxc-plan, epic-e9, effort-large, benchmarks, todo, performance

#### E9.T11 — Write migration and comparison guides for Haxe/C/hxcpp users

- **Priority / effort / type:** P2 / medium / task
- **Blocked by:** E9.T01, E9.T03, E9.T04, E9.T05
- **PRD references:** §7
- **Requirement IDs:** `HXC-PROD-007`, `HXC-DOC-001`, `HXC-DOC-002`, `HXC-DOC-004`, `HXC-DOC-005`, `HXC-DOC-007`
- **Outcome:** Provide practical paths from hxcpp/native C/Haxe libraries to hxc, with compatibility tables and tradeoffs.
- **Description:** Provide practical paths from hxcpp/native C/Haxe libraries to hxc, with compatibility tables and tradeoffs. PRD: §7
- **Acceptance criteria:**
  - Guides include code/config/build examples.
  - C ABI versus C++ ABI limitations are prominent.
  - Standard-library/profile/runtime support claims link to evidence.
  - TypeScript bridge positioning is framed as familiarity/migration, not source compatibility.
- **Labels:** hxc-plan, epic-e9, effort-medium, docs, adoption

#### E9.T12 — Prove a WebAssembly/TypeScript bridge

- **Priority / effort / type:** P3 / large / task
- **Blocked by:** E7.T10, E8.T04, E5.T04
- **PRD references:** §7.4, §19.1
- **Requirement IDs:** `HXC-PROD-010`, `HXC-EX-008`, `HXC-DOC-008`
- **Outcome:** Compile a small exported C ABI module through WASI or Emscripten and consume it from TypeScript with explicit memory/ownership wrappers.
- **Description:** Compile a small exported C ABI module through WASI or Emscripten and consume it from TypeScript with explicit memory/ownership wrappers. PRD: §7.4, §19.1
- **Acceptance criteria:**
  - Build is reproducible and uses the same export ABI model.
  - String/buffer transfer and errors are demonstrated.
  - Generated JS/TS glue is scoped and documented.
  - Example makes no claim of general TypeScript compatibility.
- **Labels:** hxc-plan, epic-e9, effort-large, examples, wasm, typescript

### 30.11 E10 — Quality, performance, security, and releases

**Milestone:** M10\
**Priority:** P1\
**Outcome:** Continuously harden correctness, ABI, security, performance, compatibility, and release provenance.

#### E10.T01 — Expand cross-platform compiler and architecture CI matrix

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E0.T05, E2.T10
- **PRD references:** §21, §25 M10, ADR 0007
- **Requirement IDs:** `HXC-QA-002`, `HXC-QA-003`, `HXC-QA-007`, `HXC-QA-010`
- **Outcome:** Run supported Haxe/C/C++ compiler versions, operating systems, architectures, profiles, runtime policies, artifacts, and environments with explicit tiering.
- **Description:** Run supported Haxe/C/C++ compiler versions, operating systems, architectures, profiles, runtime policies, artifacts, and environments with explicit tiering. PRD: §21, §25 M10
- **Acceptance criteria:**
  - Tier-1 gates cannot be skipped silently.
  - Cross compile and native run responsibilities are distinguished.
  - Matrix metadata is archived with releases.
  - Unsupported combinations fail during planning with clear reason.
- **Labels:** hxc-plan, epic-e10, effort-xlarge, ci, platform

#### E10.T02 — Fuzz C AST printer and generated translation units

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E1.T10, E2.T11
- **PRD references:** §21.6, §22
- **Requirement IDs:** `HXC-COMP-005`, `HXC-QA-006`, `HXC-QA-010`
- **Outcome:** Generate/minimize valid ASTs and adversarial literals/declarators, compile them with multiple compilers, and detect crashes/misprints/non-determinism.
- **Description:** Generate/minimize valid ASTs and adversarial literals/declarators, compile them with multiple compilers, and detect crashes/misprints/non-determinism. PRD: §21.6, §22
- **Acceptance criteria:**
  - Seed corpus and fuzz dictionaries are checked in.
  - Crashes/minimized cases become regression fixtures.
  - Resource limits prevent CI denial of service.
  - Printer never emits unterminated comments/strings/directives from source data.
- **Labels:** hxc-plan, epic-e10, effort-large, fuzzing, printer

#### E10.T03 — Fuzz bindgen inputs and mapping parser

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E6.T08, E5.T06
- **PRD references:** §15, §22
- **Requirement IDs:** `HXC-FFI-001`, `HXC-QA-006`, `HXC-QA-011`
- **Outcome:** Exercise malformed/complex headers, preprocessor configurations, mapping files, and lock/probe parsers without trusting unbounded input.
- **Description:** Exercise malformed/complex headers, preprocessor configurations, mapping files, and lock/probe parsers without trusting unbounded input. PRD: §15, §22
- **Acceptance criteria:**
  - No crash, uncontrolled resource use, path escape, or command injection.
  - Minimized findings become fixtures.
  - Clang subprocess time/memory/output limits are enforced.
  - Generated identifiers/comments cannot inject C/Haxe syntax.
- **Labels:** hxc-plan, epic-e10, effort-large, fuzzing, bindgen, security

#### E10.T04 — Run sanitizer, leak, race, and static-analysis gates

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E4.T12, E5.T11, E10.T01
- **PRD references:** §21.6
- **Requirement IDs:** `HXC-ABI-012`, `HXC-QA-004`, `HXC-QA-011`
- **Outcome:** Continuously analyze compiler helpers, runtime, generated programs, and native adapters with appropriate dynamic/static tools.
- **Description:** Continuously analyze compiler helpers, runtime, generated programs, and native adapters with appropriate dynamic/static tools. PRD: §21.6
- **Acceptance criteria:**
  - ASan/UBSan/LSan are required on supported Unix CI; TSan runs scoped thread suites.
  - Static analyzer warnings are triaged, not blanket-suppressed.
  - Suppression files are minimal, documented, and reviewed.
  - Release evidence records tool versions and scope.
- **Labels:** hxc-plan, epic-e10, effort-large, quality, sanitizer, static-analysis

#### E10.T05 — Define and enforce performance/size budgets

- **Priority / effort / type:** P2 / large / task
- **Blocked by:** E2.T10, E3.T09, E4.T12
- **PRD references:** §21.7, §26
- **Requirement IDs:** `HXC-QA-002`, `HXC-QA-008`, `HXC-QA-012`
- **Outcome:** Track compiler time/memory, generated C compile time, executable/library size, allocations, throughput, and startup with stable baselines.
- **Description:** Track compiler time/memory, generated C compile time, executable/library size, allocations, throughput, and startup with stable baselines. PRD: §21.7, §26
- **Acceptance criteria:**
  - Benchmarks distinguish noise from regression and record toolchain/hardware.
  - Budgets exist for hello, kernels, runtime slices, bindgen, export, and todo.
  - Portable/runtime costs are attributed by feature.
  - Regressions require explicit approval or blocking issue.
- **Labels:** hxc-plan, epic-e10, effort-large, performance, benchmarks

#### E10.T06 — Perform compiler/runtime/interop security review

- **Priority / effort / type:** P1 / xlarge / task
- **Blocked by:** E10.T02, E10.T03, E10.T04, E7.T08
- **PRD references:** §22
- **Requirement IDs:** `HXC-ABI-012`, `HXC-QA-011`, `HXC-QA-013`
- **Outcome:** Threat-model generated-code injection, malicious headers/configs, path traversal, tool invocation, allocator/lifetime errors, shared-library boundaries, and supply chain.
- **Description:** Threat-model generated-code injection, malicious headers/configs, path traversal, tool invocation, allocator/lifetime errors, shared-library boundaries, and supply chain. PRD: §22
- **Acceptance criteria:**
  - Threat model and mitigations are documented.
  - Security-sensitive code has targeted tests/review owners.
  - No shell command construction from untrusted arguments.
  - Disclosure/release response process is exercised.
- **Labels:** hxc-plan, epic-e10, effort-xlarge, security, review

#### E10.T07 — Publish compatibility and support matrix

- **Priority / effort / type:** P1 / medium / task
- **Blocked by:** E5.T12, E6.T12, E7.T08, E10.T01
- **PRD references:** §25 M10, §26
- **Requirement IDs:** `HXC-PROD-006`, `HXC-QA-003`, `HXC-DOC-007`
- **Outcome:** Generate evidence-backed support status for Haxe features, stdlib modules, platforms, compilers, ABIs, environments, profiles, and artifacts.
- **Description:** Generate evidence-backed support status for Haxe features, stdlib modules, platforms, compilers, ABIs, environments, profiles, and artifacts. PRD: §25 M10, §26
- **Acceptance criteria:**
  - Statuses link to tests/results and known issues.
  - Experimental/partial/unsupported are distinct.
  - README claims are generated from or reviewed against matrix.
  - Matrix is versioned per release.
- **Labels:** hxc-plan, epic-e10, effort-medium, release, compatibility

#### E10.T08 — Automate signed, reproducible release artifacts

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E8.T10, E10.T01, E10.T06
- **PRD references:** §22, §25 M10
- **Requirement IDs:** `HXC-RT-012`, `HXC-QA-011`, `HXC-QA-013`
- **Outcome:** Build immutable source/package artifacts with checksums, signatures, SBOM, provenance, release notes, and rollback instructions.
- **Description:** Build immutable source/package artifacts with checksums, signatures, SBOM, provenance, release notes, and rollback instructions. PRD: §22, §25 M10
- **Acceptance criteria:**
  - Release runs only from tagged clean commit.
  - Artifacts rebuild within documented reproducibility constraints.
  - Dependency/toolchain provenance is attached.
  - Install smoke tests run against downloaded artifacts.
- **Labels:** hxc-plan, epic-e10, effort-large, release, supply-chain

#### E10.T09 — Ratify stable public ABI and compatibility policy

- **Priority / effort / type:** P1 / medium / task
- **Blocked by:** E7.T07, E7.T08, E4.T11
- **PRD references:** §16.8, §25 M10
- **Requirement IDs:** `HXC-ABI-011`, `HXC-ABI-012`, `HXC-QA-013`
- **Outcome:** Define semantic versioning and compatibility guarantees for generated public C ABI, runtime ABI, manifests, bindings, CLI JSON, and config schema.
- **Description:** Define semantic versioning and compatibility guarantees for generated public C ABI, runtime ABI, manifests, bindings, CLI JSON, and config schema. PRD: §16.8, §25 M10
- **Acceptance criteria:**
  - Breaking/nonbreaking classifications have examples/tests.
  - ABI diff gate enforces version changes.
  - Unstable internal layouts are clearly excluded.
  - Deprecation and migration windows are documented.
- **Labels:** hxc-plan, epic-e10, effort-medium, abi, policy, release

#### E10.T10 — Meet the 1.0 stdlib and semantic parity gate

- **Priority / effort / type:** P0 / xlarge / task
- **Blocked by:** E5.T12, E10.T04, E10.T07
- **PRD references:** §5 G2, §5 G7, §25 M10
- **Requirement IDs:** `HXC-PROD-006`, `HXC-QA-005`, `HXC-QA-011`
- **Outcome:** Reach the ratified upstream test threshold with every remaining divergence explicit and release-approved.
- **Description:** Reach the ratified upstream test threshold with every remaining divergence explicit and release-approved. PRD: §5 G2, §5 G7, §25 M10
- **Acceptance criteria:**
  - No silent skips or unknown failures.
  - Tier-1 semantic/stdlib dashboard meets threshold.
  - Known divergences have user-visible diagnostics/docs and post-1.0 issues.
  - Portable source suite includes representative real applications.
- **Labels:** hxc-plan, epic-e10, effort-xlarge, release, stdlib, correctness

#### E10.T11 — Cut evidence-backed beta release

- **Priority / effort / type:** P1 / large / task
- **Blocked by:** E10.T07, E10.T08, E9.T11
- **PRD references:** §25 M10
- **Requirement IDs:** `HXC-QA-011`, `HXC-QA-013`, `HXC-QA-014`
- **Outcome:** Ship a beta only after install/onboarding/examples/compatibility/security/ABI evidence is archived.
- **Description:** Ship a beta only after install/onboarding/examples/compatibility/security/ABI evidence is archived. PRD: §25 M10
- **Acceptance criteria:**
  - Fresh-user smoke path succeeds on tier-1 platforms.
  - Known limitations and support matrix are prominent.
  - Feedback/crash/diagnostic collection path is documented.
  - No 1.0 stability promise is implied.
- **Labels:** hxc-plan, epic-e10, effort-large, release, beta

#### E10.T12 — Cut 1.0 and archive release evidence

- **Priority / effort / type:** P0 / xlarge / task
- **Blocked by:** E10.T06, E10.T08, E10.T09, E10.T10, E10.T11, E9.T10
- **PRD references:** §25 M10, §26
- **Requirement IDs:** `HXC-PROD-006`, `HXC-QA-013`, `HXC-QA-014`
- **Outcome:** Release 1.0 only after correctness, UB, security, ABI, performance, docs, examples, and support gates are satisfied.
- **Description:** Release 1.0 only after correctness, UB, security, ABI, performance, docs, examples, and support gates are satisfied. PRD: §25 M10, §26
- **Acceptance criteria:**
  - All release blockers are closed with linked evidence.
  - Signed immutable artifacts and provenance are published.
  - Post-1.0 compatibility/maintenance policy is active.
  - Release report records exact source, toolchains, tests, matrices, benchmarks, and accepted residual risks.
- **Labels:** hxc-plan, epic-e10, effort-xlarge, release, 1.0

---
## 31. Original product brief coverage

This table normalizes every material product thought from the initiating brief into an explicit design resolution and stable requirement family. It is included so Codex does not lose the original ambition while working from granular Beads issues.

| Original intent | Resolution in this PRD | Requirement IDs | Primary sections |
| --- | --- | --- | --- |
| Use Haxe knowledge to enter C without abandoning Haxe | One Haxe source language, direct C output, portable and metal contracts | HXC-PROD-001, HXC-PROD-002 | §1, §5, §7, §9 |
| Follow the sibling Reflaxe targets | Preserve family repository, bootstrap, context, AST, runtime, tests, and Beads patterns | HXC-SCAF-001–008 | §2, §10, §23, §32 |
| Learn from hxcpp but emit C, not C++ | Reuse project-generation and dependency lessons; reject C++ object/runtime assumptions | HXC-PROD-007, HXC-COMP-010 | §2.10, §7.5, §39 |
| Generate idiomatic hand-written-looking C | Idiomatic output is required in every profile and measured by review, warnings, snapshots, and runtime-helper reports | HXC-PROD-003, HXC-COMP-004 | §5, §8, §10, §21 |
| Support both Haxe-level and close-to-metal programming | Portable and metal semantic contracts share one pipeline; runtime policy is orthogonal | HXC-PROD-002–005 | §9, §13, §17 |
| Use runtime helpers only where semantics require them | Direct C/local specialization first, then a reasoned feature graph, `auto|minimal|none`, source warnings, and no-runtime proof | HXC-RT-001–004, HXC-RT-013 | §9.4, §13, §18.5 |
| Appeal to Haxe programmers | Full standard library, portability lane, familiar language, C ecosystem access | HXC-PROD-006, HXC-STD-001–010 | §7.1, §14 |
| Appeal to C programmers and purists | Typed layout, pointers, ownership, allocators, headers, ABI controls, compile-time verification, freestanding support, and readable output | HXC-PROD-004, HXC-MAC-001–007, HXC-MAC-011–012 | §7.2, §17 |
| Be useful to C++ programmers | C++-compatible C headers and optional wrappers; arbitrary C++ APIs require shims | HXC-FFI-013, HXC-ABI-003 | §7.3, §15.8, §16.4 |
| Provide a TypeScript-to-native bridge story | Position Haxe as familiar, but do not claim language identity; add onboarding examples later | HXC-PROD-010, HXC-DOC-008 | §7.4, §39 |
| Support the complete Haxe standard library | Tracked ledger, implementation ownership, differential conformance, platform capability matrix | HXC-STD-001–010 | §14, §21 |
| Keep portable application code compilable by sibling targets | Separate portable core from C adapters and run a cross-target example matrix | HXC-PROD-009, HXC-QA-010 | §5.8, §19.3, §34 |
| Make consuming C libraries dead easy | Direct externs plus deterministic Clang-based bindgen, raw and ergonomic layers, layout probes | HXC-FFI-001–012 | §15 |
| Use AI to improve bindings without inventing ABI facts | AI suggestions are reviewable sidecars; parser/layout facts are deterministic | HXC-FFI-011 | §15.7 |
| Make producing shared objects and native libraries easy | Export annotations, stable headers, ownership/error contracts, static/shared builds, ABI reports | HXC-ABI-001–012 | §16 |
| Show Rails/Ruby-like ergonomics | Compile-time model/command/schema macros generate explicit C, not a reflective ORM runtime | HXC-MAC-007–010, HXC-EX-006 | §3, §17.6, §19.2 |
| Ship meaningful examples including a todo CLI | Hello, runtime-free, C binding, shared library, C++ shim, todo CLI, embedded, WebAssembly, foreign consumer | HXC-EX-001–010 | §19, §34 |
| Let Codex persist and execute the plan with Beads | Canonical JSON plan plus current `bd` parent/dependency/discovery workflow | HXC-SCAF-008, HXC-QA-014 | §24, §36 |
| Treat C/LLM compatibility as an advantage | Keep output deterministic, simple, inspectable, and benchmark the claim rather than treating it as proven | HXC-PROD-011, HXC-QA-012 | §8, §21.7, §39 |

---

## 32. Existing scaffold inventory and gap analysis

### 32.1 Status legend

Use the status vocabulary in Section 0. A seed is an implementation input, not a completed feature.

### 32.2 Inventory

The same inventory is available as `docs/specs/bootstrap-inventory.json` for Codex and CI. The stable component identifier in each row is the join key for machine-readable gaps, owning Beads tasks, requirement IDs, and verification commands. The Markdown table is the readable normative explanation; the JSON document is the automation surface.

| Area | Existing assets | Status | What Codex must understand |
| --- | --- | --- | --- |
| Toolchain and package metadata (`toolchain-metadata`) | `.haxerc`, `haxelib.json`, `extraParams.hxml`, `haxe_libraries/`, `package.json`, `package-lock.json`, `toolchain-lock.json` | Verified dependency seed | Haxe 5.0.0-preview.1 at `2c1e544e0a2c7524ef4c8e103f1b0580362ea538`, its official Linux/macOS/Windows artifact hashes, Lix 17.0.2, and Reflaxe commit `73a983112e039daad46b37912ab238df6bf0cf53` are exact and checksum-verified. Source-checkout and temporary flattened-package probes are CWD-independent. Release assembly remains separate work. |
| Target activation (`target-activation`) | `c/Init.hx`, `TargetPlatform.hx`, `BuildDetection.hx`, `CompilerBootstrap.hx`, `CompilerInit.hx`, `test/bootstrap/**` | Production typing carrier verified; lowering fail-closed | Cold, package, and back-to-back compiler-server probes exercise real `--custom-target c=<output>` activation, target-owned platform configuration, derived/matching output transport, non-C isolation, exactly-once counts, public/internal define visibility, source-anchored conflicts, and typed upstream scalar-Unicode branches. Full Reflaxe registration reaches `HXC1000` with no artifact. Legacy Cross remains an executable `HXC0003` negative; Eval is only an oracle/non-C host. |
| Profiles, runtime policy, and environment (`configuration-policies`) | `CProfile.hx`, `CRuntimePolicy.hx`, `CEnvironment.hx`, `ProfileResolver.hx`, config schema/template | Seeded | `portable|metal`, `auto|minimal|none`, and environment presets are represented. Define/project precedence, capability matrices, canonical reports, and conflict diagnostics remain incomplete. |
| Per-build compiler state (`per-build-context`) | `CompilationContext.hx` | Seeded | Resolved policy and runtime-reason seeds exist. Symbol/type/layout registries, pass state, diagnostics, outputs, caches, and repeated-build isolation still need implementation and tests. |
| Reflaxe adapter (`reflaxe-adapter`) | `CReflaxeCompiler.hx`, `test/all_sources/**` | Compile-verified fail-closed seed | The adapter, fresh per-build context, and macro/non-macro callback branches type-check under the pinned toolchain. It collects typed modules, delegates whole-program work, and owns output saving; callback/generic behavior, provenance capture, and real deterministic multi-file output still require generated-output evidence. |
| Whole-program compiler (`whole-program-compiler`) | `CCompiler.hx`, `test/all_sources/**` | Compile-verified fail-closed seed | The complete owned source graph reaches the real custom-target boundary and deliberately reports source-anchored `HXC1000` without leaving a plausible artifact. This is correct scaffold behavior, not a working compiler; implement semantic lowering behind the boundary rather than weakening the failure or emitting fake success. |
| Structural C AST and printer (`c-ast-printer`) | `ast/CAST.hx`, `ast/CASTPrinter.hx`, `test/all_sources/**`, `test/c_ast/**` | Compile-verified C11 AST printer core | Base specifiers and grammar-level pointer/array/function/parenthesized/abstract declarators are structural; typed nodes cover C11 array bounds, prototypes, native/exact types, atomics, alignment, anonymous aggregates, incomplete types, dialect-gated attributes, validated numeric literals, generic selections, and line anchors. Deterministic Haxe-built goldens cover difficult declarators plus a literal 6×6 ordered expression-family matrix, every unary/binary operator, C grammar-category edges, all statement shapes, and adversarial source escaping. Both runtime-free C files compile and run under strict GCC and Clang. Haxe/HxcIR lowering, source-map policy/sidecar provenance, whole-AST invariants, extension-native probes, and the broad E1.T10 corpus remain. |
| Declaration and header planning (`declaration-planner`) | `plan/CDeclarationPlan.hx`, `plan/CDeclarationPlanner.hx`, typed contract schema 2, `test/declaration_plan/**` | Compile-verified deterministic planning core | Complete edges drive stable in-header order or local includes; pointer-only cycles use typed forward declarations; external opaque declarations propagate their authoritative includes; build includes retain declaration owners and source reasons; public/private complete-type leakage and invalid graphs fail with HXC5002. Portable path-derived guards, reversed-input goldens, independently compiled repeated-include headers, and a combined runtime-free C consumer pass strict GCC/Clang lanes. Default-name finalization, real Haxe declaration lowering, umbrella/runtime-feature headers, production multi-file emission, exported ABI policy, and C++ header compatibility remain with later E1/E7 work. |
| Generated-file boundary (`generated-file-boundary`) | `emit/GeneratedFile.hx` | Seeded | Safe relative paths are represented. Add output manifests, dependency ordering, changed-file writes, stale-file cleanup, content hashing, collision checks, and Reflaxe `OutputManager` integration. |
| Target-facing C API (`target-c-api`) | `std/c/**`, `TypedCContractMacro`, typed-C fixtures | Compile-verified contract seed | Pointer, span, ownership, result, allocator, syntax, calling-convention, visibility, integer, and volatile abstractions type-check as compiler contracts. Namespaced declaration/build metadata produces a deterministic schema-2 typed snapshot whose merged build facts retain sorted declaration provenance; negative fixtures prove the current HXC5002 validation slice and empty runtime effects. No `c.*` lowering, production header emission, native layout proof, unsafe operation, or stable public ABI is implemented. |
| Runtime ABI and implementation (`runtime-abi-and-implementation`) | `runtime/hxrt/include/hxc_runtime.h`, `runtime/hxrt/src/hxc_runtime.c` | Verified native seed | A provisional allocator/string/status/Int32 ABI compiles and runs natively but is not selected by generated programs. The runtime feature graph/manifest, hardened allocation contracts, arrays, objects, managed memory, dynamic values, reflection, exceptions, threads, and platform adapters remain later work. |
| Native smoke fixtures (`native-smoke-fixtures`) | `runtime/hxrt/test/**`, `scripts/ci/runtime_smoke.py`, `test/c_ast/**`, `test/declaration_plan/**`, `test/native/pointlib/**`, `test/native/cpp_shim/**` | Verified native seed, AST, and planned-header goldens | Local auto mode explicitly reports optional compiler-family skips and requires at least one complete pair. CI separately requires real GCC/G++ and Clang/Clang++ lanes with warnings as errors; each compiles/runs the runtime-free declarator and expression/statement AST goldens, independently compiles every planned declaration header, runs their combined consumer and the hosted C11 runtime, compiles the freestanding path, links/runs a C++17 runtime-header consumer, and exercises independent C-library and opaque-handle C++-shim fixtures. Sanitizers, Haxe-language generated output, broader platform matrices, ownership/failure paths, generated C++-compatible export headers, and installed external consumers remain later gates. |
| Diagnostics (`diagnostics`) | `CDiagnostic.hx`, `docs/specs/diagnostics.json`, schema | Seeded | Stable IDs and the deliberate scaffold failure are registered. Complete source ranges, severity policy, remediation, JSON output, registry drift checks, and diagnostic tests are still required. |
| Requirements and ledgers (`requirements-and-ledgers`) | `requirements.json`, `stdlib-ledger.json`, schemas | Seeded | The complete PRD has 160 stable product/semantic/quality requirements and an initial standard-library ownership ledger. Codex must keep PRD, registries, task mappings, diagnostics, runtime features, and capability evidence synchronized as scope changes. |
| Example portfolio (`example-portfolio`) | `examples/hello`, `no-runtime`, `pointlib`, `shared-library`, `cpp-shim`, `todo-cli` | Seeded | These establish intended user journeys and API shapes; only independent native fixture fragments are executable today. Do not special-case the compiler or use raw C injection to make examples appear complete. |
| Development runner and future CLI (`development-cli`) | `src/Run.hx`, `schemas/hxc.schema.json`, `templates/hxc.json` | Seeded | A development entry point and configuration shape exist. The production `hxc` command surface, stable exit categories, JSON outputs, native build adapters, bindgen/export orchestration, and packaging are planned. |
| Beads plan and materialization (`beads-bootstrap`) | `docs/specs/beads-plan.json`, schema, `scripts/beads/**`, `docs/BEADS_PLAN.md` | Seeded | The reproducible seed contains 11 epics, 122 tasks, 274 hard blocking edges, 160 covered requirements, validation, preview, and a brownfield-aware importer. No live `.beads` database is included; reconcile open and closed issues before creating anything. |
| Structural validation and archive integrity (`structural-validation`) | `check_toolchain.py`, `check_ci_policy.py`, `check_license_policy.py`, all-source HXML, runtime smoke, governance CI, package manifest | Verified native and Haxe scaffold | Present local gates verify dependency/package policy, required CI wiring, license/provenance, the complete current owned Haxe graph and macro branches, fail-closed production activation, and native fixtures. The fuller archive structure/reference/manifest scripts named elsewhere are absent from this checkout; generated-output, sanitizer, full platform, and release gates remain explicitly unverified. |
| Supplied repository evidence (`research-evidence`) | `reference/repomix/*.xml`, checksums, `docs/research/repo-patterns.md` | Evidence-only | Searchable XML snapshots cover Haxe/hxcpp, Reflaxe, and sibling targets. They are precedent and provenance evidence—not code to copy wholesale—and adapted code still requires license review. |
| Deterministic import tooling (`bindgen`) | `tools/` placeholder, pointlib extern/example contracts, PRD/task graph | Planned | No Clang parser or binding generator exists. Implement exact preprocessing/target capture, normalized ABI model, raw externs, optional ergonomic wrappers, lock/provenance files, layout probes, drift reports, callbacks, and constrained C++ shims. |
| Public C ABI and library production (`c-export`) | `std/c/Export.hx`, `examples/shared-library` | Planned | Export intent and consumer fixtures are seeded, but no Haxe export analyzer/generator exists. Build stable headers/wrappers, ownership/error adapters, visibility/version maps, install metadata, ABI manifests/diffs, and independent consumers. |
| Portable runtime and standard library (`portable-runtime-and-stdlib`) | Minimal `runtime/hxrt` seed and `stdlib-ledger.json` | Planned | Full portable Haxe semantics and standard-library parity are product requirements, not current capabilities. Ratify strings, memory, exceptions, and platform contracts before implementing selective runtime facilities and ledger-driven conformance. |
| Governance, compatibility, and releases (`governance-release`) | `LICENSE`, `LICENSES/`, `THIRD_PARTY_NOTICES.md`, `docs/specs/third-party-provenance.json`, governance CI | Partial | GPL-3.0-only, the current upstream inventory, and fail-closed source/package notice validation are checked in. Generated-output/runtime redistribution treatment, supported versions, ABI/semantic policy, and reproducible release evidence remain before release claims. |

### 32.3 Immediate scaffold audit acceptance

Before implementing broad lowering, Codex must establish all of the following:

- every documented local validation command exists and runs;
- the Haxe source tree type-checks under the pinned Haxe/Reflaxe combination;
- two-stage target activation runs exactly once and only for C builds;
- a compiler-server sequence does not leak profile/runtime/symbol/output state;
- the C AST/printer seed passes a structural golden corpus;
- the runtime seed compiles as strict hosted and freestanding C and its header includes from C++;
- the Beads plan validates and previews deterministically;
- package URLs, license placeholders, and unverified version claims are identified as blockers rather than silently published;
- example seeds are clearly marked non-functional until their milestone closes;
- CI does not reference nonexistent scripts or claim unsupported toolchains.

### 32.4 Explicit non-rework

The following are not first-pass tasks: creating a new repository layout, inventing a third profile, replacing Reflaxe, copying hxcpp wholesale, building a runtime-reflective ORM, or writing a bespoke build system before the neutral manifest exists.

---

## 33. Machine-translatable product requirements catalog

Each requirement has a stable ID so Codex can cite it in Beads descriptions, acceptance evidence, ADRs, diagnostics, tests, and capability ledgers. Priority is product priority, not an effort estimate. The suggested epic is a default ownership location; cross-cutting dependencies remain explicit. `docs/specs/requirements.json` is generated from these tables and MUST remain byte-for-byte semantically synchronized with them.

### 33.1 Product and positioning

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-PROD-001 | The product MUST compile typed Haxe programs into C source/header projects rather than hiding C behind a proprietary bytecode or opaque binary-only pipeline. | P0 | E1/E2 | A real Haxe fixture emits auditable C and builds with a supported C compiler. |
| HXC-PROD-002 | `portable` MUST be the default semantic contract and preserve Haxe behavior even when selective runtime support is required. | P0 | E2–E5 | Differential fixtures match a reference Haxe target and runtime reports explain selected helpers. |
| HXC-PROD-003 | Every supported profile MUST emit readable, idiomatic C; “idiomatic” is not a separate correctness-off mode. | P0 | E1/E10 | Generated-code review rubric, warning-clean builds, deterministic snapshots, and representative hand-written baselines pass. |
| HXC-PROD-004 | `metal` MUST expose explicit C layouts, pointers, ownership, allocators, linkage, calling conventions, and freestanding constraints. | P0 | E3/E8 | Runtime-free layout/ownership examples compile and ABI probes match native C. |
| HXC-PROD-005 | Runtime policy MUST be orthogonal to profile through `auto|minimal|none`; direct C and local specialization MUST precede selective `hxrt`; and the compiler MUST fail when a requested policy cannot satisfy semantics. | P0 | E4/E8 | Policy matrix tests prove exact feature selection, profile-default provenance, warnings, zero-runtime absence, and actionable failures. |
| HXC-PROD-006 | The 1.0 portable contract MUST support the full applicable Haxe standard library for each declared platform, with explicit capability exceptions. | P1 | E5/E10 | The standard-library ledger has no unowned applicable modules and conformance suites pass. |
| HXC-PROD-007 | `hxc` MUST complement rather than misrepresent hxcpp: use hxc for C ABI/output/freestanding workflows and hxcpp for direct C++ semantics/ecosystem integration. | P0 | E9 | README comparison is technically accurate and demonstrated by C, C++, and shim examples. |
| HXC-PROD-008 | Haxe-authored code MUST be able to consume C libraries and produce C-compatible libraries without hand-written boilerplate for ordinary cases. | P0 | E6/E7 | A generated SQLite-style binding and a generated shared library are independently consumed. |
| HXC-PROD-009 | Portable domain code SHOULD remain compilable by the sibling target family when it does not import `c.*` or C-specific adapters. | P1 | E9/E10 | A portable-core fixture builds on hxc and selected sibling targets in CI. |
| HXC-PROD-010 | TypeScript familiarity MAY be used for onboarding, but documentation MUST not claim Haxe and TypeScript have identical semantics or ecosystems. | P2 | E9 | Onboarding material calls out both similarities and important differences. |
| HXC-PROD-011 | LLM-friendly C output is a product hypothesis to test through deterministic structure and agent tasks, not an unsupported benchmark claim. | P2 | E10 | Agent comprehension/edit experiments and output-complexity metrics are published before marketing claims. |
| HXC-PROD-012 | Normal user programs MUST NOT require raw C injection; escape hatches are explicit, typed where possible, and auditable. | P0 | E2/E9 | Examples pass a raw-injection linter and supported features lower structurally. |

### 33.2 Existing scaffold and governance

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-SCAF-001 | Codex MUST extend the supplied repository rather than regenerate a new Reflaxe project over it. | P0 | E0 | Initial audit documents retained files and no scaffold reset occurs. |
| HXC-SCAF-002 | `CompilerBootstrap` and `CompilerInit` MUST remain separate phases unless an ADR proves a safer replacement. | P0 | E0/E1 | Macro-order and classpath tests pass under cold and compiler-server builds. |
| HXC-SCAF-003 | All mutable compiler state MUST belong to one `CompilationContext` per build. | P0 | E1 | Back-to-back adversarial compilations show no state leakage. |
| HXC-SCAF-004 | The compiler MUST remain fail-closed until real lowering exists; scaffold output cannot masquerade as successful compilation. | P0 | E0/E2 | Unsupported builds terminate with stable diagnostics and non-zero status. |
| HXC-SCAF-005 | Existing C AST/printer/runtime/abstraction seeds MUST be tested and evolved, not silently discarded. | P0 | E1/E4 | Beads/ADR records justify any incompatible replacement and preserve intended contracts. |
| HXC-SCAF-006 | Toolchain versions and source provenance MUST be pinned reproducibly before semantic implementation expands. | P0 | E0 | Fresh checkout resolves the same Haxe/Reflaxe/tool versions and verifies checksums. |
| HXC-SCAF-007 | CI and local validation commands named in the handoff MUST exist and be runnable without a live Beads database. | P0 | E0 | `npm test`/documented Python commands validate structure, JSON, runtime, and plan in a clean checkout. |
| HXC-SCAF-008 | The checked-in Beads plan MUST be idempotently materializable and remain recoverable from stable keys. | P0 | E0 | Dry run, graph validation, first import, and repeat import produce no duplicate issues. |

### 33.3 Compiler core and generated C

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-COMP-001 | Reflaxe typed modules MUST be collected into a normalized whole-program input with source provenance. | P0 | E1 | Typed-AST fixtures preserve module/type/field/source identity. |
| HXC-COMP-002 | A target-owned HxcIR MUST represent Haxe semantics independently of C syntax. | P0 | E1 | IR unit tests cover values, places, sequencing, calls, cleanup, dispatch, conversions, and failures. |
| HXC-COMP-003 | Evaluation order, cleanup scopes, and lifetime transitions MUST be explicit before C emission. | P0 | E1/E2 | Adversarial side-effect fixtures do not rely on unspecified C order. |
| HXC-COMP-004 | C declarations, statements, expressions, initializers, and declarators MUST be structural AST nodes, not normal-path string concatenation. | P0 | E1 | AST corpus emits nested pointer/array/function declarators correctly. |
| HXC-COMP-005 | The C printer MUST be deterministic and precedence-correct with minimal safe parentheses. | P0 | E1 | Golden corpus is stable across runs/platform paths and compiles under all baseline compilers. |
| HXC-COMP-006 | Pass ordering and prerequisites MUST be explicit and validated. | P0 | E1 | Registry rejects missing/cyclic pass dependencies and records the applied pipeline. |
| HXC-COMP-007 | Names MUST be readable, reserved-word-safe, collision-free, and deterministic across module order. | P0 | E1 | Collision and reorder fixtures produce stable symbols and a symbol map. |
| HXC-COMP-008 | Header/source placement MUST be computed from complete-type and linkage dependencies. | P0 | E1 | Mutually dependent type fixtures compile without over-including implementation details. |
| HXC-COMP-009 | All generated files MUST be written through Reflaxe output ownership with hashes and stale-output cleanup. | P0 | E1 | Rename/delete/incremental fixtures leave no stale source and avoid needless rewrites. |
| HXC-COMP-010 | The compiler MUST emit a neutral build manifest and MAY derive CMake/Meson adapters from it. | P1 | E1/E8 | Raw manifest, CMake, and Meson builds compile the same project. |
| HXC-COMP-011 | Project layout MUST support module, package, and single-translation-unit strategies without changing source semantics. | P2 | E1/E8 | Representative fixtures build in each supported layout. |
| HXC-COMP-012 | Generated code MUST record compiler/profile/runtime/dialect provenance without embedding unstable absolute paths by default. | P1 | E1/E8 | Reproducible builds produce byte-identical output in different checkout roots. |
| HXC-COMP-013 | Source mapping MUST support `#line` and an external map/manifest, with an option to inspect generated locations directly. | P1 | E8 | Compiler diagnostics and debugger stacks map to Haxe source in a supported toolchain. |
| HXC-COMP-014 | Unsupported constructs MUST emit stable `HXC…` diagnostics with source span, reason, profile, and remediation. | P0 | E0/E8 | Negative fixtures assert diagnostic IDs and essential fields. |
| HXC-COMP-015 | The compiler MUST expose an inspection report for symbols, types, passes, runtime features, ABI, and build inputs. | P1 | E8 | `hxc inspect` outputs human and JSON forms consumed by tests. |

### 33.4 Haxe semantic lowering

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-SEM-001 | `Void`, `Bool`, `Int`, `Float`, nullability, and ABI integer types MUST have documented exact representations. | P0 | E2 | Type/conversion matrix and C static assertions pass. |
| HXC-SEM-002 | Haxe Int32 arithmetic, shifts, division, modulo, and narrowing MUST avoid signed-overflow and invalid-shift undefined behavior. | P0 | E2 | Boundary differential tests pass at multiple optimization levels and under sanitizers. |
| HXC-SEM-003 | Floating-point behavior MUST document C/Haxe differences and normalize only where the portable contract requires it. | P1 | E2 | NaN, infinity, negative zero, conversion, and comparison fixtures match the chosen contract. |
| HXC-SEM-004 | Operand and argument evaluation order MUST match Haxe even where C leaves order unspecified. | P0 | E2 | Side-effect permutations produce identical traces across GCC/Clang optimization levels. |
| HXC-SEM-005 | Null checks and nullability representation MUST be explicit and profile-aware. | P0 | E2/E4 | Null dereference/cast/call fixtures have defined results or stable failures. |
| HXC-SEM-006 | Haxe strings MUST use the ADR 0004 immutable valid-UTF-8 contract with Unicode-scalar indices, no implicit normalization, deterministic malformed-input replacement, and separation from binary `Bytes` and NUL-terminated `CString`. | P0 | E4/E5 | BMP, non-BMP, malformed UTF-8, embedded-NUL, slicing, comparison, and borrowed/owned FFI conversion tests pass. |
| HXC-SEM-007 | Arrays MUST preserve Haxe mutation, length, indexing, resize, element-default, and generic specialization behavior. | P0 | E3/E4 | Array API differential suite and bounds policy tests pass. |
| HXC-SEM-008 | `haxe.io.Bytes` and typed byte views MUST preserve binary data including embedded NUL bytes. | P0 | E5 | Binary round-trip and C span interop fixtures pass. |
| HXC-SEM-009 | Anonymous structures SHOULD lower to generated concrete structs when shape and escape behavior permit. | P1 | E3 | Shape deduplication, field order, and dynamic-access fallback tests pass. |
| HXC-SEM-010 | Fieldless enums SHOULD use native enums where representation permits; payload enums MUST use readable tagged unions or equivalent layouts. | P0 | E3 | Pattern matching, equality, payload lifetime, and ABI layout tests pass. |
| HXC-SEM-011 | Functions MUST lower to prototypes/definitions with exact parameter/result and calling-convention rules. | P0 | E2 | Direct-call and function-pointer fixtures compile and run. |
| HXC-SEM-012 | Non-capturing closures SHOULD become function pointers; capturing closures MUST use explicit environment representation and lifetime management. | P0 | E3/E4 | Capture, mutation, escape, callback, and cleanup fixtures pass. |
| HXC-SEM-013 | Classes MUST use concrete structs and direct calls when possible, adding object headers or dispatch tables only when semantics require them. | P0 | E3/E4 | Allocation, fields, methods, identity, inheritance, and devirtualization fixtures pass. |
| HXC-SEM-014 | Inheritance layout and dispatch MUST be deterministic and safe under base/derived conversions. | P0 | E3 | Layout static assertions and polymorphic call suites pass. |
| HXC-SEM-015 | Interfaces MUST use a documented witness/vtable strategy and SHOULD specialize/devirtualize closed-world calls. | P1 | E3 | Multiple-interface and dynamic-dispatch fixtures pass. |
| HXC-SEM-016 | Generics SHOULD monomorphize to concrete C types/functions with deterministic deduplication and code-size reporting. | P0 | E3 | Specialization identity, recursive generic, and size-limit fixtures pass. |
| HXC-SEM-017 | `Dynamic` MUST be opt-in by source semantics and represented through a tagged runtime value rather than boxing every value. | P1 | E4 | Dynamic field/call/cast/equality fixtures pass and non-dynamic programs remain unboxed. |
| HXC-SEM-018 | Reflection metadata MUST be reachability-driven and emitted only for required types/members unless explicitly kept. | P1 | E4/E5 | `Type`/`Reflect` conformance and dead-metadata elimination tests pass. |
| HXC-SEM-019 | Portable exceptions MUST preserve throw/catch/finally behavior through ADR 0006 explicit HxcIR failure/cleanup edges, using proven result lowering first and the contained exception slice only for general behavior. | P0 | E4 | Nested throw, rethrow, finally, automatic-local, allocation cleanup, and strategy-report tests pass. |
| HXC-SEM-020 | No exception or longjmp-like control transfer may cross an exported C ABI, callback return, foreign frame, signal, or thread boundary. | P0 | E7 | Generated export/callback wrappers translate failures into the configured status/error contract. |
| HXC-SEM-021 | Static initialization order MUST be deterministic, dependency-aware, and cycle-diagnosed. | P0 | E2/E3 | Cross-module initialization fixtures have stable traces and cycle diagnostics. |
| HXC-SEM-022 | `if`, loops, switch, pattern matching, break/continue, return, and short-circuit operators MUST preserve Haxe control-flow semantics. | P0 | E2 | Structured-control differential suite passes. |
| HXC-SEM-023 | Equality, identity, comparison, casts, and type tests MUST have explicit type-directed lowering. | P0 | E2–E4 | Semantic matrix covers primitives, strings, enums, arrays, objects, dynamic values, and null. |
| HXC-SEM-024 | Unsafe pointer, aliasing, alignment, volatile, restrict, atomic, and bit-level operations MUST be isolated in typed `c.*` APIs. | P1 | E3/E8 | Metal fixtures compile with strict aliasing/sanitizers and emit intended qualifiers/intrinsics. |
| HXC-SEM-025 | Concurrency semantics MUST be defined per environment, with runtime/platform adapters for Haxe thread primitives where supported. | P1 | E4/E5 | Thread, mutex, atomic, TLS, and shutdown tests pass on declared hosted platforms. |

### 33.5 Runtime and memory

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-RT-001 | Runtime capabilities MUST be modeled as stable feature IDs with deterministic dependency closure and no unconditional baseline feature. | P0 | E4 | Feature manifest validation, empty-plan, and closure tests pass. |
| HXC-RT-002 | Every root runtime requirement MUST include a stable semantic reason, consumed typed surface, and source span; transitive features MUST retain dependency-edge provenance. | P0 | E4/E8 | Tests assert why each helper was selected without duplicate dependency warnings. |
| HXC-RT-003 | `hxc_runtime=none` MUST prove eligibility and fail with precise blockers rather than silently linking runtime code; success means no `hxrt` include, source, define, library, or symbol. | P0 | E4 | No-runtime positive/negative fixtures and artifact/symbol inspection pass. |
| HXC-RT-004 | Direct C and program-local specialization MUST precede runtime requests, and unused runtime source/metadata MUST not be emitted or linked. | P0 | E3/E4 | Representation and feature-combination tests inspect lowering decisions, sources, symbols, and binary size. |
| HXC-RT-005 | Allocation MUST use an explicit allocator ABI and permit user/platform allocators. | P0 | E4 | Hosted, custom, failure-injection, and freestanding allocator tests pass. |
| HXC-RT-006 | When selected, portable managed objects MUST use the ADR 0005 precise non-moving tracing contract with exact global/stack/thread roots; the backend MUST be pluggable behind a stable compiler/runtime interface and interior/foreign pointers MUST obey explicit root rules. | P0 | E4 | Cycle, root, interior/foreign-pointer, finalization policy, pause/allocation, stress, and backend-conformance tests pass. |
| HXC-RT-007 | The compiler SHOULD stack/region-allocate values proven not to escape without changing observable behavior. | P2 | E4/E10 | Escape-analysis fixtures and allocation benchmarks demonstrate safe reductions. |
| HXC-RT-008 | Runtime strings, arrays, objects, dynamic values, reflection, and exceptions MUST be separate features rather than one monolithic dependency. | P0 | E4 | Minimal programs select only the expected slices. |
| HXC-RT-009 | Cleanup actions MUST be explicit for owned C resources, temporary allocations, and exceptional exits. | P0 | E4 | Leak/failure-path sanitizer tests pass. |
| HXC-RT-010 | The runtime public ABI MUST be versioned and checked against generated code/manifests. | P0 | E4/E7 | Mismatched ABI versions fail clearly; compatible versions link. |
| HXC-RT-011 | Hosted, freestanding, WASI, and Emscripten environments MUST expose capability matrices and avoid unavailable libc assumptions. | P1 | E4/E8 | Environment-specific compile tests and capability diagnostics pass. |
| HXC-RT-012 | Runtime implementation MUST remain valid strict C for the declared baseline and C++-include-safe at public headers. | P0 | E4/E10 | GCC/Clang/MSVC or documented equivalent and C++ header smoke tests pass. |
| HXC-RT-013 | Runtime diagnostics MUST support silent-report, aggregate-summary, and deduplicated root-warning presentation without changing semantics or packaging. | P1 | E0/E4/E8 | Profile/default/override fixtures assert diagnostic severity, source spans, report equality, and resolution provenance. |

### 33.6 Standard library

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-STD-001 | Every applicable Haxe standard-library module MUST have an owner, implementation category, runtime features, platform status, and test status in the ledger. | P0 | E5 | Ledger validator rejects missing/unowned entries. |
| HXC-STD-002 | `Std`, `Math`, `StringTools`, core iterators, `Lambda`, and foundational helpers MUST be implemented early with differential tests. | P1 | E5 | Core stdlib suite passes. |
| HXC-STD-003 | Collections including arrays, vectors, maps, lists, option/result-like patterns, and sorting MUST preserve documented behavior. | P1 | E5 | Collection conformance suite passes. |
| HXC-STD-004 | `haxe.io` bytes, buffers, input/output streams, encodings, and compression adapters MUST have ownership-safe C implementations. | P1 | E5 | I/O round-trip and failure-path suites pass. |
| HXC-STD-005 | `sys` filesystem, process, environment, arguments, clocks, and console APIs MUST be provided on supported hosted platforms. | P1 | E5 | Platform matrix tests pass or report explicit unsupported capabilities. |
| HXC-STD-006 | Date/time behavior MUST document timezone/resolution/platform limitations and match Haxe APIs within the declared contract. | P2 | E5 | Date/time differential suite passes across supported platforms. |
| HXC-STD-007 | Regex support MUST use a selected backend with documented syntax/Unicode/packaging behavior. | P2 | E5 | EReg conformance and dependency tests pass. |
| HXC-STD-008 | Serialization, JSON, reflection-backed helpers, and resources MUST preserve wire formats or document versioned target differences. | P1 | E5 | Cross-target round-trip fixtures pass where compatibility is promised. |
| HXC-STD-009 | Threading and synchronization standard APIs MUST map to declared platform capabilities and integrate with the memory manager. | P1 | E5 | Concurrency stress and teardown tests pass. |
| HXC-STD-010 | Platform-specific APIs MUST live behind target-owned adapters and not contaminate portable core modules. | P0 | E5/E9 | Dependency/lint checks keep portable examples free of target packages. |

### 33.7 C and C++ library consumption

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-FFI-001 | Hand-authored Haxe externs MUST map directly to C symbols, types, headers, libraries, defines, and calling conventions. | P0 | E6 | Point-library fixture links without generated wrapper overhead where none is needed. |
| HXC-FFI-002 | `hxc bindgen` MUST derive declaration facts from a real Clang semantic AST using exact target, defines, include paths, and language mode. | P0 | E6 | Regeneration from locked inputs is deterministic and matches native compilation. |
| HXC-FFI-003 | Bindgen MUST emit a lock/manifest containing input hashes, tool version, target triple, flags, include graph, and mappings. | P0 | E6 | Changing any ABI-relevant input is detected and reported. |
| HXC-FFI-004 | Generated bindings MUST separate raw ABI-faithful externs from optional ergonomic wrappers. | P0 | E6 | Raw layer mirrors headers; wrapper layer can be regenerated or replaced independently. |
| HXC-FFI-005 | Bindgen MUST support typedefs, enums, structs, unions, opaque/incomplete types, pointers, arrays, and function pointers. | P0 | E6 | Declaration-category corpus compiles and layout probes match. |
| HXC-FFI-006 | Bindgen MUST handle const/volatile/restrict, alignment, packing, bitfields, flexible array members, and calling-convention attributes where the target can represent them. | P1 | E6 | ABI probes pass or unsupported constructs produce stable diagnostics. |
| HXC-FFI-007 | Variadic functions and preprocessor macros MUST be supported only through explicit safe strategies or documented restrictions. | P1 | E6 | Supported macro/vararg fixtures pass; unsafe cases fail clearly. |
| HXC-FFI-008 | Callbacks MUST model function pointer, context pointer, retention, thread, and lifetime contracts. | P0 | E6 | Synchronous/asynchronous callback fixtures and cleanup tests pass. |
| HXC-FFI-009 | Binding mappings MUST express nullability, ownership, string encoding, slices, sentinel termination, errors, and handle lifetimes. | P0 | E6 | Generated wrappers enforce mappings and failure-path tests pass. |
| HXC-FFI-010 | Build metadata MUST integrate pkg-config, explicit libraries, frameworks, search paths, compile flags, and platform conditions. | P1 | E6/E8 | Representative system and vendored libraries build through the neutral manifest. |
| HXC-FFI-011 | AI assistance MAY suggest names, docs, ownership interpretations, and wrapper APIs but MUST NOT alter parser/layout facts without deterministic configuration and review. | P0 | E6 | Suggested changes are sidecar diffs and ABI manifests remain unchanged unless config changes. |
| HXC-FFI-012 | Binding regeneration MUST preserve approved customizations through declarative config, not edits to generated raw files. | P0 | E6 | Regeneration test retains mappings and produces a reviewable diff. |
| HXC-FFI-013 | Arbitrary C++ APIs MUST be consumed through an existing or generated C-compatible shim; the product MUST not pretend C can directly represent C++ ABI features. | P0 | E6/E9 | C++ shim example covers class lifetime, methods, error translation, and `extern "C"`. |
| HXC-FFI-014 | An optional future C++ shim generator MAY cover a constrained subset, but unsupported templates/overloads/exceptions/ownership MUST remain explicit. | P3 | E6 | Experimental scope is gated and does not affect core C bindgen correctness. |

### 33.8 Producing C ABIs and libraries

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-ABI-001 | Haxe declarations marked for export MUST generate stable C symbols, prototypes, and implementation wrappers. | P0 | E7 | C consumer compiles against generated header and links without Haxe compiler knowledge. |
| HXC-ABI-002 | Generated public headers MUST be valid C11 and include cleanly from supported C++ modes. | P0 | E7 | Independent C and C++ header consumers compile warning-free. |
| HXC-ABI-003 | Public headers MUST use `extern "C"` guards for C++ consumers without exposing C++ types. | P0 | E7 | C++ symbol linkage test passes. |
| HXC-ABI-004 | ABI-safe value types MUST have explicit width, layout, alignment, nullability, and versioning rules. | P0 | E7 | Static assertions and ABI reports match across compiler matrix. |
| HXC-ABI-005 | Haxe objects SHOULD cross the ABI as opaque handles unless an explicitly frozen C struct layout is requested. | P0 | E7 | Opaque create/use/free example and layout opt-in example pass. |
| HXC-ABI-006 | Every pointer/string/buffer/handle in a public API MUST document ownership, borrow duration, mutation, nullability, and release function. | P0 | E7 | Header docs, manifest, and consumer leak tests agree. |
| HXC-ABI-007 | Errors MUST cross the ABI through configured status/result structures and thread-safe message/detail retrieval, never Haxe exceptions. | P0 | E7 | Failure injection from Haxe implementation is correctly observed by C consumers. |
| HXC-ABI-008 | Exported callbacks MUST define calling convention, context, reentrancy, thread, retention, and cancellation behavior. | P1 | E7 | Callback consumer tests cover lifecycle and cross-thread policy. |
| HXC-ABI-009 | `hxc export` MUST produce static-library and shared-library build targets for supported platforms. | P0 | E7/E8 | Linux/macOS/Windows or declared platform matrix builds and loads artifacts. |
| HXC-ABI-010 | Export output MUST include a C header, compile/link manifest, CMake/Meson/pkg-config metadata, symbol list, and ABI report. | P0 | E7 | A clean external consumer uses only installed artifacts. |
| HXC-ABI-011 | ABI comparison tooling MUST classify compatible, source-compatible, and breaking changes. | P1 | E7/E10 | Golden versions detect symbol, layout, calling-convention, ownership, and semantic contract changes. |
| HXC-ABI-012 | The release process MUST freeze/version public compiler-runtime and generated-library ABIs independently. | P1 | E7/E10 | Version policy and compatibility tests are published before 1.0. |

### 33.9 Target abstractions and macros

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-MAC-001 | `c.Ptr`, `ConstPtr`, `Ref`, `Span`, `ConstSpan`, `CString`, `Owned`, `Borrowed`, `Allocator`, `VolatilePtr`, and ABI integer abstractions MUST lower with explicit contracts. | P0 | E3/E6 | Positive and misuse diagnostics cover each abstraction. |
| HXC-MAC-002 | C structs, unions, enums, opaque handles, bitfields, alignment, and packing MUST be expressible through typed metadata/macros and verified against native layout. | P0 | E3/E6 | Layout probes and static assertions pass. |
| HXC-MAC-003 | Link/include/define/framework/section/visibility/calling-convention metadata MUST feed the neutral build and ABI manifests. | P0 | E6/E8 | Metadata fixture produces expected build plan and declarations. |
| HXC-MAC-004 | `c.Syntax` or equivalent raw-C escape MUST be explicit, restricted, source-positioned, and absent from normal examples. | P1 | E3/E9 | Escape use is listed by `hxc inspect` and lint policy enforces boundaries. |
| HXC-MAC-005 | Unsafe operations MUST require an explicit lexical/API boundary and produce diagnostics when contracts are incomplete. | P0 | E3 | Unsafe fixtures distinguish reviewed low-level code from accidental pointer use. |
| HXC-MAC-006 | Static extensions SHOULD provide ergonomic pointer/span/result APIs without hiding allocation or ownership. | P1 | E3 | Generated C and runtime reports show no unexpected helper use. |
| HXC-MAC-007 | Compile-time DSLs SHOULD generate concrete types, functions, schemas, and glue rather than runtime reflection when inputs are known. | P1 | E9 | Macro expansion artifacts are deterministic and inspectable. |
| HXC-MAC-008 | Rails/Ruby-like ergonomics MUST come from convention, generated code, and type-checked macros, not open-class/runtime `method_missing` behavior. | P1 | E9 | Todo example remains concise in Haxe and explicit in generated C. |
| HXC-MAC-009 | Macro implementations MUST account for Haxe phase/build-order limitations and avoid order-dependent global state. | P0 | E0/E9 | Randomized module order and compiler-server macro tests pass. |
| HXC-MAC-010 | Users MUST be able to inspect macro products, mappings, generated declarations, and source reasons. | P1 | E8/E9 | `hxc inspect macros` or build artifacts expose deterministic expansion reports. |
| HXC-MAC-011 | Haxe-authored C declarations MUST express public/private headers, forward declarations, qualifiers, linkage, layout, calling conventions, constants/assertions, and build facts through typed constructs, with dependency structure derived by the compiler. | P0 | E1/E6/E7 | Header-authoring fixtures produce deterministic C/C++-consumable headers and reject invalid declaration graphs before C emission. |
| HXC-MAC-012 | A first-party macro or DSL MUST justify why ordinary Haxe and existing typed APIs are insufficient, keep typed/inspectable inputs and outputs, diagnose misuse at source positions, and expose allocation, ownership, unsafe, portability, and runtime effects. | P0 | E0/E9 | Admission review plus positive/negative/determinism/no-runtime fixtures pass for every admitted surface. |

### 33.10 CLI and build experience

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-CLI-001 | `hxc doctor` MUST validate Haxe, Reflaxe, C compilers, build tools, Clang tooling, platform SDKs, and Beads separately with actionable output. | P0 | E8 | Doctor positive/negative fixtures and JSON output pass. |
| HXC-CLI-002 | `hxc init` MUST create a minimal project without overwriting user files and SHOULD offer portable, metal, library, and binding templates. | P1 | E8 | Template projects build in clean directories. |
| HXC-CLI-003 | `hxc build` MUST drive Haxe generation and optionally the native compile/link plan with reproducible command reporting. | P0 | E8 | Hello and library projects build through CLI and direct manifest paths. |
| HXC-CLI-004 | `hxc run` and `hxc test` MUST compose build/run behavior without hiding native exit status, stdout, stderr, or signals. | P1 | E8 | CLI integration fixtures propagate results correctly. |
| HXC-CLI-005 | `hxc bindgen` MUST expose deterministic parser inputs, config, dry-run, diff, and verify modes. | P0 | E6/E8 | Binding workflow is scriptable through human and JSON output. |
| HXC-CLI-006 | `hxc export` MUST generate/install C ABI artifacts and run independent consumer verification. | P0 | E7/E8 | Shared-library example completes from one documented command sequence. |
| HXC-CLI-007 | `hxc inspect` MUST report generated files, runtime reasons, symbols, layouts, ABI, dependencies, and unsafe/raw boundaries. | P1 | E8 | Human/JSON schemas are stable and tested. |
| HXC-CLI-008 | `hxc clean` MUST remove only owned generated artifacts using manifests. | P1 | E8 | User files survive clean; stale generated files are removed. |
| HXC-CLI-009 | Configuration MUST support hxml/defines plus an optional project file with documented precedence and reproducible normalization. | P0 | E8 | Conflict and precedence matrix tests pass. |
| HXC-CLI-010 | All commands MUST have stable exit categories and `--json` modes suitable for agents/CI where meaningful. | P1 | E8 | CLI schema and error-code tests pass. |

### 33.11 Examples and product proof

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-EX-001 | `hello` MUST be the first end-to-end real Haxe-to-C executable and select no unnecessary runtime feature. | P0 | E2/E9 | GCC and Clang compile/run and runtime report matches expectation. |
| HXC-EX-002 | `no-runtime` MUST prove a useful `metal`/`hxc_runtime=none` program and show failure diagnostics when a forbidden feature is added. | P0 | E4/E9 | Positive/negative fixtures and symbol inspection pass. |
| HXC-EX-003 | `pointlib` MUST demonstrate hand-authored C extern consumption with exact struct/function layout. | P1 | E6/E9 | Native library and Haxe consumer interoperate under ABI probes. |
| HXC-EX-004 | `shared-library` MUST demonstrate Haxe implementation consumed by independent C and C++ programs. | P0 | E7/E9 | Consumers build outside generated tree using installed artifacts. |
| HXC-EX-005 | `cpp-shim` MUST honestly demonstrate a C wrapper around a C++ class/library. | P1 | E6/E9 | C consumer controls C++ object lifetime and sees translated failures. |
| HXC-EX-006 | `todo-cli` MUST demonstrate Rails-like compile-time ergonomics, deterministic SQLite bindings, migrations, validation, commands, cleanup, and a portable domain core. | P1 | E9 | CLI acceptance suite and generated-C review pass; domain core builds on selected sibling targets. |
| HXC-EX-007 | An embedded/freestanding example SHOULD demonstrate custom startup/allocator, no libc assumptions, volatile/MMIO-safe APIs, and cross compilation. | P2 | E9 | Cross-compiler build and map/symbol inspection pass. |
| HXC-EX-008 | A WebAssembly example SHOULD demonstrate C toolchain reuse and optional TypeScript declarations for exported ABI. | P2 | E9 | WASI/Emscripten build and JS/TS consumer smoke pass. |
| HXC-EX-009 | At least one non-C language consumer SHOULD load an exported hxc shared library through its ordinary FFI. | P2 | E7/E9 | Python, Ruby, Elixir, Rust, or equivalent consumer test passes. |
| HXC-EX-010 | A generated-versus-hand-written benchmark corpus SHOULD measure readability proxies, code size, runtime, allocations, and agent comprehension. | P2 | E10 | Versioned results and methodology are published. |

### 33.12 Quality, security, and releases

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-QA-001 | Every lowering feature MUST have positive, negative, AST/IR or snapshot, native compile, runtime, and policy coverage as applicable. | P0 | E0/E10 | Definition-of-done validator or review checklist is satisfied. |
| HXC-QA-002 | Generated C MUST compile warning-free under strict supported compiler flags. | P0 | E10 | Compiler matrix treats warnings as errors. |
| HXC-QA-003 | The ADR 0007 Tier 1 tuples are release blockers: GCC/Clang on Linux, Apple Clang on macOS, clang-cl on Windows, and one GNU Arm Embedded GCC freestanding-metal lane; MSVC and other platform compilers require explicit promotion evidence. | P0 | E10 | CI/release evidence matches the declared tuple and distinguishes native/emulated execution from compile-only results. |
| HXC-QA-004 | Undefined-behavior, address, leak, and thread sanitizers MUST run on eligible suites. | P0 | E10 | Sanitizer matrix is green or has documented platform exclusions. |
| HXC-QA-005 | Differential tests MUST compare portable behavior against an accepted Haxe target/oracle. | P0 | E2–E5/E10 | Semantic trace/output comparisons pass. |
| HXC-QA-006 | Fuzzing SHOULD cover parser/config inputs, C AST printer, bindgen normalization, serializers, and runtime boundary APIs. | P2 | E10 | Seed corpora, crash minimization, and regression fixtures exist. |
| HXC-QA-007 | Output MUST be deterministic across repeated runs, module ordering, checkout roots, locale, and supported hosts. | P0 | E1/E10 | Reproducibility CI compares hashes. |
| HXC-QA-008 | Performance benchmarks MUST separate compiler time, C compile time, runtime, code size, allocations, and FFI overhead. | P1 | E10 | Versioned benchmark dashboard and regression thresholds exist. |
| HXC-QA-009 | ABI tests MUST include native layout probes, symbol inspection, independent consumers, and version comparisons. | P0 | E6/E7/E10 | ABI suite passes for supported triples. |
| HXC-QA-010 | Cross-target portability tests MUST compile designated portable cores on hxc and selected sibling compilers. | P1 | E9/E10 | CI matrix records compatible source subset and exclusions. |
| HXC-QA-011 | Security review MUST cover generated injection boundaries, bindgen inputs, path traversal, build flags, allocator failures, integer overflow, and supply-chain provenance. | P0 | E10 | Threat model and security test suite are release-gated. |
| HXC-QA-012 | Claims about “hand-written quality,” performance, or LLM effectiveness MUST be tied to published methodology and results. | P1 | E9/E10 | README claim checker/review verifies evidence links. |
| HXC-QA-013 | Release artifacts MUST be reproducible, checksummed, signed where configured, and include source/runtime/tool manifests. | P1 | E10 | Release rehearsal verifies provenance and clean-install examples. |
| HXC-QA-014 | Beads acceptance evidence MUST name commands, artifacts, and relevant requirement IDs before issues close. | P0 | E0–E10 | Closed issue audit finds no missing evidence for release milestones. |

### 33.13 Documentation and community contract

| ID | Normative requirement | Priority | Suggested epic | Minimum evidence |
| --- | --- | --- | --- | --- |
| HXC-DOC-001 | README MUST explain the portable/metal/runtime axes with real commands and current limitations. | P0 | E9 | Docs tests and examples remain synchronized. |
| HXC-DOC-002 | README MUST compare hxc and hxcpp honestly without claiming C is categorically superior for every use case. | P0 | E9 | Comparison includes direct C++, C ABI, embedded, runtime, tooling, and output tradeoffs. |
| HXC-DOC-003 | Documentation MUST state that C cannot directly consume arbitrary C++ classes/templates/overloads/exceptions and show the shim pattern. | P0 | E9 | Interop guide and example agree. |
| HXC-DOC-004 | A C programmer guide MUST explain generated layout, ownership, runtime slices, symbols, headers, and escape hatches. | P1 | E9 | Guide follows a metal example end to end. |
| HXC-DOC-005 | A Haxe programmer guide MUST explain C libraries, exports, portability boundaries, and when runtime support appears. | P1 | E9 | Guide follows portable and FFI examples. |
| HXC-DOC-006 | Bindgen and export guides MUST document deterministic inputs, regeneration, ownership, ABI compatibility, and troubleshooting. | P1 | E6/E7/E9 | Guides are exercised in CI scripts/examples. |
| HXC-DOC-007 | Known limitations and unsupported features MUST be explicit and generated from capability/ledger data where possible. | P0 | E8/E9 | Published capability manifest matches tests. |
| HXC-DOC-008 | TypeScript-oriented onboarding MAY highlight syntax/type-system familiarity but MUST explain macros, target semantics, nullability, and native ownership differences. | P2 | E9 | Onboarding review confirms no misleading equivalence claim. |

---

## 34. End-to-end user journeys and acceptance scenarios


### 34.1 Haxe developer: compile a normal application

**Intent:** keep writing ordinary Haxe, use familiar standard-library APIs, and receive a native C build without manually managing a runtime.

Expected flow:

```text
hxc doctor
hxc build build.hxml
hxc run build.hxml
hxc inspect runtime --json
```

Acceptance:

- the Haxe source does not import `c.*`;
- generated C is readable and maps back to Haxe source;
- the runtime report lists only features actually required;
- behavior matches the portable oracle;
- the user can stop after C generation and use an external build system.

### 34.2 C programmer: use Haxe as a typed systems language

**Intent:** define exact data layout and ownership, call libc/OS/vendor APIs, use macros/generics for zero-cost code generation, and keep control of allocation and linking.

Acceptance:

- `reflaxe_c_profile=metal` and `hxc_runtime=none` are usable for a meaningful program;
- layouts match native C probes;
- generated headers/source use expected C types, qualifiers, linkage, and calling convention;
- hidden allocation, boxing, reflection, or exception machinery is rejected;
- unsafe boundaries are visible in Haxe and inspection output.

### 34.3 Library consumer: bind an existing C API

**Intent:** point hxc at headers and exact compiler flags, receive accurate externs plus optional ergonomic wrappers, and regenerate safely as the library changes.

Expected flow:

```text
hxc bindgen vendor/libfoo/include/foo.h \
  --module vendor.foo \
  --target x86_64-unknown-linux-gnu \
  --config bindings/foo.hxc-bind.json \
  --output bindings/foo
hxc bindgen --verify bindings/foo
hxc build examples/foo/build.hxml
```

Acceptance:

- ABI facts originate from Clang and layout probes;
- all inputs are locked and diffable;
- raw bindings are faithful and wrappers are separately reviewable;
- callback/ownership/error mappings are explicit;
- AI suggestions never mutate facts silently.

### 34.4 Library author: export a shared object

**Intent:** implement performance-critical logic in Haxe and publish a stable C ABI loadable by C, C++, and other FFI-capable languages.

Expected flow:

```text
hxc export build-library.hxml --name todo_core --shared --static
hxc inspect abi build/todo_core/abi.json
cmake -S examples/consumer-c -B build/consumer-c
cmake --build build/consumer-c
```

Acceptance:

- generated public header is independent of Haxe internals;
- errors, ownership, strings, buffers, handles, and callbacks have explicit contracts;
- no exception crosses the boundary;
- C and C++ consumers build from installed artifacts;
- ABI comparison reports breaking changes before release.

### 34.5 C++ library user: consume through a shim

**Intent:** use a C++ implementation while retaining a C target and stable C-facing boundary.

Acceptance:

- wrapper implementation is compiled as C++ and header as C-compatible `extern "C"`;
- classes are represented as opaque handles;
- templates/overloads are instantiated and named explicitly by the shim;
- C++ exceptions are caught and translated;
- documentation never claims direct C access to arbitrary C++ ABI.

### 34.6 Rails/Ruby ergonomics: build the todo CLI

**Intent:** write concise declarative Haxe and use compile-time macros to generate schema, repository, validation, command routing, migrations, and cleanup-heavy SQLite glue.

Required commands:

```text
todo add "Ship hxc"
todo list
todo list --all
todo done 1
todo edit 1 "Ship reflaxe.c"
todo remove 1
todo migrate
todo help
```

The example MUST include:

- a portable domain model and validation layer;
- a C-specific SQLite adapter generated from deterministic bindings;
- a typed model macro with primary keys, defaults, nullability, indexes, and migration fingerprints;
- prepared statements and explicit transaction/error/cleanup paths;
- command/argument macros that generate help and optional shell completion;
- no runtime-reflective ORM;
- generated C reviewed against a disciplined hand-written SQLite CLI;
- a cross-target test that compiles the portable domain portion with selected sibling targets.

### 34.7 Embedded developer: freestanding build

**Intent:** use Haxe types/macros for firmware-oriented code without assuming a hosted runtime.

Acceptance:

- no libc function is referenced unless supplied by the platform adapter;
- startup, allocator, panic, and I/O hooks are explicit;
- volatile/MMIO and fixed-layout APIs lower correctly;
- generated object/map files contain only expected symbols;
- diagnostics identify any source feature that would require an unavailable runtime capability.

### 34.8 TypeScript-oriented developer: move typed application logic native

**Intent:** reuse familiar typed syntax and structural/generic concepts while learning Haxe macros and explicit native ownership.

Acceptance:

- onboarding starts with a small portable module and exported C/WebAssembly API;
- docs identify semantic differences rather than presenting a transliteration promise;
- generated TypeScript declarations, when offered, describe the exported ABI—not arbitrary Haxe internals.

### 34.9 Agent/Codex workflow: continue across sessions

**Intent:** preserve decisions, blockers, discoveries, and evidence across long implementation sessions.

Acceptance:

- `bd prime` restores project workflow context;
- `bd ready --json` shows only explicitly unblocked work;
- one issue is claimed atomically;
- discoveries are linked rather than hidden in TODO comments;
- completion reason lists tests/artifacts/requirement IDs;
- changes to product semantics include an ADR and PRD update.


---

## 35. Brownfield implementation work breakdown

The roadmap starts from the present scaffold. Each workstream names assets to extend and the observable exit rather than asking Codex to “bootstrap a compiler” generically.

| Workstream | Starts from | Required work | Exit condition | Seed Beads range |
| --- | --- | --- | --- | --- |
| W0 — Audit and ratify | Existing metadata, macros, runtime seed, docs, plan | Pin dependencies; compile the Haxe scaffold; resolve license/string/GC/exception/platform ADRs; make all named checks real | Green reproducible M0; no semantic implementation hidden behind placeholders | E0.T01–E0.T10 |
| W1 — C AST and output ownership | `CAST`, `CASTPrinter`, `GeneratedFile`, Reflaxe adapter | Complete declarators/precedence; add HxcIR; dependencies/names; deterministic project emitter; build manifest | AST fixtures compile; deterministic multi-file output exists | E1.T01–E1.T10 |
| W2 — First executable semantics | Fail-closed `CCompiler`, runtime Int32 helpers, hello seed | Primitives, locals, blocks, functions, calls, return, main, trace/stdout, explicit sequencing, UB-safe ops | Real hello builds/runs under GCC and Clang | E2.T01–E2.T11 |
| W3 — Structured language model | Target abstractions and C AST | Arrays, structs, enums, generics, classes, inheritance, interfaces, closures, initialization | Representative Haxe programs lower without universal boxing | E3.T01–E3.T10 |
| W4 — Selective runtime | Runtime header/source/features seed | Feature planner, allocator integration, strings, managed objects, memory backend, dynamic, reflection, exceptions, cleanup | Portable object graph works; runtime-free lane remains proven | E4.T01–E4.T12 |
| W5 — Standard library | Ledger seed and runtime/platform layers | Implement by dependency order; differential oracle; capability matrix; upstream/provenance tracking | Declared stdlib surface passes conformance | E5.T01–E5.T12 |
| W6 — C consumption | Pointlib seed, c.* ABI abstractions | Extern lowering, Clang model, bindgen lock/config, probes, wrappers, callbacks, C++ shims | Real-world C library bindings regenerate deterministically | E6.T01–E6.T12 |
| W7 — C production | Shared-library seed and export abstractions | Export analysis/wrappers/header/build/install/ABI reports/version diff/consumers | External C/C++ and one other language consume hxc libraries | E7.T01–E7.T10 |
| W8 — Product CLI | `Run.hx`, config seeds, neutral manifests | Implement `hxc` commands, config precedence, build adapters, inspect/doctor/clean, JSON schemas | Automation-friendly CLI drives end-to-end workflows | E8.T01–E8.T11 |
| W9 — Showcase and docs | Example source seeds and product docs | Turn seeds into acceptance suites; build Rails-style todo macro stack; write honest audience guides | Examples prove positioning and portability | E9.T01–E9.T12 |
| W10 — Hardening and release | CI/runtime smoke/plan validation seeds | Compiler/platform matrix, sanitizers, fuzzing, benchmarks, ABI/security/reproducibility/release gates | Beta and 1.0 evidence supports public claims | E10.T01–E10.T12 |

### 35.1 Parallelism rule

Only real prerequisites block work. Documentation, runtime design experiments, fixture infrastructure, and ABI research may proceed in parallel after their contracts are ratified. Broad stdlib work must not outrun primitive/class/runtime semantics, and product examples must not be made to pass through special-case code paths that bypass the compiler architecture.

### 35.2 Vertical-slice rule

Every milestone should land at least one end-to-end slice that starts in Haxe source and ends in compiled/loaded native behavior. Internal architecture without a native proof is not enough; examples without reusable compiler paths are not enough.

---

## 36. Beads translation and execution specification

### 36.1 Existing plan: import before regenerating

The handoff already contains `docs/specs/beads-plan.json` with 11 epics, 122 tasks, and explicit blocking edges. Codex SHOULD validate and materialize that graph rather than deriving an unrelated second plan from prose.

Use the current Beads agent workflow, preserving any existing project state:

```sh
bd version
[ -d .beads ] || bd init --quiet
bd setup codex
python3 scripts/beads/validate_plan.py --json
python3 scripts/beads/bootstrap.py --json
python3 scripts/beads/bootstrap.py --apply
bd prime
bd ready --json
```

Beads is installed as a CLI, not vendored into this repository. Initialize only when `.beads/` is absent, and never use a destructive force-initialization path. `bd setup codex` installs the supported Codex integration; `bd prime` supplies the current workflow context. Because Beads evolves, Codex MUST run `bd version`, `bd help`, and `bd prime` and adapt command spelling only when the installed CLI proves a documented difference. It MUST preserve the stable plan keys and acceptance semantics even when CLI syntax changes.

### 36.2 Mapping rules

- A milestone/workstream becomes one `epic`.
- A requirement or coherent acceptance slice becomes a `feature`, `task`, `bug`, or `chore` child.
- Parent-child hierarchy is structural and does not imply blocking.
- A hard prerequisite uses `bd dep add <child> <blocker>`.
- A discovered issue uses `--deps discovered-from:<source-id>`.
- Cross-cutting related work uses a non-blocking related relationship rather than a fake prerequisite.
- Stable plan keys such as `E6.T03` remain in issue metadata/title even though Beads assigns its own IDs.
- Programmatic calls use `--json`.
- Priorities use Beads `0` through `4`, with `0` highest.

### 36.3 Issue content contract

Every implementation issue MUST contain:

```text
Title: [HXC <stable-key>] <observable outcome>
Type: epic|feature|task|bug|chore
Priority: 0..4
Parent: <epic id>
Requirement IDs: HXC-...
PRD sections: §...
Context: why the work exists and which scaffold assets it extends
Scope: concrete included work
Non-scope: nearby work deliberately excluded
Dependencies: only real blockers
Acceptance: observable behavior and artifacts
Validation: exact commands/suites expected
Compatibility: profile/runtime/environment/ABI impact
Documentation: files/ledger/ADR to update
```

Acceptance criteria must be outcome-oriented. “Implement X” is not sufficient by itself. A compiler feature normally requires source fixture, IR/AST evidence, warning-clean C compilation, runtime/differential behavior, negative diagnostics, runtime-feature assertion, and documentation/ledger updates.

### 36.4 Translation algorithm for additional PRD requirements

When a requirement is not represented adequately in the seed graph, Codex should:

1. search existing Beads issues and stable plan keys for overlap;
2. add the requirement ID to an existing issue if its acceptance remains coherent;
3. otherwise create the smallest independently verifiable issue under the appropriate epic;
4. add only necessary hard blockers;
5. link it with `discovered-from` to the issue/session that exposed the gap;
6. update `docs/specs/beads-plan.json` only when the reproducible bootstrap itself should change for future clones;
7. update this PRD only for product/semantic changes, not routine status.

Do not create one issue per sentence mechanically. Group requirements when they share one implementation and one proof; split them when they can complete independently or carry different compatibility risk.

### 36.5 Brownfield labels

Recommended labels:

- `scaffold-audit` — validates or repairs an existing seed;
- `compiler`, `hxc-ir`, `c-ast`, `runtime`, `stdlib`, `bindgen`, `abi`, `cli`, `example`, `docs`;
- `profile-portable`, `profile-metal`, `runtime-none`, `environment-freestanding`;
- `semantic-risk`, `abi-risk`, `ub-risk`, `security`, `performance`;
- `decision`, `adr-required`, `needs-owner`;
- `milestone-m0` … `milestone-m10`.

### 36.6 Claim, discovery, and close loop

```text
bd ready --json
bd show <id> --json
bd update <id> --claim --json
# implement, test, document
bd create "Found: <specific issue>" \
  --description "Context, reproduction, expected outcome" \
  -t bug -p 1 --deps discovered-from:<id> --json
bd close <id> \
  --reason "Requirements HXC-... satisfied; validation: <commands>; artifacts: <paths>" \
  --json
```

The repository's default agent policy is conservative: do not invent permission to push Git or Beads remotes. When a remote workflow is explicitly configured, use `bd dolt push`/`bd dolt pull` according to project policy.

### 36.7 Initial ready work

Only the contract-ratification task is intentionally ready at first. It must determine or explicitly defer the decisions that would otherwise leak into public code: license, exact Haxe/Reflaxe pins, C baseline, string contract, managed-memory bootstrap/default, exception strategy, target defines, and initial platform matrix.

After those decisions, work should flow into scaffold compilation, CI consistency, AST/IR completion, and the first real hello vertical slice—not into broad standard-library or framework development prematurely.

### 36.8 No duplicate task systems

Markdown documents may describe milestones, requirements, and acceptance policy. They MUST NOT be maintained as a parallel live status checklist once Beads is initialized. TODO comments are allowed only for local implementation notes that are resolved in the same issue; discovered work becomes a linked Beads issue.

---

## 37. Decision register and recommended defaults

This table records accepted design gates alongside the defaults for decisions
that still have an owning implementation experiment. Accepted rows change only
through a superseding ADR. A recommendation is the default for unresolved ADR
analysis; evidence may justify a different accepted result.

| Decision | Recommended default | Owner artifact | Planning effect |
| --- | --- | --- | --- |
| Repository license and upstream-derived stdlib policy | Owner selected GPL-3.0-only on 2026-07-15; the checked inventory rejects unowned vendor/runtime files and unlisted `_std` work | E0.T02 / `third-party-provenance.json` | Repository licensing and current provenance are resolved; `haxe_c-od2.5` still blocks release packaging until generated-output/runtime redistribution treatment is ratified. |
| Exact Haxe and Reflaxe baseline | Accepted M0 pin: official Haxe 5.0.0-preview.1 at `2c1e544e0a2c7524ef4c8e103f1b0580362ea538` with release-artifact hashes, Reflaxe 4.0.0-beta-compatible commit `73a983112e039daad46b37912ab238df6bf0cf53`, and Lix 17.0.2; broaden only through explicit API audit, ADR review, and CI evidence | E0.T03 / ADR 0007 / `toolchain-lock.json` | Compiler API work uses one checksum-verified framework and custom-target surface. |
| Default C dialect | Accepted: strict ISO C11/no extensions is the normative source and public-header floor; C17 preserves it, while C23 internal syntax remains experimental and ABI-neutral | ADR 0007 / E0.T01 | AST, printer, runtime, header, and compiler gates share one floor. |
| Target define name | Accepted semantic contract: `--custom-target c=<output>` is the production carrier; `c`/`target.name=c` is target identity; `c_output` is derived Reflaxe transport; and `reflaxe_c` is implementation-owned. The target installs `target.unicode`, never `target.utf16`, with adapter-derived capabilities. | ADR 0007 / E0.T01/E0.T03 / `haxe_c-od2.6` | Real production typing snapshots the matching platform facts and reaches the fail-closed lowering boundary; Haxe 4 Cross remains a regression negative. |
| Direct C and runtime fallback | Owner accepted direct C/local specialization before selective `hxrt`; portable defaults to `auto + summary`, metal to `minimal + warn`, and explicit `none` proves complete absence | ADR 0001 / E0.T01 / E4.T01 / E4.T10 | Runtime planner, diagnostics, manifests, and no-runtime fixtures use one reason ledger. |
| Typed C authoring and DSL admission | Owner accepted Haxe-first declarations, typed `c.*`, validated metadata/macros, narrow justified DSLs, and explicit raw authority in that order | ADR 0002 / `haxe_c-od2.3` | Blocks bootstrap completion of the absent `c.*` scaffold and informs E3/E6/E7/E9. |
| String representation | Accepted: immutable valid UTF-8, Unicode-scalar indices, no normalization, deterministic U+FFFD decoding, and separate `Bytes`/`CString`/ABI-view contracts | ADR 0004 / E0.T01/E4.T03 | String/std/FFI implementation now has an observable contract. |
| Portable managed-memory bootstrap | Accepted: selective target-owned precise non-moving stop-the-world mark-and-sweep with exact roots; backend details stay internal and conservative adapters are non-default | ADR 0005 / E0.T01/E4.T06 | Object/closure/dynamic implementation may target one root and reachability model. |
| Exception lowering | Accepted: explicit HxcIR failure/cleanup edges, proven result lowering first, otherwise a contained strict-C11 frame/cleanup runtime; no non-local transfer crosses native ABI/thread boundaries | ADR 0006 / E0.T01/E4.T09 | Try/catch/finally, cleanup, callbacks, and exports share one boundary rule. |
| Array bounds and null failure behavior | Match the accepted Haxe portable contract; metal may expose checked/unchecked typed operations explicitly | E2 / ADR | Blocks array and pointer APIs. |
| Regex backend | Select based on semantics, Unicode, portability, licensing, and size; make it a runtime/platform feature | E5 / ADR | Does not block primitive vertical slice. |
| Initial supported platforms | Accepted 1.0 Tier 1: Linux/glibc x86_64+aarch64 on GCC/Clang; macOS arm64+x86_64 on Apple Clang; Windows x86_64 on clang-cl; plus one capability-limited ARM Cortex-M GCC freestanding-metal lane. WASI/Emscripten/MSVC begin below Tier 1 | ADR 0007 / E0.T01/E10 | CI must distinguish native/emulated run from compile-only evidence and publish exact version floors. |
| Build-system adapters | One neutral manifest is normative; CMake and Meson are generated adapters, not the semantic source of truth | E1/E8 / ADR | Blocks emitter interface, not semantics. |
| Public ABI stability point | Everything is experimental before 1.0; opaque handles default; freeze only after ABI diff tooling and consumers exist | E7/E10 / ADR | Prevents accidental early compatibility promises. |
| C++ shim generator scope | Manual/declared shims are core; automatic C++ subset generation is experimental and post-core | E6 / ADR | Does not block C bindgen. |
| “Best C target” marketing | Treat as aspiration until conformance, code quality, and performance evidence exists | E9/E10 | Blocks marketing claim, not implementation. |

---

## 38. Capability and release definitions

### 38.1 M0 scaffold (current handoff)

The repository may claim only:

- product/architecture specification exists;
- target bootstrap/profile/runtime/C AST/runtime/API seeds exist;
- compiler lowering is intentionally unsupported and fails closed;
- native runtime seed and deterministic Beads plan can be validated.

It MUST NOT claim a functioning Haxe-to-C target, full standard library, working bindgen, or generated shared-library product.

### 38.2 Developer preview

Minimum claim gate:

- scaffold compiles under pinned Haxe/Reflaxe;
- real hello and primitive/control-flow fixtures generate, compile, and run;
- output is deterministic and warning-clean under GCC and Clang;
- runtime reason report exists;
- no-runtime example is either working or explicitly not yet claimed;
- unsupported semantics fail with stable diagnostics.

### 38.3 Alpha

Minimum claim gate:

- useful values, functions, control flow, arrays, enums, generics, concrete classes, and closures;
- a documented portable memory backend for supported programs;
- core String/Bytes/collections/I/O subset;
- hand-authored C externs and initial deterministic bindgen;
- exported C library proof;
- hello, no-runtime, pointlib, shared-library, and C++ shim examples;
- CI, sanitizers, and capability manifest.

Alpha ABI and generated code format remain explicitly unstable.

### 38.4 Beta

Minimum claim gate:

- broad class/interface/dynamic/reflection/exception behavior;
- a materially complete hosted standard library for tier-1 platforms;
- robust bindgen declaration coverage and wrapper mappings;
- export/install/ABI diff workflows;
- production-like todo CLI;
- every accepted Tier 1 hosted OS has a fresh-user smoke path; secondary
  architectures and the freestanding-metal lane may still be completing their
  full 1.0 evidence;
- performance, code-size, security, and reproducibility evidence;
- migration notes for known breaking changes.

### 38.5 Version 1.0

Minimum claim gate:

- every ADR 0007 Tier 1 tuple has non-skippable archived evidence: native
  compile/link/run for hosted lanes and emulator or hardware execution for the
  declared freestanding-metal lane, with cross-compile-only results labeled as
  such;
- all applicable standard-library ledger entries for supported platforms are implemented, tested, or explicitly excluded by published capability contract;
- no known release-blocking semantic divergence, C undefined behavior, memory-safety issue, or ABI defect;
- public compiler/runtime/export ABI version policies are frozen and tested;
- independent C/C++ and at least one other-language consumer pass;
- portable-core cross-target promise is demonstrated;
- documentation and examples match current behavior;
- reproducible signed/checksummed release artifacts and provenance are published;
- every public marketing claim has evidence.

---

## 39. Positioning and README claim guardrails

### 39.1 hxc versus hxcpp

Use this framing:

- **Choose hxc** when the desired artifact is C, a stable C ABI, an embedded/freestanding build, straightforward C toolchain integration, inspectable C source, or a library intended for many languages.
- **Choose hxcpp** when direct C++ classes/templates/ecosystem integration, mature hxcpp platform support, or existing C++-target behavior is the primary requirement.
- C++ can consume a well-designed C API easily. C cannot directly consume arbitrary C++ language/ABI features; a C shim is required.
- C is not categorically “better than C++” for every systems problem. hxc's value is the C artifact and control model, not a universal language-ranking claim.

### 39.2 “Hand-written C”

The README may say “hand-written-looking” or “idiomatic” only while defining the standard:

- normal C declarations and control flow;
- no universal boxed value when concrete representation is possible;
- no helper call without a semantic reason;
- deterministic names/layout/comments;
- strict warning-clean compilation;
- generated output reviewed against representative hand-written baselines.

It MUST not imply that every Haxe construct can become zero-runtime C.

### 39.3 Full standard library

Before the ledger reaches the 1.0 gate, say “full standard-library support is a product requirement/roadmap,” not “fully supported.” Publish the current capability manifest.

### 39.4 TypeScript bridge

A useful onboarding statement is that Haxe offers familiar typed syntax, structural concepts, generics, and cross-target compilation. Documentation must also explain macro phases, target-dependent semantics, native ownership, C ABI constraints, and differences from the TypeScript/JavaScript runtime model.

### 39.5 LLM friendliness

Deterministic C is plausibly useful to code agents because the language and output can be simple and explicit. Market this as a design goal until benchmarked. Generated files remain compiler-owned; agents should fix Haxe/compiler source rather than hand-edit output except during diagnosis.

### 39.6 “Best C target Haxe has had”

This is an aspiration. It becomes a defensible claim only after public conformance, interoperability, code-quality, and performance evidence. The README can describe the ambition without stating an unmeasured fact.

---

## 40. Definition of done and complete handoff checklist

### 40.1 Per compiler feature

A semantic/compiler feature is done only when:

- its requirement IDs and active Beads issue are identified;
- source semantics and profile/runtime differences are documented;
- positive and relevant negative Haxe fixtures exist;
- HxcIR/C AST or direct unit coverage exists;
- generated C snapshots are deterministic;
- C compiles warning-free under the required matrix;
- behavior matches the oracle or documented target-specific contract;
- runtime feature selection/reasons are asserted;
- sanitizer/ABI/layout tests run where relevant;
- docs, diagnostics registry, capability manifest, and stdlib ledger are updated;
- no unowned TODO remains;
- Beads close reason records exact evidence.

### 40.2 Per C interop/export feature

Additionally:

- exact native headers/flags/triple and tool versions are recorded;
- layouts and symbols are probed independently;
- ownership/nullability/error/callback contracts are explicit;
- C and C++ consumer tests compile outside the generated source tree;
- regeneration and ABI-diff behavior are tested.

### 40.3 Per macro/DSL feature

Additionally:

- macro phase/build-order behavior is deterministic;
- generated APIs participate in IDE/type checking where feasible;
- expansion artifacts can be inspected;
- no runtime reflection is introduced when compile-time generation suffices;
- generated cleanup and failure paths are tested.

### 40.4 Archive completeness for Codex

The handoff archive is complete when it contains:

- this full PRD;
- accurate `CODEX_HANDOFF.md` and `AGENTS.md`;
- the existing scaffold source/runtime/std/example seeds;
- architecture/configuration/repository-pattern documents;
- deterministic Beads plan, validator, preview/import script, and plan guide;
- local structural/runtime validation scripts;
- source snapshot references or a manifest explaining how to obtain them;
- a package checksum manifest;
- no statement that a planned capability is already implemented.

### 40.5 Recommended first implementation sequence from this scaffold

1. Run all structural, plan, runtime, and Haxe type-check gates; fix the handoff before adding semantics.
2. Preserve the ratified P0 decisions in ADR 0001, 0002, and 0004–0007; create a
   superseding ADR rather than hardening contradictory code.
3. Pin and verify Reflaxe/Haxe; make the current Haxe sources compile.
4. Complete C declarator/expression printer corpus and output ownership.
5. Add HxcIR and typed-AST normalization.
6. Implement a real `main`, constants, locals, blocks, functions, calls, returns, and stdout path.
7. Compile/run hello under GCC and Clang with a runtime report.
8. Add explicit sequencing and UB-safe primitive semantics.
9. Add aggregates/enums/arrays/generics before broad runtime object semantics.
10. Keep bindgen/export/example work on the dependency graph rather than starting every workstream simultaneously.

---


## 41. References and provenance

Primary implementation references, checked for this handoff on 2026-07-15:

- Haxe Manual, Macros: `https://haxe.org/manual/macro.html`
- Haxe Manual, Type Building: `https://haxe.org/manual/macro-type-building.html`
- Haxe Manual, Macro Build Order: `https://haxe.org/manual/macro-limitations-build-order.html`
- Haxe Manual, String Unicode: `https://haxe.org/manual/std-String-unicode.html`
- Haxe Manual, String Encoding: `https://haxe.org/manual/std-String-encoding.html`
- Haxe 5.0.0-preview.1 release:
  `https://haxe.org/download/version/5.0.0-preview.1/`
- Haxe custom-target implementation and protocol:
  `https://github.com/HaxeFoundation/haxe/pull/11128`
- Reflaxe repository/new-target documentation: `https://github.com/SomeRanDev/reflaxe`
- Reflaxe MIT license: `https://github.com/SomeRanDev/reflaxe/blob/main/LICENSE`
- Haxe 5.0.0-preview.1 license bundle and standard-library MIT notice: `https://github.com/HaxeFoundation/haxe/blob/5.0.0-preview.1/extra/LICENSE.txt`
- LLVM/Clang license: `https://github.com/llvm/llvm-project/blob/main/LICENSE.TXT`
- Haxe C++ target getting started: `https://haxe.org/manual/target-cpp-getting-started.html`
- hxcpp runtime/project: `https://github.com/HaxeFoundation/hxcpp`
- ISO C++ FAQ, mixing C and C++: `https://isocpp.org/wiki/faq/mixing-c-and-cpp`
- Clang AST introduction: `https://clang.llvm.org/docs/IntroductionToTheClangAST.html`
- Beads repository: `https://github.com/gastownhall/beads`
- Beads issue/dependency concepts: `https://beads.gascity.com/core-concepts/issues`
- Beads CLI create reference: `https://beads.gascity.com/cli-reference/create`

The Repomix snapshots under `reference/repomix/` are the primary local evidence for sibling-target repository conventions, Reflaxe integration patterns, and Haxe/hxcpp implementation precedent. `docs/research/repo-patterns.md` records the extracted patterns and should be updated when Codex discovers additional relevant precedent.

The supplied repository snapshots remain the primary family-pattern evidence for this bootstrap.
