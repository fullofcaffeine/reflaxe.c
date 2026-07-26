#!/usr/bin/env python3
"""Validate the exact read-only Dolt CLI admitted for Beads history recovery.

Beads 1.1.0 cannot decode older history rows whose text columns contain SQL
NULL. The publication guard may therefore use a standalone Dolt reader, but
only when its embedded Go module revision and checksum match the Dolt library
inside the reviewed Beads build. This resolver keeps that exceptional reader
as narrow and reviewable as the normal Beads executable pin.
"""

from __future__ import annotations

import os
import shutil
import subprocess
import sys
from pathlib import Path


EXPECTED_CLI_VERSION = "dolt version 2.1.4"
EXPECTED_PACKAGE = "github.com/dolthub/dolt/go/cmd/dolt"
EXPECTED_MODULE = "github.com/dolthub/dolt/go"
EXPECTED_MODULE_VERSION = "v0.40.5-0.20260605230755-1bf533220ab0"
EXPECTED_MODULE_SUM = "h1:oPg5f5bYFy5x7Ws2qtVG7wiva96cIh9SFg7nrC4n7QA="
COMMAND_TIMEOUT_SECONDS = 10


class ResolverError(RuntimeError):
    """A candidate cannot prove the exact reviewed Dolt provenance."""


def run(command: list[str]) -> str:
    try:
        result = subprocess.run(
            command,
            check=False,
            capture_output=True,
            text=True,
            timeout=COMMAND_TIMEOUT_SECONDS,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise ResolverError(f"cannot run {command[0]}: {error}") from error
    if result.returncode != 0:
        detail = result.stderr.strip() or result.stdout.strip() or "no diagnostic"
        raise ResolverError(
            f"{command[0]} exited {result.returncode}: {detail}"
        )
    return result.stdout


def validate_version(candidate: Path) -> None:
    first_line = run([str(candidate), "version"]).splitlines()
    observed = first_line[0].strip() if first_line else ""
    if observed != EXPECTED_CLI_VERSION:
        raise ResolverError(
            f"expected {EXPECTED_CLI_VERSION}, observed {observed or 'no output'}"
        )


def validate_go_build(candidate: Path) -> None:
    go = shutil.which("go")
    if go is None:
        raise ResolverError(
            "Go is required to verify the recovery reader's embedded module provenance"
        )

    package = ""
    module: tuple[str, str, str] | None = None
    for line in run([go, "version", "-m", str(candidate)]).splitlines():
        fields = line.strip().split()
        if len(fields) == 2 and fields[0] == "path":
            package = fields[1]
        elif len(fields) >= 4 and fields[0] == "mod":
            module = (fields[1], fields[2], fields[3])

    expected_module = (
        EXPECTED_MODULE,
        EXPECTED_MODULE_VERSION,
        EXPECTED_MODULE_SUM,
    )
    if package != EXPECTED_PACKAGE or module != expected_module:
        observed_module = " ".join(module) if module is not None else "missing"
        raise ResolverError(
            "unexpected Go build provenance: "
            f"package={package or 'missing'}, module={observed_module}"
        )


def main() -> int:
    configured = os.environ.get("HXC_DOLT_BIN", "")
    candidate = Path(configured).expanduser()
    if not configured or not candidate.is_absolute():
        print(
            "beads-dolt-reader: ERROR: HXC_DOLT_BIN must name an absolute "
            "path to the reviewed recovery reader.",
            file=sys.stderr,
        )
        return 2

    candidate = candidate.resolve()
    if not candidate.is_file() or not os.access(candidate, os.X_OK):
        print(
            f"beads-dolt-reader: ERROR: not an executable file: {candidate}",
            file=sys.stderr,
        )
        return 2

    try:
        validate_version(candidate)
        validate_go_build(candidate)
    except ResolverError as error:
        print(f"beads-dolt-reader: ERROR: {error}", file=sys.stderr)
        print(
            "beads-dolt-reader: build the exact module named in "
            "docs/beads-toolchain.md; a version-compatible but differently "
            "built Dolt binary is not admitted.",
            file=sys.stderr,
        )
        return 2

    print(candidate)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
