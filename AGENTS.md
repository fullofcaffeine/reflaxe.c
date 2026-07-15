# Agent instructions

This repository uses Beads for all implementation work. The PRD explains the product; Beads is the live dependency graph.

## Session start

```text
bd prime
bd ready --json
```

Claim one primary issue atomically:

```text
bd update <id> --claim
```

Read the issue, its parent epic, dependencies, relevant PRD section, and architecture decision records before changing code.

## Non-negotiable compiler rules

- Preserve `CompilerBootstrap` and `CompilerInit` as separate phases.
- Keep `extraParams.hxml` current-working-directory agnostic.
- Keep mutable state in `CompilationContext`; reset it for every build.
- Lower through typed IR/AST nodes. Do not concatenate raw C as the normal path.
- Use Reflaxe `OutputManager` for generated files.
- Fail closed on unsupported Haxe semantics.
- Preserve evaluation order explicitly; never rely on unspecified C ordering.
- Do not introduce signed-overflow, aliasing, alignment, lifetime, or out-of-bounds undefined behavior.
- Keep examples, templates, and snapshots free of raw `__c__` except approved negative/compiler fixtures.
- Runtime helpers must have stable feature IDs and source reasons.
- Public C ABI pointers require ownership and nullability policy.
- C++ APIs require an existing or generated C shim; do not reinterpret C++ declarations as C.

## Definition of done

A feature requires:

- positive Haxe source fixture;
- negative diagnostic fixture where relevant;
- IR/AST or snapshot coverage;
- warning-clean C compilation;
- runtime/differential test;
- profile and runtime-policy coverage;
- sanitizer coverage when eligible;
- documentation/ledger updates;
- closed Beads acceptance criteria.

Create discovered work as a linked Beads issue. Do not leave unowned TODO comments.

## Beads completion

```text
bd close <id> --reason "Acceptance criteria satisfied; tests: <commands>"
bd dolt push
```

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
