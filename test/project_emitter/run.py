#!/usr/bin/env python3
"""Verify typed deterministic project emission through Reflaxe ownership."""

from __future__ import annotations

import difflib
import hashlib
import importlib.util
import json
import os
import re
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from collections.abc import Iterable, Mapping
from dataclasses import dataclass
from pathlib import Path
from types import ModuleType


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("project_emitter.hxml")
EXPECTED = Path(__file__).with_name("expected")
OWNERSHIP = "_GeneratedFiles.json"
MANIFEST = "hxc.manifest.json"
REMOVED_HEADER = "include/hxc/removed_module.h"
SUCCESS_LINES = ("project-emitter-macro: OK", "project-emitter-fixture: OK")
ORIGINAL_PAYLOADS = frozenset(
    {
        "include/hxc/detail/emitter_fixture_internal.h",
        "include/hxc/emitter_fixture.h",
        REMOVED_HEADER,
        "src/emitter_fixture.c",
        "src/hxc_boot.c",
    }
)
RENAMED_PAYLOADS = frozenset(
    {
        "include/hxc/detail/renamed_fixture_internal.h",
        "include/hxc/renamed_fixture.h",
        "src/renamed_fixture.c",
        "src/hxc_renamed_boot.c",
    }
)
ISO_TIMESTAMP = re.compile(
    rb"\b[0-9]{4}-[0-9]{2}-[0-9]{2}[T ][0-9]{2}:[0-9]{2}:[0-9]{2}"
    rb"(?:\.[0-9]+)?(?:Z|[+-][0-9]{2}:?[0-9]{2})?\b"
)
UUID = re.compile(
    rb"\b[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[1-5][0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}\b"
)
UNSTABLE_JSON_KEYS = frozenset(
    {
        "buildtime",
        "createdat",
        "generatedat",
        "nonce",
        "randomid",
        "timestamp",
        "uuid",
    }
)


class ProjectEmitterFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class SnapshotArtifact:
    relative_path: Path
    format: str
    value: object


@dataclass(frozen=True)
class ArtifactDifference:
    relative_path: str
    offset: int
    expected_exists: bool
    actual_exists: bool
    expected_byte: int | None
    actual_byte: int | None

    def message(self, label: str) -> str:
        location = (
            f"{label}: first differing artifact `{self.relative_path}` "
            f"at byte offset {self.offset}"
        )
        if self.expected_exists is not self.actual_exists:
            expected = "present" if self.expected_exists else "missing"
            actual = "present" if self.actual_exists else "missing"
            return f"{location} (expected artifact {expected}, actual artifact {actual})"
        return (
            f"{location} (expected {byte_description(self.expected_byte)}, "
            f"actual {byte_description(self.actual_byte)})"
        )


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
    connect: str | None = None,
    hxml: Path = HXML,
    environment_updates: Mapping[str, str] | None = None,
) -> subprocess.CompletedProcess[str]:
    environment = os.environ.copy()
    if connect is None:
        environment["HAXE_NO_SERVER"] = "1"
    else:
        environment.pop("HAXE_NO_SERVER", None)
    if environment_updates is not None:
        environment.update(environment_updates)
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
            str(hxml),
            "--macro",
            macro_call(mode, output),
        ]
    )
    result = subprocess.run(
        command,
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


def generated_artifact_tree(output: Path) -> dict[str, bytes]:
    ownership = json_value(output / OWNERSHIP)
    if not isinstance(ownership, dict):
        raise ProjectEmitterFailure("Reflaxe ownership metadata is not a JSON object")
    generated = ownership.get("filesGenerated")
    if not isinstance(generated, list) or not all(
        isinstance(path, str) for path in generated
    ):
        raise ProjectEmitterFailure("Reflaxe ownership metadata omitted generated paths")
    return {path: (output / path).read_bytes() for path in generated}


def byte_description(value: int | None) -> str:
    return "<EOF>" if value is None else f"0x{value:02x}"


def first_artifact_difference(
    expected: Mapping[str, bytes], actual: Mapping[str, bytes]
) -> ArtifactDifference | None:
    paths = sorted(set(expected) | set(actual), key=lambda path: path.encode("utf-8"))
    for relative in paths:
        expected_exists = relative in expected
        actual_exists = relative in actual
        if expected_exists is not actual_exists:
            expected_content = expected.get(relative, b"")
            actual_content = actual.get(relative, b"")
            return ArtifactDifference(
                relative,
                0,
                expected_exists,
                actual_exists,
                expected_content[0] if expected_content else None,
                actual_content[0] if actual_content else None,
            )
        expected_content = expected[relative]
        actual_content = actual[relative]
        shared_length = min(len(expected_content), len(actual_content))
        for offset in range(shared_length):
            if expected_content[offset] != actual_content[offset]:
                return ArtifactDifference(
                    relative,
                    offset,
                    True,
                    True,
                    expected_content[offset],
                    actual_content[offset],
                )
        if len(expected_content) != len(actual_content):
            return ArtifactDifference(
                relative,
                shared_length,
                True,
                True,
                (
                    expected_content[shared_length]
                    if shared_length < len(expected_content)
                    else None
                ),
                (
                    actual_content[shared_length]
                    if shared_length < len(actual_content)
                    else None
                ),
            )
    return None


def assert_artifact_trees_equal(
    expected: Mapping[str, bytes], actual: Mapping[str, bytes], label: str
) -> None:
    difference = first_artifact_difference(expected, actual)
    if difference is not None:
        raise ProjectEmitterFailure(difference.message(label))


def check_difference_reporting() -> None:
    cases = (
        (
            "synthetic content mismatch",
            {"é.bin": b"late", "z.bin": b"abc"},
            {"z.bin": b"abX", "é.bin": b"different"},
            "synthetic content mismatch: first differing artifact `z.bin` "
            "at byte offset 2 (expected 0x63, actual 0x58)",
        ),
        (
            "synthetic length mismatch",
            {"length.bin": b"abc"},
            {"length.bin": b"ab"},
            "synthetic length mismatch: first differing artifact `length.bin` "
            "at byte offset 2 (expected 0x63, actual <EOF>)",
        ),
        (
            "synthetic file-set mismatch",
            {"missing.bin": b"payload"},
            {},
            "synthetic file-set mismatch: first differing artifact `missing.bin` "
            "at byte offset 0 (expected artifact present, actual artifact missing)",
        ),
    )
    for label, expected_tree, actual_tree, expected_message in cases:
        try:
            assert_artifact_trees_equal(expected_tree, actual_tree, label)
        except ProjectEmitterFailure as error:
            if str(error) != expected_message:
                raise ProjectEmitterFailure(
                    f"artifact-difference report drifted: {error}"
                ) from error
        else:
            raise ProjectEmitterFailure(
                f"artifact-difference reporter missed `{label}`"
            )


def stage_crlf_fixture(directory: Path) -> Path:
    for name in ("ProjectEmitterGolden.hx", "ProjectEmitterProbe.hx"):
        source = HXML.parent / name
        contents = source.read_bytes()
        if b"\r" in contents:
            raise ProjectEmitterFailure(f"tracked fixture is not canonical LF text: {name}")
        (directory / name).write_bytes(contents.replace(b"\n", b"\r\n"))
    hxml = directory / "project_emitter_crlf.hxml"
    lines = [
        "# E1.T09 CRLF and absolute-source-root determinism probe.",
        "",
        f"-cp {directory.as_posix()}",
        "-lib reflaxe.c",
        "",
        "-main ProjectEmitterProbe",
        "--interp",
    ]
    hxml.write_bytes(("\r\n".join(lines) + "\r\n").encode("utf-8"))
    return hxml


def normalized_json_key(value: str) -> str:
    return value.replace("_", "").replace("-", "").lower()


def assert_no_unstable_json_keys(value: object, artifact: str, location: str) -> None:
    if isinstance(value, dict):
        for key, child in value.items():
            if not isinstance(key, str):
                raise ProjectEmitterFailure(
                    f"JSON artifact `{artifact}` has a non-string key at {location}"
                )
            if normalized_json_key(key) in UNSTABLE_JSON_KEYS:
                raise ProjectEmitterFailure(
                    f"JSON artifact `{artifact}` exposes unstable field `{key}` at {location}"
                )
            assert_no_unstable_json_keys(child, artifact, f"{location}.{key}")
    elif isinstance(value, list):
        for index, child in enumerate(value):
            assert_no_unstable_json_keys(child, artifact, f"{location}[{index}]")


def check_no_unstable_facts(
    tree: Mapping[str, bytes], forbidden_paths: Iterable[Path]
) -> None:
    encoded_paths = tuple(str(path).encode("utf-8") for path in forbidden_paths)
    for relative, payload in tree.items():
        if b"\r" in payload:
            raise ProjectEmitterFailure(
                f"generated artifact `{relative}` contains a non-canonical CR byte"
            )
        for forbidden in encoded_paths:
            if forbidden and forbidden in payload:
                raise ProjectEmitterFailure(
                    f"generated artifact `{relative}` leaked host path {forbidden!r}"
                )
        if ISO_TIMESTAMP.search(payload) is not None:
            raise ProjectEmitterFailure(
                f"generated artifact `{relative}` contains an unstable timestamp"
            )
        if UUID.search(payload) is not None:
            raise ProjectEmitterFailure(
                f"generated artifact `{relative}` contains a random-looking UUID"
            )
        if relative.endswith(".json"):
            assert_no_unstable_json_keys(
                json.loads(payload.decode("utf-8")), relative, "$"
            )


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
    if ownership.get("id") != 1 or ownership.get("wasCached") is not False:
        raise ProjectEmitterFailure(
            "fresh Reflaxe ownership metadata contains unstable activity state: "
            f"id={ownership.get('id')!r}, wasCached={ownership.get('wasCached')!r}, "
            f"output={output}"
        )
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


def check_cross_root_order_locale_and_line_endings() -> None:
    with (
        tempfile.TemporaryDirectory(
            prefix="reflaxe-c-project-root-a-"
        ) as first_temporary,
        tempfile.TemporaryDirectory(
            prefix="reflaxe-c-project-root-b-"
        ) as second_temporary,
        tempfile.TemporaryDirectory(
            prefix="reflaxe-c-project-variants-"
        ) as variants_temporary,
    ):
        first_root = Path(first_temporary)
        second_root = Path(second_temporary)
        variants_root = Path(variants_temporary)
        first = first_root / "project"
        second = second_root / "unrelated" / "project"
        reversed_output = variants_root / "reversed" / "project"
        locale_output = variants_root / "locale" / "project"
        crlf_output = variants_root / "crlf" / "project"
        crlf_fixture = variants_root / "crlf-fixture"
        crlf_fixture.mkdir()
        run_emitter("full", first, label="first absolute-root render")
        run_emitter("full", second, label="second absolute-root render")
        run_emitter("reverse", reversed_output, label="reverse discovery render")
        run_emitter(
            "full",
            locale_output,
            label="C.UTF-8 locale render",
            environment_updates={"LANG": "C.UTF-8", "LC_ALL": "C.UTF-8"},
        )
        run_emitter(
            "full",
            crlf_output,
            label="CRLF source/HXML render",
            hxml=stage_crlf_fixture(crlf_fixture),
        )
        first_tree = output_tree(first)
        assert_artifact_trees_equal(
            first_tree,
            output_tree(second),
            "absolute-output-root determinism failure",
        )
        assert_artifact_trees_equal(
            first_tree,
            output_tree(reversed_output),
            "module/build-fact/symbol order determinism failure",
        )
        assert_artifact_trees_equal(
            first_tree,
            output_tree(locale_output),
            "locale determinism failure",
        )
        assert_artifact_trees_equal(
            first_tree,
            output_tree(crlf_output),
            "source line-ending determinism failure",
        )
        check_no_unstable_facts(
            first_tree,
            (
                ROOT,
                first_root,
                second_root,
                variants_root,
                first,
                second,
                reversed_output,
                locale_output,
                crlf_output,
                crlf_fixture,
            ),
        )
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


def check_renamed_symbol_and_module_cleanup() -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-rename-") as temporary:
        root = Path(temporary)
        output = root / "incremental" / "project"
        fresh = root / "fresh-renamed" / "project"
        run_emitter("full", output, label="pre-rename project render")
        unlisted = output / "src/emitter_fixture.user.c"
        unlisted.write_text("user-owned renamed-neighbor\n", encoding="utf-8")
        run_emitter("renamed", output, label="renamed symbol/module render")

        for relative in ORIGINAL_PAYLOADS:
            if (output / relative).exists():
                raise ProjectEmitterFailure(
                    f"renamed build retained compiler-owned artifact: {relative}"
                )
        for relative in RENAMED_PAYLOADS:
            if not (output / relative).is_file():
                raise ProjectEmitterFailure(
                    f"renamed build omitted replacement artifact: {relative}"
                )
        if unlisted.read_text(encoding="utf-8") != "user-owned renamed-neighbor\n":
            raise ProjectEmitterFailure(
                "renamed build modified an unlisted neighboring source"
            )

        ownership = json_value(output / OWNERSHIP)
        if not isinstance(ownership, dict):
            raise ProjectEmitterFailure("renamed ownership metadata is invalid")
        generated = ownership.get("filesGenerated")
        if (
            ownership.get("id") != 2
            or ownership.get("wasCached") is not False
            or not isinstance(generated, list)
            or any(relative in generated for relative in ORIGINAL_PAYLOADS)
            or not RENAMED_PAYLOADS.issubset(generated)
            or "src/emitter_fixture.user.c" in generated
        ):
            raise ProjectEmitterFailure(
                "renamed ownership metadata retained stale or user-owned paths"
            )

        symbols = json_value(output / "hxc.symbols.json")
        if not isinstance(symbols, dict) or not isinstance(symbols.get("symbols"), list):
            raise ProjectEmitterFailure("renamed symbol table is invalid")
        raw_source_symbols = [
            symbol.get("sourceSymbol")
            for symbol in symbols["symbols"]
            if isinstance(symbol, dict)
        ]
        if not all(isinstance(symbol, str) for symbol in raw_source_symbols):
            raise ProjectEmitterFailure(
                f"renamed symbol table contains an invalid source identity: {raw_source_symbols!r}"
            )
        source_symbols = set(raw_source_symbols)
        expected_symbols = {
            "fixture.RenamedApi.renamedValue",
            "fixture.RenamedMain",
        }
        if source_symbols != expected_symbols:
            raise ProjectEmitterFailure(
                f"renamed symbol table drifted: {sorted(source_symbols)!r}"
            )

        run_emitter("renamed", fresh, label="fresh renamed project render")
        assert_artifact_trees_equal(
            generated_artifact_tree(fresh),
            generated_artifact_tree(output),
            "incremental renamed-project determinism failure",
        )
        check_no_unstable_facts(output_tree(fresh), (ROOT, root, output, fresh))
        check_manifest(fresh)


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise ProjectEmitterFailure(
                "Haxe compiler server exited early\n"
                f"stdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise ProjectEmitterFailure(
        "Haxe compiler server did not accept connections within 10 seconds"
    )


def check_compiler_server_determinism() -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-project-server-") as temporary:
        root = Path(temporary)
        cold_full = root / "cold-full"
        cold_renamed = root / "cold-renamed"
        shared = root / "server-shared"
        cached_renamed = root / "server-cached-renamed"
        cached_reverse = root / "server-cached-reverse"
        run_emitter("full", cold_full, label="cold server-reference render")
        run_emitter("renamed", cold_renamed, label="cold renamed server-reference render")

        port = available_port()
        endpoint = str(port)
        environment = os.environ.copy()
        environment.pop("HAXE_NO_SERVER", None)
        server = subprocess.Popen(
            [development_tool("haxe"), "--wait", endpoint],
            cwd=ROOT,
            env=environment,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        try:
            wait_for_server(server, port)
            run_emitter(
                "full",
                shared,
                label="compiler-server warm-up render",
                connect=endpoint,
            )
            run_emitter(
                "renamed",
                shared,
                label="compiler-server incremental rename render",
                connect=endpoint,
            )
            run_emitter(
                "renamed",
                cached_renamed,
                label="cached compiler-server renamed render",
                connect=endpoint,
            )
            run_emitter(
                "reverse",
                cached_reverse,
                label="cached compiler-server reversed-order render",
                connect=endpoint,
            )

            assert_artifact_trees_equal(
                generated_artifact_tree(cold_renamed),
                generated_artifact_tree(shared),
                "compiler-server incremental rename determinism failure",
            )
            assert_artifact_trees_equal(
                generated_artifact_tree(cold_renamed),
                generated_artifact_tree(cached_renamed),
                "cached compiler-server renamed-root determinism failure",
            )
            assert_artifact_trees_equal(
                generated_artifact_tree(cold_full),
                generated_artifact_tree(cached_reverse),
                "cached compiler-server order determinism failure",
            )

            for relative in ORIGINAL_PAYLOADS:
                if (shared / relative).exists():
                    raise ProjectEmitterFailure(
                        "compiler-server rename retained stale owned artifact "
                        f"`{relative}`"
                    )
            shared_ownership = json_value(shared / OWNERSHIP)
            if (
                not isinstance(shared_ownership, dict)
                or shared_ownership.get("id") != 2
                or not isinstance(shared_ownership.get("wasCached"), bool)
            ):
                raise ProjectEmitterFailure(
                    "compiler-server ownership activity state did not advance safely"
                )
            for output in (shared, cached_renamed, cached_reverse):
                check_no_unstable_facts(
                    generated_artifact_tree(output),
                    (ROOT, root, output),
                )
        finally:
            server.terminate()
            try:
                server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                server.kill()
                server.wait(timeout=5)


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
    for mode in (
        "duplicate",
        "invalid-path",
        "invalid-layout",
        "invalid-line-endings",
        "lowered",
    ):
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

    invalid_ownership_documents: tuple[tuple[str, object], ...] = (
        ("array root", []),
        (
            "missing field",
            {"version": 1, "id": 1, "filesGenerated": []},
        ),
        (
            "wrong version",
            {
                "version": 2,
                "id": 1,
                "wasCached": False,
                "filesGenerated": [],
            },
        ),
        (
            "negative activity id",
            {
                "version": 1,
                "id": -1,
                "wasCached": False,
                "filesGenerated": [],
            },
        ),
        (
            "non-boolean cache state",
            {
                "version": 1,
                "id": 1,
                "wasCached": 0,
                "filesGenerated": [],
            },
        ),
        (
            "non-array files",
            {
                "version": 1,
                "id": 1,
                "wasCached": False,
                "filesGenerated": "src/main.c",
            },
        ),
    )
    for label, document in invalid_ownership_documents:
        with tempfile.TemporaryDirectory(
            prefix="reflaxe-c-project-schema-shape-"
        ) as temporary:
            output = Path(temporary) / "project"
            output.mkdir()
            (output / OWNERSHIP).write_text(
                json.dumps(document, separators=(",", ":")), encoding="utf-8"
            )
            expect_guard_failure("full", output, f"ownership {label} guard")

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
        check_difference_reporting()
        check_cross_root_order_locale_and_line_endings()
        check_unchanged_write_skip()
        check_stale_and_user_ownership()
        check_renamed_symbol_and_module_cleanup()
        check_compiler_server_determinism()
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
        "project-emitter: OK: precise byte-difference reports, isolated-root/order/"
        "locale/CRLF/server determinism, renamed-symbol stale ownership, skipped "
        "writes, path guards, strict placeholders, and exact HXC1001 no-output passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
