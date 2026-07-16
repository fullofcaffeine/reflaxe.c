#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"
MODE="full"
readonly DOLT_REMOTE_REF="refs/dolt/data"
readonly DOLT_LOCAL_REF="refs/remotes/origin/dolt/data"

if [ "${1:-}" = "--staged" ]; then
  MODE="staged"
fi

if ! command -v gitleaks >/dev/null 2>&1; then
  echo "[gitleaks] ERROR: gitleaks is required but not installed." >&2
  echo "[gitleaks] Install: https://github.com/gitleaks/gitleaks#installing" >&2
  exit 1
fi

CONFIG_ARGS=(--config "$ROOT_DIR/.gitleaks.toml")
GITLEAKS_HELP="$(gitleaks --help 2>&1 || true)"

if [ "$MODE" = "staged" ]; then
  echo "[gitleaks] Scanning staged changes"
  if printf '%s' "$GITLEAKS_HELP" | grep -q '\<protect\>'; then
    (cd "$ROOT_DIR" && gitleaks protect --staged --redact "${CONFIG_ARGS[@]}")
  elif printf '%s' "$GITLEAKS_HELP" | grep -q '\<git\>'; then
    (cd "$ROOT_DIR" && gitleaks git --staged --redact "${CONFIG_ARGS[@]}")
  else
    echo "[gitleaks] ERROR: unsupported gitleaks CLI; expected 'protect' or 'git'." >&2
    exit 1
  fi
  exit 0
fi

if git -C "$ROOT_DIR" remote get-url origin >/dev/null 2>&1; then
  set +e
  GIT_TERMINAL_PROMPT=0 git -C "$ROOT_DIR" ls-remote \
    --exit-code --refs origin "$DOLT_REMOTE_REF" >/dev/null 2>&1
  DOLT_LOOKUP_STATUS=$?
  set -e
  case "$DOLT_LOOKUP_STATUS" in
    0)
      echo "[gitleaks] Fetching the remote Beads Dolt ref for audit"
      GIT_TERMINAL_PROMPT=0 git -C "$ROOT_DIR" fetch \
        --no-tags --force origin "$DOLT_REMOTE_REF:$DOLT_LOCAL_REF"
      ;;
    2)
      echo "[gitleaks] No remote Beads Dolt ref is advertised"
      ;;
    *)
      echo "[gitleaks] ERROR: could not inspect origin for $DOLT_REMOTE_REF." >&2
      exit 1
      ;;
  esac
else
  echo "[gitleaks] No origin remote is configured; scanning local refs only"
fi

echo "[gitleaks] Scanning repository history"
echo "[gitleaks] Reachable commits: $(git -C "$ROOT_DIR" rev-list --all --count)"
if printf '%s' "$GITLEAKS_HELP" | grep -q '\<git\>'; then
  (cd "$ROOT_DIR" && gitleaks git . --redact --log-opts="--all" "${CONFIG_ARGS[@]}")
elif printf '%s' "$GITLEAKS_HELP" | grep -q '\<detect\>'; then
  gitleaks detect --source "$ROOT_DIR" --redact --log-opts="--all" "${CONFIG_ARGS[@]}"
else
  echo "[gitleaks] ERROR: unsupported gitleaks CLI; expected 'detect' or 'git'." >&2
  exit 1
fi
