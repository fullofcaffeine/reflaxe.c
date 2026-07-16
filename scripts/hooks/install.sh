#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"

if [ ! -x "$ROOT_DIR/.beads/hooks/pre-commit" ] || [ ! -x "$ROOT_DIR/.beads/hooks/pre-push" ]; then
  echo "[hooks] ERROR: expected tracked Beads pre-commit and pre-push hooks." >&2
  exit 1
fi

chmod +x \
  "$ROOT_DIR/scripts/hooks/pre-commit" \
  "$ROOT_DIR/scripts/hooks/pre-push" \
  "$ROOT_DIR/scripts/hooks/install.sh" \
  "$ROOT_DIR/scripts/beads/push-safe.sh" \
  "$ROOT_DIR/scripts/lint/hx_format_guard.sh" \
  "$ROOT_DIR/scripts/lint/local_path_guard_staged.sh" \
  "$ROOT_DIR/scripts/security/run-beads-gitleaks.sh" \
  "$ROOT_DIR/scripts/security/run-gitleaks.sh"

git -C "$ROOT_DIR" config core.hooksPath .beads/hooks

echo "[hooks] Installed repository hooks through .beads/hooks."
echo "[hooks] Pre-commit checks staged content; pre-push scans full Git history."
echo "[hooks] Use npm run beads:push so decoded Dolt history is scanned before sync."
echo "[hooks] Required: gitleaks and Haxe formatter 1.18.0."
echo "[hooks] Install formatter with: haxelib install formatter 1.18.0"
