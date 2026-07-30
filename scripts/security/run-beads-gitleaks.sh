#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"

for command_name in gitleaks; do
  if ! command -v "$command_name" >/dev/null 2>&1; then
    echo "[beads-gitleaks] ERROR: $command_name is required." >&2
    exit 1
  fi
done

echo "[beads-gitleaks] Scanning the complete current Beads export"

set +e
bd -C "$ROOT_DIR" export --all | gitleaks stdin \
  --redact \
  --config "$ROOT_DIR/.gitleaks.toml" \
  --no-banner
PIPELINE_STATUS=("${PIPESTATUS[@]}")
set -e

if [[ "${PIPELINE_STATUS[1]}" -ne 0 ]]; then
  exit "${PIPELINE_STATUS[1]}"
fi
if [[ "${PIPELINE_STATUS[0]}" -ne 0 ]]; then
  echo "[beads-gitleaks] ERROR: the current Beads export could not be read completely; no push was attempted." >&2
  exit "${PIPELINE_STATUS[0]}"
fi

echo "[beads-gitleaks] OK"
