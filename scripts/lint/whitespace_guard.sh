#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"
cd "$ROOT_DIR"

DIFF_ARGS=()
if [ "${1:-}" = "--staged" ]; then
  DIFF_ARGS+=(--cached)
elif [ "$#" -ne 0 ]; then
  echo "usage: $0 [--staged]" >&2
  exit 2
fi

# Preserve immutable third-party bytes exactly as reviewed. Their content is
# protected by provenance/checksum gates instead of repository formatting.
if [ "${#DIFF_ARGS[@]}" -eq 0 ]; then
  git diff --check -- \
    . \
    ':(exclude)vendor/**' \
    ':(exclude)third_party/**' \
    ':(exclude)runtime/vendor/**' \
    ':(exclude)runtime/third_party/**'
else
  git diff "${DIFF_ARGS[@]}" --check -- \
    . \
    ':(exclude)vendor/**' \
    ':(exclude)third_party/**' \
    ':(exclude)runtime/vendor/**' \
    ':(exclude)runtime/third_party/**'
fi
