#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"
BD_BIN="$(python3 "$ROOT_DIR/scripts/beads/resolve-reviewed.py")"

echo "[beads-push] Scanning decoded Beads state and history before publication..."
HXC_BD_BIN="$BD_BIN" bash "$ROOT_DIR/scripts/security/run-beads-gitleaks.sh"

echo "[beads-push] Pushing the audited Dolt history..."
"$BD_BIN" -C "$ROOT_DIR" dolt push "$@"
