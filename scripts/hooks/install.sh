#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"

if [ ! -x "$ROOT_DIR/.beads/hooks/pre-commit" ]; then
  echo "[hooks] ERROR: expected tracked Beads hook at .beads/hooks/pre-commit." >&2
  exit 1
fi

chmod +x "$ROOT_DIR/scripts/hooks/pre-commit" "$ROOT_DIR/scripts/hooks/install.sh" "$ROOT_DIR/scripts/lint/local_path_guard_staged.sh" "$ROOT_DIR/scripts/security/run-gitleaks.sh"

git -C "$ROOT_DIR" config core.hooksPath .beads/hooks

echo "[hooks] Installed repository hooks through .beads/hooks."
echo "[hooks] Required: gitleaks. Staged Haxe also requires haxelib formatter."
echo "[hooks] Install formatter with: haxelib install formatter"
