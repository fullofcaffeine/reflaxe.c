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
        "#define WIDGET_LIMIT (1u << 5)\n"
        "typedef const volatile unsigned long widget_word;\n"
        "typedef const int * restrict widget_read_ptr;\n"
        "typedef enum { WIDGET_MODE_NEGATIVE = -1, WIDGET_MODE_READY = 7 } widget_mode;\n"
        "enum { WIDGET_ANONYMOUS = 9 };\n"
        "enum WidgetState { WIDGET_STATE_IDLE = 0 };\n"
        "typedef struct Widget { widget_id id; int count; } Widget;\n"
        "typedef struct WidgetHandle WidgetHandle;\n"
        "typedef struct __attribute__((packed, aligned(2))) {\n"
        "  int x; unsigned a : 3; unsigned : 0; unsigned b : 5;\n"
        "  union { short shortValue; char bytes[2]; };\n"
        "  char tail[];\n"
        "} WidgetPacket;\n"
        "union WidgetChoice { int integerValue; float floatValue; };\n"
        "struct WidgetPlatform { char marker; long nativeLong; void *pointer; };\n"
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
        "primitiveAbiModel",
        "primitiveAbiSha256",
        "aggregateAbiModel",
        "aggregateAbiSha256",
    }
    require(schema.get("additionalProperties") is False and set(schema.get("required", ())) == required, "binding-lock schema drifted")

    source = temporary / "source-a"
    header = write_fixture(source)
    lock, text = machine_lock(header, temporary / "output-a")
    require(lock.get("schemaVersion") == 4 and lock.get("authority") == "clang-ast-json", "Clang authority is absent")
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
    abi_arguments = invocation.get("abiArguments")
    aggregate_arguments = invocation.get("aggregateArguments")
    require(
        isinstance(arguments, list)
        and "-DHXC_WIDGET_FEATURE=1" in arguments
        and any(str(value).startswith("-I$SOURCE") for value in arguments)
        and arguments[-4:] == ["-w", "-fsyntax-only", "-Xclang", "-ast-dump=json"]
        and isinstance(diagnostic_arguments, list)
        and diagnostic_arguments[-1:] == ["-fsyntax-only"]
        and isinstance(dependency_arguments, list)
        and dependency_arguments[-3:] == ["-M", "-MT", "hxc-bindgen-input"]
        and isinstance(abi_arguments, list)
        and abi_arguments[-5:] == ["-w", "-fsyntax-only", "-Xclang", "-ast-dump=json", "-"]
        and isinstance(aggregate_arguments, list)
        and aggregate_arguments[-5:] == ["-fsyntax-only", "-Xclang", "-fdump-record-layouts-simple", "-Xclang", "-fdump-record-layouts-complete"],
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

    abi = lock.get("primitiveAbiModel")
    require(isinstance(abi, dict) and abi.get("schemaVersion") == 1, "primitive ABI model is absent")
    scalars = {item.get("id"): item for item in abi.get("scalars", ()) if isinstance(item, dict)}
    require(
        scalars.get("signed-long", {}).get("haxeType") in {"c.Int32", "c.Int64"}
        and scalars.get("unsigned-short", {}).get("haxeType") == "c.UInt16"
        and scalars.get("binary32", {}).get("haxeType") == "c.Float32"
        and scalars.get("binary64", {}).get("haxeType") == "Float",
        "target-measured scalar carriers are incomplete",
    )
    typedefs = {item.get("nativeName"): item for item in abi.get("typedefs", ()) if isinstance(item, dict)}
    word = typedefs.get("widget_word", {}).get("representation")
    pointer = typedefs.get("widget_read_ptr", {}).get("representation")
    require(
        isinstance(word, dict)
        and word.get("kind") == "qualified"
        and word.get("qualifiers") == ["const", "volatile"]
        and isinstance(word.get("inner"), dict)
        and word["inner"].get("haxeType") in {"c.UInt32", "c.UInt64"},
        "qualified scalar typedef identity was not preserved",
    )
    require(
        isinstance(pointer, dict)
        and pointer.get("kind") == "qualified"
        and pointer.get("qualifiers") == ["restrict"]
        and isinstance(pointer.get("inner"), dict)
        and pointer["inner"].get("kind") == "pointer",
        "pointer-level restrict qualifier was not preserved",
    )
    enums = abi.get("enums")
    require(
        isinstance(enums, list)
        and any(
            isinstance(item, dict)
            and item.get("stableName") == "widget_mode"
            and item.get("nativeTypedef") == "widget_mode"
            and item.get("storageBitWidth") in {8, 16, 32, 64}
            and item.get("haxeType") in {"c.Int8", "c.UInt8", "c.Int16", "c.UInt16", "c.Int32", "c.UInt32", "c.Int64", "c.UInt64"}
            and {constant.get("nativeName"): constant.get("value") for constant in item.get("constants", ()) if isinstance(constant, dict)}
            == {"WIDGET_MODE_NEGATIVE": "-1", "WIDGET_MODE_READY": "7"}
            for item in enums
        ),
        "typedef-owned anonymous enum identity or evaluated values are unstable",
    )
    require(
        any(
            isinstance(item, dict)
            and str(item.get("stableName", "")).startswith("anonymous-enum-")
            and any(
                isinstance(constant, dict)
                and constant.get("nativeName") == "WIDGET_ANONYMOUS"
                and constant.get("value") == "9"
                for constant in item.get("constants", ())
            )
            for item in enums
        ),
        "unaliased anonymous enum name is not deterministic",
    )
    require(
        any(
            isinstance(item, dict)
            and item.get("stableName") == "WidgetState"
            and item.get("nativeTag") == "WidgetState"
            and item.get("storageBitWidth") in {8, 16, 32, 64}
            for item in enums
        ),
        "named enum identity or measured storage is absent",
    )
    macros = {item.get("nativeName"): item for item in abi.get("macroConstants", ()) if isinstance(item, dict)}
    require(
        macros.get("WIDGET_LIMIT", {}).get("value") == "32"
        and macros.get("WIDGET_LIMIT", {}).get("haxeType") == "c.UInt32",
        "Clang-evaluated integer macro constant is absent",
    )
    check_compiled_primitive_probe(source, scalars, macros)

    aggregate_abi = lock.get("aggregateAbiModel")
    require(isinstance(aggregate_abi, dict) and aggregate_abi.get("schemaVersion") == 1, "aggregate ABI model is absent")
    records = {item.get("stableName"): item for item in aggregate_abi.get("records", ()) if isinstance(item, dict)}
    handle = records.get("WidgetHandle", {})
    packet = records.get("WidgetPacket", {})
    choice = records.get("WidgetChoice", {})
    require(
        handle.get("opaque") is True and handle.get("complete") is False and handle.get("layout") is None and handle.get("fields") == [],
        "incomplete handle did not remain opaque",
    )
    packet_layout = packet.get("layout")
    packet_fields = packet.get("fields")
    require(
        packet.get("nativeTypedef") == "WidgetPacket"
        and isinstance(packet_layout, dict)
        and packet_layout.get("packed") is True
        and packet_layout.get("requestedAlignmentBits") == 16
        and isinstance(packet_fields, list)
        and any(isinstance(field, dict) and field.get("bitWidth") == 0 and field.get("zeroWidthBitfield") is True for field in packet_fields)
        and any(isinstance(field, dict) and field.get("anonymous") is True for field in packet_fields)
        and any(isinstance(field, dict) and field.get("nativeName") == "tail" and field.get("flexibleArray") is True for field in packet_fields),
        "packed, bitfield, anonymous-member, or flexible-array facts are incomplete",
    )
    require(
        choice.get("kind") == "union"
        and isinstance(choice.get("fields"), list)
        and all(isinstance(field, dict) and field.get("bitOffset") == 0 for field in choice["fields"]),
        "union field offsets are incomplete",
    )
    check_compiled_aggregate_probe(source, records)

    windows_lock, _ = machine_lock(
        header,
        temporary / "output-windows-target",
        "--target",
        "x86_64-pc-windows-msvc",
    )
    windows_abi = windows_lock.get("primitiveAbiModel")
    require(isinstance(windows_abi, dict), "Windows target primitive ABI model is absent")
    windows_scalars = {
        item.get("id"): item
        for item in windows_abi.get("scalars", ())
        if isinstance(item, dict)
    }
    require(
        windows_scalars.get("signed-long", {}).get("bitWidth") == 32
        and windows_scalars.get("signed-long", {}).get("haxeType") == "c.Int32",
        "selected Windows target did not use Clang's LLP64 long mapping",
    )
    windows_aggregates = windows_lock.get("aggregateAbiModel")
    require(isinstance(windows_aggregates, dict), "Windows target aggregate ABI model is absent")
    windows_records = {
        item.get("stableName"): item
        for item in windows_aggregates.get("records", ())
        if isinstance(item, dict)
    }
    host_platform = records.get("WidgetPlatform", {}).get("layout")
    windows_platform = windows_records.get("WidgetPlatform", {}).get("layout")
    require(
        isinstance(host_platform, dict)
        and isinstance(windows_platform, dict)
        and host_platform.get("sizeBits") != windows_platform.get("sizeBits"),
        "selected Windows target did not change pointer/long-sensitive aggregate layout",
    )

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


def check_compiled_primitive_probe(
    source: Path,
    scalars: dict[object, dict[str, object]],
    macros: dict[object, dict[str, object]],
) -> None:
    probe = source / "primitive-probe.c"
    probe.write_text(
        '#include <limits.h>\n#include <stdio.h>\n#include "widget.h"\n'
        'int main(void) { printf("%zu %zu %d %u\\n", sizeof(long) * CHAR_BIT, '
        'sizeof(unsigned short) * CHAR_BIT, WIDGET_MODE_NEGATIVE, WIDGET_LIMIT); return 0; }\n',
        encoding="utf-8",
    )
    for compiler in ("clang", "gcc"):
        executable = shutil.which(compiler)
        if executable is None:
            continue
        output = source / f"primitive-probe-{compiler}"
        compiled = run_bounded_process(
            [executable, "-std=c11", "-Wall", "-Wextra", "-Werror", "-I", str(source), str(probe), "-o", str(output)],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        require(compiled.returncode == 0, f"{compiler} rejected primitive ABI probe: {compiled.stderr}")
        observed = run_bounded_process(
            [str(output)], cwd=ROOT, check=False, capture_output=True, text=True, timeout=10
        )
        require(observed.returncode == 0, f"{compiler} primitive ABI probe did not run")
        long_bits, short_bits, enum_value, macro_value = (int(value) for value in observed.stdout.split())
        require(
            scalars.get("signed-long", {}).get("bitWidth") == long_bits
            and scalars.get("unsigned-short", {}).get("bitWidth") == short_bits
            and enum_value == -1
            and str(macros.get("WIDGET_LIMIT", {}).get("value")) == str(macro_value),
            f"{compiler} primitive ABI observations differ from the lock",
        )


def check_compiled_aggregate_probe(source: Path, records: dict[object, dict[str, object]]) -> None:
    packet = records.get("WidgetPacket", {})
    packet_layout = packet.get("layout")
    packet_fields = packet.get("fields")
    require(isinstance(packet_layout, dict) and isinstance(packet_fields, list), "aggregate probe lacks packet facts")
    offsets = {
        field.get("nativeName"): field.get("bitOffset")
        for field in packet_fields
        if isinstance(field, dict) and isinstance(field.get("nativeName"), str)
    }
    probe = source / "aggregate-probe.c"
    probe.write_text(
        '#include <stddef.h>\n#include "widget.h"\n'
        f'_Static_assert(sizeof(WidgetPacket) * 8 == {packet_layout.get("sizeBits")}, "packet size");\n'
        f'_Static_assert(_Alignof(WidgetPacket) * 8 == {packet_layout.get("alignmentBits")}, "packet alignment");\n'
        f'_Static_assert(offsetof(WidgetPacket, x) * 8 == {offsets.get("x")}, "x offset");\n'
        f'_Static_assert(offsetof(WidgetPacket, tail) * 8 == {offsets.get("tail")}, "tail offset");\n'
        'int main(void) { WidgetPacket value = {0}; value.a = 5; value.b = 17; '
        'value.shortValue = 23; return value.a == 5 && value.b == 17 && value.shortValue == 23 ? 0 : 1; }\n',
        encoding="utf-8",
    )
    observed_compilers = 0
    for compiler in ("clang", "gcc"):
        executable = shutil.which(compiler)
        if executable is None:
            continue
        observed_compilers += 1
        output = source / f"aggregate-probe-{compiler}"
        compiled = run_bounded_process(
            [executable, "-std=c11", "-Wall", "-Wextra", "-Werror", "-I", str(source), str(probe), "-o", str(output)],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        require(compiled.returncode == 0, f"{compiler} rejected aggregate ABI probe: {compiled.stderr}")
        observed = run_bounded_process([str(output)], cwd=ROOT, check=False, capture_output=True, text=True, timeout=10)
        require(observed.returncode == 0, f"{compiler} aggregate ABI behavior differs from the lock")
    require(observed_compilers > 0, "no strict native compiler was available for aggregate ABI proof")


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
    require(envelope.get("exitCategory") == "command" and "widget.h:19:" in str(envelope.get("stderr")), "source file and line were lost")
    require("widget.h:19:" in result.stderr and "HXC-CLI-0803" in result.stderr, "human diagnostic stream lost Clang context")

    warning_root = temporary / "warning"
    warning = write_fixture(warning_root, warning=True)
    result = invoke(str(warning), "--include-dir", str(warning_root), "--define", "HXC_WIDGET_FEATURE=1", "--dry-run", "--json")
    require(result.returncode == 0, "non-fatal Clang diagnostic changed command success")
    envelope = json.loads(result.stdout)
    require("widget.h:1:" in str(envelope.get("stderr")) and "bindgen-warning" in result.stderr, "non-fatal source diagnostic was discarded")

    nonportable_root = temporary / "nonportable"
    nonportable_root.mkdir()
    nonportable = nonportable_root / "nonportable.h"
    nonportable.write_text("struct __attribute__((ms_struct)) Nonportable { int value; };\n", encoding="utf-8")
    result = invoke(str(nonportable), "--dry-run", "--json")
    require(
        result.returncode == 1 and "HXC-CLI-0811" in result.stderr and "unsupported nonportable layout attribute" in result.stderr,
        "unsupported nonportable aggregate layout did not fail precisely",
    )
    ambiguous = nonportable_root / "ambiguous.h"
    ambiguous.write_text(
        "typedef struct { int value; } __attribute__((packed, aligned(2))) Ambiguous;\n",
        encoding="utf-8",
    )
    result = invoke(str(ambiguous), "--dry-run", "--json")
    require(
        result.returncode == 1 and "HXC-CLI-0811" in result.stderr and "places a layout attribute after its field list" in result.stderr,
        "typedef-positioned packed layout was allowed to publish mismatched record facts",
    )

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
    print(
        "hxc-bindgen: OK: Clang authority, target-measured scalar/typedef/qualifier/enum/macro ABI facts, "
        "aggregate layout/bitfield/packing probes, compiled probes, reachability, configuration, exact inputs, and diagnostics passed"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except BindgenFailure as error:
        print(f"hxc-bindgen: ERROR: {error}", file=sys.stderr)
        raise SystemExit(1)
