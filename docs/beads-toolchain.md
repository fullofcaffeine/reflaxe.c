# Beads toolchain and shared database

Beads stores this repository's issue graph in Dolt, a versioned database. The
database is shared through the Git remote's `refs/dolt/data` ref. This is
separate from ordinary source commits on `refs/heads/main`.

The reviewed client is **Beads 1.1.0 at revision `8e4e59d39`**, using database
schema v53. Repository hooks check both values before they ask Beads to read or
write anything. `scripts/beads/resolve-reviewed.py` searches every `bd` found
on `PATH`, so an unrelated older or newer installation earlier on `PATH` cannot
silently change the passive export. Set `HXC_BD_BIN` to an exact reviewed
executable when it is installed outside the normal search path.

## Why the version check is strict

`.beads/issues.jsonl` is a Git-review copy of the live issue records. It is not
the database and it is not the synchronization protocol. Even so, changing its
ordering or shape changes the staged Git tree. The local compiler test runner
uses that exact tree as part of a **resume receipt**: a small record proving
that one exact test shard already passed. An incompatible Beads exporter once
rewrote the JSONL file and made three still-valid receipts appear stale, so the
same expensive tests started again.

The strict check prevents that chain of events:

1. resolve and verify Beads 1.1.0;
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
# Verify which reviewed executable the repository will use.
python3 scripts/beads/resolve-reviewed.py

# Install the tracked hook chain after cloning.
scripts/hooks/install.sh

# Work with issues.
bd prime
bd ready

# Scan decoded current and historical records, then publish Dolt history.
npm run beads:push
```

Never call `bd dolt push` directly. The wrapper scans decoded records because a
normal Git secret scanner sees only opaque Dolt storage chunks. Do not use the
passive JSONL file as a replacement database or run `bd import` during ordinary
synchronization.

## Recovery messages

When a hook says no reviewed client was found, install Beads 1.1.0 from revision
`8e4e59d39` or point `HXC_BD_BIN` to that build. When it says the database is
unavailable after upgrading the client, follow the adoption steps above rather
than initializing or migrating another database in place.
