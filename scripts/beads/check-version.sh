#!/usr/bin/env bash
# Check the selected stock client before any command opens the database.
set -euo pipefail

if ! command -v bd >/dev/null 2>&1; then
  echo "[beads] Stock Beads 1.1.2 is required on PATH." >&2
  exit 1
fi
if ! identity="$(bd --version)"; then
  echo "[beads] Could not read the version of bd on PATH. Install stock Beads 1.1.2." >&2
  exit 1
fi
case "$identity" in
  "bd version 1.1.2 "*) ;;
  *)
    echo "[beads] Stock Beads 1.1.2 is required. Found $identity." >&2
    exit 1
    ;;
esac
