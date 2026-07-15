#!/usr/bin/env python3
"""Fail-closed license, provenance, and release-notice validation."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from datetime import date
from pathlib import Path, PurePosixPath
from typing import Any, Iterable


EXPECTED_COMPONENT_LICENSES = {
    "reflaxe-framework": "MIT",
    "lix-package-manager": "MIT",
    "haxe-compiler": "GPL-2.0-or-later",
    "haxe-standard-library": "MIT",
    "llvm-clang-tooling": "Apache-2.0 WITH LLVM-exception",
}
EXPECTED_RELEASE_FILES = {
    "LICENSE",
    "THIRD_PARTY_NOTICES.md",
    "docs/specs/third-party-provenance.json",
    "LICENSES/HAXE-STDLIB-MIT.txt",
    "vendor/reflaxe/LICENSE",
}
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")
REVISION_RE = re.compile(r"^[0-9a-f]{40}$")


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def safe_relative_path(value: Any, label: str, errors: list[str]) -> PurePosixPath | None:
    if not isinstance(value, str) or not value:
        errors.append(f"{label} must be a non-empty repository-relative path")
        return None
    if "\\" in value:
        errors.append(f"{label} must use forward slashes: {value!r}")
        return None
    path = PurePosixPath(value)
    if (
        not path.parts
        or path.is_absolute()
        or path.as_posix() != value
        or any(part in ("", ".", "..") for part in path.parts)
    ):
        errors.append(f"{label} is not a normalized safe relative path: {value!r}")
        return None
    return path


def local_path(root: Path, relative: PurePosixPath, label: str, errors: list[str]) -> Path | None:
    candidate = root.joinpath(*relative.parts)
    try:
        candidate.resolve(strict=False).relative_to(root.resolve())
    except ValueError:
        errors.append(f"{label} escapes repository root: {relative.as_posix()}")
        return None
    return candidate


def require_file(
    root: Path,
    entry: Any,
    label: str,
    errors: list[str],
) -> tuple[str, Path] | None:
    if not isinstance(entry, dict):
        errors.append(f"{label} must be an object")
        return None
    relative = safe_relative_path(entry.get("path"), f"{label}.path", errors)
    if relative is None:
        return None
    path = local_path(root, relative, label, errors)
    if path is None:
        return None
    if path.is_symlink():
        errors.append(f"{label} must not be a symlink: {relative.as_posix()}")
        return None
    if not path.is_file() or path.stat().st_size == 0:
        errors.append(f"{label} is missing or empty: {relative.as_posix()}")
        return None
    expected_hash = entry.get("sha256")
    if expected_hash is not None:
        if not isinstance(expected_hash, str) or not SHA256_RE.fullmatch(expected_hash):
            errors.append(f"{label}.sha256 must be a lowercase SHA-256 digest")
        elif sha256(path) != expected_hash:
            errors.append(f"{label} hash mismatch: {relative.as_posix()}")
    return relative.as_posix(), path


def list_files(root: Path, relative: PurePosixPath, label: str, errors: list[str]) -> set[str]:
    directory = local_path(root, relative, label, errors)
    if directory is None or not directory.exists():
        return set()
    if directory.is_symlink() or not directory.is_dir():
        errors.append(f"{label} must be a real directory: {relative.as_posix()}")
        return set()
    files: set[str] = set()
    for path in sorted(directory.rglob("*")):
        if path.is_symlink():
            errors.append(f"{label} contains a symlink: {path.relative_to(root).as_posix()}")
            continue
        if path.is_file():
            files.add(path.relative_to(root).as_posix())
    return files


def is_revision(value: Any) -> bool:
    return isinstance(value, str) and REVISION_RE.fullmatch(value) is not None


def validate_component(
    root: Path,
    component: Any,
    label: str,
    errors: list[str],
    *,
    require_pinned_if_bundled: bool = True,
) -> tuple[str | None, list[str], bool]:
    if not isinstance(component, dict):
        errors.append(f"{label} must be an object")
        return None, [], False

    component_id = component.get("id")
    if not isinstance(component_id, str) or not component_id:
        errors.append(f"{label}.id must be a non-empty string")
        component_id = None

    for field in ("name", "role", "usage", "selectionStatus", "upstreamUrl", "licenseExpression", "licenseUrl"):
        value = component.get(field)
        if not isinstance(value, str) or not value:
            errors.append(f"{label}.{field} must be a non-empty string")
    for url_field in ("upstreamUrl", "licenseUrl"):
        value = component.get(url_field)
        if isinstance(value, str) and not value.startswith("https://"):
            errors.append(f"{label}.{url_field} must use https")

    for revision_field in ("sourceRevision", "observedRevision"):
        revision = component.get(revision_field)
        if revision is not None and not is_revision(revision):
            errors.append(f"{label}.{revision_field} must be null or a lowercase 40-hex revision")
    evidence_revision = component.get("sourceRevision") or component.get("observedRevision")
    license_url = component.get("licenseUrl")
    if is_revision(evidence_revision) and isinstance(license_url, str) and evidence_revision not in license_url:
        errors.append(f"{label}.licenseUrl must point at its immutable evidence revision")

    bundled = component.get("bundled")
    if not isinstance(bundled, bool):
        errors.append(f"{label}.bundled must be boolean")
        bundled = False

    raw_paths = component.get("paths")
    paths: list[str] = []
    if not isinstance(raw_paths, list):
        errors.append(f"{label}.paths must be an array")
    else:
        for index, value in enumerate(raw_paths):
            relative = safe_relative_path(value, f"{label}.paths[{index}]", errors)
            if relative is not None:
                paths.append(relative.as_posix())

    local_licenses = component.get("localLicenseFiles")
    if not isinstance(local_licenses, list):
        errors.append(f"{label}.localLicenseFiles must be an array")
        local_licenses = []
    for index, entry in enumerate(local_licenses):
        has_exact_hash = (
            isinstance(entry, dict)
            and isinstance(entry.get("sha256"), str)
            and SHA256_RE.fullmatch(entry["sha256"]) is not None
        )
        if not has_exact_hash:
            errors.append(f"{label}.localLicenseFiles[{index}] must include an exact SHA-256 digest")
        require_file(root, entry, f"{label}.localLicenseFiles[{index}]", errors)

    if bundled:
        if require_pinned_if_bundled and not is_revision(component.get("sourceRevision")):
            errors.append(f"{label} is bundled but has no immutable sourceRevision")
        if not paths:
            errors.append(f"{label} is bundled but owns no paths")
        if not local_licenses:
            errors.append(f"{label} is bundled but has no local license evidence")
    elif paths:
        errors.append(f"{label} owns repository paths but bundled is false")

    return component_id, paths, bundled


def path_is_within(path: str, root: str) -> bool:
    return path == root or path.startswith(root + "/")


def validate_policy(root: Path, policy_relative: str) -> tuple[dict[str, Any] | None, list[str]]:
    errors: list[str] = []
    relative = safe_relative_path(policy_relative, "policy path", errors)
    if relative is None:
        return None, errors
    policy_path = local_path(root, relative, "policy path", errors)
    if policy_path is None or not policy_path.is_file():
        errors.append(f"policy file is missing: {policy_relative}")
        return None, errors
    try:
        data = json.loads(policy_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        errors.append(f"cannot read policy JSON: {error}")
        return None, errors
    if not isinstance(data, dict):
        errors.append("policy root must be an object")
        return None, errors
    if data.get("schemaVersion") != 1:
        errors.append("schemaVersion must be 1")
    evidence_checked_at = data.get("evidenceCheckedAt")
    try:
        if not isinstance(evidence_checked_at, str):
            raise ValueError
        date.fromisoformat(evidence_checked_at)
    except ValueError:
        errors.append("evidenceCheckedAt must be an ISO YYYY-MM-DD date")

    project = data.get("project")
    if not isinstance(project, dict):
        errors.append("project must be an object")
    else:
        if project.get("licenseExpression") != "GPL-3.0-only":
            errors.append("project.licenseExpression must be GPL-3.0-only")
        license_entry = {
            "path": project.get("licenseFile"),
            "sha256": project.get("licenseSha256"),
        }
        require_file(root, license_entry, "project license", errors)

    policy = data.get("policy")
    if not isinstance(policy, dict):
        errors.append("policy must be an object")
    else:
        if policy.get("externalToolsAreRedistributed") is not False:
            errors.append("policy.externalToolsAreRedistributed must be false")
        if policy.get("bundledComponentsRequireImmutableRevision") is not True:
            errors.append("policy.bundledComponentsRequireImmutableRevision must be true")
        if policy.get("unlistedReservedPaths") != "reject":
            errors.append("policy.unlistedReservedPaths must be reject")
        if policy.get("generatedOutputDecision") != "haxe_c-od2.5":
            errors.append("policy.generatedOutputDecision must retain the owning decision haxe_c-od2.5")

    release = data.get("release")
    release_entries: list[Any] = []
    if not isinstance(release, dict) or not isinstance(release.get("requiredFiles"), list):
        errors.append("release.requiredFiles must be an array")
    else:
        release_entries = release["requiredFiles"]
    release_paths: list[str] = []
    release_entries_by_path: dict[str, dict[str, Any]] = {}
    for index, entry in enumerate(release_entries):
        result = require_file(root, entry, f"release.requiredFiles[{index}]", errors)
        if result is not None:
            release_paths.append(result[0])
            if isinstance(entry, dict):
                release_entries_by_path[result[0]] = entry
    if len(release_paths) != len(set(release_paths)):
        errors.append("release.requiredFiles contains duplicate paths")
    missing_release = EXPECTED_RELEASE_FILES - set(release_paths)
    if missing_release:
        errors.append(f"release.requiredFiles is missing: {', '.join(sorted(missing_release))}")

    raw_components = data.get("components")
    components: list[Any] = raw_components if isinstance(raw_components, list) else []
    if not isinstance(raw_components, list):
        errors.append("components must be an array")
    component_map: dict[str, dict[str, Any]] = {}
    owned_paths: list[tuple[str, str]] = []
    for index, component in enumerate(components):
        component_id, paths, bundled = validate_component(
            root, component, f"components[{index}]", errors
        )
        if component_id is None:
            continue
        if component_id in component_map:
            errors.append(f"duplicate component id: {component_id}")
            continue
        component_map[component_id] = component
        if bundled:
            owned_paths.extend((path, component_id) for path in paths)
    for component_id, expected_license in EXPECTED_COMPONENT_LICENSES.items():
        component = component_map.get(component_id)
        if component is None:
            errors.append(f"required component is missing: {component_id}")
        elif component.get("licenseExpression") != expected_license:
            errors.append(f"{component_id} license must be {expected_license}")

    haxe_compiler = component_map.get("haxe-compiler", {})
    haxe_stdlib = component_map.get("haxe-standard-library", {})
    for component_id, component in (("haxe-compiler", haxe_compiler), ("haxe-standard-library", haxe_stdlib)):
        if component and component.get("version") != "5.0.0-preview.1":
            errors.append(f"{component_id} version must be 5.0.0-preview.1")
        if component and component.get("sourceRevision") != "2c1e544e0a2c7524ef4c8e103f1b0580362ea538":
            errors.append(f"{component_id} must resolve the Haxe 5.0.0-preview.1 tag commit")

    raw_runtime = data.get("runtimeDependencies")
    runtime_dependencies: list[Any] = raw_runtime if isinstance(raw_runtime, list) else []
    if not isinstance(raw_runtime, list):
        errors.append("runtimeDependencies must be an array; use [] when none are selected")
    runtime_ids: set[str] = set()
    for index, dependency in enumerate(runtime_dependencies):
        dependency_id, paths, bundled = validate_component(
            root,
            dependency,
            f"runtimeDependencies[{index}]",
            errors,
        )
        if dependency_id is None:
            continue
        if dependency_id in runtime_ids or dependency_id in component_map:
            errors.append(f"duplicate dependency/component id: {dependency_id}")
        runtime_ids.add(dependency_id)
        if dependency.get("licenseExpression") in (None, "", "UNKNOWN", "NONE"):
            errors.append(f"runtime dependency {dependency_id} needs a concrete license expression")
        if bundled:
            owned_paths.extend((path, dependency_id) for path in paths)

    for component in [*components, *runtime_dependencies]:
        if not isinstance(component, dict):
            continue
        for entry in component.get("localLicenseFiles", []):
            if not isinstance(entry, dict) or not isinstance(entry.get("path"), str):
                continue
            license_path = entry["path"]
            release_entry = release_entries_by_path.get(license_path)
            if release_entry is None:
                errors.append(
                    f"component local license is absent from release.requiredFiles: {license_path}"
                )
            elif release_entry.get("sha256") != entry.get("sha256"):
                errors.append(
                    f"component and release hashes differ for local license: {license_path}"
                )

    raw_reserved = data.get("reservedThirdPartyRoots")
    reserved: list[PurePosixPath] = []
    if not isinstance(raw_reserved, list) or not raw_reserved:
        errors.append("reservedThirdPartyRoots must be a non-empty array")
    else:
        for index, value in enumerate(raw_reserved):
            relative = safe_relative_path(value, f"reservedThirdPartyRoots[{index}]", errors)
            if relative is not None:
                reserved.append(relative)
    if len({path.as_posix() for path in reserved}) != len(reserved):
        errors.append("reservedThirdPartyRoots contains duplicates")

    for owned_path, owner in owned_paths:
        if not any(path_is_within(owned_path, reserved_root.as_posix()) for reserved_root in reserved):
            errors.append(f"bundled path {owned_path} for {owner} is outside reserved third-party roots")
        owned_local = root.joinpath(*PurePosixPath(owned_path).parts)
        if not owned_local.exists():
            errors.append(f"bundled path does not exist for {owner}: {owned_path}")

    for reserved_root in reserved:
        for file_path in list_files(root, reserved_root, "reserved third-party root", errors):
            owners = [owner for owned_path, owner in owned_paths if path_is_within(file_path, owned_path)]
            if not owners:
                errors.append(f"unowned third-party file under reserved root: {file_path}")
            elif len(owners) > 1:
                errors.append(f"third-party file has multiple owners ({', '.join(owners)}): {file_path}")

    raw_derived = data.get("derivedSourcePolicies")
    derived_policies: list[Any] = raw_derived if isinstance(raw_derived, list) else []
    if not isinstance(raw_derived, list):
        errors.append("derivedSourcePolicies must be an array")
    expected_derived_root = "std/c/_std"
    matching_policies = [
        entry
        for entry in derived_policies
        if isinstance(entry, dict) and entry.get("root") == expected_derived_root
    ]
    if len(matching_policies) != 1:
        errors.append(f"derivedSourcePolicies must contain exactly one {expected_derived_root} policy")
    for policy_index, derived_policy in enumerate(derived_policies):
        if not isinstance(derived_policy, dict):
            errors.append(f"derivedSourcePolicies[{policy_index}] must be an object")
            continue
        derived_root = safe_relative_path(
            derived_policy.get("root"), f"derivedSourcePolicies[{policy_index}].root", errors
        )
        raw_files = derived_policy.get("files")
        entries: list[Any] = raw_files if isinstance(raw_files, list) else []
        if not isinstance(raw_files, list):
            errors.append(f"derivedSourcePolicies[{policy_index}].files must be an array")
        declared: set[str] = set()
        for file_index, entry in enumerate(entries):
            label = f"derivedSourcePolicies[{policy_index}].files[{file_index}]"
            if not isinstance(entry, dict):
                errors.append(f"{label} must be an object")
                continue
            relative = safe_relative_path(entry.get("path"), f"{label}.path", errors)
            if relative is None:
                continue
            path_text = relative.as_posix()
            if path_text in declared:
                errors.append(f"duplicate derived-source entry: {path_text}")
            declared.add(path_text)
            if derived_root is not None and not path_is_within(path_text, derived_root.as_posix()):
                errors.append(f"{label}.path is outside {derived_root.as_posix()}: {path_text}")
            if not isinstance(entry.get("sha256"), str) or not SHA256_RE.fullmatch(entry["sha256"]):
                errors.append(f"{label}.sha256 must record the exact derived file content")
            result = require_file(root, entry, label, errors)
            classification = entry.get("classification")
            if classification == "repository-authored":
                if entry.get("licenseExpression") != "GPL-3.0-only":
                    errors.append(f"{label} repository-authored license must be GPL-3.0-only")
            elif classification == "upstream-derived":
                upstream_id = entry.get("upstreamComponent") or derived_policy.get("defaultUpstreamComponent")
                upstream = component_map.get(upstream_id)
                if upstream is None:
                    errors.append(f"{label} names unknown upstream component: {upstream_id!r}")
                else:
                    if entry.get("licenseExpression") != upstream.get("licenseExpression"):
                        errors.append(f"{label} license does not match upstream component")
                safe_relative_path(entry.get("upstreamPath"), f"{label}.upstreamPath", errors)
                if not is_revision(entry.get("upstreamRevision")):
                    errors.append(f"{label}.upstreamRevision must be an immutable 40-hex revision")
                notice_files = entry.get("noticeFiles")
                if not isinstance(notice_files, list) or not notice_files:
                    errors.append(f"{label}.noticeFiles must retain at least one notice")
                else:
                    for notice_index, notice in enumerate(notice_files):
                        notice_path = safe_relative_path(notice, f"{label}.noticeFiles[{notice_index}]", errors)
                        if notice_path is not None:
                            local = local_path(root, notice_path, label, errors)
                            invalid_notice = local is not None and (
                                local.is_symlink()
                                or not local.is_file()
                                or local.stat().st_size == 0
                            )
                            if invalid_notice:
                                errors.append(f"{label} notice is missing or empty: {notice_path.as_posix()}")
                            if notice_path.as_posix() not in release_entries_by_path:
                                errors.append(
                                    f"{label} notice is absent from release.requiredFiles: "
                                    f"{notice_path.as_posix()}"
                                )
            else:
                errors.append(f"{label}.classification must be repository-authored or upstream-derived")
            if result is None:
                continue
        actual = list_files(root, derived_root, "derived source root", errors) if derived_root is not None else set()
        for path_text in sorted(actual - declared):
            errors.append(f"derived source is not in the provenance ledger: {path_text}")
        for path_text in sorted(declared - actual):
            errors.append(f"derived-source ledger entry has no file: {path_text}")

    return data, errors


def validate_package(
    root: Path,
    package_root: Path,
    policy: dict[str, Any],
) -> list[str]:
    errors: list[str] = []
    if package_root.is_symlink() or not package_root.is_dir():
        return [f"package root is not a real directory: {package_root}"]
    entries = policy.get("release", {}).get("requiredFiles", [])
    for index, entry in enumerate(entries):
        if not isinstance(entry, dict):
            continue
        relative = safe_relative_path(entry.get("path"), f"package requiredFiles[{index}].path", errors)
        if relative is None:
            continue
        source = local_path(root, relative, "repository release file", errors)
        packaged = local_path(package_root, relative, "package release file", errors)
        if source is None or packaged is None:
            continue
        if source.is_symlink() or not source.is_file() or source.stat().st_size == 0:
            errors.append(f"reviewed release file is missing or empty: {relative.as_posix()}")
            continue
        if packaged.is_symlink() or not packaged.is_file() or packaged.stat().st_size == 0:
            errors.append(f"release package is missing required file: {relative.as_posix()}")
            continue
        if source.read_bytes() != packaged.read_bytes():
            errors.append(f"release package file differs from reviewed copy: {relative.as_posix()}")
        expected_hash = entry.get("sha256")
        if isinstance(expected_hash, str) and SHA256_RE.fullmatch(expected_hash) and sha256(packaged) != expected_hash:
            errors.append(f"release package file hash mismatch: {relative.as_posix()}")
    return errors


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    default_root = Path(__file__).resolve().parents[2]
    parser.add_argument("--root", type=Path, default=default_root, help="repository root")
    parser.add_argument(
        "--policy",
        default="docs/specs/third-party-provenance.json",
        help="policy path relative to --root",
    )
    parser.add_argument(
        "--package-root",
        type=Path,
        help="also verify required notices inside an assembled release tree",
    )
    parser.add_argument("--quiet", action="store_true")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    root = args.root.resolve()
    policy, errors = validate_policy(root, args.policy)
    if policy is not None and args.package_root is not None:
        errors.extend(validate_package(root, args.package_root.resolve(), policy))
    if errors:
        for error in errors:
            print(f"license-policy: ERROR: {error}", file=sys.stderr)
        return 1
    if not args.quiet:
        suffix = f"; package={args.package_root.resolve()}" if args.package_root is not None else ""
        print(f"license-policy: OK: root={root}{suffix}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
