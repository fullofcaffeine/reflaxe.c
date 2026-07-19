#!/usr/bin/env python3
"""Prove native/tagged Haxe-enum lowering, recursive layout, and fail-closed edges."""

from __future__ import annotations

import argparse
import copy
import difflib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Iterable
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("enum_lowering.hxml")
FIXTURES = Path(__file__).with_name("fixtures")
POSITIVE = FIXTURES / "positive"
NATIVE = Path(__file__).with_name("native")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_ENUM_LOWERING="
PRODUCTION_FILES = {
    "_GeneratedFiles.json",
    "cmake/CMakeLists.txt",
    "hxc.abi.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.specializations.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
    "include/hxc/program.h",
    "meson.build",
    "src/program.c",
}

if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.c_fixture_harness import (  # noqa: E402
    C11_STRICT_FLAGS,
    CFixtureFailure,
    CFixtureProject,
    report_json,
    resolve_toolchains,
    run_c_fixture_corpus,
    validate_report,
)


CXX_STRICT_FLAGS = (
    "-std=c++17",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)
CXX_COMMANDS = {"gcc": "g++", "clang": "clang++"}
REQUIRED_COVERAGE = frozenset(
    {
        "checked-payload-projection",
        "c-consumer",
        "exhaustive-tag-switch",
        "fieldless-native-enum",
        "generated-executable",
        "recursive-finite-layout",
        "runtime-free",
        "tagged-union",
    }
)


class EnumLoweringFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return environment


def render(
    label: str, *, reverse: bool = False, profile: str = "portable"
) -> tuple[str, dict[str, object]]:
    if profile not in ("portable", "metal"):
        raise EnumLoweringFailure(f"unknown enum profile {profile!r}")
    command = [development_tool("haxe"), str(HXML)]
    if reverse:
        command.extend(["-D", "enum_lowering_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "enum_lowering_profile=metal"])
    result = subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if result.returncode != 0 or result.stderr or len(lines) != 1:
        raise EnumLoweringFailure(
            f"{label} emitted an invalid report\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise EnumLoweringFailure(f"{label} report is not an object")
    return payload, report


def required_text(report: dict[str, object], key: str) -> str:
    value = report.get(key)
    if not isinstance(value, str):
        raise EnumLoweringFailure(f"enum report field {key!r} must be text")
    return value


def source_records(report: dict[str, object]) -> dict[str, str]:
    records = report.get("sources")
    if not isinstance(records, list):
        raise EnumLoweringFailure("enum report omitted source records")
    result: dict[str, str] = {}
    ordered: list[str] = []
    for record in records:
        if (
            not isinstance(record, dict)
            or not isinstance(record.get("path"), str)
            or not isinstance(record.get("content"), str)
        ):
            raise EnumLoweringFailure(f"malformed enum source record: {record!r}")
        path = record["path"]
        if (
            path in result
            or path.startswith("/")
            or "\\" in path
            or any(part in ("", ".", "..") for part in path.split("/"))
        ):
            raise EnumLoweringFailure(f"unsafe or duplicate source path {path!r}")
        ordered.append(path)
        result[path] = record["content"]
    if ordered != sorted(ordered, key=lambda value: value.encode("utf-8")):
        raise EnumLoweringFailure("enum source records are not UTF-8 ordered")
    return result


def enum_records(report: dict[str, object]) -> list[dict[str, object]]:
    value = report.get("enums")
    if not isinstance(value, list) or not all(isinstance(item, dict) for item in value):
        raise EnumLoweringFailure("enum report omitted representation records")
    return value


def enum_by_name(report: dict[str, object], display_name: str) -> dict[str, object]:
    matches = [item for item in enum_records(report) if item.get("displayName") == display_name]
    if len(matches) != 1:
        raise EnumLoweringFailure(
            f"expected one enum record for {display_name!r}, found {len(matches)}"
        )
    return matches[0]


def case_by_name(record: dict[str, object], name: str) -> dict[str, object]:
    cases = record.get("cases")
    if not isinstance(cases, list) or not all(isinstance(item, dict) for item in cases):
        raise EnumLoweringFailure("enum case records are malformed")
    matches = [item for item in cases if item.get("semanticName") == name]
    if len(matches) != 1:
        raise EnumLoweringFailure(f"expected one constructor {name!r}")
    return matches[0]


def payload_by_name(record: dict[str, object], name: str) -> dict[str, object]:
    payload = record.get("payload")
    if not isinstance(payload, list) or not all(isinstance(item, dict) for item in payload):
        raise EnumLoweringFailure("enum payload records are malformed")
    matches = [item for item in payload if item.get("semanticName") == name]
    if len(matches) != 1:
        raise EnumLoweringFailure(f"expected one payload field {name!r}")
    return matches[0]


def required_identifier(record: dict[str, object], key: str) -> str:
    value = record.get(key)
    if not isinstance(value, str) or not value:
        raise EnumLoweringFailure(f"enum record omitted finalized C identifier {key!r}")
    return value


def enum_names(report: dict[str, object]) -> dict[str, str]:
    mode = enum_by_name(report, "Mode")
    option_int = enum_by_name(report, "Option<i32>")
    option_bool = enum_by_name(report, "Option<bool>")
    chain = enum_by_name(report, "Chain<i32>")
    mode_on = case_by_name(mode, "On")
    option_int_some = case_by_name(option_int, "Some")
    option_bool_some = case_by_name(option_bool, "Some")
    chain_end = case_by_name(chain, "End")
    chain_link = case_by_name(chain, "Link")
    values = {
        "mode_tag": mode.get("valueTag"),
        "mode_on": mode_on.get("discriminant"),
        "option_int_tag": option_int.get("valueTag"),
        "option_int_tag_member": option_int.get("tagMember"),
        "option_int_payload_member": option_int.get("payloadMember"),
        "option_int_some": option_int_some.get("discriminant"),
        "option_int_some_union": option_int_some.get("unionMember"),
        "option_int_some_value": payload_by_name(option_int_some, "value").get("cName"),
        "option_bool_tag": option_bool.get("valueTag"),
        "option_bool_tag_member": option_bool.get("tagMember"),
        "option_bool_payload_member": option_bool.get("payloadMember"),
        "option_bool_some": option_bool_some.get("discriminant"),
        "option_bool_some_union": option_bool_some.get("unionMember"),
        "option_bool_some_value": payload_by_name(option_bool_some, "value").get("cName"),
        "chain_tag": chain.get("valueTag"),
        "chain_tag_member": chain.get("tagMember"),
        "chain_payload_member": chain.get("payloadMember"),
        "chain_end": chain_end.get("discriminant"),
        "chain_end_union": chain_end.get("unionMember"),
        "chain_end_value": payload_by_name(chain_end, "value").get("cName"),
        "chain_link": chain_link.get("discriminant"),
        "chain_link_union": chain_link.get("unionMember"),
        "chain_link_value": payload_by_name(chain_link, "value").get("cName"),
        "chain_link_next": payload_by_name(chain_link, "next").get("cName"),
    }
    if not all(isinstance(value, str) and value for value in values.values()):
        raise EnumLoweringFailure("enum report omitted native-consumer identifiers")
    return {key: str(value) for key, value in values.items()}


def function_section(hxcir: str, field: str) -> str:
    start_marker = f'  function "function.EnumFixture.{field}"'
    end_marker = f'  end function "function.EnumFixture.{field}"'
    start = hxcir.find(start_marker)
    end = hxcir.find(end_marker, start)
    if start == -1 or end == -1:
        raise EnumLoweringFailure(f"HxcIR omitted function {field}")
    return hxcir[start : end + len(end_marker)]


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "haxe-enums-direct-runtime-free"
        or report.get("profile") != profile
        or report.get("runtimeFeatures") != []
        or report.get("runtimeArtifacts") != []
    ):
        raise EnumLoweringFailure(f"enum report contract drifted for {profile}")
    records = enum_records(report)
    if len(records) != 4:
        raise EnumLoweringFailure(f"expected four concrete enum instances, found {len(records)}")
    names = enum_names(report)
    mode = enum_by_name(report, "Mode")
    option_int = enum_by_name(report, "Option<i32>")
    option_bool = enum_by_name(report, "Option<bool>")
    chain = enum_by_name(report, "Chain<i32>")
    if (
        mode.get("representation") != "native-enum"
        or mode.get("recursive") is not False
        or mode.get("scopedLifetime") is not False
        or option_int.get("representation") != "tagged-union"
        or option_bool.get("representation") != "tagged-union"
        or chain.get("representation") != "tagged-union"
        or chain.get("recursive") is not True
        or chain.get("scopedLifetime") is not True
    ):
        raise EnumLoweringFailure("enum representation or recursion policy drifted")
    link_next = payload_by_name(case_by_name(chain, "Link"), "next")
    if link_next.get("indirect") is not True or not str(link_next.get("type", "")).startswith(
        "pointer:nonnull<instance:"
    ):
        raise EnumLoweringFailure("recursive Chain edge is not an explicit non-null pointer")
    for record in records:
        cases = record.get("cases")
        if not isinstance(cases, list) or [case.get("tagValue") for case in cases] != list(
            range(len(cases))
        ):
            raise EnumLoweringFailure("enum discriminants lost Haxe source order")

    hxcir = required_text(report, "hxcir")
    header = required_text(report, "header")
    sources = source_records(report)
    if set(sources) != {"src/program.c"}:
        raise EnumLoweringFailure(f"enum source partition drifted: {sorted(sources)!r}")
    source = sources["src/program.c"]
    for label, value in (("HxcIR", hxcir), ("header", header), ("source", source)):
        if str(ROOT) in value or "\\" in value or "hxrt" in value.lower():
            raise EnumLoweringFailure(f"{label} leaked a host path or runtime dependency")
    if not hxcir.startswith("hxcir schema=10\n") or hxcir.count(" representation=tagged ") != 3:
        raise EnumLoweringFailure("schema-10 tagged-union HxcIR inventory drifted")
    option_section = function_section(hxcir, "optionValue")
    recursive_section = function_section(hxcir, "recursiveLocal")
    main_section = function_section(hxcir, "main")
    option_int_instance = required_identifier(option_int, "instanceId")
    identity_call = main_section.find(
        'call dispatch=direct("function.EnumFixture.identity")'
    )
    identity_result_constructor = main_section.find(
        f'construct-tag instance="{option_int_instance}"', identity_call
    )
    if (
        "terminator tag-switch" not in option_section
        or "default=none" not in option_section
        or 'project-tag value=' not in option_section
        or f'check=checked-abort(profile="{profile}",build="debug")'
        not in option_section
        or "enum-recursive-payload-address" not in recursive_section
        or "enum-recursive-payload-load" not in recursive_section
        or identity_call == -1
        or identity_result_constructor == -1
        or identity_call > identity_result_constructor
    ):
        raise EnumLoweringFailure("enum construction, checking, switch, or recursive HxcIR drifted")
    if (
        f"enum {names['mode_tag']} {{" not in header
        or f"struct {names['option_int_tag']} {{" not in header
        or f"union {required_identifier(option_int, 'payloadUnionTag')} {{" not in header
        or f"struct {names['chain_tag']} *" not in header
        or "#include <stddef.h>" not in header
        or "#include <stdlib.h>" not in header
    ):
        raise EnumLoweringFailure("private native/tagged enum header drifted")
    if (
        source.count("_Static_assert(") < 20
        or "offsetof(" not in source
        or "switch (" not in source
        or "abort();" not in source
        or f"(struct {names['option_int_tag']}){{" not in source
        or "int main(void)" not in source
    ):
        raise EnumLoweringFailure("structural enum CAST emission or checks drifted")
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v1":
        raise EnumLoweringFailure("enum report omitted its finalized symbol table")


def normalized_profile(report: dict[str, object]) -> dict[str, object]:
    value = copy.deepcopy(report)
    value["profile"] = "<profile>"
    value["hxcir"] = str(value["hxcir"]).replace(
        'checked-abort(profile="metal",', 'checked-abort(profile="portable",'
    )
    return value


def snapshot_values(report: dict[str, object]) -> dict[str, object]:
    return {
        "enums.hxcir": required_text(report, "hxcir"),
        "program.h": required_text(report, "header"),
        "program.c": source_records(report)["src/program.c"],
        "enums.json": enum_records(report),
        "symbols.json": report.get("symbols"),
    }


def difference(expected: str, actual: str, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def check_snapshots(report: dict[str, object]) -> None:
    for name, actual in snapshot_values(report).items():
        path = EXPECTED / name
        if not path.is_file():
            raise EnumLoweringFailure(
                "enum snapshots are missing; run "
                "npm run snapshots:update -- --suite enum-lowering"
            )
        if name.endswith(".json"):
            expected = json.loads(path.read_text(encoding="utf-8"))
            if actual != expected:
                raise EnumLoweringFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise EnumLoweringFailure(f"{name} snapshot is not text")
            expected = path.read_text(encoding="utf-8")
            if actual != expected:
                raise EnumLoweringFailure(
                    f"{name} drifted:\n" + difference(expected, actual, name)
                )


def macro_definitions(report: dict[str, object]) -> tuple[str, ...]:
    names = enum_names(report)
    pairs = (
        ("HXC_MODE_TAG", "mode_tag"),
        ("HXC_MODE_ON", "mode_on"),
        ("HXC_OPTION_INT_TAG", "option_int_tag"),
        ("HXC_OPTION_INT_TAG_MEMBER", "option_int_tag_member"),
        ("HXC_OPTION_INT_PAYLOAD_MEMBER", "option_int_payload_member"),
        ("HXC_OPTION_INT_SOME", "option_int_some"),
        ("HXC_OPTION_INT_SOME_UNION", "option_int_some_union"),
        ("HXC_OPTION_INT_SOME_VALUE", "option_int_some_value"),
        ("HXC_OPTION_BOOL_TAG", "option_bool_tag"),
        ("HXC_OPTION_BOOL_TAG_MEMBER", "option_bool_tag_member"),
        ("HXC_OPTION_BOOL_PAYLOAD_MEMBER", "option_bool_payload_member"),
        ("HXC_OPTION_BOOL_SOME", "option_bool_some"),
        ("HXC_OPTION_BOOL_SOME_UNION", "option_bool_some_union"),
        ("HXC_OPTION_BOOL_SOME_VALUE", "option_bool_some_value"),
        ("HXC_CHAIN_TAG", "chain_tag"),
        ("HXC_CHAIN_TAG_MEMBER", "chain_tag_member"),
        ("HXC_CHAIN_PAYLOAD_MEMBER", "chain_payload_member"),
        ("HXC_CHAIN_END", "chain_end"),
        ("HXC_CHAIN_END_UNION", "chain_end_union"),
        ("HXC_CHAIN_END_VALUE", "chain_end_value"),
        ("HXC_CHAIN_LINK", "chain_link"),
        ("HXC_CHAIN_LINK_UNION", "chain_link_union"),
        ("HXC_CHAIN_LINK_VALUE", "chain_link_value"),
        ("HXC_CHAIN_LINK_NEXT", "chain_link_next"),
    )
    return tuple(f"-D{macro}={names[key]}" for macro, key in pairs)


def write_native_fixture(report: dict[str, object], root: Path) -> None:
    header = root / "include/hxc/program.h"
    source = root / "src/program.c"
    native = root / "native/layout_consumer.c"
    header.parent.mkdir(parents=True)
    source.parent.mkdir(parents=True)
    native.parent.mkdir(parents=True)
    header.write_text(required_text(report, "header"), encoding="utf-8", newline="\n")
    source.write_text(source_records(report)["src/program.c"], encoding="utf-8", newline="\n")
    shutil.copyfile(NATIVE / "layout_consumer.c", native)


def run_harness_matrix(
    report: dict[str, object], root: Path, *, requested_toolchain: str
) -> list[dict[str, object]]:
    fixture_root = root / "fixture"
    write_native_fixture(report, fixture_root)
    projects = (
        CFixtureProject(
            "enum-layout",
            ("native/layout_consumer.c",),
            ("include/hxc/program.h",),
            ("include",),
            "",
            (
                "c-consumer",
                "fieldless-native-enum",
                "tagged-union",
                "recursive-finite-layout",
            ),
        ),
        CFixtureProject(
            "generated-program",
            ("src/program.c",),
            ("include/hxc/program.h",),
            ("include",),
            "",
            (
                "checked-payload-projection",
                "exhaustive-tag-switch",
                "generated-executable",
                "runtime-free",
            ),
        ),
    )
    reports: list[dict[str, object]] = []
    for optimization in ("-O0", "-O2"):
        report_value = run_c_fixture_corpus(
            suite=f"enum-lowering-{optimization[1:].lower()}",
            projects=projects,
            fixture_root=fixture_root,
            build_root=root / f"c-build-{optimization[1:].lower()}",
            repository_root=ROOT,
            requested_toolchain=requested_toolchain,
            strict_flags=(*C11_STRICT_FLAGS, optimization, *macro_definitions(report)),
        )
        validate_report(report_value, required_coverage=REQUIRED_COVERAGE)
        encoded = report_json(report_value, compact=True)
        for forbidden in (str(ROOT), str(fixture_root), str(root)):
            if forbidden in encoded:
                raise EnumLoweringFailure(
                    f"native harness report leaked absolute path {forbidden}"
                )
        reports.append(report_value)
    return reports


def compiler_identity(executable: str) -> str:
    result = subprocess.run(
        [executable, "--version"],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        raise EnumLoweringFailure(f"cannot identify C++ compiler {executable}")
    text = (result.stdout + result.stderr).lower()
    if "clang" in text:
        return "clang"
    if "free software foundation" in text or "gcc" in text or "g++" in text:
        return "gcc"
    return "unknown"


def require_silent_success(command: list[str], *, label: str, cwd: Path = ROOT) -> None:
    result = subprocess.run(
        command,
        cwd=cwd,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise EnumLoweringFailure(
            f"{label} failed\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def check_cpp_layout(
    report: dict[str, object], root: Path, *, requested_toolchain: str
) -> None:
    toolchains = resolve_toolchains(requested_toolchain, repository_root=ROOT)
    definitions = macro_definitions(report)
    for toolchain in toolchains:
        cxx_name = CXX_COMMANDS[toolchain.family]
        cxx = shutil.which(cxx_name)
        if cxx is None or compiler_identity(cxx) != toolchain.family:
            if requested_toolchain != "auto":
                raise EnumLoweringFailure(
                    f"required {toolchain.family} C++17 companion {cxx_name!r} is unavailable"
                )
            print(
                f"enum-lowering: SKIP optional {toolchain.family} C++17 consumer: "
                f"missing or mismatched {cxx_name}"
            )
            continue
        for optimization in ("-O0", "-O2"):
            build = root / "cxx-build" / toolchain.family / optimization[1:].lower()
            build.mkdir(parents=True, exist_ok=True)
            provider_object = build / "layout_provider.o"
            consumer_object = build / "layout_consumer.o"
            executable = build / "layout_consumer"
            require_silent_success(
                [
                    toolchain.compiler,
                    *C11_STRICT_FLAGS,
                    optimization,
                    *definitions,
                    f"-I{root / 'fixture/include'}",
                    "-c",
                    str(NATIVE / "layout_provider.c"),
                    "-o",
                    str(provider_object),
                ],
                label=f"{toolchain.family} {optimization} C enum layout provider",
            )
            require_silent_success(
                [
                    cxx,
                    *CXX_STRICT_FLAGS,
                    optimization,
                    *definitions,
                    f"-I{root / 'fixture/include'}",
                    "-c",
                    str(NATIVE / "layout_consumer.cpp"),
                    "-o",
                    str(consumer_object),
                ],
                label=f"{toolchain.family} {optimization} C++17 enum layout consumer",
            )
            require_silent_success(
                [cxx, str(provider_object), str(consumer_object), "-o", str(executable)],
                label=f"{toolchain.family} {optimization} C/C++17 enum layout link",
            )
            require_silent_success(
                [str(executable)],
                label=f"{toolchain.family} {optimization} C/C++17 enum layout run",
                cwd=build,
            )


def check_native(
    report: dict[str, object], *, requested_toolchain: str = "auto"
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-enum-native-") as temporary:
        root = Path(temporary)
        run_harness_matrix(report, root, requested_toolchain=requested_toolchain)
        check_cpp_layout(report, root, requested_toolchain=requested_toolchain)


def custom_target(
    fixture: Path,
    output: Path,
    *,
    main: str,
    profile: str = "portable",
    runtime: str | None = None,
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        main,
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    command.extend(["--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def generated_tree(root: Path) -> dict[str, bytes]:
    return {
        path.relative_to(root).as_posix(): path.read_bytes()
        for path in sorted(root.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def generated_files(root: Path) -> set[str]:
    return {
        path.relative_to(root).as_posix()
        for path in root.rglob("*")
        if path.is_file()
    }


def require_compile_success(result: subprocess.CompletedProcess[str], label: str) -> None:
    if result.returncode != 0 or result.stdout or result.stderr:
        raise EnumLoweringFailure(
            f"{label} failed\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def validate_production(root: Path, *, profile: str, policy: str) -> None:
    if generated_files(root) != PRODUCTION_FILES:
        raise EnumLoweringFailure(
            f"{profile}/{policy} production artifact set drifted: "
            f"{sorted(generated_files(root))!r}"
        )
    manifest = json.loads((root / "hxc.manifest.json").read_text(encoding="utf-8"))
    runtime_plan = json.loads((root / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    specializations = json.loads(
        (root / "hxc.specializations.json").read_text(encoding="utf-8")
    )
    proof = runtime_plan.get("noRuntimeProof")
    reachability = proof.get("reachability") if isinstance(proof, dict) else None
    specialization_summary = specializations.get("summary")
    if (
        manifest.get("compilationStatus") != "lowered-direct-value-executable"
        or manifest.get("configuration", {}).get("profile") != profile
        or runtime_plan.get("schemaVersion") != 2
        or runtime_plan.get("status") != "analyzed-runtime-free"
        or runtime_plan.get("profile") != profile
        or runtime_plan.get("resolvedPolicy") != policy
        or runtime_plan.get("features") != []
        or runtime_plan.get("artifacts") != []
        or "bounded-haxe-enum-values" not in runtime_plan.get("directDecisions", [])
        or "closed-generic-specializations"
        not in runtime_plan.get("directDecisions", [])
        or "closed-anonymous-value-records" in runtime_plan.get("directDecisions", [])
        or specializations.get("schemaVersion") != 1
        or specializations.get("algorithm") != "hxc-generic-specialization-v1"
        or specializations.get("status") != "analyzed-closed-specializations"
        or specializations.get("functionSpecializations") != []
        or not isinstance(specializations.get("typeSpecializations"), list)
        or len(specializations["typeSpecializations"]) != 3
        or not isinstance(specialization_summary, dict)
        or specialization_summary.get("functionSpecializations") != 0
        or specialization_summary.get("typeSpecializations") != 3
        or not isinstance(proof, dict)
        or proof.get("status") != "eligible"
        or proof.get("directDecisions") != runtime_plan.get("directDecisions")
        or reachability is None
        or reachability.get("typeInstances") != 4
        or reachability.get("runtimeIntents") != 0
    ):
        raise EnumLoweringFailure(f"{profile}/{policy} lost its runtime-free enum proof")
    combined = b"\n".join(
        path.read_bytes()
        for path in root.rglob("*")
        if path.is_file() and path.suffix in {".c", ".h"}
    ).lower()
    if b"hxrt" in combined or b"hxc_runtime" in combined:
        raise EnumLoweringFailure("enum production project selected runtime code")


def check_production(*, requested_toolchain: str) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-enum-production-") as temporary:
        root = Path(temporary)
        matrix = (
            ("first", "portable", None, "auto"),
            ("repeat", "portable", None, "auto"),
            ("none", "portable", "none", "none"),
            ("metal", "metal", None, "minimal"),
        )
        for name, profile, runtime, policy in matrix:
            output = root / name
            result = custom_target(
                POSITIVE,
                output,
                main="EnumFixture",
                profile=profile,
                runtime=runtime,
            )
            require_compile_success(result, f"{name} enum production compile")
            validate_production(output, profile=profile, policy=policy)
        if generated_tree(root / "first") != generated_tree(root / "repeat"):
            raise EnumLoweringFailure("two enum production roots were not byte-identical")
        for relative in ("include/hxc/program.h", "src/program.c"):
            if (root / "first" / relative).read_bytes() != (
                root / "metal" / relative
            ).read_bytes():
                raise EnumLoweringFailure(
                    f"portable and metal changed direct enum artifact {relative}"
                )
        project = CFixtureProject(
            "production-enum-program",
            ("src/program.c",),
            ("include/hxc/program.h",),
            ("include",),
            "",
            ("generated-executable", "runtime-free"),
        )
        for optimization in ("-O0", "-O2"):
            native_report = run_c_fixture_corpus(
                suite=f"enum-production-{optimization[1:].lower()}",
                projects=(project,),
                fixture_root=root / "first",
                build_root=root / f"production-build-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            validate_report(
                native_report,
                required_coverage=frozenset({"generated-executable", "runtime-free"}),
            )


def check_negative_cases() -> None:
    cases = {
        "recursive_escape": (
            "HXC1001",
            "TFunction(argument:value:recursive-enum-requires-escape-analysis)",
        ),
        "recursive_return": (
            "HXC1001",
            "TFunction(return-type:recursive-enum-requires-escape-analysis)",
        ),
        "reference_payload": ("HXC1001", "Text.value:reference-String-non-null"),
        "aggregate_payload": ("HXC1001", "unsupported-payload:Bad.PairValue.value:closed-record:"),
        "nonexhaustive": ("Unmatched patterns: On", "fixtures/nonexhaustive/Main.hx:"),
    }
    with tempfile.TemporaryDirectory(prefix="hxc-enum-negative-") as temporary:
        root = Path(temporary)
        for directory, markers in cases.items():
            output = root / directory
            result = custom_target(FIXTURES / directory, output, main="Main")
            combined = (result.stdout + result.stderr).replace("\\", "/")
            if (
                result.returncode != 1
                or any(marker not in combined for marker in markers)
                or f"fixtures/{directory}/Main.hx:" not in combined
                or generated_files(output)
            ):
                raise EnumLoweringFailure(
                    f"{directory} did not fail closed at its exact source with no output\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )


def snapshot_report() -> dict[str, object]:
    return {
        "schemaVersion": 1,
        "status": "haxe-enums-direct-runtime-free",
        "profile": "portable",
        "hxcir": (EXPECTED / "enums.hxcir").read_text(encoding="utf-8"),
        "header": (EXPECTED / "program.h").read_text(encoding="utf-8"),
        "sources": [
            {
                "path": "src/program.c",
                "content": (EXPECTED / "program.c").read_text(encoding="utf-8"),
            }
        ],
        "enums": json.loads((EXPECTED / "enums.json").read_text(encoding="utf-8")),
        "symbols": json.loads((EXPECTED / "symbols.json").read_text(encoding="utf-8")),
        "runtimeFeatures": [],
        "runtimeArtifacts": [],
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("enum-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            report = snapshot_report()
            validate(report)
            check_native(report, requested_toolchain=args.toolchain)
            print("enum-lowering: OK: required enum native matrix passed")
            return 0

        first_payload, first = render("first enum render")
        second_payload, second = render("second enum render")
        reverse_payload, reverse = render("reverse-input enum render", reverse=True)
        metal_payload, metal = render("metal enum render", profile="metal")
        if first_payload != second_payload or first != second:
            raise EnumLoweringFailure("two enum renders differed")
        if first_payload != reverse_payload or first != reverse:
            raise EnumLoweringFailure("enum render changed with discovery order")
        validate(first)
        validate(metal, profile="metal")
        if normalized_profile(first) != normalized_profile(metal):
            raise EnumLoweringFailure("portable and metal enum lowering diverged")
        check_snapshots(first)
        check_native(first, requested_toolchain=args.toolchain)
        check_production(requested_toolchain=args.toolchain)
        check_negative_cases()
    except (
        EnumLoweringFailure,
        CFixtureFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"enum-lowering: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "enum-lowering: OK: native and tagged representations, concrete generic "
        "specialization, checked exhaustive matches, finite recursive layout, strict "
        "C11 and C++17 agreement, runtime-free production, and fail-closed edges passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
