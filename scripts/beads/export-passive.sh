#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"

bash "$ROOT_DIR/scripts/beads/check-version.sh"
if ! bd -C "$ROOT_DIR" --readonly info --json >/dev/null; then
  echo "[beads-export] Could not open the repository database for reading. See docs/beads-toolchain.md for recovery." >&2
  exit 1
fi

BEADS_EXPORT_TMP="$(mktemp "$ROOT_DIR/.beads/issues.jsonl.export.XXXXXX")"
BEADS_EXPORT_NORMALIZED="$(mktemp "$ROOT_DIR/.beads/issues.jsonl.normalized.XXXXXX")"
trap 'rm -f "$BEADS_EXPORT_TMP" "$BEADS_EXPORT_NORMALIZED"' EXIT

bd -C "$ROOT_DIR" --readonly export > "$BEADS_EXPORT_TMP"
# JSON Lines (JSONL) stores one JSON object per non-empty line. Keeping that
# small normalization makes the passive Git review copy stable across tools.
awk 'NF { print }' "$BEADS_EXPORT_TMP" > "$BEADS_EXPORT_NORMALIZED"
mv -f "$BEADS_EXPORT_NORMALIZED" "$ROOT_DIR/.beads/issues.jsonl"
git -C "$ROOT_DIR" add -- .beads/issues.jsonl

rm -f "$BEADS_EXPORT_TMP"
trap - EXIT
