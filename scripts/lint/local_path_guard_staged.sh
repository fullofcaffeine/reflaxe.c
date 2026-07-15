#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(git rev-parse --show-toplevel)"
cd "$ROOT_DIR"

STAGED_ADDED_LINES="$(
  git diff --cached --unified=0 --no-color -- . +    | awk '
      /^diff --git / { file = ""; next }
      /^\+\+\+ / {
        file = $2
        if (file == "/dev/null") {
          file = ""
        } else {
          sub(/^[a-z]\//, "", file)
        }
        next
      }
      /^\+/ && $0 !~ /^\+\+\+/ && file != "" {
        print file ":" substr($0, 2)
      }
    '
)"

if [[ -z "$STAGED_ADDED_LINES" ]]; then
  exit 0
fi

ABSOLUTE_LOCAL_PATTERN="(/Users/[^[:space:]\"'<>()[\\]{}]+|/home/[^[:space:]\"'<>()[\\]{}]+|/var/folders/[^[:space:]\"'<>()[\\]{}]+|/private/var/folders/[^[:space:]\"'<>()[\\]{}]+|[A-Za-z]:\\\\Users\\\\[^[:space:]\"'<>()[\\]{}]+)"
RELATIVE_PATH_PATTERN="(^|[^[:alnum:]_])(\\./|\\.\\./)[^[:space:]\"'<>()[\\]{}]+"

if command -v rg >/dev/null 2>&1; then
  ABSOLUTE_HITS="$(printf '%s\n' "$STAGED_ADDED_LINES" | rg -n -P "$ABSOLUTE_LOCAL_PATTERN" || true)"
  RELATIVE_HITS="$(printf '%s\n' "$STAGED_ADDED_LINES" | rg -n -P "$RELATIVE_PATH_PATTERN" || true)"
else
  ABSOLUTE_HITS="$(printf '%s\n' "$STAGED_ADDED_LINES" | grep -En "$ABSOLUTE_LOCAL_PATTERN" || true)"
  RELATIVE_HITS="$(printf '%s\n' "$STAGED_ADDED_LINES" | grep -En "$RELATIVE_PATH_PATTERN" || true)"
fi

if [[ -n "$ABSOLUTE_HITS" ]]; then
  echo "[guard:local-paths] ERROR: absolute local filesystem paths detected." >&2
  echo "[guard:local-paths] Use repository-relative paths instead." >&2
  echo "$ABSOLUTE_HITS" >&2
  exit 1
fi

if [[ -z "$RELATIVE_HITS" ]]; then
  exit 0
fi

if [[ "${ALLOW_RELATIVE_PATH_REFERENCES:-0}" = "1" ]]; then
  echo "[guard:local-paths] ALLOW_RELATIVE_PATH_REFERENCES=1; skipping relative-path validation."
  exit 0
fi

TMP_RELATIVE_SCAN="$(mktemp)"
trap 'rm -f "$TMP_RELATIVE_SCAN"' EXIT
printf '%s\n' "$STAGED_ADDED_LINES" > "$TMP_RELATIVE_SCAN"

RELATIVE_OUTSIDE_HITS="$(
  python3 - "$ROOT_DIR" "$TMP_RELATIVE_SCAN" <<'PY'
import re
import sys
from pathlib import Path

root = Path(sys.argv[1]).resolve()
scan_file = Path(sys.argv[2])
pattern = re.compile(r"(?<![A-Za-z0-9_])((?:\./|\.\./)[^\s\"'<>()[\]{}]+)")

findings = []
for entry in scan_file.read_text(encoding="utf-8").splitlines():
    if ":" not in entry:
        continue
    file_path, content = entry.split(":", 1)
    source_dir = (root / file_path).resolve().parent
    for match in pattern.finditer(content):
        reference = match.group(1)
        resolved = (source_dir / reference).resolve(strict=False)
        try:
            resolved.relative_to(root)
        except ValueError:
            findings.append(f"{file_path}: {reference} -> outside repository")

if findings:
    print("\n".join(findings))
PY
)"

if [[ -n "$RELATIVE_OUTSIDE_HITS" ]]; then
  echo "[guard:local-paths] ERROR: relative path references escape the repository." >&2
  echo "$RELATIVE_OUTSIDE_HITS" >&2
  exit 1
fi

echo "[guard:local-paths] Relative path references resolve inside the repository."
