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
