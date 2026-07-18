#!/usr/bin/env python3
"""Adversarial tests for the raylib provisioning trust boundaries."""

from __future__ import annotations

import copy
import io
import tarfile
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from scripts.raylib.provision import (
    ProvisionFailure,
    assert_report_redacted,
    build_source,
    canonical_tree_identity,
    find_system_library,
    host_platform_key,
    load_lock,
    normalize_text,
    path_replacements,
    pinned_source,
    resolve_system_pkg_config,
    safe_extract_archive,
    sha256_file,
    split_pkg_config_flags,
    validate_lock,
    verify_archive,
    verify_source,
)


ARCHIVE_ROOT = "raylib-" + "a" * 40
VERSION_HEADER = """\
#define RAYLIB_VERSION_MAJOR 6
#define RAYLIB_VERSION_MINOR 0
#define RAYLIB_VERSION_PATCH 0
#define RAYLIB_VERSION \"6.0\"
"""


def archive_lock(path: Path) -> dict[str, object]:
    return {
        "upstream": {
            "archive": {
                "url": "https://invalid.example/raylib.tar.gz",
                "sha256": sha256_file(path),
                "sizeBytes": path.stat().st_size,
                "rootDirectory": ARCHIVE_ROOT,
            }
        }
    }


def write_archive(
    path: Path, members: list[tuple[tarfile.TarInfo, bytes | None]]
) -> None:
    with tarfile.open(path, mode="w:gz") as archive:
        for member, content in members:
            if isinstance(content, bytes):
                member.size = len(content)
                archive.addfile(member, io.BytesIO(content))
            else:
                archive.addfile(member)


def file_member(
    name: str, content: bytes = b"fixture"
) -> tuple[tarfile.TarInfo, bytes]:
    return tarfile.TarInfo(name), content


def synthetic_source_lock(root: Path) -> dict[str, object]:
    identity = canonical_tree_identity(root)
    header = root / "src/raylib.h"
    return {
        "upstream": {
            "sourceTree": {
                "algorithm": "hxc-path-size-content-sha256-v1",
                "sha256": identity.sha256,
                "fileCount": identity.file_count,
                "sizeBytes": identity.size_bytes,
            }
        },
        "reviewedInputs": [
            {
                "path": "src/raylib.h",
                "sha256": sha256_file(header),
            }
        ],
    }


class RaylibProvisioningTests(unittest.TestCase):
    def test_repository_lock_rejects_semantic_version_drift(self) -> None:
        lock = copy.deepcopy(load_lock())
        upstream = lock["upstream"]
        self.assertIsInstance(upstream, dict)
        upstream["release"] = "6.1"
        with self.assertRaisesRegex(ProvisionFailure, "upstream.release"):
            validate_lock(lock)

    def test_repository_lock_rejects_pin_configuration_and_link_drift(self) -> None:
        cases: list[tuple[str, dict[str, object], str]] = []

        revision = copy.deepcopy(load_lock())
        revision_upstream = revision["upstream"]
        self.assertIsInstance(revision_upstream, dict)
        revision_upstream["commit"] = "b" * 40
        cases.append(("revision", revision, "commit must equal"))

        configuration = copy.deepcopy(load_lock())
        configurations = configuration["configurations"]
        self.assertIsInstance(configurations, dict)
        desktop = configurations["desktop"]
        self.assertIsInstance(desktop, dict)
        definitions = desktop["cmakeDefinitions"]
        self.assertIsInstance(definitions, list)
        definitions.remove("SUPPORT_MODULE_RAUDIO=OFF")
        cases.append(("configuration", configuration, "CMake definitions drifted"))

        link = copy.deepcopy(load_lock())
        platforms = link["platforms"]
        self.assertIsInstance(platforms, dict)
        linux = platforms["linux"]
        self.assertIsInstance(linux, dict)
        linux_desktop = linux["desktop"]
        self.assertIsInstance(linux_desktop, dict)
        libraries = linux_desktop["libraries"]
        self.assertIsInstance(libraries, list)
        libraries.reverse()
        cases.append(("link", link, "libraries are invalid"))

        report = copy.deepcopy(load_lock())
        report_policy = report["report"]
        self.assertIsInstance(report_policy, dict)
        placeholders = report_policy["pathPlaceholders"]
        self.assertIsInstance(placeholders, list)
        placeholders.reverse()
        cases.append(("report", report, "report placeholders drifted"))

        for label, candidate, message in cases:
            with self.subTest(label=label), self.assertRaisesRegex(
                ProvisionFailure, message
            ):
                validate_lock(candidate)

    def test_archive_rejects_size_and_hash_drift(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-archive-") as raw:
            archive = Path(raw) / "raylib.tar.gz"
            archive.write_bytes(b"locked bytes")
            lock = archive_lock(archive)
            verify_archive(archive, lock)
            archive.write_bytes(b"changed bytes")
            with self.assertRaisesRegex(ProvisionFailure, "size mismatch"):
                verify_archive(archive, lock)
            archive.write_bytes(b"x" * len(b"locked bytes"))
            with self.assertRaisesRegex(ProvisionFailure, "SHA-256 mismatch"):
                verify_archive(archive, lock)

    def test_archive_rejects_parent_traversal_without_writing_outside(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-traversal-") as raw:
            root = Path(raw)
            archive = root / "raylib.tar.gz"
            write_archive(
                archive,
                [file_member(f"{ARCHIVE_ROOT}/../escaped.txt", b"unsafe")],
            )
            sources = root / "sources"
            with self.assertRaisesRegex(ProvisionFailure, "escapes the locked root"):
                safe_extract_archive(archive, sources, archive_lock(archive))
            self.assertFalse((root / "escaped.txt").exists())

    def test_archive_rejects_links_and_duplicate_members(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-members-") as raw:
            root = Path(raw)
            link_archive = root / "link.tar.gz"
            link = tarfile.TarInfo(f"{ARCHIVE_ROOT}/link")
            link.type = tarfile.SYMTYPE
            link.linkname = "/outside"
            write_archive(link_archive, [(link, None)])
            with self.assertRaisesRegex(ProvisionFailure, "link or special member"):
                safe_extract_archive(link_archive, root / "link-sources", archive_lock(link_archive))

            duplicate_archive = root / "duplicate.tar.gz"
            duplicate_name = f"{ARCHIVE_ROOT}/src/raylib.h"
            write_archive(
                duplicate_archive,
                [file_member(duplicate_name, b"first"), file_member(duplicate_name, b"second")],
            )
            with self.assertRaisesRegex(ProvisionFailure, "duplicate member"):
                safe_extract_archive(
                    duplicate_archive,
                    root / "duplicate-sources",
                    archive_lock(duplicate_archive),
                )

    def test_full_source_identity_catches_unreviewed_file_drift(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-source-") as raw:
            root = Path(raw)
            (root / "src").mkdir()
            (root / "src/raylib.h").write_text(VERSION_HEADER, encoding="utf-8")
            (root / "unreviewed.txt").write_text("original", encoding="utf-8")
            lock = synthetic_source_lock(root)
            verify_source(root, lock)
            (root / "unreviewed.txt").write_text("changed", encoding="utf-8")
            with self.assertRaisesRegex(ProvisionFailure, "source-tree identity mismatch"):
                verify_source(root, lock)

    def test_pinned_source_requires_explicit_network_authority(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-cache-") as raw:
            archive = Path(raw) / "identity"
            archive.write_bytes(b"identity")
            lock = archive_lock(archive)
            with self.assertRaisesRegex(ProvisionFailure, "network access was not explicitly enabled"):
                pinned_source(Path(raw) / "empty-cache", lock, False)

    def test_source_authorities_do_not_mix_inputs(self) -> None:
        lock = load_lock()
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-authority-") as raw:
            root = Path(raw)
            common = {
                "lock": lock,
                "configuration": "desktop",
                "platform_name": host_platform_key(),
                "build_root": root / "build",
                "compiler": "unused-cc",
                "cxx_compiler": "unused-cxx",
                "cmake": "unused-cmake",
                "generator": "Ninja",
            }
            with self.assertRaisesRegex(ProvisionFailure, "rejects --allow-network"):
                build_source(
                    **common,
                    authority="offline-source",
                    source_root=root / "source",
                    allow_network=True,
                )
            with self.assertRaisesRegex(ProvisionFailure, "does not accept --cache-root"):
                build_source(
                    **common,
                    authority="offline-source",
                    cache_root=root / "cache",
                    source_root=root / "source",
                )
            with self.assertRaisesRegex(ProvisionFailure, "does not accept --source"):
                build_source(
                    **common,
                    authority="pinned-source",
                    cache_root=root / "cache",
                    source_root=root / "source",
                )

    def test_system_authority_rejects_headless_configuration(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-system-") as raw:
            with self.assertRaisesRegex(ProvisionFailure, "cannot prove PLATFORM_MEMORY"):
                resolve_system_pkg_config(
                    lock=load_lock(),
                    pkg_config="unused-pkg-config",
                    configuration="memory-software",
                    work_root=Path(raw),
                )

    def test_system_authority_rejects_version_mismatch_before_flags(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-system-version-") as raw:
            with patch(
                "scripts.raylib.provision.tool_identity",
                return_value=({"command": "pkg-config", "versionLine": "2.0"}, {}),
            ), patch(
                "scripts.raylib.provision.pkg_config_value",
                return_value=("6.1.0", {}),
            ):
                with self.assertRaisesRegex(ProvisionFailure, "expected 6.0.0, found 6.1.0"):
                    resolve_system_pkg_config(
                        lock=load_lock(),
                        pkg_config="pkg-config",
                        configuration="desktop",
                        work_root=Path(raw),
                    )

    def test_system_resolution_records_normalized_argument_arrays(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-system-ok-") as raw:
            root = Path(raw)
            include = root / "include dir"
            library_dir = root / "lib dir"
            include.mkdir()
            library_dir.mkdir()
            header = include / "raylib.h"
            header.write_text(VERSION_HEADER, encoding="utf-8")
            library = library_dir / "libraylib.a"
            library.write_bytes(b"synthetic static library")
            lock = copy.deepcopy(load_lock())
            reviewed = lock["reviewedInputs"]
            self.assertIsInstance(reviewed, list)
            for entry in reviewed:
                if isinstance(entry, dict) and entry.get("path") == "src/raylib.h":
                    entry["sha256"] = sha256_file(header)

            values = {
                ("--modversion", "raylib"): "6.0.0",
                ("--cflags", "raylib"): f"-I'{include}' -DRAYLIB_SYSTEM=1",
                ("--libs", "raylib"): f"-L'{library_dir}' -lraylib -lm",
                ("--variable=includedir", "raylib"): str(include),
                ("--variable=libdir", "raylib"): str(library_dir),
            }

            def pkg_value(
                executable: str,
                arguments: list[str] | tuple[str, ...],
                **_: object,
            ) -> tuple[str, dict[str, object]]:
                self.assertEqual(executable, "pkg-config")
                return values[tuple(arguments)], {"arguments": [executable, *arguments], "exitCode": 0}

            with patch(
                "scripts.raylib.provision.tool_identity",
                return_value=({"command": "pkg-config", "versionLine": "2.0"}, {}),
            ), patch("scripts.raylib.provision.pkg_config_value", side_effect=pkg_value):
                result = resolve_system_pkg_config(
                    lock=lock,
                    pkg_config="pkg-config",
                    configuration="desktop",
                    work_root=root / "work",
                )

            self.assertEqual(result.include_directories, (include.resolve(),))
            self.assertEqual(result.library_file, library.resolve())
            self.assertEqual(result.cflags, (f"-I{include}", "-DRAYLIB_SYSTEM=1"))
            self.assertEqual(result.link_flags, (f"-L{library_dir}", "-lraylib", "-lm"))
            rendered = str(result.report)
            self.assertNotIn(str(root), rendered)
            self.assertIn("${SYSTEM_INCLUDE}", rendered)
            self.assertIn("${SYSTEM_LIBRARY}", rendered)

    def test_pkg_config_flags_and_library_ambiguity_fail_closed(self) -> None:
        with self.assertRaisesRegex(ProvisionFailure, "cannot parse"):
            split_pkg_config_flags("-I'unclosed", "pkg-config cflags")
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-libraries-") as raw:
            root = Path(raw)
            (root / "libraylib.a").write_bytes(b"static")
            (root / "libraylib.dylib").write_bytes(b"dynamic")
            with self.assertRaisesRegex(ProvisionFailure, "exactly one"):
                find_system_library(root, None)

    def test_report_normalization_replaces_paths_and_rejects_leaks(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-redaction-") as raw:
            secret = Path(raw).resolve()
            replacements = path_replacements({"${SECRET}": secret})
            normalized = normalize_text(f"input={secret}\\child\r\n", replacements)
            self.assertEqual(normalized, "input=${SECRET}/child\n")
            assert_report_redacted({"path": "${SECRET}/child"}, [secret])
            with self.assertRaisesRegex(ProvisionFailure, "leaked a host path"):
                assert_report_redacted({"path": str(secret)}, [secret])


if __name__ == "__main__":
    unittest.main()
