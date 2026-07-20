#!/usr/bin/env python3
"""Verify deterministic, namespace-aware C symbol finalization."""

from __future__ import annotations

import difflib
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("symbol_registry.hxml")
EXPECTED = Path(__file__).with_name("expected") / "symbol-registry.json"
REPORT_PREFIX = "HXC_SYMBOL_REGISTRY="
C_IDENTIFIER = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
C_KEYWORDS = {
    "_Alignas",
    "_Alignof",
    "_Atomic",
    "_Bool",
    "_Complex",
    "_Generic",
    "_Imaginary",
    "_Noreturn",
    "_Static_assert",
    "_Thread_local",
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "inline",
    "int",
    "long",
    "register",
    "restrict",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
}


class SymbolRegistryFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def render(label: str) -> tuple[str, dict[str, Any]]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [development_tool("haxe"), str(HXML)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        raise SymbolRegistryFailure(
            f"{label} failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if len(lines) != 1 or result.stderr:
        raise SymbolRegistryFailure(
            f"{label} emitted an invalid report envelope\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    parsed = json.loads(payload)
    if not isinstance(parsed, dict):
        raise SymbolRegistryFailure(f"{label} report must be a JSON object")
    return payload, parsed


def validate(report: dict[str, Any]) -> None:
    table = report.get("symbolTable")
    if not isinstance(table, dict):
        raise SymbolRegistryFailure("report omitted symbolTable")
    if table.get("schemaVersion") != 2 or table.get("algorithm") != "hxc-c-symbol-v2":
        raise SymbolRegistryFailure("symbol table schema/algorithm drifted")
    symbols = table.get("symbols")
    collisions = table.get("collisions")
    if not isinstance(symbols, list) or not isinstance(collisions, list):
        raise SymbolRegistryFailure("symbol table omitted symbols or collisions")
    names_by_namespace: dict[tuple[str, str], set[str]] = {}
    source_symbols: set[str] = set()
    escape_reasons: set[str] = set()
    for symbol in symbols:
        if not isinstance(symbol, dict):
            raise SymbolRegistryFailure(f"invalid symbol record: {symbol!r}")
        name = symbol.get("cName")
        namespace = symbol.get("namespace")
        source = symbol.get("sourceSymbol")
        if not isinstance(name, str) or not C_IDENTIFIER.fullmatch(name) or name in C_KEYWORDS:
            raise SymbolRegistryFailure(f"invalid finalized C identifier: {name!r}")
        if not isinstance(namespace, dict) or not isinstance(source, str):
            raise SymbolRegistryFailure(f"symbol lacks namespace/provenance: {symbol!r}")
        key = (str(namespace.get("kind")), str(namespace.get("scope")))
        used = names_by_namespace.setdefault(key, set())
        if name in used:
            raise SymbolRegistryFailure(f"duplicate C identifier {name!r} in namespace {key!r}")
        used.add(name)
        source_symbols.add(source)
        reasons = symbol.get("escapeReasons")
        if isinstance(reasons, list):
            escape_reasons.update(str(reason) for reason in reasons)
        if symbol.get("naming") == "generated":
            if not name.startswith("hxc_") or name.startswith("_") or "__" in name:
                raise SymbolRegistryFailure(f"generated name violates reserved namespace policy: {name}")
            if len(name) > 120:
                raise SymbolRegistryFailure(f"generated name exceeds length policy: {name}")
    required_sources = {
        "demo.Worker.run(c.Int32, Void)",
        "demo.Worker.run(String, Void)",
        "demo.Box.map<c.Int32>",
        "demo.Box.map<demo.User>",
        "demo.Worker.run.lambda#2",
    }
    if not required_sources.issubset(source_symbols):
        raise SymbolRegistryFailure("overload/specialization/anonymous provenance is incomplete")
    required_reasons = {
        "c-keyword",
        "implementation-reserved",
        "reflaxe-reserved",
        "standard-library-reserved",
        "non-c-identifier-byte",
        "separator-normalized",
        "c-namespace-escape",
        "length-limit",
    }
    if not required_reasons.issubset(escape_reasons):
        raise SymbolRegistryFailure(
            f"escape ledger is incomplete: missing {sorted(required_reasons - escape_reasons)!r}"
        )
    collision_bases = {
        collision.get("baseName")
        for collision in collisions
        if isinstance(collision, dict)
        and isinstance(collision.get("symbols"), list)
        and len(collision["symbols"]) == 2
    }
    if collision_bases != {"hxc_demo_Box_map", "hxc_demo_Worker_run"}:
        raise SymbolRegistryFailure(
            f"overload/specialization collision ledger is missing or unstable: {collision_bases!r}"
        )

    by_source = {
        symbol.get("sourceSymbol"): symbol
        for symbol in symbols
        if isinstance(symbol, dict) and isinstance(symbol.get("sourceSymbol"), str)
    }
    readable_expectations = {
        "compiler.closed-record.4aec2e39ec8810b7": (
            "hxc_caxecraft_domain_BlockCoord",
            ["caxecraft", "domain", "BlockCoord"],
        ),
        "compiler.closed-record.4aec2e39ec8810b7.x": ("hxc_x", ["x"]),
        "compiler.closed-record.4aec2e39ec8810b7.NULL": ("hxc_NULL", ["NULL"]),
        "compiler.closed-record.4aec2e39ec8810b7.NAN": ("hxc_NAN", ["NAN"]),
        "demo.Worker.run.value": ("hxc_value", ["value"]),
        "demo.Worker.run.while": ("hxc_while", ["while"]),
        "demo.Worker.run.bool": ("hxc_bool", ["bool"]),
    }
    for source, (expected_name, expected_readable) in readable_expectations.items():
        symbol = by_source.get(source)
        if (
            not isinstance(symbol, dict)
            or symbol.get("cName") != expected_name
            or symbol.get("readableName") != expected_readable
        ):
            raise SymbolRegistryFailure(
                f"readable generated-name contract drifted for {source!r}: {symbol!r}"
            )
    block_coord = by_source["compiler.closed-record.4aec2e39ec8810b7"]
    if block_coord.get("collisionResolved") is not False or "_h" in str(block_coord.get("cName")):
        raise SymbolRegistryFailure("collision-free named record received an unnecessary hash")
    for collision in collisions:
        for entry in collision.get("symbols", []):
            if not isinstance(entry, dict) or re.search(r"_h[0-9a-f]{12,64}$", str(entry.get("cName"))) is None:
                raise SymbolRegistryFailure(f"genuine collision lacks a compact semantic suffix: {entry!r}")

    check_macro_safe_native(by_source)

    if report.get("readableIdentityProof") != (
        "readable-display-is-not-semantic-identity-but-is-a-validated-naming-fact"
    ):
        raise SymbolRegistryFailure("readable-name identity separation proof drifted")

    diagnostics = report.get("diagnostics")
    if not isinstance(diagnostics, dict):
        raise SymbolRegistryFailure("report omitted registry diagnostics")
    for key in (
        "exactCollision",
        "keyword",
        "leadingUnderscore",
        "doubleUnderscore",
        "compilerNamespace",
        "lexical",
    ):
        diagnostic = diagnostics.get(key)
        if not isinstance(diagnostic, dict) or diagnostic.get("id") != "HXC5002":
            raise SymbolRegistryFailure(f"{key} did not fail with HXC5002")
    collision = diagnostics["exactCollision"]
    if collision.get("sourceSymbols") != ["alpha.open", "beta.open"]:
        raise SymbolRegistryFailure("exact collision did not identify both source symbols")
    for key in ("conflictingFacts", "hostPathIdentity", "sealedRegistry"):
        diagnostic = diagnostics.get(key)
        if not isinstance(diagnostic, dict) or diagnostic.get("id") != "HXC9000":
            raise SymbolRegistryFailure(f"{key} did not fail with HXC9000")

    finalization = report.get("contractFinalization")
    if not isinstance(finalization, dict):
        raise SymbolRegistryFailure("report omitted typed contract finalization")
    snapshot = finalization.get("snapshot")
    if not isinstance(snapshot, dict) or snapshot.get("status") != "contract-seed-names-finalized-no-lowering":
        raise SymbolRegistryFailure("typed contract name-finalization status drifted")
    declarations = snapshot.get("declarations")
    if not isinstance(declarations, list) or any(
        not isinstance(declaration, dict) or not isinstance(declaration.get("cName"), str)
        for declaration in declarations
    ):
        raise SymbolRegistryFailure("typed declaration defaults were not finalized")
    fields = [
        field
        for declaration in declarations
        for field in declaration.get("fields", [])
        if isinstance(declaration, dict) and isinstance(field, dict)
    ]
    if any(not isinstance(field.get("cName"), str) for field in fields):
        raise SymbolRegistryFailure("typed field defaults were not finalized")
    effects = snapshot.get("effects")
    if not isinstance(effects, dict) or effects.get("runtimeFeatures") != []:
        raise SymbolRegistryFailure("name finalization selected a hidden runtime feature")
    forward_names = report.get("plannedForwardNames")
    if not isinstance(forward_names, list) or not any(
        isinstance(entry, dict)
        and entry.get("modulePath") == "demo.Value"
        and isinstance(entry.get("cName"), str)
        for entry in forward_names
    ):
        raise SymbolRegistryFailure("declaration planning did not consume finalized default names")

    serialized = json.dumps(report, sort_keys=True, ensure_ascii=False)
    if str(ROOT) in serialized or "/Users/" in serialized or "\\" in serialized:
        raise SymbolRegistryFailure("symbol report leaked a host filesystem path")
    if "hxrt_" in serialized:
        raise SymbolRegistryFailure("symbol finalization introduced an hxrt symbol")


def check_macro_safe_native(by_source: dict[str, dict[str, Any]]) -> None:
    """Compile source-shaped locals/members beside common object macros."""

    local_bool = by_source["demo.Worker.run.bool"]["cName"]
    member_null = by_source["compiler.closed-record.4aec2e39ec8810b7.NULL"]["cName"]
    member_nan = by_source["compiler.closed-record.4aec2e39ec8810b7.NAN"]["cName"]
    source = f"""#include <stdbool.h>
#include <stddef.h>
#include <math.h>

struct hxc_macro_probe {{
  int {member_null};
  int {member_nan};
}};

static int hxc_macro_probe_run(void)
{{
  int {local_bool} = 3;
  struct hxc_macro_probe value = {{ .{member_null} = 4, .{member_nan} = 5 }};
  return {local_bool} + value.{member_null} + value.{member_nan};
}}

int main(void)
{{
  return hxc_macro_probe_run() == 12 ? 0 : 1;
}}
"""
    with tempfile.TemporaryDirectory(prefix="hxc-symbol-macro-") as temporary:
        root = Path(temporary)
        source_path = root / "macro_probe.c"
        executable = root / "macro_probe"
        source_path.write_text(source, encoding="utf-8")
        compiler = os.environ.get("CC", "cc")
        compiled = subprocess.run(
            [
                compiler,
                "-std=c11",
                "-Wall",
                "-Wextra",
                "-Werror",
                "-pedantic",
                str(source_path),
                "-o",
                str(executable),
            ],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if compiled.returncode != 0:
            raise SymbolRegistryFailure(
                "macro-safe native symbol probe failed to compile\n"
                f"stdout:\n{compiled.stdout}\nstderr:\n{compiled.stderr}"
            )
        executed = subprocess.run(
            [str(executable)],
            cwd=root,
            check=False,
            capture_output=True,
            text=True,
            timeout=10,
        )
        if executed.returncode != 0 or executed.stdout or executed.stderr:
            raise SymbolRegistryFailure(
                "macro-safe native symbol probe failed at runtime\n"
                f"exit={executed.returncode}\nstdout:\n{executed.stdout}\nstderr:\n{executed.stderr}"
            )


def check_snapshot(report: dict[str, Any]) -> None:
    expected = json.loads(EXPECTED.read_text(encoding="utf-8"))
    if report != expected:
        diff = "".join(
            difflib.unified_diff(
                (json.dumps(expected, indent=2, sort_keys=True) + "\n").splitlines(keepends=True),
                (json.dumps(report, indent=2, sort_keys=True) + "\n").splitlines(keepends=True),
                fromfile=str(EXPECTED.relative_to(ROOT)),
                tofile="actual symbol registry",
            )
        )
        raise SymbolRegistryFailure("symbol registry snapshot drifted\n" + diff)


def main() -> int:
    try:
        first_payload, first = render("first symbol registry render")
        second_payload, second = render("second symbol registry render")
        if first_payload != second_payload or first != second:
            raise SymbolRegistryFailure("two symbol registry renders were not byte-identical")
        validate(first)
        check_snapshot(first)
    except (OSError, UnicodeError, json.JSONDecodeError, subprocess.TimeoutExpired, SymbolRegistryFailure) as error:
        print(f"symbol-registry: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "symbol-registry: OK: namespace-aware exact names, deterministic overload/specialization mangling, "
        "typed-contract finalization, collision provenance, and zero-runtime policy passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
