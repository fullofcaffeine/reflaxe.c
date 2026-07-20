# Project Instructions for AI Agents

This file provides instructions and context for AI coding agents working on this project.

`AGENTS.md` is the canonical complete project contract. Read and follow it
before changing compiler, runtime, target API, ABI, build, examples, tests, or
documentation. The flagship rule below is mirrored here because it directly
governs application-driven compiler work.

## Flagship End-to-End Feedback Loop

Treat realistic applications as active compiler-development instruments, not
as demos that are polished only after the compiler is finished. Caxecraft is
the current flagship application and primary product-level end-to-end (E2E)
quality-assurance workload for haxe.c. It should continually exercise ordinary
Haxe, typed `c.*` interop, generated-C quality, runtime selection, native tools,
performance, packaging, and developer experience together.

- When a flagship workload breaks or exposes weak output, first reproduce and
  classify the problem: application design, binding, compiler semantics,
  generated-C structure/readability, runtime/ABI, build tooling, or test cost.
  Do not hide a compiler defect with example-only raw C, copied logic, a looser
  assertion, or a Caxecraft name/path special case.
- Reduce each compiler defect to the smallest reusable positive, negative,
  semantic, structural, or native fixture that explains it. Fix it in the
  narrowest owning compiler/runtime/tooling layer, then retain both that fast
  focused regression and the flagship E2E path that originally exposed it.
  The focused test diagnoses the rule; the flagship proves the layers work
  together.
- Apply the same loop to improvements, not only failures. Measure readability,
  output shape, compile time, runtime cost, allocations, code size, diagnostics,
  and workflow friction where relevant. Turn a broadly useful improvement into
  a compiler capability rather than local game scaffolding.
- Preserve the accepted architecture and policy hierarchy while improving the
  workload: typed source and plans, HxcIR where C semantic gaps require it,
  structural CAST, explicit ownership/runtime decisions, formatting-only
  printing, fail-closed unsupported behavior, and ordinary Haxe plus narrow
  typed C adapters at the application edge.
- A bounded discovery that is required for the active issue's acceptance may
  be fixed with that issue. A material scope expansion gets a durable Beads
  owner and dependency instead of silently turning the active task into a
  rewrite. Prioritize discoveries that block correctness, safety, the playable
  path, or trustworthy QA.
- If a critical correctness or safety issue appears to require violating an
  accepted architectural decision, stop before implementing the deviation.
  Record the minimal reproducer, observed evidence, affected invariants,
  plausible options, migration cost, and regression gates; check the governing
  PRD/ADR; and ask the maintainer for an explicit decision. When the decision is
  unusually broad, irreversible, or uncertain, prepare that evidence as a
  focused deep-research brief for the designated Oracle model rather than
  improvising a new architecture.
- Keep claims evidence-bounded. Caxecraft is the primary integrated proof, not
  permission to infer general language support from one game. Conversely, a
  focused compiler fixture passing is not enough to claim the flagship is
  playable, attractive, fast, or portable.

The long-term intent is a constructive flywheel: Caxecraft finds real pressure,
haxe.c gains a reusable principled capability, focused tests make the fix fast
to maintain, and Caxecraft keeps the complete user journey honest.

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

**When ending a work session**, you MUST complete ALL steps below. Work is NOT complete until `git push` succeeds.

**MANDATORY WORKFLOW:**

1. **File issues for remaining work** - Create issues for anything that needs follow-up
2. **Run quality gates** (if code changed) - Tests, linters, builds
3. **Update issue status** - Close finished work, update in-progress items
4. **PUSH TO REMOTE** - This is MANDATORY:
   ```bash
   git pull --rebase
   git push
   git status  # MUST show "up to date with origin"
   ```
5. **Clean up** - Clear stashes, prune remote branches
6. **Verify** - All changes committed AND pushed
7. **Hand off** - Provide context for next session

**CRITICAL RULES:**
- Work is NOT complete until `git push` succeeds
- NEVER stop before pushing - that leaves work stranded locally
- NEVER say "ready to push when you are" - YOU must push
- If push fails, resolve and retry until it succeeds
<!-- END BEADS INTEGRATION -->


## Build & Test

_Add your build and test commands here_

```bash
# Example:
# npm install
# npm test
```

## Architecture Overview

_Add a brief overview of your project architecture_

## Conventions & Patterns

_Add your project-specific conventions here_
