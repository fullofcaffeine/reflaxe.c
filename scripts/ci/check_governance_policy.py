#!/usr/bin/env python3
"""Validate contribution, disclosure, and release governance contracts."""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[2]
PRIVATE_CONTACT = "boss@fullofcaffeine.com"
MARKDOWN_LINK = re.compile(r"\[[^\]]+\]\(([^)]+)\)")


@dataclass(frozen=True)
class DocumentContract:
    path: str
    headings: tuple[str, ...]
    required_text: tuple[str, ...]


DOCUMENTS = (
    DocumentContract(
        "CONTRIBUTING.md",
        (
            "# Contributing to reflaxe.c",
            "## Before you start",
            "## Work through Beads",
            "## Make typed, reviewable changes",
            "## Definition of done",
            "## Generated files and snapshots",
            "## Dependencies, vendoring, and provenance",
            "## Security-sensitive changes",
            "## Releases and signing",
            "## Commit and handoff",
        ),
        (
            "[SECURITY.md](SECURITY.md)",
            ".beads/issues.jsonl",
            "Reflect",
            "Dynamic",
            "Any",
            "untyped",
            "PRD Section 40",
            "npm run snapshots:update -- --suite hxc-ir",
            "docs/specs/third-party-provenance.json",
            "GPL-3.0-only",
            "haxe_c-od2.5",
            "E10.T06",
            "E10.T08",
            "E10.T11",
            "release manager",
            "Artifact builders",
            "provenance reviewer",
            "owner-authorized signer",
            "private key material outside the repository and logs",
            "missing or invalid\n  configured signature blocks publication",
            "bd close <id> --reason",
            "Commit and push the implementation",
            "git rev-list --left-right --count origin/main...HEAD",
        ),
    ),
    DocumentContract(
        "SECURITY.md",
        (
            "# Security policy",
            "## Reporting a vulnerability",
            "## Supported versions",
            "## Security scope",
            "## What to include",
            "## Coordinated disclosure",
        ),
        (
            f"mailto:{PRIVATE_CONTACT}",
            "[reflaxe.c security]",
            "Do not open a GitHub issue",
            "available only for public repositories",
            "There is no published or supported release yet.",
            "Best-effort triage only; not a supported release",
            "| Published releases | None |",
            "path traversal",
            "build-flag",
            "integer-overflow",
            "provenance compromise",
            "within three business days",
            "within seven business days",
            "does not currently offer a bug bounty",
            "E10.T06",
            "E10.T08",
        ),
    ),
    DocumentContract(
        "AGENTS.md",
        ("### Contribution, security, and release governance",),
        (
            "CONTRIBUTING.md",
            "SECURITY.md",
            PRIVATE_CONTACT,
            "There is no supported release today.",
            "release manager",
            "artifact builders",
            "provenance reviewer",
            "owner-authorized signer",
            "push the implementation and require remote Governance success",
            "python3 scripts/ci/check_governance_policy.py",
        ),
    ),
    DocumentContract(
        "README.md",
        (),
        (
            "[Contributing](CONTRIBUTING.md)",
            "[Security policy](SECURITY.md)",
            "[Agent rules](AGENTS.md)",
            "python3 scripts/ci/check_governance_policy.py",
        ),
    ),
    DocumentContract(
        "docs/PRD.md",
        (),
        (
            "CONTRIBUTING.md",
            "non-public channel named in `SECURITY.md`",
            (
                "release manager, artifact\n"
                "  builder, provenance reviewer, and authorized signer"
            ),
            "private key material never enters the repository or logs",
        ),
    ),
)

SHARED_WORKFLOW_TEXT = (
    "bd list --ready --type task --json",
    "bd update <id> --claim",
    "bd dolt push",
    "npm test",
    "requirement IDs",
    "exact commands",
    "haxe_c-od2.5",
)


def read_text(root: Path, relative: str, errors: list[str]) -> str:
    path = root / relative
    try:
        text = path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        errors.append(f"cannot read {relative}: {error}")
        return ""
    if not text.strip():
        errors.append(f"{relative} must not be empty")
    return text


def require_in_order(
    relative: str, text: str, headings: tuple[str, ...], errors: list[str]
) -> None:
    offset = 0
    for heading in headings:
        location = text.find(heading, offset)
        if location < 0:
            errors.append(f"{relative} is missing ordered heading: {heading}")
            continue
        offset = location + len(heading)


def validate_local_links(
    root: Path, relative: str, text: str, errors: list[str]
) -> None:
    base = PurePosixPath(relative).parent
    for match in MARKDOWN_LINK.finditer(text):
        target = match.group(1).strip()
        if target.startswith(("https://", "http://", "mailto:", "#")):
            continue
        path_text = target.split("#", 1)[0]
        if not path_text:
            continue
        if "\\" in path_text:
            errors.append(f"{relative} contains a non-POSIX local link: {target}")
            continue
        path = PurePosixPath(path_text)
        if path.is_absolute() or any(part in ("", ".", "..") for part in path.parts):
            errors.append(f"{relative} contains an unsafe local link: {target}")
            continue
        resolved = root.joinpath(*(base / path).parts)
        if not resolved.exists():
            errors.append(f"{relative} links to a missing path: {target}")


def validate(root: Path) -> list[str]:
    errors: list[str] = []
    contents: dict[str, str] = {}
    for contract in DOCUMENTS:
        text = read_text(root, contract.path, errors)
        contents[contract.path] = text
        require_in_order(contract.path, text, contract.headings, errors)
        for required in contract.required_text:
            if required not in text:
                errors.append(
                    f"{contract.path} is missing governance contract: {required}"
                )

    for relative in ("CONTRIBUTING.md", "SECURITY.md"):
        validate_local_links(root, relative, contents.get(relative, ""), errors)

    contributing = contents.get("CONTRIBUTING.md", "")
    agents = contents.get("AGENTS.md", "")
    for required in SHARED_WORKFLOW_TEXT:
        if required not in contributing:
            errors.append(f"CONTRIBUTING.md lost shared workflow text: {required}")
        if required not in agents:
            errors.append(f"AGENTS.md lost shared workflow text: {required}")

    security = contents.get("SECURITY.md", "")
    if security.count(PRIVATE_CONTACT) != 2:
        errors.append(
            "SECURITY.md must name the private contact once as text and once "
            "in its mailto link"
        )
    if "/security/advisories/new" in security:
        errors.append(
            "SECURITY.md must not advertise GitHub private reporting before "
            "the public-repository feature is enabled"
        )
    return errors


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(
        description="Validate contribution, security, and release governance."
    )
    result.add_argument("--root", type=Path, default=ROOT)
    result.add_argument("--quiet", action="store_true")
    return result


def main(arguments: list[str] | None = None) -> int:
    options = parser().parse_args(arguments)
    root = options.root.resolve()
    errors = validate(root)
    if errors:
        for error in errors:
            print(f"governance-policy: ERROR: {error}", file=sys.stderr)
        return 1
    if not options.quiet:
        print(
            "governance-policy: OK: contributor workflow, private disclosure, "
            "supported versions, generated files, vendoring, release roles, "
            "signing, and provenance agree"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
