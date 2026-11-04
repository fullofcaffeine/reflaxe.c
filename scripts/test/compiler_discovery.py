#!/usr/bin/env python3
"""Find an identity-matching native compiler, including versioned Homebrew GCC."""

from __future__ import annotations

import shutil
import subprocess


FAMILIES = ("gcc", "clang")
LANGUAGES = ("c", "c++")


def candidate_names(family: str, language: str = "c") -> tuple[str, ...]:
    """Return stable command candidates without treating Apple Clang as GCC."""

    if family not in FAMILIES:
        raise ValueError(f"unknown compiler family: {family!r}")
    if language not in LANGUAGES:
        raise ValueError(f"unknown compiler language: {language!r}")
    if family == "clang":
        return ("clang++" if language == "c++" else "clang",)
    base = "g++" if language == "c++" else "gcc"
    return (base, *(f"{base}-{major}" for major in range(30, 4, -1)))


def compiler_family(executable: str) -> str | None:
    """Identify the implementation from its version output, not its command alias."""

    result = subprocess.run(
        [executable, "--version"],
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        return None
    identity = (result.stdout + result.stderr).lower()
    if "clang" in identity:
        return "clang"
    if "gcc" in identity or "free software foundation" in identity:
        return "gcc"
    return None


def resolve_compiler(family: str, language: str = "c") -> str | None:
    """Select the first PATH command whose reported family matches the request."""

    for name in candidate_names(family, language):
        executable = shutil.which(name)
        if executable is not None and compiler_family(executable) == family:
            return executable
    return None
