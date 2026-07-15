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
assets, including `CODEX_HANDOFF.md`, some schemas, examples,
Reflaxe/Haxe package metadata, runtime implementation sources, and most
compiler lifecycle classes referenced by the adapter.

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
  concatenate user-derived C strings. Raw compiler-owned AST nodes remain
  narrow, validated escape valves.
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
- Only `c_output=<non-empty directory>` or real custom-target identity activates
  the compiler. A caller's `-D c` is not activation. On valid activation expose
  `c`, `reflaxe_c`, and `target.unicode`; reject `target.utf16` and do not invent
  other target capability defines.
- `reflaxe_c_lifecycle_probe` is an internal test seam that avoids entering the
  still-incomplete adapter during E0 bootstrap tests. Never document it as a
  user option or let it bypass a production compilation.
- The package-layout probe may flatten the pinned framework into a temporary
  classpath to prove installed resolution. It is not release assembly or
  permission to publish while `haxe_c-od2.5` remains unresolved.
- The future `hxc` command is an optional orchestration layer. Bootstrap its
  target-neutral core on Eval, later dogfood the C target for the native binary,
  and retain direct Haxe/HXML plus the C toolchain as the recovery build path.
  Compiler registration must never depend on an existing `hxc` executable.

### Typed C M0 seed

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
- Direct Haxe/Reflaxe builds activate through `c_output=<directory>` and expose
  the public `c` target conditional. Use `target.name=c` when available,
  `target.unicode` without `target.utf16`, and set `target.sys`,
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
jq empty docs/specs/*.json .haxerc haxelib.json package.json package-lock.json
bd dep cycles --json
bd lint --json
bash scripts/lint/whitespace_guard.sh
```

`npm test` verifies the exact dependency lock, vendored Reflaxe checksum,
source/package lifecycle behavior, cold/compiler-server C/non-C isolation,
macro order, notices, and governance policy. It does not type-check the
incomplete compiler adapter or claim generated-C/native-runtime success; those
remain later Beads gates.

After cloning, run `scripts/hooks/install.sh`. The tracked pre-commit chain
keeps `.beads/hooks` as `core.hooksPath` so Beads checkout/merge/push hooks
remain active while repository checks run first. It exports/stages Beads JSONL,
formats repository-owned staged Haxe, preserves immutable vendor bytes, rejects
staged secrets and machine-local paths, checks whitespace, verifies dependency
checksums, enforces the license/provenance inventory, and runs relevant JSON or
public-header gates. Gitleaks is required; the formatter haxelib is required when
repository-owned Haxe files are staged. Do not bypass the hook to publish a
failing change; record and fix the underlying gate instead.

Do not claim Haxe/Reflaxe type-checking, runtime linking, generated-program
execution, sanitizers, cross-platform CI, bindgen, export, or stdlib parity
until the responsible issue adds a real command and that command passes.

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
