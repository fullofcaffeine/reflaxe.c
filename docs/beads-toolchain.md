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

## Nullable-history recovery reader

The reviewed Beads 1.1.0 build has one upstream read bug: `bd history` scans
old text columns into non-null Go strings, but 46 pre-migration revisions
legitimately contain SQL `NULL`. The command stops at the first such revision.
That is not evidence of database damage, and the publication guard must not
skip the unreadable rows. Upstream
[issue 4867](https://github.com/gastownhall/beads/issues/4867) and
[pull request 4912](https://github.com/gastownhall/beads/pull/4912) own the
Beads fix. Repository issue `haxe_c-od2.10` owns this recovery path and its
removal once a fixed Beads build becomes the reviewed pin.

Until then, the guard admits one read-only Dolt command-line build whose Go
module revision and checksum exactly match the Dolt library embedded in the
reviewed Beads binary. Build it from the pinned module:

```sh
HXC_DOLT_TOOL_DIR="$PWD/.cache/tools/dolt-2.1.4"
mkdir -p "$HXC_DOLT_TOOL_DIR"

# CGO builds need the ICU compiler and linker flags. On Homebrew systems,
# pkg-config obtains them from the installed icu4c package.
CGO_CPPFLAGS="$(pkg-config --cflags icu-uc icu-i18n)" \
CGO_LDFLAGS="$(pkg-config --libs icu-uc icu-i18n)" \
GOBIN="$HXC_DOLT_TOOL_DIR" \
  go install github.com/dolthub/dolt/go/cmd/dolt@v0.40.5-0.20260605230755-1bf533220ab0

HXC_DOLT_BIN="$HXC_DOLT_TOOL_DIR/dolt" npm run beads:push
```

`resolve-reviewed-dolt.py` checks the executable's user-facing version, Go
package path, exact module revision, and module checksum. A different binary
that merely prints “2.1.4” is rejected.

This recovery does not edit, normalize, compact, or replace the database. It
feeds Gitleaks three decoded sources:

1. `bd export --all`, covering the complete current Beads records;
2. `dolt_log`, covering database commit metadata; and
3. each `dolt_diff_<table>` system table, whose `from_*` and `to_*` columns
   contain every changed row throughout the current branch's history.

The scanner discovers every base table, validates each table name before
constructing the fixed `SELECT`, and fails before publication if any query,
decode, provenance check, or Gitleaks scan fails. The reviewed `bd` binary
still performs the eventual push. The standalone Dolt binary has read
authority only.

## Recovery messages

When a hook says no reviewed client was found, install Beads 1.1.0 from revision
`8e4e59d39` or point `HXC_BD_BIN` to that build. When it says the database is
unavailable after upgrading the client, follow the adoption steps above rather
than initializing or migrating another database in place. When `bd history`
reports a legacy `NULL` conversion failure, use the exact recovery reader above;
do not bypass `push-safe.sh`, skip the affected issue, or rewrite history.
