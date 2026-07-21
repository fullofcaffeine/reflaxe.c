#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"
BD_BIN="$(python3 "$ROOT_DIR/scripts/beads/resolve-reviewed.py")"

if ! "$BD_BIN" -C "$ROOT_DIR" context >/dev/null 2>&1; then
  echo "[beads-export] ERROR: the repository Beads database is unavailable." >&2
  echo "[beads-export] Pull or bootstrap the migrated database before committing." >&2
  exit 1
fi

BEADS_EXPORT_TMP="$(mktemp "$ROOT_DIR/.beads/issues.jsonl.export.XXXXXX")"
BEADS_EXPORT_NORMALIZED="$(mktemp "$ROOT_DIR/.beads/issues.jsonl.normalized.XXXXXX")"
trap 'rm -f "$BEADS_EXPORT_TMP" "$BEADS_EXPORT_NORMALIZED"' EXIT

"$BD_BIN" -C "$ROOT_DIR" export > "$BEADS_EXPORT_TMP"
# JSON Lines (JSONL) stores one JSON object per non-empty line. Keeping that
# small normalization makes the passive Git review copy stable across tools.
awk 'NF { print }' "$BEADS_EXPORT_TMP" > "$BEADS_EXPORT_NORMALIZED"
mv -f "$BEADS_EXPORT_NORMALIZED" "$ROOT_DIR/.beads/issues.jsonl"
git -C "$ROOT_DIR" add -- .beads/issues.jsonl

rm -f "$BEADS_EXPORT_TMP"
trap - EXIT
