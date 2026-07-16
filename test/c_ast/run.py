#!/usr/bin/env python3
"""Render, compile, link, run, and record the direct structural C AST corpus."""

from __future__ import annotations

import argparse
import difflib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Mapping


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.c_fixture_harness import (  # noqa: E402
    CFixtureFailure,
    CFixtureProject,
    report_json,
    run_c_fixture_corpus,
    validate_report,
    write_report,
)


HXML = Path(__file__).with_name("fixture_compiler.hxml")
EXPECTED = Path(__file__).with_name("expected")
OWNERSHIP = "_GeneratedFiles.json"
SUCCESS_LINES = ("c-ast-fixture-macro: OK", "c-ast-fixture-probe: OK")
HAXE_TIMEOUT_SECONDS = 90
PAYLOAD_PATHS = (
    "attributes.c",
    "declarators.c",
    "expressions.c",
    "include/hxc/ast_fixture.h",
    "src/ast_fixture.c",
    "src/main.c",
)
REQUIRED_COVERAGE = frozenset(
    {
        "attributes",
        "control-flow",
        "declarations",
        "header-source-split",
        "initializers",
        "precedence",
    }
)
PROJECTS = (
    CFixtureProject(
        "attributes",
        ("attributes.c",),
        (),
        (),
        "c-ast-attributes: OK\n",
        ("attributes",),
    ),
    CFixtureProject(
        "declarators",
        ("declarators.c",),
        (),
        (),
        "c-ast-golden: OK\n",
        ("declarations", "initializers"),
    ),
    CFixtureProject(
        "expressions",
        ("expressions.c",),
        (),
        (),
        "c-expression-golden: OK\n",
        ("control-flow", "initializers", "precedence"),
    ),
    CFixtureProject(
        "header-source",
        ("src/ast_fixture.c", "src/main.c"),
        ("include/hxc/ast_fixture.h",),
        ("include",),
        "c-ast-header-source: OK\n",
        ("declarations", "header-source-split", "initializers"),
    ),
)


class CASTGoldenFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class SnapshotArtifact:
    relative_path: Path
    format: str
    value: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def macro_call(output: Path) -> str:
    return "ASTFixtureCompiler.run(" + json.dumps(str(output), ensure_ascii=True) + ")"


def run_fixture_compiler(output: Path, *, label: str) -> None:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [
            development_tool("haxe"),
            str(HXML),
            "--macro",
            macro_call(output),
        ],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=HAXE_TIMEOUT_SECONDS,
    )
    if result.returncode != 0:
        raise CASTGoldenFailure(
            f"{label} returned {result.returncode}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    lines = tuple(line for line in result.stdout.splitlines() if line)
    if lines[-2:] != SUCCESS_LINES or result.stderr:
        raise CASTGoldenFailure(
            f"{label} emitted an invalid success envelope\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )


def output_files(output: Path) -> list[Path]:
    return sorted(path for path in output.rglob("*") if path.is_file())


def validate_ownership(output: Path) -> None:
    path = output / OWNERSHIP
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise CASTGoldenFailure(f"invalid Reflaxe ownership manifest: {error}") from error
    if not isinstance(value, dict):
        raise CASTGoldenFailure("Reflaxe ownership manifest is not an object")
    if (
        value.get("version") != 1
        or not isinstance(value.get("id"), int)
        or value.get("id", -1) < 0
        or not isinstance(value.get("wasCached"), bool)
        or value.get("filesGenerated") != list(PAYLOAD_PATHS)
    ):
        raise CASTGoldenFailure("Reflaxe ownership manifest lost the AST corpus")
    actual = tuple(
        path.relative_to(output).as_posix() for path in output_files(output)
    )
    if actual != (OWNERSHIP, *PAYLOAD_PATHS):
        raise CASTGoldenFailure(
            f"AST fixture compiler emitted an unexpected file set: {actual!r}"
        )


def payload_tree(output: Path) -> dict[str, bytes]:
    return {relative: (output / relative).read_bytes() for relative in PAYLOAD_PATHS}


def complete_tree(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in output_files(output)
    }


def render_corpus(output: Path, *, label: str) -> dict[str, bytes]:
    run_fixture_compiler(output, label=label)
    validate_ownership(output)
    tree = payload_tree(output)
    for relative, contents in tree.items():
        if b"hxrt" in contents or b"hxc_runtime" in contents:
            raise CASTGoldenFailure(
                f"direct AST artifact {relative!r} unexpectedly selected runtime code"
            )
        if b"\r" in contents or b"\x00" in contents:
            raise CASTGoldenFailure(
                f"direct AST artifact {relative!r} is not canonical text"
            )
    return tree


def difference(expected: bytes, actual: bytes, relative: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.decode("utf-8").splitlines(keepends=True),
            actual.decode("utf-8").splitlines(keepends=True),
            fromfile=f"expected/{relative}",
            tofile=f"actual/{relative}",
        )
    )


def check_expected(tree: Mapping[str, bytes]) -> None:
    expected_paths = (
        tuple(
            path.relative_to(EXPECTED).as_posix()
            for path in sorted(EXPECTED.rglob("*"))
            if path.is_file()
        )
        if EXPECTED.is_dir()
        else ()
    )
    if expected_paths != PAYLOAD_PATHS:
        raise CASTGoldenFailure(
            "C AST snapshots are missing or stale; run "
            "npm run snapshots:update -- --suite c-ast"
        )
    for relative in PAYLOAD_PATHS:
        expected = (EXPECTED / relative).read_bytes()
        actual = tree[relative]
        if expected != actual:
            raise CASTGoldenFailure(
                f"C AST printer output drifted for {relative}:\n"
                + difference(expected, actual, relative)
            )


def snapshot_format(relative: str) -> str:
    return "header" if relative.endswith(".h") else "c"


def render_snapshot() -> list[SnapshotArtifact]:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-ast-snapshot-") as temporary:
        root = Path(temporary)
        first = root / "first AST root"
        second = root / "second AST root's copy"
        first_tree = render_corpus(first, label="first C AST snapshot render")
        second_tree = render_corpus(second, label="second C AST snapshot render")
        if complete_tree(first) != complete_tree(second) or first_tree != second_tree:
            raise CASTGoldenFailure(
                "two C AST fixture compiler renders were not byte-identical"
            )
        return [
            SnapshotArtifact(
                Path("test/c_ast/expected") / relative,
                snapshot_format(relative),
                first_tree[relative].decode("utf-8"),
            )
            for relative in PAYLOAD_PATHS
        ]


def check_harness_guards() -> None:
    invalid = CFixtureProject(
        "invalid-path",
        ("../escape.c",),
        (),
        (),
        "",
        ("guard",),
    )
    try:
        invalid.validate()
    except CFixtureFailure:
        return
    raise CASTGoldenFailure("C fixture harness accepted a parent-traversal source")


def run_native(
    fixture_root: Path,
    build_root: Path,
    *,
    requested_toolchain: str,
) -> dict[str, object]:
    report = run_c_fixture_corpus(
        suite="c-ast-roundtrip",
        projects=PROJECTS,
        fixture_root=fixture_root,
        build_root=build_root,
        repository_root=ROOT,
        requested_toolchain=requested_toolchain,
    )
    validate_report(report, required_coverage=REQUIRED_COVERAGE)
    encoded = report_json(report, compact=True)
    forbidden_paths = {
        str(path)
        for root in (fixture_root, build_root, ROOT)
        for path in (root.absolute(), root.resolve())
    }
    for forbidden in forbidden_paths:
        if forbidden in encoded:
            raise CASTGoldenFailure(
                f"C fixture report leaked absolute host path {forbidden}"
            )
    if "hxrt" in encoded or "hxc_runtime" in encoded:
        raise CASTGoldenFailure("C fixture report unexpectedly selected runtime code")
    return report


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--toolchain",
        choices=("auto", "gcc", "clang"),
        default="auto",
        help="auto-runs identity-verified available compilers; an explicit family is required",
    )
    parser.add_argument(
        "--native-only",
        action="store_true",
        help="compile the checked-in corpus without invoking the pinned Haxe renderer",
    )
    parser.add_argument(
        "--report",
        type=Path,
        help="also write the normalized compiler invocation/output report to this path",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print(
            "c-ast-golden: ERROR: pinned Haxe executable is unavailable",
            file=sys.stderr,
        )
        return 1

    try:
        check_harness_guards()
        with tempfile.TemporaryDirectory(
            prefix="reflaxe-c-ast-fixture-"
        ) as temporary:
            root = Path(temporary)
            if args.native_only:
                fixture_root = EXPECTED
                check_expected(
                    {
                        relative: (EXPECTED / relative).read_bytes()
                        for relative in PAYLOAD_PATHS
                    }
                )
            else:
                fixture_root = root / "rendered corpus's root"
                repeated_root = root / "repeated corpus root"
                first = render_corpus(
                    fixture_root, label="first direct C AST fixture render"
                )
                second = render_corpus(
                    repeated_root, label="second direct C AST fixture render"
                )
                if (
                    first != second
                    or complete_tree(fixture_root) != complete_tree(repeated_root)
                ):
                    raise CASTGoldenFailure(
                        "two direct C AST fixture renders were not byte-identical"
                    )
                check_expected(first)

            report = run_native(
                fixture_root,
                root / "native build",
                requested_toolchain=args.toolchain,
            )
            if args.report is not None:
                write_report(args.report, report)
    except (
        CASTGoldenFailure,
        CFixtureFailure,
        OSError,
        UnicodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"c-ast-golden: ERROR: {error}", file=sys.stderr)
        return 1

    print("c-ast-fixture-report: " + report_json(report, compact=True))
    families = ", ".join(
        toolchain["family"]
        for toolchain in report["toolchains"]
        if isinstance(toolchain, dict) and isinstance(toolchain.get("family"), str)
    )
    print(
        "c-ast-golden: OK: deterministic owned declarations, precedence, "
        "initializers, control flow, attributes, and header/source projects "
        f"compiled and ran with {families} without runtime selection"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
