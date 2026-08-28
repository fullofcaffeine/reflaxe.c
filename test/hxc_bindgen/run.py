#!/usr/bin/env python3
"""Prove deterministic Clang authority, locks, input hashing, and diagnostics."""

from __future__ import annotations

import json
import hashlib
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402


HAXE = ROOT / "node_modules/.bin/haxe"
SCHEMA = ROOT / "schemas/hxc-bindings-lock.schema.json"


class BindgenFailure(RuntimeError):
    pass


def require(condition: bool, message: str) -> None:
    if not condition:
        raise BindgenFailure(message)


def environment() -> dict[str, str]:
    value = os.environ.copy()
    value["HAXE_NO_SERVER"] = "1"
    value["LC_ALL"] = "C"
    return value


def invoke(*arguments: str) -> subprocess.CompletedProcess[str]:
    return run_bounded_process(
        [str(HAXE), "--cwd", str(ROOT), "-cp", "src", "--run", "Run", "bindgen", *arguments],
        cwd=ROOT,
        env=environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def check_large_child_streams(temporary: Path) -> None:
    child = temporary / "large-streams.py"
    child.write_text(
        "import sys\n"
        "stream = sys.stdout if sys.argv[1] == 'stdout' else sys.stderr\n"
        "stream.write(('o' if sys.argv[1] == 'stdout' else 'e') * 262144)\n"
        "stream.flush()\n",
        encoding="utf-8",
    )
    result = run_bounded_process(
        [
            str(HAXE),
            "--cwd",
            str(ROOT),
            "-cp",
            "src",
            "-cp",
            "test/hxc_bindgen",
            "--run",
            "HxcBindgenProcessProbe",
            sys.executable,
            str(child),
        ],
        cwd=ROOT,
        env=environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    require(
        result.returncode == 0 and result.stdout == "hxc-bindgen-process: OK\n",
        f"large constrained child stream was not drained: {result.stderr}",
    )


def write_fixture(root: Path, *, broken: bool = False, warning: bool = False) -> Path:
    root.mkdir(parents=True)
    (root / "detail.h").write_text(
        "#ifndef DETAIL_H\n#define DETAIL_H\ntypedef unsigned short widget_id;\n#endif\n",
        encoding="utf-8",
    )
    header = root / "widget.h"
    header.write_text(
        ("#warning bindgen-warning\n" if warning else "")
        + '#include "detail.h"\n'
        "typedef struct Widget { widget_id id; int count; } Widget;\n"
        "#if HXC_WIDGET_FEATURE\n"
        + ("int widget_sum(Widget value {\n" if broken else "int widget_sum(Widget value);\n")
        + "#endif\n",
        encoding="utf-8",
    )
    return header


def machine_lock(header: Path, output: Path, *options: str) -> tuple[dict[str, object], str]:
    return configured_lock(
        [header],
        output,
        "--include-dir",
        str(header.parent),
        "--define",
        "HXC_WIDGET_FEATURE=1",
        *options,
    )


def configured_lock(entries: list[Path], output: Path, *options: str) -> tuple[dict[str, object], str]:
    result = invoke(
        *(str(entry) for entry in entries),
        "--output",
        str(output),
        "--json",
        *options,
    )
    require(result.returncode == 0 and result.stderr == "", f"bindgen failed: {result.stderr}")
    envelope = json.loads(result.stdout)
    require(envelope.get("command") == "bindgen" and envelope.get("exitCategory") == "success", "CLI envelope drifted")
    payload = envelope.get("stdout")
    require(isinstance(payload, str) and payload.endswith("\n"), "bindgen JSON payload framing drifted")
    lock = json.loads(payload)
    require((output / "hxc.bindings.lock.json").read_text(encoding="utf-8") == payload, "written lock and machine payload differ")
    return lock, payload


def walk(value: object):
    yield value
    if isinstance(value, dict):
        for child in value.values():
            yield from walk(child)
    elif isinstance(value, list):
        for child in value:
            yield from walk(child)


def has_object_key(value: object, key: str) -> bool:
    if isinstance(value, dict):
        return key in value or any(has_object_key(child, key) for child in value.values())
    if isinstance(value, list):
        return any(has_object_key(child, key) for child in value)
    return False


def check_schema_and_semantics(temporary: Path) -> None:
    schema = json.loads(SCHEMA.read_text(encoding="utf-8"))
    required = {
        "schemaVersion",
        "authority",
        "generator",
        "toolchain",
        "configuration",
        "configurationSha256",
        "invocation",
        "inputs",
        "inputSetSha256",
        "semanticModel",
        "semanticSha256",
    }
    require(schema.get("additionalProperties") is False and set(schema.get("required", ())) == required, "binding-lock schema drifted")

    source = temporary / "source-a"
    header = write_fixture(source)
    lock, text = machine_lock(header, temporary / "output-a")
    require(lock.get("schemaVersion") == 2 and lock.get("authority") == "clang-ast-json", "Clang authority is absent")
    toolchain = lock.get("toolchain")
    configuration = lock.get("configuration")
    require(isinstance(configuration, dict), "effective bindgen configuration is absent")
    effective = configuration.get("effective")
    provenance = configuration.get("provenance")
    invocation = lock.get("invocation")
    require(isinstance(toolchain, dict) and "clang" in str(toolchain.get("version", "")).lower(), "Clang version is absent")
    require(isinstance(toolchain.get("dumpMachine"), str) and toolchain.get("dumpMachine"), "Clang target identity is absent")
    require(isinstance(effective, dict) and effective.get("target") == toolchain.get("dumpMachine"), "effective target is not locked")
    require(
        isinstance(provenance, dict)
        and provenance.get("language") == "default"
        and provenance.get("target") == "clang-default"
        and provenance.get("sysroot") == "absent",
        "configuration provenance is incomplete",
    )
    encoded_effective = json.dumps(effective, ensure_ascii=False, separators=(",", ":")).encode("utf-8")
    require(
        lock.get("configurationSha256") == hashlib.sha256(encoded_effective).hexdigest(),
        "effective configuration hash does not match its canonical values",
    )
    require(isinstance(invocation, dict), "exact Clang invocation is absent")
    diagnostic_arguments = invocation.get("diagnosticArguments")
    arguments = invocation.get("semanticArguments")
    dependency_arguments = invocation.get("dependencyArguments")
    require(
        isinstance(arguments, list)
        and "-DHXC_WIDGET_FEATURE=1" in arguments
        and any(str(value).startswith("-I$SOURCE") for value in arguments)
        and arguments[-4:] == ["-w", "-fsyntax-only", "-Xclang", "-ast-dump=json"]
        and isinstance(diagnostic_arguments, list)
        and diagnostic_arguments[-1:] == ["-fsyntax-only"]
        and isinstance(dependency_arguments, list)
        and dependency_arguments[-3:] == ["-M", "-MT", "hxc-bindgen-input"],
        "exact semantic invocation is not inspectable",
    )
    inputs = lock.get("inputs")
    require(
        isinstance(inputs, list)
        and [item.get("path") for item in inputs if isinstance(item, dict)] == ["$SOURCE/detail.h", "$SOURCE/widget.h"],
        "transitive input inventory is incomplete or unstable",
    )
    model = lock.get("semanticModel")
    require(isinstance(model, dict) and model.get("declarationCount", 0) > 0, "semantic declaration model is empty")
    nodes = list(walk(model.get("translationUnit")))
    require(any(isinstance(node, dict) and node.get("kind") == "RecordDecl" and node.get("name") == "Widget" for node in nodes), "record declaration is absent")
    require(any(isinstance(node, dict) and node.get("kind") == "FunctionDecl" and node.get("name") == "widget_sum" for node in nodes), "function declaration is absent")
    require(not has_object_key(model.get("translationUnit"), "id"), "ephemeral Clang node identities leaked into the lock")
    require(str(source) not in text, "absolute source-root path leaked into the lock")

    explicit_target = toolchain.get("dumpMachine")
    require(isinstance(explicit_target, str), "Clang target identity has the wrong type")
    explicit_lock, explicit_text = machine_lock(header, temporary / "output-explicit-target", "--target", explicit_target)
    explicit_configuration = explicit_lock.get("configuration")
    require(
        explicit_lock.get("configurationSha256") == lock.get("configurationSha256")
        and isinstance(explicit_configuration, dict)
        and isinstance(explicit_configuration.get("provenance"), dict)
        and explicit_configuration["provenance"].get("target") == "command-line",
        "an explicit equivalent target changed effective configuration identity or lost provenance",
    )
    require(explicit_text != text, "configuration provenance did not distinguish an explicit target")


def check_configuration_and_reachability(temporary: Path) -> None:
    root = temporary / "entry-set"
    root.mkdir()
    first = root / "first.h"
    second = root / "second.h"
    hidden = root / "not-in-entry-set.h"
    first.write_text("#define HXC_ENTRY_VALUE 7\n", encoding="utf-8")
    second.write_text(
        "#if HXC_ENTRY_VALUE != 7\n#error entry order lost\n#endif\n"
        "#if HXC_VISIBLE\nint entry_visible(void);\n#endif\n",
        encoding="utf-8",
    )
    hidden.write_text("int must_not_be_reachable(void);\n", encoding="utf-8")
    lock, text = configured_lock(
        [first, second],
        temporary / "entry-output",
        "--include-dir",
        str(root),
        "--define",
        "HXC_VISIBLE=1",
    )
    configuration = lock.get("configuration")
    require(isinstance(configuration, dict) and isinstance(configuration.get("effective"), dict), "entry-set configuration is absent")
    effective = configuration["effective"]
    require(effective.get("entryHeaders") == ["$SOURCE/first.h", "$SOURCE/second.h"], "ordered entry headers were not locked")
    semantic_arguments = lock.get("invocation", {}).get("semanticArguments") if isinstance(lock.get("invocation"), dict) else None
    require(
        isinstance(semantic_arguments, list)
        and ["-include", "$SOURCE/first.h"] == semantic_arguments[-7:-5]
        and semantic_arguments[-5] == "$SOURCE/second.h",
        "ordered entry headers did not form one inspectable Clang translation unit",
    )
    nodes = list(walk(lock.get("semanticModel", {}).get("translationUnit") if isinstance(lock.get("semanticModel"), dict) else None))
    require(any(isinstance(node, dict) and node.get("name") == "entry_visible" for node in nodes), "configured conditional declaration is absent")
    require(not any(isinstance(node, dict) and node.get("name") == "must_not_be_reachable" for node in nodes), "unconfigured header leaked into generation scope")
    require("not-in-entry-set.h" not in text, "unconfigured header leaked into the lock")

    first_order, _ = configured_lock(
        [first, second],
        temporary / "define-order-a",
        "--define",
        "HXC_VISIBLE=1",
        "--define",
        "ALPHA=1",
        "--define",
        "BETA=2",
    )
    second_order, _ = configured_lock(
        [first, second],
        temporary / "define-order-b",
        "--define",
        "BETA=2",
        "--define",
        "ALPHA=1",
        "--define",
        "HXC_VISIBLE=1",
    )
    require(first_order.get("configurationSha256") == second_order.get("configurationSha256"), "equivalent define order changed configuration identity")
    require(first_order.get("semanticSha256") == second_order.get("semanticSha256"), "equivalent define order changed semantic declarations")

    disabled, _ = configured_lock([first, second], temporary / "conditional-off", "--define", "HXC_VISIBLE=0")
    require(disabled.get("configurationSha256") != lock.get("configurationSha256"), "conditional define drift did not change configuration identity")
    disabled_nodes = list(walk(disabled.get("semanticModel", {}).get("translationUnit") if isinstance(disabled.get("semanticModel"), dict) else None))
    require(not any(isinstance(node, dict) and node.get("name") == "entry_visible" for node in disabled_nodes), "disabled declaration remained reachable")


def check_language_sysroot_and_conflicts(temporary: Path) -> None:
    root = temporary / "configuration"
    root.mkdir()
    cxx = root / "entry.hpp"
    cxx.write_text("namespace hxc_bindgen { struct Entry {}; }\n", encoding="utf-8")
    sysroot = root / "sysroot"
    sysroot.mkdir()
    lock, _ = configured_lock(
        [cxx],
        temporary / "cxx-output",
        "--language",
        "c++",
        "--sysroot",
        str(sysroot),
    )
    configuration = lock.get("configuration")
    effective = configuration.get("effective") if isinstance(configuration, dict) else None
    provenance = configuration.get("provenance") if isinstance(configuration, dict) else None
    require(
        isinstance(effective, dict)
        and effective.get("language") == "c++"
        and effective.get("sysroot") == "$SYSROOT"
        and isinstance(provenance, dict)
        and provenance.get("language") == "command-line"
        and provenance.get("sysroot") == "command-line",
        "language/sysroot values or provenance are incomplete",
    )

    duplicate_path = str(root / ".." / root.name)
    conflicts = (
        ([str(cxx), str(cxx), "--dry-run"], "duplicate entry header"),
        ([str(cxx), "--include-dir", str(root), "--include-dir", duplicate_path, "--dry-run"], "duplicate include directory"),
        ([str(cxx), "--define", "SAME=1", "--define", "SAME=2", "--dry-run"], "repeated preprocessor definition"),
        ([str(cxx), "--language", "objective-c", "--dry-run"], "unsupported bindgen language"),
    )
    for arguments, expected in conflicts:
        result = invoke(*arguments, "--json")
        require(result.returncode == 64 and expected in result.stderr, f"configuration conflict did not fail early: {expected}")


def check_relocation_and_dry_run(temporary: Path) -> None:
    first = temporary / "relocated-a"
    second = temporary / "relocated-b"
    first_header = write_fixture(first)
    second_header = write_fixture(second)
    _, first_text = machine_lock(first_header, temporary / "lock-a")
    _, second_text = machine_lock(second_header, temporary / "lock-b")
    require(first_text == second_text, "equivalent source roots changed binding-lock bytes")

    dry_output = temporary / "dry-output"
    dry = invoke(
        str(first_header),
        "--include-dir",
        str(first),
        "--define",
        "HXC_WIDGET_FEATURE=1",
        "--output",
        str(dry_output),
        "--dry-run",
    )
    require(dry.returncode == 0 and dry.stderr == "" and dry.stdout.startswith("{\n"), "human dry-run did not return the lock")
    require(not dry_output.exists(), "dry-run wrote output")


def check_diagnostics_and_input_drift(temporary: Path) -> None:
    broken_root = temporary / "broken"
    broken = write_fixture(broken_root, broken=True)
    result = invoke(str(broken), "--include-dir", str(broken_root), "--define", "HXC_WIDGET_FEATURE=1", "--dry-run", "--json")
    require(result.returncode == 1, "invalid header did not preserve Clang failure")
    envelope = json.loads(result.stdout)
    require(envelope.get("exitCategory") == "command" and "widget.h:4:" in str(envelope.get("stderr")), "source file and line were lost")
    require("widget.h:4:" in result.stderr and "HXC-CLI-0803" in result.stderr, "human diagnostic stream lost Clang context")

    warning_root = temporary / "warning"
    warning = write_fixture(warning_root, warning=True)
    result = invoke(str(warning), "--include-dir", str(warning_root), "--define", "HXC_WIDGET_FEATURE=1", "--dry-run", "--json")
    require(result.returncode == 0, "non-fatal Clang diagnostic changed command success")
    envelope = json.loads(result.stdout)
    require("widget.h:1:" in str(envelope.get("stderr")) and "bindgen-warning" in result.stderr, "non-fatal source diagnostic was discarded")

    root = temporary / "drift"
    header = write_fixture(root)
    first, _ = machine_lock(header, temporary / "drift-output-a")
    (root / "detail.h").write_text("typedef unsigned int widget_id;\n", encoding="utf-8")
    second, _ = machine_lock(header, temporary / "drift-output-b")
    require(first.get("inputSetSha256") != second.get("inputSetSha256"), "transitive header change did not invalidate input lock")
    require(first.get("semanticSha256") != second.get("semanticSha256"), "semantic model ignored changed declaration facts")


def main() -> int:
    require(HAXE.is_file(), "hxc bindgen tests require the pinned Haxe executable")
    require(shutil.which("clang") is not None, "hxc bindgen tests require Clang")
    with tempfile.TemporaryDirectory(prefix="hxc-bindgen-") as temporary:
        root = Path(temporary)
        check_large_child_streams(root)
        check_schema_and_semantics(root)
        check_configuration_and_reachability(root)
        check_language_sysroot_and_conflicts(root)
        check_relocation_and_dry_run(root)
        check_diagnostics_and_input_drift(root)
    print("hxc-bindgen: OK: Clang authority, entry-set reachability, normalized configuration/provenance, conflicts, exact inputs, and diagnostics passed")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except BindgenFailure as error:
        print(f"hxc-bindgen: ERROR: {error}", file=sys.stderr)
        raise SystemExit(1)
