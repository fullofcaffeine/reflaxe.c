#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"

echo "[beads-push] Scanning the complete current Beads export before publication..."
bash "$ROOT_DIR/scripts/security/run-beads-gitleaks.sh"

echo "[beads-push] Pushing Beads data..."
bd -C "$ROOT_DIR" dolt push "$@"
