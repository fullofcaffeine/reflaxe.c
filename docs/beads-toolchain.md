# Beads toolchain and shared database

Beads stores this repository's issue graph in Dolt, a versioned database. The
database is shared through the Git remote's `refs/dolt/data` ref. This is
separate from ordinary source commits on `refs/heads/main`.

The reviewed client is the stock **Beads 1.1.2** release, using database
schema v53. The hook installer checks that
Beads 1.1.2 is on `PATH`; after setup, the tracked hooks and scripts call the
stock `bd` command directly.

`bd hooks list` currently labels the installed hook template as version
`1.1.0`, even when stock Beads 1.1.2 installs it. That is the hook-template
format, not the client version; `Outdated: false` is the relevant health check.

## Why the version check is strict

`.beads/issues.jsonl` is a Git-review copy of the live issue records. It is not
the database and it is not the synchronization protocol. Even so, changing its
ordering or shape changes the staged Git tree. The local compiler test runner
uses that exact tree as part of a **resume receipt**: a small record proving
that one exact test shard already passed. An incompatible Beads exporter once
rewrote the JSONL file and made three still-valid receipts appear stale, so the
same expensive tests started again.

The strict check prevents that chain of events:

1. resolve and verify Beads 1.1.2;
2. confirm the local database is available;
3. export to temporary files beside the destination;
4. replace and stage `.beads/issues.jsonl` only after export succeeds.

If version discovery or export fails, the existing JSONL bytes and Git index
remain unchanged.

## The 1.0.4 to 1.1.0 migration

On 2026-07-21, this checkout was deliberately selected as the only migrator for
the shared database. It saved the complete schema-v32 embedded Dolt directory
and a 213-issue JSONL recovery copy, applied all 21 migrations to schema v53,
and compared the old and new issue records. After sorting relationship arrays
and ignoring migration-refreshed `updated_at` fields, the semantic records were
identical. The migrated history was then published through the guarded push.

Only one clone may migrate a remote-backed Dolt schema. Two clones applying the
same migrations independently create different database histories that cannot
be merged safely. Therefore:

- do not set `BD_ALLOW_REMOTE_MIGRATE` during normal development;
- do not run `bd migrate` merely because an old local clone reports schema
  drift;
- update to the reviewed Beads client, save any unpushed local issue work, and
  run `bd bootstrap` to adopt the already-migrated remote database; and
- coordinate a future schema upgrade as a separate, single-migrator operation
  with a backup, semantic comparison, guarded push, and tests.

`bd bootstrap` replaces the local database. Export or publish genuinely local
issue work first; otherwise it will be lost.

## Normal commands

```sh
# Verify the installed stock client.
bd --version

# Install the tracked hook chain after cloning.
scripts/hooks/install.sh

# Work with issues.
bd prime
bd ready

# Scan the complete current decoded export, then publish Dolt history.
npm run beads:push
```

Never call `bd dolt push` directly. The wrapper scans the complete current
decoded export because a normal Git secret scanner sees only opaque Dolt
storage chunks. Git hooks already scan staged source and reachable Git history;
the Beads wrapper adds the current database records those scans cannot decode.
A retrospective audit of every old database row is deliberately not part of
every push.

Some pre-migration history rows contain SQL `NULL` in old text fields, and
Beads 1.1.2 may report `converting NULL to string is unsupported` when
`bd history` reaches them. This is an upstream history-display limitation, not
database corruption: current reads, updates, deterministic exports, dependency
queries, and Dolt synchronization remain the health criteria used here. Do not
rewrite or normalize the database to make old history displayable.

## Recovery messages

When setup says the required client is missing, install stock Beads 1.1.2 and
rerun `scripts/hooks/install.sh`. When the database is unavailable after an
upgrade, follow the adoption steps above rather than initializing or migrating
another database in place.
