#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"

echo "[beads-push] Scanning decoded Beads state and history before publication..."
bash "$ROOT_DIR/scripts/security/run-beads-gitleaks.sh"

echo "[beads-push] Pushing the audited Dolt history..."
bd -C "$ROOT_DIR" dolt push "$@"
