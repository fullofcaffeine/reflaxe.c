#!/usr/bin/env python3
"""Find the exact Beads client reviewed for this shared Dolt database.

The first ``bd`` on ``PATH`` is not necessarily safe. A newer or older client
can serialize the passive JSONL export differently, and a client with a
different schema may be unable to write the shared database. This resolver
checks every candidate before another repository script is allowed to use it.
"""

from __future__ import annotations

import os
import subprocess
import sys
from pathlib import Path


EXPECTED_VERSION = "1.1.0"
EXPECTED_REVISION = "8e4e59d39"
EXPECTED_OUTPUT = f"bd version {EXPECTED_VERSION} ({EXPECTED_REVISION})"
VERSION_TIMEOUT_SECONDS = 5


def candidate_paths() -> list[Path]:
    explicit = os.environ.get("HXC_BD_BIN")
    if explicit:
        return [Path(explicit).expanduser().resolve()]

    names = ("bd.exe", "bd") if os.name == "nt" else ("bd",)
    candidates: list[Path] = []
    seen: set[Path] = set()
    for directory in os.environ.get("PATH", "").split(os.pathsep):
        # An empty PATH component means the current directory. Hooks must not
        # trust a checkout-local executable merely because PATH contains one.
        if not directory:
            continue
        for name in names:
            candidate = (Path(directory).expanduser() / name).resolve()
            if candidate in seen or not candidate.is_file():
                continue
            if os.name != "nt" and not os.access(candidate, os.X_OK):
                continue
            seen.add(candidate)
            candidates.append(candidate)
    return candidates


def version_output(candidate: Path) -> str:
    try:
        result = subprocess.run(
            [str(candidate), "--version"],
            check=False,
            capture_output=True,
            text=True,
            timeout=VERSION_TIMEOUT_SECONDS,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        return f"unavailable: {error}"
    output = result.stdout.strip()
    if result.returncode != 0:
        detail = result.stderr.strip() or output or "no diagnostic"
        return f"exit {result.returncode}: {detail}"
    return output


def main() -> int:
    candidates = candidate_paths()
    observations: list[str] = []
    for candidate in candidates:
        observed = version_output(candidate)
        if observed == EXPECTED_OUTPUT:
            print(candidate)
            return 0
        observations.append(f"{candidate}: {observed}")

    print(
        "beads-toolchain: ERROR: this repository requires "
        f"{EXPECTED_OUTPUT}; no reviewed client was found before any Beads "
        "operation was attempted.",
        file=sys.stderr,
    )
    if observations:
        print(
            "beads-toolchain: inspected " + "; ".join(observations),
            file=sys.stderr,
        )
    else:
        print("beads-toolchain: PATH contains no bd executable.", file=sys.stderr)
    print(
        "beads-toolchain: install Beads 1.1.0 from revision 8e4e59d39 or set "
        "HXC_BD_BIN to that executable. If this clone still has schema v32, "
        "adopt the migrated remote with `bd bootstrap`; do not migrate a "
        "second clone independently.",
        file=sys.stderr,
    )
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
