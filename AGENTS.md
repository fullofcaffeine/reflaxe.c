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
3. `docs/architecture.md` for compiler-layer invariants;
4. `docs/research/repo-patterns.md` for sibling-target and hxcpp precedent;
5. `docs/BEADS_PLAN.md` and the active issue from
   `docs/specs/beads-plan.json`;
6. every existing source/runtime file in the area being changed.

An accepted ADR that explicitly supersedes the PRD has highest authority.
Otherwise use this order: PRD, architecture/configuration docs, active Beads
acceptance criteria, scaffold code, then examples/comments.

### Trust the checkout, not the aspirational inventory

The current checkout is a deliberately partial slice of the fuller scaffold
described by PRD Section 32. Verify files with `rg --files` before citing or
running them. In particular, this checkout currently lacks several documented
assets, including `CODEX_HANDOFF.md`, `scripts/`, schemas, ADRs, examples,
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
- Public C ABI types are separate from internal object/runtime layouts. Every
  exported pointer, string, buffer, handle, callback, error, and allocator
  boundary needs explicit layout, ownership, nullability, lifetime, calling
  convention, and failure policy. No exception may cross the C ABI.

### Product contracts

- One compiler pipeline serves both profiles. `portable` is the default Haxe
  semantic contract; `metal` exposes explicit C layout/ownership constraints.
- Runtime policy (`auto|minimal|none`) and environment
  (`hosted|freestanding|wasi|emscripten`) are orthogonal to profile.
- Strict hosted C11 with no extensions is the default until an accepted ADR or
  issue explicitly selects another dialect/environment.
- Idiomatic, warning-clean C is required in every profile; it is not a third
  profile or a correctness-off mode.
- Clang-derived facts and compiled probes are authoritative for bindgen and
  ABI layout. AI suggestions may improve names/docs but never invent ABI facts.
- Arbitrary C++ APIs require an existing or generated `extern "C"` shim. Do
  not represent C++ classes, templates, overloads, exceptions, or ABI details
  as if they were ordinary C declarations.

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
stage, the locally available baseline is limited to:

```bash
jq empty docs/specs/beads-plan.json docs/specs/diagnostics.json docs/specs/stdlib-ledger.json
cc -std=c11 -Wall -Wextra -Werror -pedantic -fsyntax-only -x c-header runtime/hxrt/include/hxc_runtime.h
c++ -std=c++17 -Wall -Wextra -Werror -pedantic -fsyntax-only -x c++-header runtime/hxrt/include/hxc_runtime.h
bd dep cycles --json
bd lint --json
git diff --check
```

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
