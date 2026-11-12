#!/usr/bin/env python3
"""Select native compilers and pair sanitized executions with their own runtimes."""

from __future__ import annotations

import os
import sys
from pathlib import Path
from collections.abc import Mapping, Sequence

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


def sanitizer_execution_environment(
    compiler: str,
    family: str,
    flags: Sequence[str],
    *,
    environment: Mapping[str, str] | None = None,
) -> dict[str, str]:
    """Give a sanitized child the runtime shipped with its selected Clang.

    macOS shells can remove DYLD_LIBRARY_PATH before a Python test starts.
    Restore the selected runtime only at the final executable boundary; never
    change the parent environment or search a different compiler installation.
    Other platforms, compiler families, and unsanitized runs keep their existing
    environment. A missing runtime is a test setup failure, not a sanitizer skip.
    """
    child = dict(os.environ if environment is None else environment)
    address_enabled = any(
        flag.startswith("-fsanitize=") and "address" in flag.split("=", 1)[1].split(",")
        for flag in flags
    )
    if sys.platform != "darwin" or family != "clang" or not address_enabled:
        return child
    result = subprocess.run(
        [compiler, "--print-runtime-dir"], capture_output=True, text=True,
        check=False, timeout=10,
    )
    value = result.stdout.strip()
    directory = Path(value)
    if (
        result.returncode != 0 or result.stderr or not value
        or "\n" in value or "\r" in value or "\0" in value
        or not directory.is_absolute() or not directory.is_dir()
        or directory.resolve() != directory
    ):
        raise RuntimeError(
            f"selected Clang {compiler!r} returned an invalid sanitizer runtime directory: "
            f"exit={result.returncode}, stdout={result.stdout!r}, stderr={result.stderr!r}"
        )
    runtime = directory / "libclang_rt.asan_osx_dynamic.dylib"
    if not runtime.is_file():
        raise RuntimeError(f"selected Clang {compiler!r} has no AddressSanitizer runtime: {runtime}")
    previous = child.get("DYLD_LIBRARY_PATH", "")
    entries = [entry for entry in previous.split(":") if entry != value] if previous else []
    child["DYLD_LIBRARY_PATH"] = ":".join([value, *entries])
    return child
