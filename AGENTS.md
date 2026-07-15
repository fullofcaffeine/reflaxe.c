# Agent Instructions

This project uses **bd** (beads) for issue tracking. Run `bd prime` for full workflow context.

> **Architecture in one line:** Issues live in a local Dolt database
> (`.beads/dolt/`); cross-machine sync uses `bd dolt push/pull` (a
> git-compatible protocol), stored under `refs/dolt/data` on your git
> remote — separate from `refs/heads/*` where your code lives.
> `.beads/issues.jsonl` is a passive export, not the wire protocol.
>
> See [SYNC_CONCEPTS.md](https://github.com/gastownhall/beads/blob/main/docs/SYNC_CONCEPTS.md)
> for the one-screen overview and anti-patterns (don't treat JSONL as the
> source of truth; don't `bd import` during normal operation; don't
> reach for third-party Dolt hosting before trying the default).

## Quick Reference

```bash
bd ready              # Find available work
bd show <id>          # View issue details
bd update <id> --claim  # Claim work atomically
bd close <id>         # Complete work
bd dolt push          # Push beads data to remote
```

## Non-Interactive Shell Commands

**ALWAYS use non-interactive flags** with file operations to avoid hanging on confirmation prompts.

Shell commands like `cp`, `mv`, and `rm` may be aliased to include `-i` (interactive) mode on some systems, causing the agent to hang indefinitely waiting for y/n input.

**Use these forms instead:**
```bash
# Force overwrite without prompting
cp -f source dest           # NOT: cp source dest
mv -f source dest           # NOT: mv source dest
rm -f file                  # NOT: rm file

# For recursive operations
rm -rf directory            # NOT: rm -r directory
cp -rf source dest          # NOT: cp -r source dest
```

**Other commands that may prompt:**
- `scp` - use `-o BatchMode=yes` for non-interactive
- `ssh` - use `-o BatchMode=yes` to fail instead of prompting
- `apt-get` - use `-y` flag
- `brew` - use `HOMEBREW_NO_AUTO_UPDATE=1` env var

## Project-Specific Contract

### Normative sources and read order

This is a brownfield M0 compiler scaffold. Do not treat it as a blank Reflaxe
target or as a functioning Haxe-to-C compiler.

Before changing compiler, runtime, target API, ABI, or build behavior, read:

1. `AGENTS.md` and `bd prime` for the current operating rules;
2. `docs/PRD.md` completely, especially Sections 0, 10-17, 24, 29, 32,
   36, and 40;
3. the relevant accepted records under `docs/adr/` for public decisions that
   refine the PRD;
4. `docs/architecture.md` for compiler-layer invariants;
5. `docs/research/repo-patterns.md` for sibling-target and hxcpp precedent;
6. `docs/BEADS_PLAN.md` and the active issue from
   `docs/specs/beads-plan.json`;
7. every existing source/runtime file in the area being changed.

An accepted ADR that explicitly supersedes the PRD has highest authority.
Otherwise use this order: PRD, architecture/configuration docs, active Beads
acceptance criteria, scaffold code, then examples/comments.

### Trust the checkout, not the aspirational inventory

The current checkout is a deliberately partial slice of the fuller scaffold
described by PRD Section 32. Verify files with `rg --files` before citing or
running them. In particular, this checkout currently lacks several documented
assets, including `CODEX_HANDOFF.md`, some schemas, examples, runtime feature
planning and broad runtime implementation, and the CLI implementation. The
pinned Haxe/Reflaxe/Lix package metadata, M0 target API contracts, and minimal
compiler lifecycle are present and compile-verified; do not repeat older
missing-metadata or missing-adapter assumptions.

- A path named in the PRD is not proof that it exists here.
- A present seed is not proof that its capability works.
- Do not invent passing validation output for absent scripts or toolchains.
- Track missing or contradictory scaffold assets as Beads discoveries under
  the appropriate epic; do not silently expand the issue already claimed.
- Keep README and capability claims at M0 until executable evidence advances
  the release gate.

### Compiler and runtime invariants

- Extend the existing architecture; do not run a new-target generator over
  the repository or replace it with a generic transpiler template.
- Preserve the intended two-stage `CompilerBootstrap` / `CompilerInit`
  activation and exactly-once Reflaxe registration when those files are
  implemented or restored.
- `CReflaxeCompiler` is an adapter. Whole-program semantics belong behind
  `CompilationContext` and `CCompiler`, with fresh mutable state per build.
- Preserve fail-closed behavior. `HXC1000` is the deliberate scaffold result,
  not a failure to hide, weaken, or report as successful compilation.
- Lower Haxe semantics into an explicit target-owned HxcIR before choosing C
  syntax when evaluation order, lifetime, ownership, cleanup, dispatch,
  boxing, or failure edges matter.
- Extend `CAST` and `CASTPrinter` structurally. Normal lowering must not
  concatenate user-derived C strings. No raw expression or declaration node is
  currently admitted; any future compiler-owned escape valve needs a narrow
  typed authority, validation, executable negative tests, and an owning issue.
- Treat `CDeclarator` as the C grammar tree, not as a semantic type string:
  `DGroup` is the parenthesized-declarator production, `DName(null)` is an
  abstract declarator, and array/function variants retain their distinct C11
  forms. Never infer grouping in the printer or encode `*`, `[]`, or `()` in an
  identifier.
- Construct finalized output names through `CIdentifier`. It checks lexical
  C11 spelling; E1.T04 still owns mangling, reserved namespaces, and collision
  policy. Typed attributes, alignment specifiers, and atomic type names have
  dedicated nodes—do not restore `Array<String>` attributes.
- Preserve expression-tree association exactly; do not algebraically
  reassociate, commute, or flatten nodes even when a C operator looks
  associative. Numeric precedence alone is insufficient for C grammar:
  assignment left operands and prefix increment/decrement operands require a
  `unary-expression`, and adjacent unary tokens need separation when they
  could fuse into `++`, `--`, or `&&`.
- Construct integer and floating constants through validated literal nodes,
  `_Generic` through typed associations, and source anchors through structured
  `DLineDirective`/`SLineDirective` nodes. Never restore opaque literal or
  preprocessor-token strings. A `#line` mapping applies to the immediately
  following physical line, so the printer must not insert a blank after it.
- Treat generated C as adversarial source text: identifiers, strings, byte
  characters, comments, and line filenames must pass their dedicated
  validators/escapers. Preserve trigraph safety and reject malformed Unicode;
  never interpolate user text into comments, literals, directives, or tokens.
- `CASTPrinter` defaults to strict ISO C11. GNU/Clang attribute output requires
  an explicit dialect and must never leak into the normative strict lane.
  Update `test/c_ast/expected/declarators.c` or `expressions.c` only from an
  intentional structural fixture change, then run both `npm run test:c-ast`
  and the native matrix. The expression fixture's literal 6×6 family matrix is
  the reviewable contract; do not replace it with expected values derived from
  the printer's own precedence table.
- Write generated artifacts only through Reflaxe output ownership. Never
  hand-manage stale output outside its generated-file manifest.
- Preserve Haxe evaluation order explicitly and never depend on C undefined
  behavior, including signed overflow, invalid shifts, aliasing violations,
  misalignment, uninitialized reads, or lifetime bugs.
- Output must be deterministic across module order, checkout path, locale, and
  repeated/compiler-server builds. Do not embed unstable absolute paths,
  timestamps, random IDs, or map iteration order in normal artifacts.
- Examples, tests, templates, and standard-library surfaces must not use raw
  `__c__` injection to make an unsupported path appear complete. Add a typed
  `c.*` abstraction or a reusable compiler lowering instead.
- Runtime support is a feature graph. Every selected helper needs a stable
  feature ID and a source reason; unused features must remain absent.
- The checked-in `runtime/hxrt` implementation is a provisional native smoke
  seed for its public header, not an unconditional runtime core and not evidence
  that generated Haxe selects or links it. Keep fixture-only C/C++ code under
  `runtime/hxrt/test` or `test/native`; never describe those independent native
  inputs as generated output.
- Runtime planning happens only after direct-representation, escape/lifetime,
  and specialization decisions. Prefer direct idiomatic C, then a program-local
  specialized helper, then the narrowest dependency-closed `hxrt` slice. Never
  add an unconditional runtime `core`; a runtime-free build contains no `hxrt`
  include, source, define, library, or symbol.
- Runtime warnings identify deduplicated root semantic requirements at source
  spans. Transitive dependencies belong in `hxc.runtime-plan.json`, not as
  repetitive warnings. Every successful build, including an empty plan, records
  the resolved policy and its provenance.
- C-facing features use ordinary Haxe first, then typed `c.*` abstractions,
  validated metadata/macros, a narrow typed DSL only for a demonstrated language
  gap, and finally explicit raw C authority. Repeated raw snippets indicate a
  missing abstraction.
- A new macro, metadata spelling, or DSL needs typed/inspectable inputs and
  outputs, source-positioned negative diagnostics, deterministic expansion
  evidence, explicit allocation/ownership/unsafe/runtime/portability effects,
  and a documented benefit that justifies its surface area. It must not bypass
  profile, runtime, environment, ABI, or raw-boundary policy.
- Use Haxe compile-time checks for declaration graphs, layouts, qualifiers,
  ownership, lifetimes, constants, and schemas where sound, but retain native
  authority: imported ABI facts come from Clang/probes and generated layout
  claims receive `_Static_assert` plus C/C++ consumer tests.
- Public C ABI types are separate from internal object/runtime layouts. Every
  exported pointer, string, buffer, handle, callback, error, and allocator
  boundary needs explicit layout, ownership, nullability, lifetime, calling
  convention, and failure policy. No exception may cross the C ABI.
- Preserve ADR 0004 string units: portable `String` is valid UTF-8 but every
  ordinary Haxe index is a Unicode-scalar index. Never expose byte offsets or
  UTF-16 surrogate halves through `String`, normalize implicitly, store malformed
  UTF-8, or conflate `String` with binary `Bytes` or NUL-terminated `c.CString`.
- The default tracing backend, when selected, is ADR 0005 precise non-moving
  mark-and-sweep with compiler-emitted exact roots. Do not treat arbitrary raw,
  interior, or foreign pointers as roots; require a typed live base/root or
  pinned handle, and keep collector details out of generated public types.
- Preserve ADR 0006 explicit exceptional and cleanup edges through HxcIR.
  Prefer proven result/status lowering; otherwise isolate exception frames and
  `setjmp`/`longjmp` in the selected runtime slice. Never transfer across an
  export, callback return, foreign frame, signal, or thread boundary, and never
  rely on indeterminate C locals after `longjmp`.

### Pinned toolchain and activation

- Treat `.haxerc`, `package.json`, `package-lock.json`,
  `docs/specs/toolchain-lock.json`, `haxe_libraries/*.hxml`, and
  `vendor/reflaxe` as one reviewed dependency contract. Never float Haxe,
  Reflaxe, or Lix versions and never refresh one pin without its checksums,
  provenance, notices, and lifecycle evidence.
- The production typing carrier is exactly Haxe `5.0.0-preview.1` at the
  revision and official artifact hashes in `toolchain-lock.json`. Its custom-
  target `PlatformConfig` API is preview surface. A pin change requires a source
  API audit, intentional platform/stdlib snapshot review, full package/server
  probes, provenance updates, and ADR review; do not broaden the version range.
- The reviewed Reflaxe bundle is compiler-only source. It must not appear in
  generated C, become an `hxrt` feature, or be described as a generated-program
  runtime dependency. Do not patch it silently; document target-required patches
  and include them in the bundle digest, or prefer a new upstream commit.
- `haxe_libraries/reflaxe.hxml` supplies only the vendored classpath and version.
  It must not start Reflaxe. `CompilerInit` owns the exactly-once
  `ReflectCompiler.Start()` and `AddCompiler(...)` sequence.
- Keep `extraParams.hxml` caller-CWD-independent: no relative `-cp`/`-p` entries,
  and `CompilerBootstrap.Start()` must precede `CompilerInit.Start()`. Scoped
  source-checkout paths use `${SCOPE_DIR}` and precedence-sensitive `_std` roots
  must exist on the initial classpath rather than being injected late.
- Production builds use `--custom-target c=<output>`. Haxe must invoke
  `c.Init.init()` and `TargetPlatform.configure()` before target facts are
  finalized. Validate a static, scalar-Unicode platform snapshot before
  registration; derive `target.sys` from the resolved environment and advertise
  no threading or atomics until the owning adapter has evidence.
- `c_output` is the internal Reflaxe output transport and legacy early detector.
  Derive it from `Compiler.getOutput()`; if explicitly supplied it must
  normalize to the same path. `c_output` alone, `-D c`, or a rewritten
  `target.name` is not a supported production carrier. On valid activation
  expose `c`, `reflaxe_c`, and `target.unicode`; reject `target.utf16` and do not
  invent other target capability defines.
- `reflaxe_c_lifecycle_probe` is an internal test seam that avoids entering the
  incomplete lowering boundary during E0 bootstrap tests. It must retain the
  real `CustomTarget(c)` platform and may skip only Reflaxe registration/
  lowering. Never document it as a user option or let it bypass a production
  compilation claim.
- Eval is only a target-neutral future `hxc` host, a differential oracle, and a
  non-C isolation test. It is never a user-program production carrier, and its
  host capabilities must never be copied into C manifests.
- Legacy Reflaxe `Cross` predefines `target.name=cross`, `target.utf16`, and
  `utf16`. Preserve its source-anchored `HXC0003` regression failure. Never
  relabel Cross, patch upstream conditionals ad hoc, maintain an implicit Haxe
  fork, or weaken the UTF-8 scalar contract to make bootstrap work.
- Keep `test/bootstrap/expected/target-contract.json` structural and
  path-stable. It must prove cold/package/server behavior, exactly-once counts,
  public versus internal defines, and the typed upstream
  `String`/`StringTools`/`UnicodeString` branch. A pin or carrier change requires
  an intentional snapshot and ADR review.
- Keep `test/all_sources/all_sources.hxml` and `test/all_sources/run.py`
  exhaustive. The runner automatically enumerates every repository-owned
  `src/**/*.hx` and `std/c/**/*.hx` module, checks explicit compiler-macro
  branches under Eval, and re-enters the real custom target to prove
  source-anchored `HXC1000` with no plausible artifact. New owned Haxe sources
  must stay visible to this gate; never replace its dynamic inventory with a
  frozen module count or treat its Eval pass as a production carrier.
- The package-layout probe may flatten the pinned framework into a temporary
  classpath to prove installed resolution. It is not release assembly or
  permission to publish while `haxe_c-od2.5` remains unresolved.
- The future `hxc` command is an optional orchestration layer. Bootstrap its
  target-neutral core on Eval, later dogfood the C target for the native binary,
  and retain direct Haxe/HXML plus the C toolchain as the recovery build path.
  Compiler registration must never depend on an existing `hxc` executable.

### Typed-AST input boundary

- `CReflaxeCompiler.filterTypes` is the complete typed-module capture boundary:
  it runs before Reflaxe callback filters. Preserve typedefs, externs,
  abstracts, secondary declarations, metadata, and module ownership there;
  never reconstruct whole-program input from class/enum callback order.
- Treat the pending/current module arrays as request-local handoff state.
  `onCompileStart` must create a fresh `CompilationContext`, consume and clear
  the pending capture, and `onCompileEnd` must clear current storage before
  invoking code that can fail. No capture or `TypedProgramInput` may survive a
  compiler-server request.
- Normalize unordered typed input by logical module/declaration coordinates
  while retaining raw Haxe compiler objects for semantic lowering. Preserve
  compiler/source order for field groups, enum constructors, metadata, and
  initializer roots with explicit ordinals; determinism must never erase enum
  indices or initialization/annotation order. Primary versus secondary
  ownership is explicit. Do not serialize raw
  positions, absolute roots, backslashes, timestamps, map iteration, or other
  host/process facts into an inventory.
- Reflaxe's exact process-local
  `@:build(reflaxe.ReflectCompiler.addToBuildCache())` hook is excluded from the
  normalized metadata view because it appears only after cache reuse. Do not
  generalize that exception to arbitrary `@:build` or source-authored metadata;
  the raw declaration remains untouched.
- `reflaxe_c_typed_ast_report` is an implementation-only unsupported-node
  inventory, and `reflaxe_c_test_reverse_typed_modules` is a test-only order
  seam. Neither is public application configuration. An intentional normalized
  input/report change updates `docs/typed-ast-input.md`, the fixture catalog,
  and `test/typed_ast/expected/`, then runs `npm run test:typed-ast`, the central
  snapshot check, and the exhaustive all-source gate.
- Typed-input collection is not Haxe-to-HxcIR lowering. Preserve the
  source-anchored `HXC1000` no-output boundary until E1.T07 implements and tests
  actual semantic lowering.

### HxcIR semantic boundary

- Treat HxcIR instruction arrays as semantic evaluation order. Never reorder
  them or fuse side-effecting instructions into a C expression whose operand or
  argument order is weaker. Canonicalization may sort only collections that the
  model declares unordered.
- Immutable value IDs are function-wide unique but block-local in availability:
  an instruction may use function parameters, its block parameters, and earlier
  results in that block. Pass cross-block data through typed block arguments;
  never reach into another block's temporary namespace.
- Places are mutable addressable storage, not values. Preserve structural local,
  global, field, index, and dereference places through load/store/address and
  lifetime operations; do not encode an lvalue as a value string or C fragment.
- Every block terminates. Calls and allocations carry explicit failure edges;
  successful instruction continuation is ordered locally. Throw, result,
  allocation, and native-status failures must remain distinguishable until a
  validated exception/result strategy pass chooses C control flow.
- Cleanup actions are stored in registration order. Edge cleanup steps are the
  actual execution order: reverse within a region, then direct parent regions
  from inner to outer. Do not infer cleanup later from C lexical nesting, and do
  not put dereference/index temporaries in cleanup actions—materialize a stable
  local place first.
- Runtime intent is explicit: `IRIStatic`, then `IRIProgramLocal`, then a named
  `IRIRuntime` feature. A runtime call also names its feature and operation.
  Never turn an allocation, `Dynamic` type, failure edge, or cleanup region into
  an implicit `hxrt` request.
- Every semantic node carries a normalized repository-relative source span.
  Reject and redact absolute/backslash/dot-segment paths before dumping. Dumps
  retain ordered fields and use UTF-8 sorting only for unordered collections;
  do not add timestamps, locale formatting, absolute roots, or map iteration.
- `IRTDynamic` means the real Haxe `Dynamic` semantic type; it is not an opaque
  unsupported-node escape. Frontend lowering must emit source-positioned
  `HXC1001` and stop that path. Internal invalid IR uses `HXC9000`; never repair
  either by inventing a constant, null, raw C, or placeholder value.
- An intentional HxcIR model, validation, dump, or diagnostic change updates
  `docs/hxc-ir.md` and the reviewable files under `test/hxc_ir/expected/`, then
  runs `npm run test:hxc-ir` and the exhaustive all-source gate. HxcIR-only
  fixtures prove pre-emission semantics, not generated C or runtime selection.

### Typed C contract and declaration planning

- Files under `std/c/` are compiler contracts, not implemented wrapper objects.
  Importing or mentioning them selects no runtime feature. Do not add fallback
  allocations or ordinary runtime method bodies to make an unsupported operation
  appear usable; implement the owning typed lowering and runtime-plan evidence.
- Canonical declaration metadata is namespaced: `@:c.layout`, `@:c.header`,
  `@:c.name`, `@:c.include`, `@:c.link`, `@:c.define`, `@:c.pkgConfig`,
  `@:c.framework`, `@:c.pack`, `@:c.align`, `@:c.bitField`, `@:c.linkage`,
  `@:c.callingConvention`, `@:c.visibility`, `@:c.section`, `@:c.export`, and
  `@:c.constant`. Unknown `@:c.*` spellings fail with `HXC5002`; never add an
  alias without updating ADR 0002, the PRD, typed-C guide, validator, snapshot,
  and negative tests together.
- `TypedCContractMacro` rebuilds a structural snapshot from the current typed
  module set. It must remain deterministic and per-compilation, must not consult
  ambient process state, and must not write files. Later compiler stages route
  snapshot-derived artifacts through Reflaxe output ownership.
- Typed C contract schema 2 retains sorted `ownerModulePaths` on every merged
  build fact. Do not discard that provenance: declaration-owned includes are
  what let the planner keep generated headers minimal and explain why each
  include exists.
- `CDeclarationPlanner` is a pure structural analysis. Complete-type edges order
  declarations or select the owning header; pointer-only edges select typed
  forward declarations; authoritative includes on headerless opaque imports are
  propagated instead of guessed declarations. Never turn a forward edge into a
  convenience include or rebuild these decisions from C strings in an emitter.
- Public headers may forward-declare a type whose definition is private, but
  must not include a private header for a complete type. Header guards are the
  deterministic, collision-free path encoding from the plan; users never author
  or override them as declaration strings.
- `test/declaration_plan/` is a test-only AST adapter over the planner, not the
  production multi-file emitter. An intentional plan or header change requires
  `npm run test:declaration-plan` and the strict native matrix; preserve the
  independently compilable, repeated-include, pointer-cycle, and no-`hxrt`
  evidence.
- `reflaxe_c_contract_report` is an implementation-only inspection/test seam.
  Do not document it as an application option or confuse its empty M0 effect
  list with a whole-program `hxc.runtime-plan.json` proof.
- `c.StaticAssert.require` proves only literal Haxe compile-time facts. Layout,
  size, offset, alignment, toolchain, and imported-header claims still require
  generated `_Static_assert`, Clang/native probes, and independent consumers.
- `c.Syntax` and `c.Unsafe` intentionally expose no operation. Adding raw or
  unsafe authority requires the owning Beads issue, a searchable lexical/API
  boundary, inspection output, strict-mode enforcement, and positive/negative
  no-hidden-runtime evidence.
- The source vocabulary is ratified but its generated public ABI is not stable.
  E7 owns ABI-safe export/header implementation and E10.T09 owns stabilization;
  do not publish compatibility claims from the M0 snapshot.

### Fixture taxonomy and snapshot ownership

- Treat `docs/specs/fixture-taxonomy.json` as the machine-readable registry and
  `docs/testing.md` as its human contract. New cases belong in one of the eight
  canonical lanes under
  `test/{positive,negative,ast,snapshot,runtime,differential,abi,performance}/`
  and require a schema-valid `case.json`. Existing focused M0 suites remain in
  place only because the registry maps their runners and evidence explicitly.
- Every case declares its inputs, argument-array runner, working directory,
  timeout, network policy, exact exit code, and at least one semantic assertion.
  Do not infer success from a zero exit alone, silently skip required tools, or
  normalize output without naming the field and semantic reason.
- Checked-in expected artifacts are owned by the central snapshot registry.
  Check them with `npm run snapshots:check`; regenerate intentionally with
  `npm run snapshots:update -- --suite <suite-id>` (or the visibly broader
  `--all`). The updater must render twice, print semantic diffs before writing,
  stay inside catalog-owned roots, and run the owning suite afterward. CI never
  updates snapshots. Do not hand-edit expected files to bless a regression.
- A snapshot is evidence for the artifact it asserts, not for a neighboring
  layer. Direct HxcIR/C AST and planned-header fixtures do not prove
  Haxe-to-C lowering; independent native C/C++ fixtures do not prove generated
  runtime or ABI behavior. Preserve `seed-only` and `contract-only` labels until
  the corresponding generated-Haxe, sanitizer, differential, consumer, or
  benchmark evidence actually exists.
- Examples are explicit product proofs, never an implicit test glob. Every
  future `examples/<name>/` requires a `case.json` with `role: "example"` and
  declared assertions before a runner may execute it. Put reusable compiler
  regressions in a focused fixture when that gives a smaller diagnostic proof.
- When adding a runner or expected tree, update the taxonomy, snapshot registry,
  package entry points, hook/CI guards, and testing guide together. The fixture
  policy guard must retain complete ownership of every checked-in `expected`
  file so stale or orphaned goldens cannot pass unnoticed.

### Product contracts

- One compiler pipeline serves both profiles. `portable` is the default Haxe
  semantic contract; `metal` exposes explicit C layout/ownership constraints.
- Runtime policy (`auto|minimal|none`) and environment
  (`hosted|freestanding|wasi|emscripten`) are orthogonal to profile.
- With no explicit runtime override, `portable` resolves to `auto + summary` and
  `metal` to `minimal + warn`; explicit valid combinations still win. Use
  `hxc_runtime=none` for the hard whole-program no-runtime proof.
- Strict ISO C11 with no extensions is the normative generated-source, runtime,
  fixture, and public-header floor. C17 preserves that contract; C23 internal
  syntax remains experimental and cannot silently alter semantics or ABI.
- `scripts/ci/runtime_smoke.py` owns the native warning baseline and must
  compile/run both structural C AST goldens, independently compile every
  declaration-plan header, run their combined consumer, compile the
  hosted/freestanding runtime paths, consume the runtime public header from
  C++17, and run the C-library and opaque-handle C++ shim fixtures. Auto mode
  may skip an unavailable optional compiler family only with an explicit `SKIP`
  reason and must run at least one complete C/C++ pair. CI passes `--toolchain
  gcc` and `--toolchain clang` in separate required lanes; either missing,
  mislabeled, unrun, warning-producing, or failing lane is a hard failure.
- Direct Haxe/Reflaxe builds activate through
  `--custom-target c=<directory>` and expose the public `c` target conditional.
  The compiler owns `target.name=c`; use `target.unicode` without
  `target.utf16`, and set `target.sys`,
  `target.threaded`, or `target.atomics` only from proven adapter capabilities.
  Do not use `reflaxe_c` as the recommended application portability condition.
- A platform support claim names the whole environment/runtime/architecture/
  compiler/capability tuple. Do not report a cross-compile as a native or
  emulated run, a capability-limited freestanding lane as hosted stdlib support,
  or a Tier 2/3 combination as part of the ADR 0007 Tier 1 release gate.
- Idiomatic, warning-clean C is required in every profile; it is not a third
  profile or a correctness-off mode.
- Clang-derived facts and compiled probes are authoritative for bindgen and
  ABI layout. AI suggestions may improve names/docs but never invent ABI facts.
- Arbitrary C++ APIs require an existing or generated `extern "C"` shim. Do
  not represent C++ classes, templates, overloads, exceptions, or ABI details
  as if they were ordinary C declarations.

### License and provenance invariants

- Repository-owned source is `GPL-3.0-only`. Treat
  `docs/specs/third-party-provenance.json` as the machine-readable inventory
  and `THIRD_PARTY_NOTICES.md` as its human-readable explanation; update both
  in the same change whenever distribution facts change.
- An external compiler, framework, or analysis tool is not redistributed merely
  because it is used during development. Conversely, never label copied source
  as “external only.” Distribution status must match the checkout and package.
- Do not add files under `vendor/`, `third_party/`, `runtime/vendor/`, or
  `runtime/third_party/` without an owning manifest entry, immutable source
  revision, concrete SPDX license expression, exact governed paths, and local
  license evidence. `runtimeDependencies: []` is an audited statement that no
  third-party runtime dependency is present.
- Every file under `std/c/_std/` must have a derived-source ledger entry with a
  content hash. Copied or adapted Haxe standard-library source records its exact
  upstream path/revision, MIT license, and retained notice; a genuinely
  repository-authored override is classified separately as `GPL-3.0-only`.
- Run `python3 scripts/ci/check_license_policy.py` after any dependency,
  vendoring, `_std`, runtime, license, notice, or package-layout change. Release
  assembly must additionally run it with `--package-root <assembled-tree>` so
  reviewed license and notice files are present byte-for-byte.
- Do not infer or promise a license for generated C, generated headers,
  templates, or selectively emitted/linked runtime slices from the repository
  license. That public treatment is owned by Beads decision `haxe_c-od2.5` and
  remains a release-packaging blocker until ratified.

### Beads plan execution

- The durable product source is `docs/PRD.md`; live ownership/readiness/status
  belongs only in Beads. Do not maintain a parallel Markdown task checklist.
- Seed issues retain `E0`...`E10` / `E<n>.T<nn>` in metadata and use
  `hxc-plan:<stable-key>` as the external reference. Search/reconcile these
  before creating plan work again.
- Parent-child hierarchy is structural and does not block siblings. A hard
  edge is always `child needs blocker`: `bd dep add <child> <blocker>`.
- With the installed Beads version, `bd ready` also lists unblocked epics.
  Use `bd list --ready --type task --json` to select actionable seed work.
  The intended first seed task is `E0.T01`.
- Claim one primary actionable task. Read its parent, blockers, requirement
  IDs, PRD references, acceptance criteria, compatibility impact, and
  documentation obligations before editing.
- Record discoveries with a specific parent and
  `--deps discovered-from:<source-id>`. Add a blocking edge only when the
  discovery genuinely prevents acceptance.
- Close an implementation issue only after positive/negative fixtures,
  IR/AST or snapshot evidence, strict native compilation, runtime/differential
  behavior, policy/runtime-plan assertions, sanitizer/ABI checks where
  relevant, and documentation/ledger updates required by PRD Section 40.
- Close reasons must name requirement IDs, exact commands, and produced
  artifacts. “Implemented” or “tests pass” alone is not sufficient evidence.
- Never use `bd init --force`, hand-edit `.beads`/Dolt internals, or treat
  `.beads/issues.jsonl` as the live source of truth.
- Do not use `bd import` for normal synchronization. Use `bd dolt pull/push`
  only when a Dolt remote is actually configured and policy authorizes it.

#### Local Beads 1.0.4 cautions

- `bd create --graph ... --dry-run` is not a safe preview in the installed
  1.0.4 CLI: it writes graph nodes. Do not use it. Validate plan JSON with
  read-only tooling and use the accepted idempotent materializer when one is
  present.
- `bd preflight` currently prints the Beads tool repository's Go checks, not
  this Haxe/C project's quality gates. Do not report it as project validation.

### Validation and evidence

Run the exact gates owned by the active issue. At the present partial-scaffold
stage, the locally available baseline is:

```bash
npm ci
npm test
npm run test:fixture-policy
npm run snapshots:check
jq empty docs/specs/*.json .haxerc haxelib.json package.json package-lock.json
bd dep cycles --json
bd lint --json
bash scripts/lint/whitespace_guard.sh
```

`npm test` verifies the exact dependency lock, vendored Reflaxe checksum, the
complete current target-owned Haxe graph and explicit macro branches,
source/package lifecycle behavior, cold/compiler-server C/non-C isolation,
macro order, typed-AST normalization/order/server isolation, the structural
declarator and expression/statement goldens, the
declaration/include plan and header goldens, the canonical HxcIR semantic and
negative-diagnostic goldens, the available local strict native smoke lanes,
registered snapshot reproduction, fixture/example ownership, notices, and
governance policy.
GitHub CI additionally requires distinct GCC/G++ and Clang/Clang++ lanes. The
AST and declaration-header goldens are generated by Haxe code that directly
constructs target-owned structures; they prove C syntax and dependency
planning, not Haxe-language lowering or production generated-program success.

After cloning, run `scripts/hooks/install.sh`. The tracked pre-commit chain
keeps `.beads/hooks` as `core.hooksPath` so Beads checkout/merge/push hooks
remain active while repository checks run first. It exports/stages Beads JSONL,
formats repository-owned staged Haxe, preserves immutable vendor bytes, rejects
staged secrets and machine-local paths, checks whitespace, verifies dependency
checksums, enforces the license/provenance and fixture/snapshot inventories, and
runs relevant JSON, Haxe, or strict native gates. Gitleaks is required; the
formatter haxelib is required when repository-owned Haxe files are staged. Do
not bypass the hook to publish a failing change; record and fix the underlying
gate instead.

Claim Haxe/Reflaxe source-graph type-checking only with the dedicated all-source
gate. Do not claim generated-program runtime linking/execution, sanitizers,
cross-platform CI, bindgen, export, or stdlib parity until the responsible issue
adds a real command and that command passes.

### Git and remote completion

Preserve unrelated user changes and untracked scaffold files. Stage and commit
only the paths in the active issue unless the user explicitly broadens scope.

Before session close, inspect `git remote -v`:

- If a Git remote and the corresponding Beads remote are configured and the
  workflow authorizes publishing, rebase safely, push Git, push Beads with
  `bd dolt push`, and verify both.
- If no remote is configured, do not invent one and do not claim a push. Commit
  the in-scope tracked changes locally, verify the worktree/Beads state, and
  state explicitly in the handoff that synchronization was unavailable.

<!-- BEGIN BEADS INTEGRATION v:1 profile:minimal hash:7510c1e2 -->
## Beads Issue Tracker

This project uses **bd (beads)** for issue tracking. Run `bd prime` to see full workflow context and commands.

### Quick Reference

```bash
bd ready              # Find available work
bd show <id>          # View issue details
bd update <id> --claim  # Claim work
bd close <id>         # Complete work
```

### Rules

- Use `bd` for ALL task tracking — do NOT use TodoWrite, TaskCreate, or markdown TODO lists
- Run `bd prime` for detailed command reference and session close protocol
- Use `bd remember` for persistent knowledge — do NOT use MEMORY.md files

**Architecture in one line:** issues live in a local Dolt DB; sync uses `refs/dolt/data` on your git remote; `.beads/issues.jsonl` is a passive export. See https://github.com/gastownhall/beads/blob/main/docs/SYNC_CONCEPTS.md for details and anti-patterns.

## Session Completion

**When ending a work session**, complete all applicable steps below. A push is
mandatory only when the relevant remote is configured and publishing is
authorized; otherwise follow the repository-specific remote rule above.

**MANDATORY WORKFLOW:**

1. **File issues for remaining work** - Create issues for anything that needs follow-up
2. **Run quality gates** (if code changed) - Tests, linters, builds
3. **Update issue status** - Close finished work, update in-progress items
4. **PUSH TO REMOTE WHEN CONFIGURED AND AUTHORIZED**:
   ```bash
   git pull --rebase
   git push
   git status  # MUST show "up to date with origin"
   ```
5. **Clean up** - Clear stashes, prune remote branches
6. **Verify** - All changes committed and, when a remote is configured, pushed
7. **Hand off** - Provide context for next session

**CRITICAL RULES:**
- When a configured workflow requires a push, work is not complete until it
  succeeds; resolve failures and retry.
- When no remote exists, do not create one or imply synchronization occurred.
- Never say "ready to push when you are" when the existing workflow already
  authorizes and requires the agent to push.
<!-- END BEADS INTEGRATION -->
