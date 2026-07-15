#!/usr/bin/env python3
"""Verify typed deterministic project emission through Reflaxe ownership."""

from __future__ import annotations

import difflib
import hashlib
import importlib.util
import json
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from types import ModuleType
from typing import Iterable


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("project_emitter.hxml")
EXPECTED = Path(__file__).with_name("expected")
OWNERSHIP = "_GeneratedFiles.json"
MANIFEST = "hxc.manifest.json"
REMOVED_HEADER = "include/hxc/removed_module.h"
SUCCESS_LINES = ("project-emitter-macro: OK", "project-emitter-fixture: OK")


class ProjectEmitterFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class SnapshotArtifact:
    relative_path: Path
    format: str
    value: object


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def macro_call(mode: str, output: Path) -> str:
    return (
        "ProjectEmitterGolden.run("
        + json.dumps(mode, ensure_ascii=True)
        + ","
        + json.dumps(str(output), ensure_ascii=True)
        + ")"
    )


def run_emitter(
    mode: str,
    output: Path,
    *,
    label: str,
    expected_code: int = 0,
) -> subprocess.CompletedProcess[str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [
            development_tool("haxe"),
            str(HXML),
            "--macro",
            macro_call(mode, output),
        ],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=45,
    )
    if result.returncode != expected_code:
        raise ProjectEmitterFailure(
            f"{label} returned {result.returncode}, expected {expected_code}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if expected_code == 0:
        lines = tuple(line for line in result.stdout.splitlines() if line)
        if lines[-2:] != SUCCESS_LINES or result.stderr:
            raise ProjectEmitterFailure(
                f"{label} emitted an invalid success envelope\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
    elif "HXC9000" not in result.stdout + result.stderr:
        raise ProjectEmitterFailure(
            f"{label} did not fail through the typed HXC9000 boundary\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return result


def output_files(output: Path) -> list[Path]:
    return sorted(path for path in output.rglob("*") if path.is_file())


def output_tree(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in output_files(output)
    }


def json_value(path: Path) -> object:
    return json.loads(path.read_text(encoding="utf-8"))


def snapshot_format(relative: str) -> str:
    suffix = Path(relative).suffix
    if suffix == ".json":
        return "json"
    if suffix == ".h":
        return "header"
    if suffix == ".c":
        return "c"
    raise ProjectEmitterFailure(f"unregistered project snapshot format: {relative}")


def snapshot_artifacts_from(output: Path) -> list[SnapshotArtifact]:
    artifacts: list[SnapshotArtifact] = []
    for path in output_files(output):
        relative = path.relative_to(output)
        format_name = snapshot_format(relative.as_posix())
        value: object
        if format_name == "json":
            value = json_value(path)
        else:
            value = path.read_text(encoding="utf-8")
        artifacts.append(SnapshotArtifact(relative, format_name, value))
    return artifacts


def render_snapshot() -> list[SnapshotArtifact]:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-snapshot-") as temporary:
        output = Path(temporary) / "project"
        run_emitter("full", output, label="project emitter snapshot render")
        return snapshot_artifacts_from(output)


def check_expected(output: Path) -> None:
    if not EXPECTED.is_dir():
        raise ProjectEmitterFailure(
            "project emitter snapshot is missing; run "
            "npm run snapshots:update -- --suite project-emitter"
        )
    actual_paths = sorted(output_tree(output))
    expected_paths = sorted(
        path.relative_to(EXPECTED).as_posix()
        for path in EXPECTED.rglob("*")
        if path.is_file()
    )
    if actual_paths != expected_paths:
        raise ProjectEmitterFailure(
            f"project snapshot file set drifted: expected {expected_paths!r}, "
            f"got {actual_paths!r}"
        )
    for relative in actual_paths:
        actual = output / relative
        expected = EXPECTED / relative
        if actual.suffix == ".json":
            if json_value(actual) != json_value(expected):
                raise ProjectEmitterFailure(
                    f"project JSON snapshot drifted: {relative}\n"
                    + "".join(
                        difflib.unified_diff(
                            (
                                json.dumps(json_value(expected), indent=2, sort_keys=True)
                                + "\n"
                            ).splitlines(keepends=True),
                            (
                                json.dumps(json_value(actual), indent=2, sort_keys=True)
                                + "\n"
                            ).splitlines(keepends=True),
                            fromfile=f"expected/{relative}",
                            tofile=f"actual/{relative}",
                        )
                    )
                )
        elif actual.read_bytes() != expected.read_bytes():
            raise ProjectEmitterFailure(
                f"project text snapshot drifted: {relative}\n"
                + "".join(
                    difflib.unified_diff(
                        expected.read_text(encoding="utf-8").splitlines(keepends=True),
                        actual.read_text(encoding="utf-8").splitlines(keepends=True),
                        fromfile=f"expected/{relative}",
                        tofile=f"actual/{relative}",
                    )
                )
            )


def check_manifest(output: Path) -> None:
    tree = output_tree(output)
    manifest = json_value(output / MANIFEST)
    if not isinstance(manifest, dict):
        raise ProjectEmitterFailure("compiler manifest is not a JSON object")
    if (
        manifest.get("schemaVersion") != 1
        or manifest.get("generator") != "reflaxe.c"
        or manifest.get("compilationStatus")
        != "structural-fixture-no-haxe-lowering"
    ):
        raise ProjectEmitterFailure("compiler manifest schema/status drifted")
    configuration = manifest.get("configuration")
    if configuration != {
        "profile": "portable",
        "environment": "hosted",
        "cStandard": "c11",
        "runtimePolicy": "none",
        "runtimeDiagnostics": "off",
    }:
        raise ProjectEmitterFailure("compiler manifest lost resolved structural configuration")
    raw_artifacts = manifest.get("artifacts")
    if not isinstance(raw_artifacts, list):
        raise ProjectEmitterFailure("compiler manifest omitted content-addressed artifacts")
    addressed: dict[str, str] = {}
    for entry in raw_artifacts:
        if not isinstance(entry, dict):
            raise ProjectEmitterFailure(f"invalid addressed artifact: {entry!r}")
        path = entry.get("path")
        digest = entry.get("sha256")
        if not isinstance(path, str) or not isinstance(digest, str):
            raise ProjectEmitterFailure(f"invalid addressed artifact: {entry!r}")
        if path in addressed:
            raise ProjectEmitterFailure(f"duplicate addressed artifact: {path}")
        addressed[path] = digest
    expected_addressed = set(tree) - {MANIFEST, OWNERSHIP}
    if set(addressed) != expected_addressed:
        raise ProjectEmitterFailure(
            f"manifest addressing set drifted: expected {sorted(expected_addressed)!r}, "
            f"got {sorted(addressed)!r}"
        )
    for path, digest in addressed.items():
        actual_digest = hashlib.sha256(tree[path]).hexdigest()
        if digest != actual_digest:
            raise ProjectEmitterFailure(
                f"content digest mismatch for {path}: {digest} != {actual_digest}"
            )

    ownership = json_value(output / OWNERSHIP)
    if not isinstance(ownership, dict):
        raise ProjectEmitterFailure("Reflaxe ownership metadata is not a JSON object")
    files_generated = ownership.get("filesGenerated")
    expected_owned = sorted(set(tree) - {OWNERSHIP})
    if files_generated != expected_owned:
        raise ProjectEmitterFailure(
            f"Reflaxe ownership set drifted: expected {expected_owned!r}, "
            f"got {files_generated!r}"
        )

    runtime_plan = json_value(output / "hxc.runtime-plan.json")
    if not isinstance(runtime_plan, dict) or (
        runtime_plan.get("status") != "placeholder-no-runtime-analysis"
        or runtime_plan.get("features") != []
        or runtime_plan.get("noRuntimeProof") is not None
    ):
        raise ProjectEmitterFailure(
            "runtime placeholder claimed analysis, selected a feature, or faked no-runtime proof"
        )
    abi = json_value(output / "hxc.abi.json")
    if not isinstance(abi, dict) or (
        abi.get("status") != "placeholder-no-export-analysis"
        or abi.get("stability") != "experimental"
        or abi.get("exports") != []
    ):
        raise ProjectEmitterFailure("ABI placeholder made an unsupported stability/export claim")
    combined = b"\n".join(tree.values())
    if b"hxrt" in combined or b"hxc_runtime" in combined:
        raise ProjectEmitterFailure("structural project emission selected a hidden runtime")


def check_cross_root_and_order() -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-roots-") as temporary:
        root = Path(temporary)
        first = root / "absolute-a" / "project"
        second = root / "unrelated" / "absolute-b" / "project"
        reversed_output = root / "reversed" / "project"
        run_emitter("full", first, label="first absolute-root render")
        run_emitter("full", second, label="second absolute-root render")
        run_emitter("reverse", reversed_output, label="reverse discovery render")
        first_tree = output_tree(first)
        if first_tree != output_tree(second):
            raise ProjectEmitterFailure(
                "fresh project bytes changed with the absolute output directory"
            )
        if first_tree != output_tree(reversed_output):
            raise ProjectEmitterFailure(
                "project bytes changed with unit, build-fact, or symbol discovery order"
            )
        serialized = b"\n".join(first_tree.values())
        for forbidden in (str(first).encode(), str(second).encode(), str(ROOT).encode()):
            if forbidden in serialized:
                raise ProjectEmitterFailure("project artifacts leaked an absolute host path")
        check_manifest(first)
        check_expected(first)


def check_unchanged_write_skip() -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-unchanged-") as temporary:
        output = Path(temporary) / "project"
        run_emitter("full", output, label="initial unchanged-write render")
        ownership = json_value(output / OWNERSHIP)
        assert isinstance(ownership, dict)
        paths = ownership.get("filesGenerated")
        if not isinstance(paths, list) or not all(isinstance(path, str) for path in paths):
            raise ProjectEmitterFailure("initial ownership metadata omitted generated paths")
        fixed_ns = 946684800_000_000_000
        original: dict[str, bytes] = {}
        for relative in paths:
            artifact = output / relative
            original[relative] = artifact.read_bytes()
            os.utime(artifact, ns=(fixed_ns, fixed_ns))
        result = run_emitter("full", output, label="repeated unchanged-write render")
        if "No files updated." not in result.stdout:
            raise ProjectEmitterFailure("Reflaxe did not report the unchanged artifact set")
        for relative in paths:
            artifact = output / relative
            if artifact.read_bytes() != original[relative]:
                raise ProjectEmitterFailure(f"unchanged artifact bytes changed: {relative}")
            if artifact.stat().st_mtime_ns != fixed_ns:
                raise ProjectEmitterFailure(f"unchanged artifact was rewritten: {relative}")
        repeated_ownership = json_value(output / OWNERSHIP)
        if not isinstance(repeated_ownership, dict) or repeated_ownership.get("id") != 2:
            raise ProjectEmitterFailure(
                "Reflaxe activity metadata did not advance independently of skipped artifacts"
            )


def check_stale_and_user_ownership() -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-stale-") as temporary:
        output = Path(temporary) / "project"
        run_emitter("full", output, label="full stale-output render")
        user_file = output / "user-notes.txt"
        user_file.write_text("user-owned\n", encoding="utf-8")
        run_emitter("trimmed", output, label="trimmed stale-output render")
        if (output / REMOVED_HEADER).exists():
            raise ProjectEmitterFailure("removed compiler-owned header survived stale cleanup")
        if user_file.read_text(encoding="utf-8") != "user-owned\n":
            raise ProjectEmitterFailure("stale cleanup modified a user-owned file")
        ownership = json_value(output / OWNERSHIP)
        if not isinstance(ownership, dict):
            raise ProjectEmitterFailure("trimmed ownership metadata is invalid")
        generated = ownership.get("filesGenerated")
        if not isinstance(generated, list) or (
            REMOVED_HEADER in generated or "user-notes.txt" in generated
        ):
            raise ProjectEmitterFailure("trimmed ownership set retained stale/user paths")


def expect_guard_failure(mode: str, output: Path, label: str) -> None:
    result = run_emitter(mode, output, label=label, expected_code=1)
    if "project-emitter-fixture: OK" in result.stdout + result.stderr:
        raise ProjectEmitterFailure(f"{label} reached the runtime success sentinel")


def ownership_document(files: Iterable[object]) -> str:
    return json.dumps(
        {
            "version": 1,
            "id": 1,
            "wasCached": False,
            "filesGenerated": list(files),
        },
        separators=(",", ":"),
    )


def check_negative_guards() -> None:
    for mode in ("duplicate", "invalid-path", "invalid-layout", "lowered"):
        with tempfile.TemporaryDirectory(
            prefix=f"reflaxe-c-project-{mode}-"
        ) as temporary:
            output = Path(temporary) / "project"
            expect_guard_failure(mode, output, f"{mode} project-plan guard")
            if output_files(output):
                raise ProjectEmitterFailure(f"{mode} guard left generated artifacts")

    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-collision-") as temporary:
        output = Path(temporary) / "project"
        collision = output / "src/emitter_fixture.c"
        collision.parent.mkdir(parents=True)
        collision.write_text("user-owned collision\n", encoding="utf-8")
        expect_guard_failure("full", output, "unowned overwrite guard")
        if collision.read_text(encoding="utf-8") != "user-owned collision\n":
            raise ProjectEmitterFailure("unowned overwrite guard modified the collision")
        if output_files(output) != [collision]:
            raise ProjectEmitterFailure("unowned overwrite guard wrote partial output")

    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-escape-") as temporary:
        root = Path(temporary)
        output = root / "project"
        output.mkdir()
        outside = root / "outside.txt"
        outside.write_text("outside\n", encoding="utf-8")
        (output / OWNERSHIP).write_text(
            ownership_document(["../outside.txt"]), encoding="utf-8"
        )
        expect_guard_failure("full", output, "ownership traversal guard")
        if outside.read_text(encoding="utf-8") != "outside\n":
            raise ProjectEmitterFailure("ownership traversal guard touched an outside file")

    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-symlink-") as temporary:
        root = Path(temporary)
        output = root / "project"
        source_dir = output / "src"
        source_dir.mkdir(parents=True)
        outside = root / "outside.c"
        outside.write_text("outside symlink target\n", encoding="utf-8")
        (source_dir / "emitter_fixture.c").symlink_to(outside)
        expect_guard_failure("full", output, "output symlink guard")
        if outside.read_text(encoding="utf-8") != "outside symlink target\n":
            raise ProjectEmitterFailure("output symlink guard modified its outside target")

    with tempfile.TemporaryDirectory(
        prefix="reflaxe-c-project-dangling-symlink-"
    ) as temporary:
        root = Path(temporary)
        output = root / "project"
        source_dir = output / "src"
        source_dir.mkdir(parents=True)
        outside = root / "missing-outside.c"
        dangling = source_dir / "emitter_fixture.c"
        dangling.symlink_to(outside)
        expect_guard_failure("full", output, "dangling output symlink guard")
        if outside.exists():
            raise ProjectEmitterFailure(
                "dangling output symlink guard created its outside target"
            )
        if not dangling.is_symlink():
            raise ProjectEmitterFailure("dangling output symlink guard removed the link")

    with tempfile.TemporaryDirectory(
        prefix="reflaxe-c-project-dangling-root-"
    ) as temporary:
        root = Path(temporary)
        outside = root / "missing-project"
        output = root / "project"
        output.symlink_to(outside, target_is_directory=True)
        expect_guard_failure("full", output, "dangling output-root symlink guard")
        if outside.exists():
            raise ProjectEmitterFailure(
                "dangling output-root symlink guard created its outside target"
            )
        if not output.is_symlink():
            raise ProjectEmitterFailure("dangling output-root symlink guard removed the link")

    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-schema-") as temporary:
        output = Path(temporary) / "project"
        output.mkdir()
        (output / OWNERSHIP).write_text(
            ownership_document([7]), encoding="utf-8"
        )
        expect_guard_failure("full", output, "typed ownership JSON guard")

    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-json-") as temporary:
        output = Path(temporary) / "project"
        output.mkdir()
        (output / OWNERSHIP).write_text("{", encoding="utf-8")
        expect_guard_failure("full", output, "malformed ownership JSON guard")


def load_all_sources_runner() -> ModuleType:
    path = ROOT / "test/all_sources/run.py"
    spec = importlib.util.spec_from_file_location("project_emitter_all_sources", path)
    if spec is None or spec.loader is None:
        raise ProjectEmitterFailure("cannot load the all-source production-boundary runner")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    finally:
        del sys.modules[spec.name]
    return module


def check_production_boundary() -> None:
    module = load_all_sources_runner()
    try:
        module.check_production_boundary()
    except module.AllSourcesFailure as error:
        raise ProjectEmitterFailure(str(error)) from error


def main() -> int:
    if not HXML.is_file():
        print(f"project-emitter: ERROR: missing HXML: {HXML}", file=sys.stderr)
        return 1
    if shutil.which(development_tool("haxe")) is None:
        print("project-emitter: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        check_cross_root_and_order()
        check_unchanged_write_skip()
        check_stale_and_user_ownership()
        check_negative_guards()
        check_production_boundary()
    except (
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
        ProjectEmitterFailure,
    ) as error:
        print(f"project-emitter: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "project-emitter: OK: typed deterministic manifests, cross-root bytes, "
        "content-addressed skipped writes, Reflaxe-only stale ownership, path guards, "
        "strict runtime/ABI placeholders, and HXC1000 no-output passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
