#!/usr/bin/env python3
"""Keep CLAUDE.md compatibility paths as links to canonical AGENTS.md files."""

from __future__ import annotations

import argparse
import os
import subprocess
import sys
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[2]
REQUIRED_LINKS = ("CLAUDE.md", "examples/caxecraft/CLAUDE.md")


class InstructionLinkError(RuntimeError):
    pass


def tracked_entries(root: Path) -> dict[str, str]:
    """Return the Git mode for every tracked path without following links."""

    result = subprocess.run(
        ["git", "-C", str(root), "ls-files", "--stage", "-z"],
        check=False,
        capture_output=True,
    )
    if result.returncode != 0:
        detail = result.stderr.decode("utf-8", errors="replace").strip()
        raise InstructionLinkError(f"cannot inventory tracked files: {detail}")

    entries: dict[str, str] = {}
    for raw_entry in result.stdout.split(b"\0"):
        if not raw_entry:
            continue
        try:
            metadata, raw_path = raw_entry.split(b"\t", 1)
            mode = metadata.split(b" ", 1)[0].decode("ascii")
            relative = raw_path.decode("utf-8")
        except (UnicodeDecodeError, ValueError) as error:
            raise InstructionLinkError(
                "git returned a malformed or non-UTF-8 tracked path"
            ) from error
        entries[relative] = mode
    return entries


def validate_instruction_link(
    root: Path, relative: str, tracked: dict[str, str]
) -> list[str]:
    """Validate one compatibility link and its same-directory canonical file."""

    errors: list[str] = []
    logical = PurePosixPath(relative)
    path = root.joinpath(*logical.parts)
    if tracked.get(relative) != "120000":
        errors.append(f"{relative} must be tracked by Git as a symbolic link")
    if not path.is_symlink():
        errors.append(f"{relative} must be a symbolic link, not a copied file")
        return errors

    target = os.readlink(path)
    if target != "AGENTS.md":
        errors.append(
            f"{relative} must link to same-directory AGENTS.md, not {target!r}"
        )
        return errors

    target_relative = str(logical.parent / "AGENTS.md")
    if target_relative.startswith("./"):
        target_relative = target_relative[2:]
    target_path = path.parent / target
    if not target_path.is_file() or target_path.is_symlink():
        errors.append(f"{relative} resolves to a missing or non-canonical AGENTS.md")
    if tracked.get(target_relative) != "100644":
        errors.append(
            f"{relative} must resolve to a tracked, non-executable AGENTS.md"
        )
    return errors


def validate(root: Path) -> list[str]:
    tracked = tracked_entries(root)
    links = sorted(
        relative
        for relative in tracked
        if PurePosixPath(relative).name == "CLAUDE.md"
    )
    errors: list[str] = []
    for required in REQUIRED_LINKS:
        if required not in links:
            errors.append(f"required compatibility link is not tracked: {required}")
    for relative in links:
        errors.extend(validate_instruction_link(root, relative, tracked))
    return errors


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Validate AGENTS.md/CLAUDE.md instruction ownership."
    )
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--quiet", action="store_true")
    arguments = parser.parse_args()

    try:
        errors = validate(arguments.root.resolve())
    except (InstructionLinkError, OSError) as error:
        errors = [str(error)]
    if errors:
        for error in errors:
            print(f"agent-instructions: ERROR: {error}", file=sys.stderr)
        return 1
    if not arguments.quiet:
        print(
            "agent-instructions: OK: every CLAUDE.md is a relative symlink "
            "to its tracked same-directory AGENTS.md"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
