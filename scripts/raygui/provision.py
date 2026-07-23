#!/usr/bin/env python3
"""Provision raygui 5.0 and build its one owned implementation unit.

raygui is distributed as a header. Declarations are always visible, but the
function bodies exist only in a translation unit that defines
``RAYGUI_IMPLEMENTATION``. This tool gives that definition one deterministic
owner and archives the resulting object as ``libraygui.a``. Game code links
that library; it never injects or copies C implementation text.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tarfile
import tempfile
import urllib.request
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Mapping, Sequence

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.raygui.core_binding import (
    IMPLEMENTATION_SOURCE_SHA256,
    PINNED_TREE,
    BindingFailure,
    load_lock,
    sha256_file,
    source_inputs,
)
from scripts.raylib.provision import canonical_json, canonical_tree_identity


IMPLEMENTATION_SOURCE = '#define RAYGUI_IMPLEMENTATION\n#include "raygui.h"\n'
STRICT_C_FLAGS = (
    "-std=c11",
    "-O2",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wpedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    # raygui 5.0 performs reviewed int/unsigned style-color conversions in its
    # implementation. Keep those warnings visible, but do not silently fork
    # upstream merely to make that one warning family fatal.
    "-Wno-error=sign-conversion",
    "-Wno-error=unused-parameter",
    "-Wno-error=unused-function",
    "-Wno-error=shadow",
    "-Wundef",
    "-Wformat=2",
)


class RayguiProvisionFailure(RuntimeError):
    """Pinned source or the exactly-once native build could not be proven."""


@dataclass(frozen=True)
class RayguiBuild:
    source_root: Path
    include_directory: Path
    library_file: Path
    implementation_source: Path
    report: dict[str, object]


def implementation_bytes() -> bytes:
    content = IMPLEMENTATION_SOURCE.encode("utf-8")
    if hashlib.sha256(content).hexdigest() != IMPLEMENTATION_SOURCE_SHA256:
        raise RayguiProvisionFailure("raygui implementation source template drifted")
    if IMPLEMENTATION_SOURCE.count("RAYGUI_IMPLEMENTATION") != 1:
        raise RayguiProvisionFailure("raygui implementation macro must have exactly one owner")
    return content


def upstream_fact(lock: Mapping[str, object]) -> Mapping[str, object]:
    value = lock.get("upstream")
    if not isinstance(value, dict):
        raise RayguiProvisionFailure("validated raygui lock lost its upstream identity")
    return value


def archive_fact(lock: Mapping[str, object]) -> Mapping[str, object]:
    value = upstream_fact(lock).get("archive")
    if not isinstance(value, dict):
        raise RayguiProvisionFailure("validated raygui lock lost its archive identity")
    return value


def verify_raygui_source(source_root: Path, lock: Mapping[str, object]) -> Path:
    source_root = source_root.resolve()
    if source_root.is_symlink() or not source_root.is_dir():
        raise RayguiProvisionFailure(f"raygui source root is missing or not a real directory: {source_root}")
    upstream = upstream_fact(lock)
    for key, label in (("header", "header"), ("license", "license")):
        fact = upstream.get(key)
        if not isinstance(fact, dict):
            raise RayguiProvisionFailure(f"validated raygui lock lost its {label} identity")
        raw_path = fact.get("path")
        expected_hash = fact.get("sha256")
        if not isinstance(raw_path, str) or not isinstance(expected_hash, str):
            raise RayguiProvisionFailure(f"validated raygui {label} identity is malformed")
        relative = PurePosixPath(raw_path)
        if relative.is_absolute() or any(part in ("", ".", "..") for part in relative.parts):
            raise RayguiProvisionFailure(f"raygui {label} path is not a safe relative path")
        path = source_root.joinpath(*relative.parts)
        if path.is_symlink() or not path.is_file() or sha256_file(path) != expected_hash:
            raise RayguiProvisionFailure(f"raygui {label} does not match the immutable lock")
    identity = canonical_tree_identity(source_root)
    if (identity.sha256, identity.file_count, identity.size_bytes) != PINNED_TREE:
        raise RayguiProvisionFailure(
            "raygui source-tree identity mismatch: "
            f"sha256={identity.sha256} files={identity.file_count} bytes={identity.size_bytes}"
        )
    return source_root


def verify_archive(path: Path, lock: Mapping[str, object]) -> None:
    fact = archive_fact(lock)
    if path.is_symlink() or not path.is_file():
        raise RayguiProvisionFailure(f"raygui archive is missing or not a real file: {path}")
    if path.stat().st_size != fact.get("sizeBytes") or sha256_file(path) != fact.get("sha256"):
        raise RayguiProvisionFailure("raygui archive size or SHA-256 does not match the immutable lock")


def download_archive(destination: Path, lock: Mapping[str, object]) -> None:
    fact = archive_fact(lock)
    url = fact.get("url")
    expected_size = fact.get("sizeBytes")
    if not isinstance(url, str) or not isinstance(expected_size, int):
        raise RayguiProvisionFailure("validated raygui lock lost its download facts")
    destination.parent.mkdir(parents=True, exist_ok=True)
    partial = destination.with_name(destination.name + ".part")
    if partial.exists() or partial.is_symlink():
        raise RayguiProvisionFailure(f"partial raygui download already exists: {partial}")
    request = urllib.request.Request(url, headers={"User-Agent": "reflaxe.c-raygui-provision/1"})
    try:
        with urllib.request.urlopen(request, timeout=60) as response, partial.open("xb") as output:
            received = 0
            while True:
                chunk = response.read(1024 * 1024)
                if not chunk:
                    break
                received += len(chunk)
                if received > expected_size:
                    raise RayguiProvisionFailure("raygui archive exceeded its locked byte size")
                output.write(chunk)
        verify_archive(partial, lock)
        os.replace(partial, destination)
    except Exception:
        if partial.is_file() and not partial.is_symlink():
            partial.unlink()
        raise


def extract_archive(archive: Path, sources_root: Path, lock: Mapping[str, object]) -> Path:
    verify_archive(archive, lock)
    root_name = archive_fact(lock).get("rootDirectory")
    if not isinstance(root_name, str):
        raise RayguiProvisionFailure("validated raygui lock lost its archive root")
    destination = sources_root / root_name
    if destination.exists() or destination.is_symlink():
        raise RayguiProvisionFailure(f"raygui source destination already exists: {destination}")
    sources_root.mkdir(parents=True, exist_ok=True)
    staging = Path(tempfile.mkdtemp(prefix=".raygui-extract-", dir=sources_root))
    seen: set[str] = set()
    try:
        with tarfile.open(archive, mode="r:gz") as bundle:
            for member in bundle.getmembers():
                raw_name = member.name.rstrip("/")
                if not raw_name:
                    continue
                relative = PurePosixPath(raw_name)
                if (
                    relative.is_absolute()
                    or any(part in ("", ".", "..") for part in relative.parts)
                    or relative.parts[0] != root_name
                ):
                    raise RayguiProvisionFailure(f"raygui archive member escapes its locked root: {member.name!r}")
                normalized = relative.as_posix()
                if normalized in seen:
                    raise RayguiProvisionFailure(f"raygui archive contains a duplicate member: {normalized}")
                seen.add(normalized)
                target = staging.joinpath(*relative.parts)
                if member.isdir():
                    target.mkdir(parents=True, exist_ok=True)
                elif member.isfile():
                    target.parent.mkdir(parents=True, exist_ok=True)
                    source = bundle.extractfile(member)
                    if source is None:
                        raise RayguiProvisionFailure(f"cannot read raygui archive member: {normalized}")
                    with source, target.open("xb") as output:
                        shutil.copyfileobj(source, output, length=1024 * 1024)
                else:
                    raise RayguiProvisionFailure(f"raygui archive contains a link or special member: {normalized}")
        extracted = staging / root_name
        verify_raygui_source(extracted, lock)
        os.replace(extracted, destination)
        return destination
    finally:
        shutil.rmtree(staging, ignore_errors=True)


def pinned_source(cache_root: Path, lock: Mapping[str, object], allow_network: bool) -> Path:
    fact = archive_fact(lock)
    root_name = fact.get("rootDirectory")
    if not isinstance(root_name, str):
        raise RayguiProvisionFailure("validated raygui lock lost its archive root")
    cache_root = cache_root.resolve()
    archive = cache_root / "archives" / f"{root_name}.tar.gz"
    source = cache_root / "sources" / root_name
    if archive.exists():
        verify_archive(archive, lock)
    elif source.exists():
        raise RayguiProvisionFailure("pinned raygui source requires its locked archive beside the extracted tree")
    elif allow_network:
        download_archive(archive, lock)
    else:
        raise RayguiProvisionFailure("pinned raygui archive is absent; pass --allow-network explicitly")
    if source.exists():
        return verify_raygui_source(source, lock)
    return extract_archive(archive, cache_root / "sources", lock)


def run(arguments: Sequence[str], *, cwd: Path, label: str) -> subprocess.CompletedProcess[str]:
    try:
        result = subprocess.run(
            list(arguments),
            cwd=cwd,
            env={**os.environ, "LC_ALL": "C"},
            check=False,
            capture_output=True,
            text=True,
            timeout=120,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise RayguiProvisionFailure(f"{label} could not run: {error}") from error
    if result.returncode != 0:
        detail = "\n".join(value.strip() for value in (result.stdout, result.stderr) if value.strip())
        raise RayguiProvisionFailure(f"{label} failed with exit {result.returncode}\n{detail}".rstrip())
    return result


def tool_identity(tool: str, argument: str, cwd: Path) -> str:
    result = run((tool, argument), cwd=cwd, label=f"{tool} identity")
    lines = (result.stdout + result.stderr).strip().splitlines()
    if not lines:
        raise RayguiProvisionFailure(f"{tool} did not report its identity")
    return lines[0]


def tool_file_identity(tool: str) -> dict[str, object]:
    resolved = shutil.which(tool)
    if resolved is None:
        raise RayguiProvisionFailure(f"native tool is unavailable: {tool}")
    path = Path(resolved).resolve()
    if not path.is_file():
        raise RayguiProvisionFailure(f"native tool is not a regular file: {path}")
    return {"name": path.name, "sha256": sha256_file(path)}


def compiler_warning_flags(identity: str) -> tuple[str, ...]:
    """Return only warning spellings understood by the selected C family."""
    if "clang" in identity.lower():
        # Clang separates this conversion from -Wconversion; GCC has no option
        # with this name and must not receive an unknown -Wno-error switch.
        return ("-Wno-error=shorten-64-to-32",)
    return ()


def normalize_archive_headers(path: Path) -> None:
    """Remove host time/user IDs from a Unix ``ar`` archive.

    GNU ``ar`` is deterministic by default, while the BSD ``ar`` shipped on
    macOS records the current second and local numeric user/group IDs. Member
    contents are already identical. Normalizing only those documented header
    fields makes the same object archive byte-for-byte reproducible without
    parsing or rewriting machine code.
    """

    data = bytearray(path.read_bytes())
    if data[:8] != b"!<arch>\n":
        raise RayguiProvisionFailure("raygui archiver emitted an unknown archive format")
    offset = 8
    while offset < len(data):
        if offset + 60 > len(data) or data[offset + 58 : offset + 60] != b"`\n":
            raise RayguiProvisionFailure("raygui archive contains a malformed member header")
        try:
            size = int(bytes(data[offset + 48 : offset + 58]).decode("ascii").strip())
        except (UnicodeError, ValueError) as error:
            raise RayguiProvisionFailure("raygui archive contains a malformed member size") from error
        data[offset + 16 : offset + 28] = b"0           "
        data[offset + 28 : offset + 34] = b"0     "
        data[offset + 34 : offset + 40] = b"0     "
        offset += 60 + size + (size & 1)
    if offset != len(data):
        raise RayguiProvisionFailure("raygui archive member extends beyond the archive")
    path.write_bytes(data)


def build_static(
    *,
    source_root: Path,
    raylib_source: Path,
    build_root: Path,
    compiler: str,
    archiver: str,
) -> RayguiBuild:
    lock = load_lock()
    try:
        _, include_directory, raylib_include = source_inputs(source_root, raylib_source)
    except BindingFailure as error:
        raise RayguiProvisionFailure(str(error)) from error
    build_root = build_root.resolve()
    if build_root.exists() and (build_root.is_symlink() or not build_root.is_dir() or any(build_root.iterdir())):
        raise RayguiProvisionFailure(f"raygui build root must be an empty real directory: {build_root}")
    build_root.mkdir(parents=True, exist_ok=True)
    implementation = build_root / "raygui_implementation.c"
    object_file = build_root / "raygui_implementation.o"
    library = build_root / "libraygui.a"
    implementation.write_bytes(implementation_bytes())
    compiler_version = tool_identity(compiler, "--version", build_root)
    compile_arguments = (
        compiler,
        *STRICT_C_FLAGS,
        *compiler_warning_flags(compiler_version),
        "-I",
        str(include_directory),
        "-I",
        str(raylib_include),
        "-c",
        str(implementation),
        "-o",
        str(object_file),
    )
    run(compile_arguments, cwd=build_root, label="raygui implementation compile")
    run((archiver, "rcs", str(library), str(object_file)), cwd=build_root, label="raygui static archive")
    normalize_archive_headers(library)
    if library.is_symlink() or not library.is_file() or library.stat().st_size == 0:
        raise RayguiProvisionFailure("raygui static archive was not produced")
    implementation_fact = lock.get("implementation")
    if not isinstance(implementation_fact, dict):
        raise RayguiProvisionFailure("validated raygui lock lost implementation ownership")
    report: dict[str, object] = {
        "schemaVersion": 1,
        "upstream": {
            "name": "raygui",
            "release": "5.0",
            "commit": upstream_fact(lock)["commit"],
            "sourceTreeSha256": PINNED_TREE[0],
        },
        "implementation": implementation_fact,
        "upstreamWarningExceptions": [
            "shadow",
            "sign-conversion",
            "unused-parameter",
            "unused-function",
        ] + (["shorten-64-to-32"] if compiler_warning_flags(compiler_version) else []),
        "tools": {
            "compiler": compiler_version,
            "archiver": tool_file_identity(archiver),
        },
        "inputs": {
            "rayguiHeaderSha256": sha256_file(include_directory / "raygui.h"),
            "raylibHeaderSha256": sha256_file(raylib_include / "raylib.h"),
            "implementationSha256": sha256_file(implementation),
        },
        "output": {
            "librarySha256": sha256_file(library),
            "librarySizeBytes": library.stat().st_size,
        },
    }
    return RayguiBuild(source_root.resolve(), include_directory, library, implementation, report)


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache-root", type=Path)
    parser.add_argument("--source", type=Path)
    parser.add_argument("--raylib-source", type=Path, required=True)
    parser.add_argument("--build-root", type=Path, required=True)
    parser.add_argument("--cc", default="cc")
    parser.add_argument("--ar", default="ar")
    parser.add_argument("--allow-network", action="store_true")
    parser.add_argument("--report", type=Path)
    return parser.parse_args(argv)


def main(argv: Sequence[str]) -> int:
    args = parse_args(argv)
    try:
        lock = load_lock()
        if (args.source is None) == (args.cache_root is None):
            raise RayguiProvisionFailure("choose exactly one of --source or --cache-root")
        if args.source is not None:
            if args.allow_network:
                raise RayguiProvisionFailure("offline --source rejects --allow-network")
            source = verify_raygui_source(args.source, lock)
        else:
            source = pinned_source(args.cache_root, lock, args.allow_network)
        result = build_static(
            source_root=source,
            raylib_source=args.raylib_source,
            build_root=args.build_root,
            compiler=args.cc,
            archiver=args.ar,
        )
        report = canonical_json(result.report)
        if args.report is not None:
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(report, encoding="utf-8", newline="\n")
        sys.stdout.write(report)
    except (RayguiProvisionFailure, BindingFailure, OSError, UnicodeError, tarfile.TarError) as error:
        print(f"raygui-provision: ERROR: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
