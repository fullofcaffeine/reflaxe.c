# Contributing to reflaxe.c

Thank you for helping build `reflaxe.c`. This repository is still a brownfield
M0 compiler scaffold, not a complete Haxe-to-C product. Keep capability claims
limited to executable evidence, and read [AGENTS.md](AGENTS.md) before changing
compiler, runtime, target, ABI, or build behavior.

Security vulnerabilities follow the private process in
[SECURITY.md](SECURITY.md). Do not put vulnerability details in a GitHub issue,
pull request, discussion, Beads record, CI log, or generated artifact.

## Before you start

Install the pinned development dependencies and the tracked Git hooks:

```sh
npm ci
haxelib install formatter
scripts/hooks/install.sh
bd prime
```

Read the active issue, its PRD references, relevant accepted ADRs, and every
existing file in the area you will change. The checkout is deliberately
partial, so verify paths with `rg --files` and do not turn aspirational
inventory entries into capability claims.

## Work through Beads

The PRD records product intent; Beads records live ownership and execution
state. `.beads/issues.jsonl` is a passive export, not the synchronization
protocol.

```sh
bd list --ready --type task --json
bd show <id> --json
bd update <id> --claim --json
```

- Claim one actionable primary issue at a time.
- Keep work inside its outcome and acceptance criteria. Record newly discovered
  work as a child issue with a `discovered-from` relationship instead of
  widening scope or leaving an unowned TODO.
- Parent-child relationships organize work but do not block it. Add a blocking
  edge only when the child genuinely needs the blocker.
- Never hand-edit Beads database internals, use `bd init --force`, or use
  `bd import` for routine synchronization.

## Make typed, reviewable changes

Use the Haxe type system to express invariants. `Reflect`, `Dynamic`, `Any`,
`untyped`, raw C injection, unchecked casts, and stringly typed protocols are
not substitutes for a sound model. Use a narrow, documented boundary only when
the language or an external ABI genuinely requires one, and test both admitted
and rejected inputs.

Preserve the architecture in [docs/architecture.md](docs/architecture.md):
semantic ordering and failure edges belong in typed HxcIR, C syntax belongs in
the structural C AST, generated names go through the symbol registry, and
unsupported behavior fails with a source-positioned diagnostic. Prefer direct,
idiomatic C and compiler-time specialization before selecting a narrow `hxrt`
feature.

## Definition of done

Every change identifies its requirement IDs and active Beads issue. Evidence is
proportional to the affected layer; a documentation-only policy does not invent
native behavior, while a lowering feature is not complete without its relevant
semantic and native proof.

Before asking for review or closing an issue:

- satisfy every acceptance criterion and the applicable PRD Section 40
  checklist;
- add positive and relevant negative fixtures, typed IR/AST or snapshot
  evidence, native compilation, runtime/differential behavior, policy checks,
  sanitizers, and ABI/layout consumers where applicable;
- assert profile, runtime-policy, environment, generated-C, and public-ABI
  impact instead of assuming they are unchanged;
- keep output deterministic and warning-clean, with unsupported scope failing
  closed;
- update affected documentation, diagnostics, manifests, provenance records,
  fixture registrations, and standard-library ledgers together;
- leave no unowned TODO, generated drift, secret, or machine-local path; and
- run the issue-owned commands, the relevant focused tests, `npm test`, and the
  tracked `scripts/hooks/pre-commit` gate.

A close reason must name the requirement IDs, exact commands and their results,
produced artifacts, remote CI evidence when required, compatibility impact, and
intentionally deferred scope. “Implemented” or “tests pass” is not sufficient.

## Generated files and snapshots

- Production compiler output is written only through Reflaxe generated-file
  ownership. Never delete stale output by scanning directories or bypass its
  manifest.
- Checked-in expected artifacts belong to the central snapshot registry. Check
  them with `npm run snapshots:check`; update them intentionally with a targeted
  command such as `npm run snapshots:update -- --suite hxc-ir`, inspect the
  semantic diff, and rerun the owning suite. CI and pre-commit never bless
  changes automatically.
- A checked-in generated artifact must have an owning source, deterministic
  regeneration command, reviewable diff, and applicable native/oracle proof.
  Do not hand-edit it to hide a generator regression.
- Do not commit build directories, compiler caches, temporary probes, or
  machine-specific absolute paths.

## Dependencies, vendoring, and provenance

The Haxe, Reflaxe, Lix, and package locks are one reviewed dependency contract.
Do not float or refresh one pin without its checksums, source revision, notices,
and lifecycle evidence.

- New files under `vendor/`, `third_party/`, `runtime/vendor/`, or
  `runtime/third_party/` require an immutable upstream revision, concrete SPDX
  expression, exact governed paths, local license evidence, and an entry in
  [docs/specs/third-party-provenance.json](docs/specs/third-party-provenance.json).
- Every copied or adapted Haxe `_std` file requires per-file upstream or
  repository-authored provenance and a content hash.
- Update [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) with the
  machine-readable inventory whenever distribution facts change.
- Run `python3 scripts/ci/check_license_policy.py`; release assembly must also
  run it with `--package-root <assembled-tree>`.
- Repository-owned source is `GPL-3.0-only`. Do not infer terms for generated
  C, headers, templates, or emitted runtime slices: decision `haxe_c-od2.5`
  remains a release blocker until ratified.

## Security-sensitive changes

Treat identifiers, source text, configuration, manifests, headers, paths,
native flags, and tool output as hostile inputs. Security review must consider
generated-code injection, malicious bindgen inputs, traversal and stale-file
deletion, shell or flag injection, allocation and cleanup failure, integer
overflow and C undefined behavior, ABI boundaries, and supply-chain
provenance. External commands use argument arrays rather than shell-built
strings. E10.T06 owns the complete compiler/runtime/interop threat model; do not
claim that later review from this contributor policy alone.

## Releases and signing

No publishable compiler release exists today. E10.T08 owns automated signed,
reproducible artifacts, and E10.T11 owns the evidence-backed beta. Until those
issues pass, development archives and package-layout probes must not be called
releases.

Release responsibilities are deliberately separated:

- The release manager owns the release Bead, approved version and tag, release
  notes/changelog, clean-checkout rehearsal, required platform evidence,
  rollback instructions, and final publication decision.
- Artifact builders produce byte-reproducible source and binary archives from
  the reviewed commit, with checksums plus source, runtime-feature, toolchain,
  dependency, license, and provenance manifests. They must not reuse an
  untracked local build.
- The provenance reviewer verifies manifest contents, third-party hashes,
  retained notices, generated/runtime licensing decisions, and clean-install
  examples before signing or publication.
- An owner-authorized signer signs the checksum/provenance set when signing is
  configured, keeps private key material outside the repository and logs, and
  records the public identity plus verification command. A missing or invalid
  configured signature blocks publication; an unconfigured signature must be
  recorded honestly and must never be described as signed.

No contributor may upload packages, create release tags, rotate signing keys,
or publish provenance on behalf of the project without the owning release issue
and explicit maintainer authority.

## Commit and handoff

Use a concise conventional-commit subject and a friendly body for every
non-trivial change. Explain the old problem, the practical new behavior, why it
matters, exact verification, and any deferred scope.

Maintainers complete work in this order:

1. Commit and push the implementation, then require the repository Governance
   workflow to pass.
2. Close the issue with `bd close <id> --reason "Requirements HXC-...; ..."`.
3. Synchronize execution state with `bd dolt push`.
4. Commit and push the passive Beads export/interaction record.
5. Verify a clean worktree and `0 0` from
   `git rev-list --left-right --count origin/main...HEAD`.

Contributors without publication authority provide the same evidence in their
pull request but leave final Beads closure, release actions, and remote
synchronization to a maintainer.
