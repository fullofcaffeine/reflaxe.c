#!/usr/bin/env python3
"""Provision and resolve the one reviewed raylib 6.0 native dependency."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import platform as host_platform
import re
import shlex
import shutil
import subprocess
import sys
import tarfile
import tempfile
import urllib.request
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Iterable, Mapping, Sequence


ROOT = Path(__file__).resolve().parents[2]
LOCK_PATH = ROOT / "docs/specs/raylib-provisioning-lock.json"
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")
REVISION_RE = re.compile(r"^[0-9a-f]{40}$")
SAFE_DEFINITION_RE = re.compile(r"^[A-Z][A-Z0-9_]*=[A-Za-z0-9.+_-]+$")
SAFE_LINK_NAME_RE = re.compile(r"^[A-Za-z0-9_][A-Za-z0-9_+.-]*$")
VERSION_MACROS = {
    "RAYLIB_VERSION_MAJOR": "6",
    "RAYLIB_VERSION_MINOR": "0",
    "RAYLIB_VERSION_PATCH": "0",
    "RAYLIB_VERSION": '"6.0"',
}
SUPPORTED_PLATFORMS = ("linux", "macos", "windows")
SUPPORTED_CONFIGURATIONS = ("memory-software", "desktop")
SUPPORTED_AUTHORITIES = (
    "pinned-source",
    "offline-source",
    "system-pkg-config",
)
PINNED_COMMIT = "dbc56a87da87d973a9c5baa4e7438a9d20121d28"
PINNED_ARCHIVE_SHA256 = "81b06ce7c19cf3b634b0271c23c361ba6ad8bf45fb8b036abbfeb4260ec1e126"
PINNED_ARCHIVE_SIZE = 52562578
PINNED_SOURCE_TREE = (
    "09abd409023b82fbe0723931aa751258b5052b058a2a1cf7c2613388ee309281",
    1384,
    97543278,
)
PINNED_REVIEWED_INPUTS = (
    ("CMakeLists.txt", "66c4dcfcdb20754e57cfa8eee23473d300c95405512417b09e41652b5b5263bd"),
    ("CMakeOptions.txt", "4f58628ebdcde3a8075ca5a10871ea2901ddf1f0b4486ca9a29b76b3d0c4b844"),
    ("LICENSE", "882a5a819cf562aa3583aae3af3f2211dda15c63de9fc8cc4b399a2f9e78d799"),
    ("cmake/CompileDefinitions.cmake", "068806d90e22a29e08e7cca18451ba776c79fa4676fa35c8dd5f68492129a402"),
    ("cmake/GlfwImport.cmake", "533dc29c0392035afe1790c8bb35d1b5a7dc83d9eae12fcc0a24ee31535b845f"),
    ("cmake/LibraryConfigurations.cmake", "d325845a807cb18e5585327cf29660750d15c07637fe7cacc8d88b1687ce3618"),
    ("src/CMakeLists.txt", "c4438de5db90fe00ccf7b62dbfcc4128fa08b9f1ef6306b62f019e1c50fc9d51"),
    ("src/Makefile", "91b091069276eaa062eff235e6990c9810ad64f0a199fb1586505b8713294e49"),
    ("src/config.h", "0495bce56ae6e1cd5c4e3e1e635fec8915c551e41ee3b62c7841537bed3878d7"),
    ("src/external/glfw/src/CMakeLists.txt", "2cb9fd73457e14a751f12fac3556c576fcffb54e25c462b5aeb7fe76d4d94aae"),
    ("src/external/rlsw.h", "c878af1036f0f20ad716be61f0f2269f33626fc101fd4ef36dd8c664b49836e9"),
    ("src/platforms/rcore_memory.c", "1f7f92694d696c81e631638d477354a275e1daf41dac31a89622aae1a66e2809"),
    ("src/raylib.h", "047e7255f93f8c34039cab906ad76136706b5c7b4c5b5b065d84141963ee9b6b"),
    ("src/raymath.h", "2b8b88f5b3f748e3cf8bdbfb8b7da23a76c755dc40f9c6e455bfc09b3669d028"),
    ("src/rlgl.h", "1c9b802c219971dbf2ba9725fb8cea3f14616c039891810a2d31aa442aea4be3"),
    (
        "tools/rlparser/output/raylib_api.json",
        "d4e2dd0727a8edf4d4f828e23f86e3bb74e373861edd0eabef4a634ae74936d6",
    ),
)
EXPECTED_CONFIGURATION_DEFINITIONS = {
    "memory-software": (
        "BUILD_EXAMPLES=OFF",
        "BUILD_SHARED_LIBS=OFF",
        "CMAKE_BUILD_TYPE=Release",
        "CMAKE_EXPORT_COMPILE_COMMANDS=ON",
        "CUSTOMIZE_BUILD=ON",
        "OPENGL_VERSION=Software",
        "PLATFORM=Memory",
        "SUPPORT_MODULE_RAUDIO=OFF",
        "USE_EXTERNAL_GLFW=OFF",
    ),
    "desktop": (
        "BUILD_EXAMPLES=OFF",
        "BUILD_SHARED_LIBS=OFF",
        "CMAKE_BUILD_TYPE=Release",
        "CMAKE_EXPORT_COMPILE_COMMANDS=ON",
        "CUSTOMIZE_BUILD=ON",
        "OPENGL_VERSION=3.3",
        "PLATFORM=Desktop",
        "SUPPORT_MODULE_RAUDIO=OFF",
        "USE_EXTERNAL_GLFW=OFF",
    ),
}
EXPECTED_LINK_FACTS = {
    ("linux", "memory-software"): (("raylib", "m"), (), ()),
    (
        "linux",
        "desktop",
    ): (
        ("raylib", "m", "pthread", "GL", "X11", "dl", "rt"),
        (),
        ("GLFW_BUILD_WAYLAND=OFF", "GLFW_BUILD_X11=ON"),
    ),
    ("macos", "memory-software"): (("raylib",), (), ()),
    (
        "macos",
        "desktop",
    ): (("raylib",), ("OpenGL", "Cocoa", "IOKit", "CoreFoundation"), ()),
    (
        "windows",
        "memory-software",
    ): (
        ("raylib", "winmm"),
        (),
        ("CMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL",),
    ),
    (
        "windows",
        "desktop",
    ): (
        ("raylib", "opengl32", "gdi32", "winmm", "shell32", "user32"),
        (),
        ("CMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL",),
    ),
}
EXPECTED_REPORT_PLACEHOLDERS = (
    "${CACHE_ROOT}",
    "${RAYLIB_SOURCE}",
    "${RAYLIB_BUILD}",
    "${GENERATED_ROOT}",
    "${FIXTURE_ROOT}",
    "${NATIVE_ROOT}",
    "${SYSTEM_INCLUDE}",
    "${SYSTEM_LIBRARY_DIR}",
    "${SYSTEM_LIBRARY}",
    "${C_COMPILER}",
    "${CXX_COMPILER}",
    "${CMAKE}",
    "${BUILD_TOOL}",
    "${PKG_CONFIG}",
    "${HAXE}",
    "${SUPPORT_INCLUDE}",
)
EXPECTED_FORBIDDEN_HOST_FACTS = (
    "absolute paths",
    "timestamps",
    "locale-formatted values",
    "shell command strings",
)


class ProvisionFailure(RuntimeError):
    """A reviewed authority could not be established."""


@dataclass(frozen=True)
class TreeIdentity:
    sha256: str
    file_count: int
    size_bytes: int


@dataclass(frozen=True)
class CommandResult:
    process: subprocess.CompletedProcess[bytes]
    evidence: dict[str, object]


@dataclass(frozen=True)
class SourceBuild:
    source_root: Path
    build_root: Path
    include_directory: Path
    library_file: Path
    report: dict[str, object]


@dataclass(frozen=True)
class SystemResolution:
    include_directories: tuple[Path, ...]
    library_file: Path
    cflags: tuple[str, ...]
    link_flags: tuple[str, ...]
    report: dict[str, object]


def sha256_bytes(value: bytes) -> str:
    return hashlib.sha256(value).hexdigest()


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def canonical_json(value: object) -> str:
    return json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True) + "\n"


def safe_relative_path(value: object, label: str) -> PurePosixPath:
    if not isinstance(value, str) or not value or "\\" in value:
        raise ProvisionFailure(f"{label} must be a non-empty POSIX relative path")
    relative = PurePosixPath(value)
    if (
        relative.is_absolute()
        or relative.as_posix() != value
        or any(part in ("", ".", "..") for part in relative.parts)
    ):
        raise ProvisionFailure(f"{label} must be a normalized POSIX relative path")
    return relative


def require_string(mapping: Mapping[str, object], key: str, label: str) -> str:
    value = mapping.get(key)
    if not isinstance(value, str) or not value:
        raise ProvisionFailure(f"{label}.{key} must be a non-empty string")
    return value


def require_integer(mapping: Mapping[str, object], key: str, label: str) -> int:
    value = mapping.get(key)
    if not isinstance(value, int) or isinstance(value, bool) or value < 0:
        raise ProvisionFailure(f"{label}.{key} must be a non-negative integer")
    return value


def require_exact_keys(
    mapping: Mapping[str, object], expected: Sequence[str], label: str
) -> None:
    actual = set(mapping)
    required = set(expected)
    if actual != required:
        raise ProvisionFailure(
            f"{label} keys drifted: expected {sorted(required)!r}, found {sorted(actual)!r}"
        )


def validate_lock(lock: Mapping[str, object]) -> None:
    require_exact_keys(
        lock,
        (
            "schemaVersion",
            "upstream",
            "reviewedInputs",
            "configurations",
            "platforms",
            "authorities",
            "report",
        ),
        "raylib lock",
    )
    if lock.get("schemaVersion") != 1:
        raise ProvisionFailure("raylib lock must use schemaVersion 1")
    upstream = lock.get("upstream")
    if not isinstance(upstream, dict):
        raise ProvisionFailure("raylib lock omitted upstream")
    require_exact_keys(
        upstream,
        (
            "name",
            "release",
            "packageVersion",
            "apiVersion",
            "commit",
            "archive",
            "sourceTree",
            "licenseExpression",
            "licenseUrl",
        ),
        "raylib lock upstream",
    )
    expected_scalars = {
        "name": "raylib",
        "release": "6.0",
        "packageVersion": "6.0.0",
        "apiVersion": 600,
        "licenseExpression": "Zlib",
    }
    for key, expected in expected_scalars.items():
        if upstream.get(key) != expected:
            raise ProvisionFailure(f"raylib lock upstream.{key} must equal {expected!r}")
    commit = require_string(upstream, "commit", "upstream")
    if REVISION_RE.fullmatch(commit) is None or commit != PINNED_COMMIT:
        raise ProvisionFailure(f"raylib lock commit must equal {PINNED_COMMIT}")
    license_url = require_string(upstream, "licenseUrl", "upstream")
    if license_url != f"https://github.com/raysan5/raylib/blob/{PINNED_COMMIT}/LICENSE":
        raise ProvisionFailure("raylib license URL must use the immutable locked commit")

    archive = upstream.get("archive")
    if not isinstance(archive, dict):
        raise ProvisionFailure("raylib lock omitted archive")
    require_exact_keys(
        archive,
        ("url", "sha256", "sizeBytes", "rootDirectory"),
        "raylib lock upstream.archive",
    )
    archive_url = require_string(archive, "url", "upstream.archive")
    archive_hash = require_string(archive, "sha256", "upstream.archive")
    archive_root = require_string(archive, "rootDirectory", "upstream.archive")
    archive_size = require_integer(archive, "sizeBytes", "upstream.archive")
    if archive_hash is None or SHA256_RE.fullmatch(archive_hash) is None:
        raise ProvisionFailure("raylib archive SHA-256 is malformed")
    if archive_size == 0:
        raise ProvisionFailure("raylib archive size must be positive")
    if archive_url != f"https://codeload.github.com/raysan5/raylib/tar.gz/{PINNED_COMMIT}":
        raise ProvisionFailure("raylib archive URL must equal the immutable commit archive")
    if archive_hash != PINNED_ARCHIVE_SHA256 or archive_size != PINNED_ARCHIVE_SIZE:
        raise ProvisionFailure("raylib archive size or SHA-256 drifted from the accepted 6.0 archive")
    if archive_root != "raylib-" + PINNED_COMMIT:
        raise ProvisionFailure("raylib archive root must encode the accepted commit")

    source_tree = upstream.get("sourceTree")
    if not isinstance(source_tree, dict):
        raise ProvisionFailure("raylib lock omitted sourceTree")
    require_exact_keys(
        source_tree,
        ("algorithm", "sha256", "fileCount", "sizeBytes"),
        "raylib lock upstream.sourceTree",
    )
    if source_tree.get("algorithm") != "hxc-path-size-content-sha256-v1":
        raise ProvisionFailure("raylib source-tree algorithm drifted")
    if SHA256_RE.fullmatch(require_string(source_tree, "sha256", "upstream.sourceTree")) is None:
        raise ProvisionFailure("raylib source-tree SHA-256 is malformed")
    source_file_count = require_integer(source_tree, "fileCount", "upstream.sourceTree")
    if source_file_count == 0:
        raise ProvisionFailure("raylib source tree must contain files")
    source_size = require_integer(source_tree, "sizeBytes", "upstream.sourceTree")
    if source_size == 0:
        raise ProvisionFailure("raylib source tree must contain bytes")
    if (
        source_tree.get("sha256"),
        source_file_count,
        source_size,
    ) != PINNED_SOURCE_TREE:
        raise ProvisionFailure("raylib source-tree identity drifted from the accepted 6.0 tree")

    reviewed = lock.get("reviewedInputs")
    if not isinstance(reviewed, list) or not reviewed:
        raise ProvisionFailure("raylib lock must list reviewed inputs")
    paths: list[str] = []
    for index, entry in enumerate(reviewed):
        if not isinstance(entry, dict):
            raise ProvisionFailure(f"reviewedInputs[{index}] must be an object")
        require_exact_keys(
            entry,
            ("path", "sha256"),
            f"raylib lock reviewedInputs[{index}]",
        )
        relative = safe_relative_path(entry.get("path"), f"reviewedInputs[{index}].path")
        digest = entry.get("sha256")
        if not isinstance(digest, str) or SHA256_RE.fullmatch(digest) is None:
            raise ProvisionFailure(f"reviewedInputs[{index}].sha256 is malformed")
        paths.append(relative.as_posix())
    if paths != sorted(paths, key=lambda value: value.encode("utf-8")):
        raise ProvisionFailure("reviewed raylib inputs must be UTF-8 sorted")
    if len(paths) != len(set(paths)):
        raise ProvisionFailure("reviewed raylib input paths must be unique")
    actual_reviewed = tuple(
        (entry["path"], entry["sha256"])
        for entry in reviewed
        if isinstance(entry, dict)
    )
    if actual_reviewed != PINNED_REVIEWED_INPUTS:
        raise ProvisionFailure("reviewed raylib input identities drifted from the accepted 6.0 inputs")
    for required in (
        "CMakeLists.txt",
        "CMakeOptions.txt",
        "LICENSE",
        "cmake/LibraryConfigurations.cmake",
        "src/CMakeLists.txt",
        "src/config.h",
        "src/platforms/rcore_memory.c",
        "src/raylib.h",
    ):
        if required not in paths:
            raise ProvisionFailure(f"raylib lock omitted build input {required}")

    configurations = lock.get("configurations")
    if not isinstance(configurations, dict) or set(configurations) != set(SUPPORTED_CONFIGURATIONS):
        raise ProvisionFailure("raylib lock must define exactly memory-software and desktop")
    expected_config = {
        "memory-software": ("Memory", "Software", True),
        "desktop": ("Desktop", "3.3", False),
    }
    for name, (platform_name, graphics, run_evidence) in expected_config.items():
        entry = configurations.get(name)
        if not isinstance(entry, dict):
            raise ProvisionFailure(f"raylib configuration {name} must be an object")
        require_exact_keys(
            entry,
            ("platform", "graphicsApi", "runEvidence", "cmakeDefinitions"),
            f"raylib lock configurations.{name}",
        )
        if (
            entry.get("platform") != platform_name
            or entry.get("graphicsApi") != graphics
            or entry.get("runEvidence") is not run_evidence
        ):
            raise ProvisionFailure(f"raylib configuration {name} identity drifted")
        definitions = validate_definitions(
            entry.get("cmakeDefinitions"), f"configurations.{name}"
        )
        if definitions != EXPECTED_CONFIGURATION_DEFINITIONS[name]:
            raise ProvisionFailure(f"raylib configuration {name} CMake definitions drifted")

    platforms = lock.get("platforms")
    if not isinstance(platforms, dict) or set(platforms) != set(SUPPORTED_PLATFORMS):
        raise ProvisionFailure("raylib lock must define exactly linux, macos, and windows")
    for platform_name in SUPPORTED_PLATFORMS:
        platform_entry = platforms.get(platform_name)
        if not isinstance(platform_entry, dict) or set(platform_entry) != set(SUPPORTED_CONFIGURATIONS):
            raise ProvisionFailure(f"raylib platform {platform_name} has an incomplete configuration matrix")
        for configuration in SUPPORTED_CONFIGURATIONS:
            facts = platform_entry.get(configuration)
            if not isinstance(facts, dict):
                raise ProvisionFailure(f"raylib link facts missing for {platform_name}/{configuration}")
            expected_libraries, expected_frameworks, expected_definitions = EXPECTED_LINK_FACTS[
                (platform_name, configuration)
            ]
            fact_keys = ["libraries", "frameworks"]
            if expected_definitions:
                fact_keys.append("cmakeDefinitions")
            require_exact_keys(
                facts,
                fact_keys,
                f"raylib lock platforms.{platform_name}.{configuration}",
            )
            libraries = facts.get("libraries")
            frameworks = facts.get("frameworks")
            if (
                not isinstance(libraries, list)
                or not libraries
                or libraries[0] != "raylib"
                or not all(isinstance(value, str) and SAFE_LINK_NAME_RE.fullmatch(value) for value in libraries)
                or len(libraries) != len(set(libraries))
            ):
                raise ProvisionFailure(f"raylib libraries are invalid for {platform_name}/{configuration}")
            if (
                not isinstance(frameworks, list)
                or not all(isinstance(value, str) and SAFE_LINK_NAME_RE.fullmatch(value) for value in frameworks)
                or len(frameworks) != len(set(frameworks))
            ):
                raise ProvisionFailure(f"raylib frameworks are invalid for {platform_name}/{configuration}")
            definitions = (
                validate_definitions(
                    facts["cmakeDefinitions"],
                    f"platforms.{platform_name}.{configuration}",
                )
                if "cmakeDefinitions" in facts
                else ()
            )
            if (
                tuple(libraries),
                tuple(frameworks),
                definitions,
            ) != (expected_libraries, expected_frameworks, expected_definitions):
                raise ProvisionFailure(
                    f"raylib link/configuration facts drifted for {platform_name}/{configuration}"
                )

    if lock.get("authorities") != list(SUPPORTED_AUTHORITIES):
        raise ProvisionFailure("raylib provisioning authorities drifted")
    report = lock.get("report")
    if not isinstance(report, dict) or report.get("schemaVersion") != 1:
        raise ProvisionFailure("raylib report policy must use schemaVersion 1")
    require_exact_keys(
        report,
        ("schemaVersion", "pathPlaceholders", "forbiddenHostFacts"),
        "raylib lock report",
    )
    placeholders = report.get("pathPlaceholders")
    if (
        not isinstance(placeholders, list)
        or not placeholders
        or not all(isinstance(value, str) and re.fullmatch(r"\$\{[A-Z_]+\}", value) for value in placeholders)
        or len(placeholders) != len(set(placeholders))
    ):
        raise ProvisionFailure("raylib report placeholders are invalid")
    if tuple(placeholders) != EXPECTED_REPORT_PLACEHOLDERS:
        raise ProvisionFailure("raylib report placeholders drifted")
    forbidden_host_facts = report.get("forbiddenHostFacts")
    if forbidden_host_facts != list(EXPECTED_FORBIDDEN_HOST_FACTS):
        raise ProvisionFailure("raylib forbidden host facts drifted")


def validate_definitions(value: object, label: str) -> tuple[str, ...]:
    if (
        not isinstance(value, list)
        or not value
        or not all(isinstance(entry, str) and SAFE_DEFINITION_RE.fullmatch(entry) for entry in value)
        or len(value) != len(set(value))
    ):
        raise ProvisionFailure(f"{label}.cmakeDefinitions must be unique NAME=value tokens")
    return tuple(value)


def load_lock(path: Path = LOCK_PATH) -> dict[str, object]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise ProvisionFailure(f"cannot read raylib lock: {error}") from error
    if not isinstance(value, dict):
        raise ProvisionFailure("raylib lock root must be an object")
    validate_lock(value)
    return value


def canonical_tree_identity(root: Path) -> TreeIdentity:
    if root.is_symlink() or not root.is_dir():
        raise ProvisionFailure(f"raylib source root is not a real directory: {root}")
    files: list[Path] = []
    for path in root.rglob("*"):
        relative = path.relative_to(root)
        if relative.parts and relative.parts[0] == ".git":
            continue
        if path.is_symlink():
            raise ProvisionFailure(f"raylib source tree contains a symlink: {relative.as_posix()}")
        if path.is_file():
            files.append(path)
        elif not path.is_dir():
            raise ProvisionFailure(f"raylib source tree contains a special file: {relative.as_posix()}")
    files.sort(key=lambda path: path.relative_to(root).as_posix().encode("utf-8"))
    digest = hashlib.sha256()
    size_bytes = 0
    for path in files:
        relative_bytes = path.relative_to(root).as_posix().encode("utf-8")
        content = path.read_bytes()
        digest.update(len(relative_bytes).to_bytes(8, "big"))
        digest.update(relative_bytes)
        digest.update(len(content).to_bytes(8, "big"))
        digest.update(hashlib.sha256(content).digest())
        size_bytes += len(content)
    return TreeIdentity(digest.hexdigest(), len(files), size_bytes)


def source_tree_expectation(lock: Mapping[str, object]) -> Mapping[str, object]:
    upstream = lock["upstream"]
    if not isinstance(upstream, dict) or not isinstance(upstream.get("sourceTree"), dict):
        raise ProvisionFailure("validated lock lost its source-tree identity")
    return upstream["sourceTree"]


def verify_source(source_root: Path, lock: Mapping[str, object]) -> TreeIdentity:
    source_root = source_root.resolve()
    if source_root.is_symlink() or not source_root.is_dir():
        raise ProvisionFailure(f"raylib source root is missing or not a real directory: {source_root}")
    reviewed = lock.get("reviewedInputs")
    if not isinstance(reviewed, list):
        raise ProvisionFailure("validated lock lost reviewed inputs")
    for entry in reviewed:
        if not isinstance(entry, dict):
            raise ProvisionFailure("validated lock contains an invalid reviewed input")
        relative = safe_relative_path(entry.get("path"), "reviewed input path")
        path = source_root.joinpath(*relative.parts)
        try:
            path.resolve(strict=False).relative_to(source_root)
        except ValueError as error:
            raise ProvisionFailure(f"reviewed raylib input escapes source root: {relative}") from error
        if path.is_symlink() or not path.is_file():
            raise ProvisionFailure(f"reviewed raylib input is missing or not a real file: {relative}")
        expected_hash = entry.get("sha256")
        if sha256_file(path) != expected_hash:
            raise ProvisionFailure(f"reviewed raylib input hash mismatch: {relative}")

    header = (source_root / "src/raylib.h").read_text(encoding="utf-8")
    for name, expected in VERSION_MACROS.items():
        match = re.search(rf"^#define\s+{re.escape(name)}\s+(.+?)\s*$", header, re.MULTILINE)
        if match is None or match.group(1) != expected:
            raise ProvisionFailure(f"raylib header version mismatch for {name}")

    actual = canonical_tree_identity(source_root)
    expected = source_tree_expectation(lock)
    if (
        actual.sha256 != expected.get("sha256")
        or actual.file_count != expected.get("fileCount")
        or actual.size_bytes != expected.get("sizeBytes")
    ):
        raise ProvisionFailure(
            "raylib source-tree identity mismatch: "
            f"sha256={actual.sha256} files={actual.file_count} bytes={actual.size_bytes}"
        )
    return actual


def archive_expectation(lock: Mapping[str, object]) -> Mapping[str, object]:
    upstream = lock.get("upstream")
    if not isinstance(upstream, dict) or not isinstance(upstream.get("archive"), dict):
        raise ProvisionFailure("validated lock lost its archive identity")
    return upstream["archive"]


def verify_archive(archive_path: Path, lock: Mapping[str, object]) -> None:
    expected = archive_expectation(lock)
    if archive_path.is_symlink() or not archive_path.is_file():
        raise ProvisionFailure(f"raylib archive is missing or not a real file: {archive_path}")
    actual_size = archive_path.stat().st_size
    if actual_size != expected.get("sizeBytes"):
        raise ProvisionFailure(
            f"raylib archive size mismatch: expected {expected.get('sizeBytes')}, found {actual_size}"
        )
    actual_hash = sha256_file(archive_path)
    if actual_hash != expected.get("sha256"):
        raise ProvisionFailure(
            f"raylib archive SHA-256 mismatch: expected {expected.get('sha256')}, found {actual_hash}"
        )


def download_archive(destination: Path, lock: Mapping[str, object]) -> None:
    expected = archive_expectation(lock)
    url = expected.get("url")
    expected_size = expected.get("sizeBytes")
    if not isinstance(url, str) or not isinstance(expected_size, int):
        raise ProvisionFailure("validated lock lost archive download facts")
    destination.parent.mkdir(parents=True, exist_ok=True)
    temporary = destination.with_name(destination.name + ".part")
    if temporary.exists():
        raise ProvisionFailure(f"partial raylib archive already exists; inspect or remove it: {temporary}")
    request = urllib.request.Request(url, headers={"User-Agent": "reflaxe.c-raylib-provision/1"})
    try:
        with urllib.request.urlopen(request, timeout=60) as response, temporary.open("xb") as output:
            total = 0
            while True:
                chunk = response.read(1024 * 1024)
                if not chunk:
                    break
                total += len(chunk)
                if total > expected_size:
                    raise ProvisionFailure("raylib archive exceeded its locked byte size")
                output.write(chunk)
        verify_archive(temporary, lock)
        os.replace(temporary, destination)
    except Exception:
        if temporary.exists():
            temporary.unlink()
        raise


def safe_extract_archive(archive_path: Path, sources_root: Path, lock: Mapping[str, object]) -> Path:
    verify_archive(archive_path, lock)
    expected = archive_expectation(lock)
    root_name = expected.get("rootDirectory")
    if not isinstance(root_name, str):
        raise ProvisionFailure("validated lock lost archive root")
    destination = sources_root / root_name
    if destination.exists() or destination.is_symlink():
        raise ProvisionFailure(f"raylib source destination already exists: {destination}")
    sources_root.mkdir(parents=True, exist_ok=True)
    staging_root = Path(tempfile.mkdtemp(prefix=".raylib-extract-", dir=sources_root))
    seen: set[str] = set()
    try:
        with tarfile.open(archive_path, mode="r:gz") as archive:
            for member in archive.getmembers():
                name = member.name.rstrip("/")
                if not name:
                    continue
                relative = PurePosixPath(name)
                if (
                    relative.is_absolute()
                    or any(part in ("", ".", "..") for part in relative.parts)
                    or not relative.parts
                    or relative.parts[0] != root_name
                ):
                    raise ProvisionFailure(f"raylib archive member escapes the locked root: {member.name!r}")
                normalized = relative.as_posix()
                if normalized in seen:
                    raise ProvisionFailure(f"raylib archive contains a duplicate member: {normalized}")
                seen.add(normalized)
                target = staging_root.joinpath(*relative.parts)
                try:
                    target.resolve(strict=False).relative_to(staging_root.resolve())
                except ValueError as error:
                    raise ProvisionFailure(f"raylib archive member escapes extraction: {normalized}") from error
                if member.isdir():
                    target.mkdir(parents=True, exist_ok=True)
                elif member.isfile():
                    target.parent.mkdir(parents=True, exist_ok=True)
                    source = archive.extractfile(member)
                    if source is None:
                        raise ProvisionFailure(f"cannot read raylib archive member: {normalized}")
                    with source, target.open("xb") as output:
                        shutil.copyfileobj(source, output, length=1024 * 1024)
                else:
                    raise ProvisionFailure(
                        f"raylib archive contains a link or special member: {normalized}"
                    )
        extracted = staging_root / root_name
        verify_source(extracted, lock)
        os.replace(extracted, destination)
        return destination
    finally:
        shutil.rmtree(staging_root, ignore_errors=True)


def pinned_source(cache_root: Path, lock: Mapping[str, object], allow_network: bool) -> Path:
    cache_root = cache_root.resolve()
    expected = archive_expectation(lock)
    root_name = expected.get("rootDirectory")
    if not isinstance(root_name, str):
        raise ProvisionFailure("validated lock lost archive root")
    archive_path = cache_root / "archives" / f"{root_name}.tar.gz"
    source_root = cache_root / "sources" / root_name
    if archive_path.exists():
        verify_archive(archive_path, lock)
    elif source_root.exists():
        raise ProvisionFailure(
            "pinned-source authority requires the locked archive alongside its extracted source"
        )
    elif allow_network:
        download_archive(archive_path, lock)
    else:
        raise ProvisionFailure(
            "pinned raylib archive is absent and network access was not explicitly enabled"
        )
    if source_root.exists():
        verify_source(source_root, lock)
        return source_root
    return safe_extract_archive(archive_path, cache_root / "sources", lock)


def path_replacements(values: Mapping[str, Path | str]) -> dict[str, str]:
    replacements: dict[str, str] = {}
    for placeholder, raw in values.items():
        text = str(raw)
        if not text:
            continue
        is_pathlike = isinstance(raw, Path) or Path(text).is_absolute() or "/" in text or "\\" in text
        if not is_pathlike:
            continue
        replacements[text] = placeholder
        replacements[text.replace("\\", "/")] = placeholder
        try:
            resolved = Path(text).resolve().as_posix()
        except OSError:
            continue
        replacements[resolved] = placeholder
    return replacements


def normalize_text(value: str, replacements: Mapping[str, str]) -> str:
    normalized = value.replace("\r\n", "\n").replace("\r", "\n").replace("\\", "/")
    for source in sorted(replacements, key=lambda item: (-len(item), item)):
        normalized = normalized.replace(source.replace("\\", "/"), replacements[source])
    return normalized


def normalize_argument(value: str, replacements: Mapping[str, str]) -> str:
    return normalize_text(value, replacements)


def stream_evidence(value: bytes, replacements: Mapping[str, str]) -> dict[str, object]:
    normalized = normalize_text(value.decode("utf-8", errors="replace"), replacements)
    return {
        "sha256": sha256_bytes(normalized.encode("utf-8")),
        "lineCount": len(normalized.splitlines()),
        "byteCount": len(normalized.encode("utf-8")),
    }


def run_command(
    arguments: Sequence[str],
    *,
    cwd: Path,
    replacements: Mapping[str, str],
    timeout: int,
    label: str,
    environment: Mapping[str, str] | None = None,
) -> CommandResult:
    if not arguments or not all(isinstance(value, str) and value and "\x00" not in value for value in arguments):
        raise ProvisionFailure(f"{label} must use a non-empty argument array")
    try:
        process = subprocess.run(
            list(arguments),
            cwd=cwd,
            env=dict(environment) if environment is not None else None,
            check=False,
            capture_output=True,
            timeout=timeout,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise ProvisionFailure(f"{label} could not run: {error}") from error
    evidence = {
        "arguments": [normalize_argument(value, replacements) for value in arguments],
        "exitCode": process.returncode,
        "stdout": stream_evidence(process.stdout, replacements),
        "stderr": stream_evidence(process.stderr, replacements),
    }
    if process.returncode != 0:
        output_sections = []
        for stream_name, stream in (("stdout", process.stdout), ("stderr", process.stderr)):
            normalized = normalize_text(
                stream.decode("utf-8", errors="replace"), replacements
            )
            tail = "\n".join(normalized.splitlines()[-12:])
            if tail:
                output_sections.append(f"{stream_name}:\n{tail}")
        joined_output = "\n".join(output_sections)
        suffix = f"\n{joined_output}" if joined_output else ""
        raise ProvisionFailure(f"{label} failed with exit {process.returncode}{suffix}")
    return CommandResult(process, evidence)


def first_nonempty_line(value: bytes, label: str) -> str:
    for line in value.decode("utf-8", errors="replace").splitlines():
        if line.strip():
            return line.strip()
    raise ProvisionFailure(f"{label} produced no identity line")


def tool_identity(
    executable: str,
    arguments: Sequence[str],
    *,
    cwd: Path,
    replacements: Mapping[str, str],
    label: str,
) -> tuple[dict[str, object], dict[str, object]]:
    result = run_command(
        [executable, *arguments],
        cwd=cwd,
        replacements=replacements,
        timeout=30,
        label=label,
    )
    return (
        {
            "command": normalize_argument(executable, replacements),
            "versionLine": normalize_text(first_nonempty_line(result.process.stdout, label), replacements),
        },
        result.evidence,
    )


def compiler_identity(
    compiler: str,
    *,
    cwd: Path,
    replacements: Mapping[str, str],
) -> tuple[dict[str, object], dict[str, object]]:
    result = run_command(
        [compiler, "--version"],
        cwd=cwd,
        replacements=replacements,
        timeout=30,
        label="C compiler identity",
    )
    text = result.process.stdout.decode("utf-8", errors="replace")
    version_line = first_nonempty_line(result.process.stdout, "C compiler identity")
    lower = (Path(compiler).name + " " + version_line).lower()
    if "clang-cl" in lower:
        family = "clang-cl"
    elif "apple clang" in lower or "appleclang" in lower:
        family = "apple-clang"
    elif "clang" in lower:
        family = "clang"
    elif "gcc" in lower or "free software foundation" in lower:
        family = "gcc"
    else:
        raise ProvisionFailure(f"unsupported C compiler identity: {version_line}")
    target = None
    for line in text.splitlines():
        if line.lower().startswith("target:"):
            target = line.split(":", 1)[1].strip()
            break
    target_evidence: dict[str, object] | None = None
    if target is None and family == "gcc":
        machine = run_command(
            [compiler, "-dumpmachine"],
            cwd=cwd,
            replacements=replacements,
            timeout=30,
            label="C compiler target",
        )
        target = first_nonempty_line(machine.process.stdout, "C compiler target")
        target_evidence = machine.evidence
    if target is None:
        raise ProvisionFailure("C compiler identity omitted its target tuple")
    identity: dict[str, object] = {
        "command": normalize_argument(compiler, replacements),
        "family": family,
        "versionLine": normalize_text(version_line, replacements),
        "target": normalize_text(target, replacements),
    }
    evidence: dict[str, object] = {"version": result.evidence}
    if target_evidence is not None:
        evidence["target"] = target_evidence
    return identity, evidence


def host_platform_key() -> str:
    value = host_platform.system()
    mapping = {"Linux": "linux", "Darwin": "macos", "Windows": "windows"}
    if value not in mapping:
        raise ProvisionFailure(f"unsupported raylib provisioning host: {value}")
    return mapping[value]


def configuration_definitions(
    lock: Mapping[str, object], platform_name: str, configuration: str
) -> tuple[str, ...]:
    configurations = lock.get("configurations")
    platforms = lock.get("platforms")
    if not isinstance(configurations, dict) or not isinstance(platforms, dict):
        raise ProvisionFailure("validated lock lost configuration facts")
    base = configurations.get(configuration)
    platform_entry = platforms.get(platform_name)
    if not isinstance(base, dict) or not isinstance(platform_entry, dict):
        raise ProvisionFailure(f"unknown raylib configuration: {platform_name}/{configuration}")
    facts = platform_entry.get(configuration)
    if not isinstance(facts, dict):
        raise ProvisionFailure(f"unknown raylib configuration: {platform_name}/{configuration}")
    result = list(validate_definitions(base.get("cmakeDefinitions"), f"configurations.{configuration}"))
    if "cmakeDefinitions" in facts:
        result.extend(validate_definitions(facts["cmakeDefinitions"], f"platforms.{platform_name}.{configuration}"))
    if len(result) != len(set(result)):
        raise ProvisionFailure(f"duplicate combined CMake definition for {platform_name}/{configuration}")
    return tuple(result)


def link_facts(
    lock: Mapping[str, object], platform_name: str, configuration: str
) -> tuple[tuple[str, ...], tuple[str, ...]]:
    platforms = lock.get("platforms")
    if not isinstance(platforms, dict):
        raise ProvisionFailure("validated lock lost platform link facts")
    platform_entry = platforms.get(platform_name)
    facts = platform_entry.get(configuration) if isinstance(platform_entry, dict) else None
    if not isinstance(facts, dict):
        raise ProvisionFailure(f"unknown raylib link facts: {platform_name}/{configuration}")
    libraries = facts.get("libraries")
    frameworks = facts.get("frameworks")
    if not isinstance(libraries, list) or not isinstance(frameworks, list):
        raise ProvisionFailure("validated lock lost library/framework arrays")
    return tuple(libraries), tuple(frameworks)


def locate_raylib_library(build_root: Path, platform_name: str) -> Path:
    names = ("raylib.lib",) if platform_name == "windows" else ("libraylib.a",)
    candidates = sorted(
        (
            path
            for name in names
            for path in build_root.rglob(name)
            if path.is_file() and not path.is_symlink()
        ),
        key=lambda path: path.as_posix().encode("utf-8"),
    )
    if len(candidates) != 1:
        rendered = ", ".join(path.as_posix() for path in candidates) or "none"
        raise ProvisionFailure(f"expected exactly one static raylib library; found {rendered}")
    return candidates[0]


def source_report_inputs(lock: Mapping[str, object]) -> list[dict[str, object]]:
    reviewed = lock.get("reviewedInputs")
    if not isinstance(reviewed, list):
        raise ProvisionFailure("validated lock lost reviewed inputs")
    return [dict(entry) for entry in reviewed if isinstance(entry, dict)]


def assert_report_redacted(report: object, forbidden: Sequence[Path | str]) -> None:
    rendered = canonical_json(report).replace("\\", "/")
    for raw in forbidden:
        value = str(raw)
        if not value:
            continue
        is_pathlike = isinstance(raw, Path) or Path(value).is_absolute() or "/" in value or "\\" in value
        if not is_pathlike:
            continue
        candidates = {value.replace("\\", "/")}
        try:
            candidates.add(Path(value).resolve().as_posix())
        except OSError:
            pass
        for candidate in candidates:
            if candidate and candidate in rendered:
                raise ProvisionFailure(f"normalized report leaked a host path: {candidate}")


def write_report(path: Path, report: Mapping[str, object], forbidden: Sequence[Path | str]) -> None:
    assert_report_redacted(report, forbidden)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(canonical_json(report), encoding="utf-8", newline="\n")


def build_source(
    *,
    lock: Mapping[str, object],
    authority: str,
    configuration: str,
    platform_name: str,
    build_root: Path,
    compiler: str,
    cxx_compiler: str,
    cmake: str,
    generator: str,
    cache_root: Path | None = None,
    source_root: Path | None = None,
    allow_network: bool = False,
) -> SourceBuild:
    if authority not in ("pinned-source", "offline-source"):
        raise ProvisionFailure("source build authority must be pinned-source or offline-source")
    if configuration not in SUPPORTED_CONFIGURATIONS:
        raise ProvisionFailure(f"unknown raylib configuration: {configuration}")
    if platform_name not in SUPPORTED_PLATFORMS:
        raise ProvisionFailure(f"unknown raylib platform: {platform_name}")
    actual_platform = host_platform_key()
    if platform_name != actual_platform:
        raise ProvisionFailure(
            f"requested platform {platform_name} does not match provisioning host {actual_platform}"
        )
    if authority == "pinned-source":
        if cache_root is None:
            raise ProvisionFailure("pinned-source authority requires --cache-root")
        if source_root is not None:
            raise ProvisionFailure("pinned-source authority does not accept --source")
        source_root = pinned_source(cache_root, lock, allow_network)
    else:
        if source_root is None:
            raise ProvisionFailure("offline-source authority requires --source")
        if cache_root is not None:
            raise ProvisionFailure("offline-source authority does not accept --cache-root")
        if allow_network:
            raise ProvisionFailure("offline-source authority rejects --allow-network")
        source_root = source_root.resolve()
        verify_source(source_root, lock)

    build_root = build_root.resolve()
    if build_root.exists() and any(build_root.iterdir()):
        raise ProvisionFailure(f"raylib build root must be empty: {build_root}")
    build_root.mkdir(parents=True, exist_ok=True)
    if source_root == build_root or source_root in build_root.parents or build_root in source_root.parents:
        raise ProvisionFailure("raylib source and build roots must be disjoint")

    replacements = path_replacements(
        {
            "${CACHE_ROOT}": cache_root.resolve() if cache_root is not None else "",
            "${RAYLIB_SOURCE}": source_root,
            "${RAYLIB_BUILD}": build_root,
            "${C_COMPILER}": compiler,
            "${CXX_COMPILER}": cxx_compiler,
            "${CMAKE}": cmake,
        }
    )
    compiler_info, compiler_commands = compiler_identity(
        compiler, cwd=build_root, replacements=replacements
    )
    cxx_info, cxx_command = tool_identity(
        cxx_compiler,
        ["--version"],
        cwd=build_root,
        replacements=replacements,
        label="C++ compiler identity",
    )
    cmake_info, cmake_command = tool_identity(
        cmake,
        ["--version"],
        cwd=build_root,
        replacements=replacements,
        label="CMake identity",
    )
    if generator == "Ninja":
        build_tool = "ninja"
    elif generator == "Unix Makefiles":
        build_tool = "make"
    else:
        raise ProvisionFailure("raylib source builds support only Ninja or Unix Makefiles")
    replacements.update(path_replacements({"${BUILD_TOOL}": build_tool}))
    build_tool_info, build_tool_command = tool_identity(
        build_tool,
        ["--version"],
        cwd=build_root,
        replacements=replacements,
        label="native build-tool identity",
    )

    definitions = configuration_definitions(lock, platform_name, configuration)
    configure_arguments = [
        cmake,
        "-S",
        str(source_root),
        "-B",
        str(build_root),
        "-G",
        generator,
        *[f"-D{definition}" for definition in definitions],
        f"-DCMAKE_C_COMPILER={compiler}",
        f"-DCMAKE_CXX_COMPILER={cxx_compiler}",
    ]
    configure = run_command(
        configure_arguments,
        cwd=build_root,
        replacements=replacements,
        timeout=300,
        label="raylib CMake configure",
    )
    build = run_command(
        [cmake, "--build", str(build_root), "--target", "raylib", "--parallel", "2"],
        cwd=build_root,
        replacements=replacements,
        timeout=900,
        label="raylib static-library build",
    )

    library_file = locate_raylib_library(build_root, platform_name)
    include_directory = source_root / "src"
    header = include_directory / "raylib.h"
    libraries, frameworks = link_facts(lock, platform_name, configuration)
    source_identity = canonical_tree_identity(source_root)
    compile_commands = build_root / "compile_commands.json"
    if compile_commands.is_symlink() or not compile_commands.is_file():
        raise ProvisionFailure("raylib build did not emit compile_commands.json")
    upstream = lock.get("upstream")
    if not isinstance(upstream, dict):
        raise ProvisionFailure("validated lock lost upstream identity")
    report: dict[str, object] = {
        "schemaVersion": 1,
        "authority": authority,
        "upstream": {
            "name": upstream["name"],
            "release": upstream["release"],
            "packageVersion": upstream["packageVersion"],
            "apiVersion": upstream["apiVersion"],
            "commit": upstream["commit"],
            "archiveSha256": archive_expectation(lock)["sha256"],
            "sourceTree": {
                "algorithm": "hxc-path-size-content-sha256-v1",
                "sha256": source_identity.sha256,
                "fileCount": source_identity.file_count,
                "sizeBytes": source_identity.size_bytes,
            },
        },
        "target": {
            "platform": platform_name,
            "architecture": host_platform.machine().lower(),
            "compiler": compiler_info,
            "cxxCompiler": cxx_info,
            "cmake": cmake_info,
            "generator": generator,
            "buildTool": build_tool_info,
        },
        "configuration": {
            "id": configuration,
            "cmakeDefinitions": list(definitions),
        },
        "commands": {
            "compilerIdentity": compiler_commands,
            "cxxCompilerIdentity": cxx_command,
            "cmakeIdentity": cmake_command,
            "buildToolIdentity": build_tool_command,
            "configure": configure.evidence,
            "build": build.evidence,
        },
        "inputs": source_report_inputs(lock),
        "outputs": {
            "includeDirectories": ["${RAYLIB_SOURCE}/src"],
            "headers": [
                {
                    "path": "${RAYLIB_SOURCE}/src/raylib.h",
                    "sha256": sha256_file(header),
                }
            ],
            "library": {
                "path": normalize_argument(str(library_file), replacements),
                "sha256": sha256_file(library_file),
                "sizeBytes": library_file.stat().st_size,
            },
            "normalizedCompileCommandsSha256": sha256_bytes(
                normalize_text(
                    compile_commands.read_text(encoding="utf-8"), replacements
                ).encode("utf-8")
            ),
            "libraries": list(libraries),
            "frameworks": list(frameworks),
        },
        "claims": {
            "archiveVerified": authority == "pinned-source",
            "sourceTreeVerified": True,
            "raylibBuilt": True,
            "generatedCCompiled": False,
            "linked": False,
            "executed": False,
        },
    }
    forbidden: list[Path | str] = [source_root, build_root, compiler, cxx_compiler, cmake, build_tool]
    if cache_root is not None:
        forbidden.append(cache_root.resolve())
    assert_report_redacted(report, forbidden)
    return SourceBuild(source_root, build_root, include_directory, library_file, report)


def split_pkg_config_flags(value: str, label: str) -> tuple[str, ...]:
    try:
        result = tuple(shlex.split(value, posix=os.name != "nt"))
    except ValueError as error:
        raise ProvisionFailure(f"cannot parse {label} argument array: {error}") from error
    for item in result:
        if not item or "\x00" in item or "\n" in item or "\r" in item:
            raise ProvisionFailure(f"{label} returned a malformed argument")
    return result


def pkg_config_value(
    executable: str,
    arguments: Sequence[str],
    *,
    cwd: Path,
    replacements: Mapping[str, str],
    label: str,
) -> tuple[str, dict[str, object]]:
    result = run_command(
        [executable, *arguments],
        cwd=cwd,
        replacements=replacements,
        timeout=30,
        label=label,
    )
    value = result.process.stdout.decode("utf-8", errors="strict").strip()
    if not value:
        raise ProvisionFailure(f"{label} returned an empty value")
    return value, result.evidence


def find_system_library(libdir: Path, explicit: Path | None) -> Path:
    if explicit is not None:
        candidate = explicit.resolve()
        if candidate.is_symlink():
            candidate = candidate.resolve(strict=True)
        if not candidate.is_file():
            raise ProvisionFailure(f"explicit system raylib library is missing: {candidate}")
        return candidate
    candidates: list[Path] = []
    for name in ("libraylib.a", "libraylib.so", "libraylib.dylib", "raylib.lib"):
        path = libdir / name
        if path.exists():
            resolved = path.resolve(strict=True)
            if resolved.is_file() and resolved not in candidates:
                candidates.append(resolved)
    if len(candidates) != 1:
        raise ProvisionFailure(
            "system pkg-config resolution needs exactly one raylib library; "
            "pass --library-file when static/shared candidates are ambiguous"
        )
    return candidates[0]


def resolve_system_pkg_config(
    *,
    lock: Mapping[str, object],
    pkg_config: str,
    configuration: str,
    work_root: Path,
    explicit_library: Path | None = None,
) -> SystemResolution:
    if configuration != "desktop":
        raise ProvisionFailure(
            "system-pkg-config authority cannot prove PLATFORM_MEMORY/software configuration"
        )
    work_root = work_root.resolve()
    work_root.mkdir(parents=True, exist_ok=True)
    initial_replacements = path_replacements({"${PKG_CONFIG}": pkg_config})
    pkg_identity, identity_command = tool_identity(
        pkg_config,
        ["--version"],
        cwd=work_root,
        replacements=initial_replacements,
        label="pkg-config identity",
    )
    version, version_command = pkg_config_value(
        pkg_config,
        ["--modversion", "raylib"],
        cwd=work_root,
        replacements=initial_replacements,
        label="raylib pkg-config version",
    )
    upstream = lock.get("upstream")
    if not isinstance(upstream, dict) or version != upstream.get("packageVersion"):
        raise ProvisionFailure(
            f"raylib pkg-config version mismatch: expected {upstream.get('packageVersion') if isinstance(upstream, dict) else '6.0.0'}, found {version}"
        )
    cflags_text, cflags_command = pkg_config_value(
        pkg_config,
        ["--cflags", "raylib"],
        cwd=work_root,
        replacements=initial_replacements,
        label="raylib pkg-config cflags",
    )
    libs_text, libs_command = pkg_config_value(
        pkg_config,
        ["--libs", "raylib"],
        cwd=work_root,
        replacements=initial_replacements,
        label="raylib pkg-config libraries",
    )
    includedir_text, includedir_command = pkg_config_value(
        pkg_config,
        ["--variable=includedir", "raylib"],
        cwd=work_root,
        replacements=initial_replacements,
        label="raylib pkg-config include directory",
    )
    libdir_text, libdir_command = pkg_config_value(
        pkg_config,
        ["--variable=libdir", "raylib"],
        cwd=work_root,
        replacements=initial_replacements,
        label="raylib pkg-config library directory",
    )
    cflags = split_pkg_config_flags(cflags_text, "pkg-config cflags")
    link_flags = split_pkg_config_flags(libs_text, "pkg-config libraries")
    include_directories = [Path(includedir_text).resolve()]
    for index, value in enumerate(cflags):
        if value == "-I" and index + 1 < len(cflags):
            include_directories.append(Path(cflags[index + 1]).resolve())
        elif value.startswith("-I") and len(value) > 2:
            include_directories.append(Path(value[2:]).resolve())
    include_directories = list(dict.fromkeys(include_directories))
    reviewed = lock.get("reviewedInputs")
    header_hash = None
    if isinstance(reviewed, list):
        for entry in reviewed:
            if isinstance(entry, dict) and entry.get("path") == "src/raylib.h":
                header_hash = entry.get("sha256")
                break
    if not isinstance(header_hash, str):
        raise ProvisionFailure("validated lock lost raylib.h identity")
    matching_headers = [
        directory / "raylib.h"
        for directory in include_directories
        if (directory / "raylib.h").is_file()
        and not (directory / "raylib.h").is_symlink()
        and sha256_file(directory / "raylib.h") == header_hash
    ]
    if len(matching_headers) != 1:
        raise ProvisionFailure("system pkg-config did not resolve exactly one locked raylib.h")
    library_file = find_system_library(Path(libdir_text).resolve(), explicit_library)
    replacements = path_replacements(
        {
            "${PKG_CONFIG}": pkg_config,
            "${SYSTEM_INCLUDE}": matching_headers[0].parent,
            "${SYSTEM_LIBRARY_DIR}": Path(libdir_text).resolve(),
            "${SYSTEM_LIBRARY}": library_file,
        }
    )
    replacements.update(
        path_replacements(
            {
                "${SYSTEM_INCLUDE}": includedir_text,
                "${SYSTEM_LIBRARY_DIR}": libdir_text,
            }
        )
    )
    normalized_cflags = [normalize_argument(value, replacements) for value in cflags]
    normalized_link_flags = [normalize_argument(value, replacements) for value in link_flags]
    commands = {
        "identity": identity_command,
        "version": version_command,
        "cflags": cflags_command,
        "libraries": libs_command,
        "includeDirectory": includedir_command,
        "libraryDirectory": libdir_command,
    }
    commands = json.loads(normalize_text(canonical_json(commands), replacements))
    report: dict[str, object] = {
        "schemaVersion": 1,
        "authority": "system-pkg-config",
        "upstream": {
            "name": "raylib",
            "release": "6.0",
            "packageVersion": version,
            "apiVersion": 600,
            "commit": None,
            "archiveSha256": None,
            "sourceTree": None,
        },
        "target": {
            "platform": host_platform_key(),
            "architecture": host_platform.machine().lower(),
            "pkgConfig": pkg_identity,
        },
        "configuration": {
            "id": "desktop",
            "cmakeDefinitions": [],
        },
        "commands": commands,
        "inputs": [
            {
                "path": "${SYSTEM_INCLUDE}/raylib.h",
                "sha256": header_hash,
            },
            {
                "path": "${SYSTEM_LIBRARY}",
                "sha256": sha256_file(library_file),
            },
        ],
        "outputs": {
            "includeDirectories": ["${SYSTEM_INCLUDE}"],
            "headers": [
                {
                    "path": "${SYSTEM_INCLUDE}/raylib.h",
                    "sha256": header_hash,
                }
            ],
            "library": {
                "path": "${SYSTEM_LIBRARY}",
                "sha256": sha256_file(library_file),
                "sizeBytes": library_file.stat().st_size,
            },
            "normalizedCompileCommandsSha256": None,
            "libraries": [],
            "frameworks": [],
            "pkgConfigPackages": ["raylib"],
            "cflags": normalized_cflags,
            "linkFlags": normalized_link_flags,
        },
        "claims": {
            "archiveVerified": False,
            "sourceTreeVerified": False,
            "raylibBuilt": False,
            "generatedCCompiled": False,
            "linked": False,
            "executed": False,
        },
    }
    assert_report_redacted(
        report,
        [*include_directories, Path(libdir_text), library_file, pkg_config],
    )
    return SystemResolution(
        tuple(include_directories),
        library_file,
        cflags,
        link_flags,
        report,
    )


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--lock",
        type=Path,
        default=LOCK_PATH,
        help="reviewed lock JSON (defaults to the repository authority)",
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser("verify-lock", help="validate the repository lock without provisioning")

    source = subparsers.add_parser("build-source", help="verify, configure, and build pinned source")
    source.add_argument("--authority", choices=("pinned-source", "offline-source"), required=True)
    source.add_argument("--configuration", choices=SUPPORTED_CONFIGURATIONS, required=True)
    source.add_argument("--platform", choices=SUPPORTED_PLATFORMS, required=True)
    source.add_argument("--cache-root", type=Path)
    source.add_argument("--source", type=Path)
    source.add_argument("--build-root", type=Path, required=True)
    source.add_argument("--cc", required=True)
    source.add_argument("--cxx", required=True)
    source.add_argument("--cmake", default="cmake")
    source.add_argument("--generator", choices=("Ninja", "Unix Makefiles"), required=True)
    source.add_argument("--allow-network", action="store_true")
    source.add_argument("--report", type=Path, required=True)

    system = subparsers.add_parser("resolve-system", help="validate an explicit pkg-config authority")
    system.add_argument("--configuration", choices=SUPPORTED_CONFIGURATIONS, required=True)
    system.add_argument("--pkg-config", required=True)
    system.add_argument("--work-root", type=Path, required=True)
    system.add_argument("--library-file", type=Path)
    system.add_argument("--report", type=Path, required=True)
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    try:
        args = parse_args(argv)
        lock = load_lock(args.lock)
        if args.command == "verify-lock":
            print("raylib-provision: OK: raylib 6.0 archive, tree, configuration, and platform lock")
            return 0
        if args.command == "build-source":
            result = build_source(
                lock=lock,
                authority=args.authority,
                configuration=args.configuration,
                platform_name=args.platform,
                cache_root=args.cache_root,
                source_root=args.source,
                build_root=args.build_root,
                compiler=args.cc,
                cxx_compiler=args.cxx,
                cmake=args.cmake,
                generator=args.generator,
                allow_network=args.allow_network,
            )
            forbidden: list[Path | str] = [
                result.source_root,
                result.build_root,
                args.cc,
                args.cxx,
                args.cmake,
            ]
            if args.cache_root is not None:
                forbidden.append(args.cache_root.resolve())
            write_report(args.report, result.report, forbidden)
            print(
                "raylib-provision: OK: "
                f"{args.authority} {args.configuration} {args.platform}; report={args.report}"
            )
            return 0
        if args.command == "resolve-system":
            result = resolve_system_pkg_config(
                lock=lock,
                pkg_config=args.pkg_config,
                configuration=args.configuration,
                work_root=args.work_root,
                explicit_library=args.library_file,
            )
            write_report(
                args.report,
                result.report,
                [*result.include_directories, result.library_file, args.pkg_config],
            )
            print(f"raylib-provision: OK: system-pkg-config desktop; report={args.report}")
            return 0
        raise ProvisionFailure(f"unknown command: {args.command}")
    except (OSError, UnicodeError, json.JSONDecodeError, ProvisionFailure) as error:
        print(f"raylib-provision: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
