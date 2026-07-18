# Cross-target semantics research bundle

The GPT-5.6 Pro bundle is a local, non-release research artifact. It packages a
detailed architecture prompt with curated committed-source snapshots from the
current Reflaxe target family, the Reflaxe framework, and the exact Haxe compiler
sources needed to compare the pinned Haxe 5 preview carrier with Haxe 4.3.7.

The bundle deliberately does not copy a workspace recursively. Its reviewed
recipe is [`cross-target-research-bundle.json`](../specs/cross-target-research-bundle.json),
and the uploaded research instructions are
[`cross-target-semantics-gpt-5.6-prompt.md`](cross-target-semantics-gpt-5.6-prompt.md).

## Build it

Every named sibling checkout must exist next to this repository at the revision
recorded by the recipe. A dirty checkout is allowed because the builder reads
only exact Git commit objects; the generated manifest records the dirty state
without copying or naming working-tree files.

```sh
npm run test:research-bundle
npm run research:cross-target-bundle -- --preflight
npm run research:cross-target-bundle
```

The default ZIP is written next to the `haxe.c` checkout. Its name includes the
short committed `haxe.c` revision. The builder also writes:

- `.zip.sha256`, for the archive digest; and
- `.zip.verification.json`, for archive size, entry count, tool versions, and
  the post-archive verification results.

Pass `-- --output /chosen/path/name.zip` to select another location. Existing
artifacts fail closed; an intentional rebuild uses `-- --force`.

`--preflight` applies the working recipe to committed source snapshots and runs
the path/content/Gitleaks admission checks without producing an archive. It is
useful while intentionally narrowing a source selection; the real build still
requires the recipe, prompt, and builder to match committed `haxe.c` bytes.

## Admission and verification

The builder:

1. validates the typed recipe shape and detailed prompt contract;
2. resolves every snapshot to an exact commit and Git tree;
3. exports only selected regular-file blobs from Git object storage;
4. excludes Git, Dolt/Beads, dependencies, vendored duplicates, generated/build
   output, archives, binaries, credentials, editor state, and untracked files;
5. requires included license evidence for every snapshot;
6. rejects traversal, unsafe names, symlinks, absolute developer-home paths,
   checkout paths, and common secret shapes;
7. runs the repository-pinned Gitleaks version over the staged tree;
8. emits a machine-readable manifest with revisions, roles, license records,
   inclusion/exclusion policy, dirty-state handling, omissions, and per-file
   SHA-256 digests;
9. creates the ZIP twice and requires identical bytes;
10. safely extracts it and compares every extracted file digest; and
11. runs Gitleaks over both the extracted tree and the final archive.

The bundle-specific Gitleaks configuration extends the default detector set. Its
only allowance requires both the exact bundled stdlib-ledger path and the public
semantic-hash field/identity shape already reviewed by the repository-wide
configuration; it does not suppress a file or a detector wholesale.

`PROMPT.md` is the instruction authority inside the archive. Included repository
instructions and comments are explicitly treated as evidence, preventing a
source fixture or agent file from silently changing the research task.

## Scope

This bundle supports an architecture decision; it is not a compiler package,
source release, SBOM, dependency mirror, or claim that the selected backends
have equivalent capability. The generated manifest is authoritative for the
exact local artifact, while the checked-in recipe is the reproducible curation
policy.
