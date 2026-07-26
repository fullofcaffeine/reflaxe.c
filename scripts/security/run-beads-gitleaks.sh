#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"
BD_BIN="$(python3 "$ROOT_DIR/scripts/beads/resolve-reviewed.py")"
readonly EXPECTED_DOLT_VERSION="dolt version 2.1.4"
HISTORY_READER="beads"
DOLT_BIN=""
DOLT_DATABASE_DIR=""

for command_name in jq gitleaks; do
  if ! command -v "$command_name" >/dev/null 2>&1; then
    echo "[beads-gitleaks] ERROR: $command_name is required." >&2
    exit 1
  fi
done

if ! CONTEXT_JSON="$("$BD_BIN" -C "$ROOT_DIR" context --json)"; then
  echo "[beads-gitleaks] ERROR: the repository Beads database is unavailable." >&2
  exit 1
fi

if [[ -n "${HXC_DOLT_BIN:-}" ]]; then
  DOLT_BIN="$(python3 "$ROOT_DIR/scripts/beads/resolve-reviewed-dolt.py")"

  DOLT_MODE="$(printf '%s' "$CONTEXT_JSON" | jq -er '.dolt_mode')"
  BEADS_DIR="$(printf '%s' "$CONTEXT_JSON" | jq -er '.beads_dir')"
  DOLT_DATABASE="$(printf '%s' "$CONTEXT_JSON" | jq -er '.database')"
  if [[ "$DOLT_MODE" != "embedded" ]]; then
    echo "[beads-gitleaks] ERROR: HXC_DOLT_BIN recovery is admitted only for an embedded Dolt database." >&2
    exit 1
  fi
  if [[ ! "$DOLT_DATABASE" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]]; then
    echo "[beads-gitleaks] ERROR: unsafe embedded Dolt database name: $DOLT_DATABASE" >&2
    exit 1
  fi
  DOLT_DATABASE_DIR="$BEADS_DIR/embeddeddolt/$DOLT_DATABASE"
  if [[ ! -d "$DOLT_DATABASE_DIR" ]]; then
    echo "[beads-gitleaks] ERROR: embedded Dolt database directory is missing: $DOLT_DATABASE_DIR" >&2
    exit 1
  fi
  HISTORY_READER="dolt-diff"
fi

issue_ids=""
issue_count=0
if [[ "$HISTORY_READER" == "beads" ]]; then
  issue_ids="$(
    "$BD_BIN" -C "$ROOT_DIR" export --all |
      jq -r 'select(type == "object" and ._type == "issue" and (.id | type == "string")) | .id'
  )"
  issue_count="$(printf '%s\n' "$issue_ids" | awk 'NF { count++ } END { print count + 0 }')"
fi

emit_dolt_history() {
  local table_payload
  local table_names
  local table_name

  (
    cd "$DOLT_DATABASE_DIR"
    "$DOLT_BIN" sql -q "SELECT * FROM dolt_log" -r json
  ) || return 1
  table_payload="$(
    cd "$DOLT_DATABASE_DIR"
    "$DOLT_BIN" sql \
      -q "SELECT table_name FROM information_schema.tables WHERE table_schema = DATABASE() AND table_type = 'BASE TABLE' ORDER BY table_name" \
      -r json
  )" || return 1
  table_names="$(printf '%s' "$table_payload" | jq -er '.rows[].TABLE_NAME')" || return 1

  while IFS= read -r table_name; do
    if [[ ! "$table_name" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]]; then
      echo "[beads-gitleaks] ERROR: unsafe Beads table name returned by Dolt: $table_name" >&2
      return 1
    fi
    (
      cd "$DOLT_DATABASE_DIR"
      "$DOLT_BIN" sql -q "SELECT * FROM dolt_diff_${table_name}" -r json
    ) || return 1
  done <<< "$table_names"
}

emit_beads_records() {
  "$BD_BIN" -C "$ROOT_DIR" export --all || return 1
  if [[ "$HISTORY_READER" == "dolt-diff" ]]; then
    emit_dolt_history
    return
  fi

  while IFS= read -r issue_id; do
    if [[ -n "$issue_id" ]]; then
      "$BD_BIN" -C "$ROOT_DIR" history "$issue_id" --json || return 1
    fi
  done <<< "$issue_ids"
}

if [[ "$HISTORY_READER" == "dolt-diff" ]]; then
  echo "[beads-gitleaks] Scanning all current records and every changed row in embedded Dolt history with $EXPECTED_DOLT_VERSION at $DOLT_BIN"
else
  echo "[beads-gitleaks] Scanning all current Beads records and the history of $issue_count issues"
fi

set +e
emit_beads_records | gitleaks stdin \
  --redact \
  --config "$ROOT_DIR/.gitleaks.toml" \
  --no-banner
PIPELINE_STATUS=("${PIPESTATUS[@]}")
set -e

if [[ "${PIPELINE_STATUS[1]}" -ne 0 ]]; then
  exit "${PIPELINE_STATUS[1]}"
fi
if [[ "${PIPELINE_STATUS[0]}" -ne 0 ]]; then
  echo "[beads-gitleaks] ERROR: decoded Beads history could not be read completely; no push was attempted." >&2
  if [[ "$HISTORY_READER" == "beads" ]]; then
    echo "[beads-gitleaks] The reviewed Beads 1.1.0 history command cannot decode legacy NULL text fields. Use the exact Dolt recovery reader documented in docs/beads-toolchain.md." >&2
  fi
  exit "${PIPELINE_STATUS[0]}"
fi

echo "[beads-gitleaks] OK"
