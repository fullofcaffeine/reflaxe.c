#!/usr/bin/env python3
"""Generate, inspect, compile, and run the bounded C AST printer fuzz corpus."""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Callable, Iterable
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402
from scripts.test.c_fixture_harness import (  # noqa: E402
    CFixtureFailure,
    CFixtureProject,
    report_json,
    run_c_fixture_corpus,
    validate_report,
)


HXML = Path(__file__).with_name("fixture_compiler.hxml")
CORPUS = Path(__file__).with_name("fuzz")
SEEDS = CORPUS / "seeds.tsv"
DICTIONARY = CORPUS / "dictionary.tsv"
MINIMIZER_REGRESSION = CORPUS / "regressions/minimizer.json"
MAX_SEEDS = 16
MAX_CASES = 64
MAX_TOTAL_CASES = 512
DEFAULT_CASES = 8
MAX_SOURCE_BYTES = 1024 * 1024
MAX_FILE_BYTES = 256 * 1024
HAXE_TIMEOUT_SECONDS = 120
EXPECTED_STDOUT = "c-ast-fuzz: OK\n"


class CASTFuzzFailure(RuntimeError):
    """Reports a bounded fuzz contract failure without hiding its seed."""


@dataclass(frozen=True)
class Seed:
    """Names one stable integer input from the checked-in corpus."""

    name: str
    value: int


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def read_seeds(path: Path = SEEDS) -> tuple[Seed, ...]:
    seeds: list[Seed] = []
    for line_number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        if not line or line.startswith("#"):
            continue
        fields = line.split("\t")
        if len(fields) != 2 or re.fullmatch(r"[a-z0-9][a-z0-9-]*", fields[0]) is None:
            raise CASTFuzzFailure(f"invalid seed row at {path}:{line_number}")
        try:
            value = int(fields[1], 10)
        except ValueError as error:
            raise CASTFuzzFailure(
                f"invalid seed value at {path}:{line_number}"
            ) from error
        if value < 0:
            raise CASTFuzzFailure(f"negative seed at {path}:{line_number}")
        seeds.append(Seed(fields[0], value))
    if not 1 <= len(seeds) <= MAX_SEEDS or len({seed.name for seed in seeds}) != len(seeds):
        raise CASTFuzzFailure("seed corpus must contain 1 to 16 unique names")
    return tuple(seeds)


def validate_dictionary() -> None:
    entries = [
        line
        for line in DICTIONARY.read_text(encoding="utf-8").splitlines()
        if line and not line.startswith("#")
    ]
    if not 1 <= len(entries) <= 64:
        raise CASTFuzzFailure("dictionary must contain 1 to 64 entries")
    for line in entries:
        fields = line.split("\t")
        if len(fields) != 2 or len(fields[1].split(",")) > 128:
            raise CASTFuzzFailure(f"invalid dictionary row: {line!r}")
        for value in fields[1].split(","):
            try:
                code_point = int(value, 16)
            except ValueError as error:
                raise CASTFuzzFailure(
                    f"invalid dictionary code point: {value!r}"
                ) from error
            if not 0 <= code_point <= 0x10FFFF or 0xD800 <= code_point <= 0xDFFF:
                raise CASTFuzzFailure(f"invalid Unicode code point: {value!r}")


def macro_call(output: Path, seeds: Path, cases: int) -> str:
    arguments = (str(output), str(seeds), str(DICTIONARY), cases)
    encoded = ",".join(json.dumps(value, ensure_ascii=True) for value in arguments)
    return f"CASTFuzzCompiler.run({encoded})"


def render(output: Path, seeds: Path, cases: int) -> Path:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = run_bounded_process(
        [
            development_tool("haxe"),
            str(HXML),
            "--macro",
            macro_call(output, seeds, cases),
        ],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=HAXE_TIMEOUT_SECONDS,
    )
    if result.returncode != 0:
        raise CASTFuzzFailure(
            f"Haxe render failed with exit {result.returncode}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    expected = f"c-ast-fuzz-macro: OK: {len(read_seeds(seeds))} seeds x {cases} cases"
    lines = tuple(line for line in result.stdout.splitlines() if line)
    if lines[-2:] != (expected, "c-ast-fixture-probe: OK") or result.stderr:
        raise CASTFuzzFailure(
            "Haxe render emitted an invalid success envelope\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    first = output / "first"
    second = output / "second"
    first_tree = read_owned_tree(first)
    second_tree = read_owned_tree(second)
    if first_tree != second_tree:
        raise CASTFuzzFailure("two fuzz renders were not byte-identical")
    expected_payload = {
        f"seed-{seed.name}.c" for seed in read_seeds(seeds)
    }
    actual_payload = set(first_tree) - {"_GeneratedFiles.json"}
    if actual_payload != expected_payload:
        raise CASTFuzzFailure(
            f"fuzz render emitted the wrong seed files: {sorted(actual_payload)!r}"
        )
    return first


def read_owned_tree(root: Path) -> dict[str, bytes]:
    files = sorted(path for path in root.rglob("*") if path.is_file())
    tree = {path.relative_to(root).as_posix(): path.read_bytes() for path in files}
    try:
        ownership = json.loads(tree["_GeneratedFiles.json"].decode("utf-8"))
    except (KeyError, UnicodeError, json.JSONDecodeError) as error:
        raise CASTFuzzFailure("invalid fuzz ownership manifest") from error
    payload = sorted(path for path in tree if path != "_GeneratedFiles.json")
    if (
        ownership.get("version") != 1
        or not isinstance(ownership.get("id"), int)
        or ownership.get("id", -1) < 0
        or not isinstance(ownership.get("wasCached"), bool)
        or ownership.get("filesGenerated") != payload
    ):
        raise CASTFuzzFailure("fuzz ownership manifest does not own the exact payload")
    if any(not path.startswith("seed-") or not path.endswith(".c") for path in payload):
        raise CASTFuzzFailure(f"unexpected fuzz output paths: {payload!r}")
    size = sum(len(tree[path]) for path in payload)
    if size > MAX_SOURCE_BYTES or any(len(tree[path]) > MAX_FILE_BYTES for path in payload):
        raise CASTFuzzFailure("generated fuzz source exceeded its byte limit")
    for path in payload:
        validate_terminated_source(path, tree[path])
    return tree


def validate_terminated_source(path: str, contents: bytes) -> None:
    if b"\x00" in contents or b"\r" in contents:
        raise CASTFuzzFailure(f"{path} contains a raw NUL or carriage return")
    try:
        source = contents.decode("ascii")
    except UnicodeError as error:
        raise CASTFuzzFailure(f"{path} contains an unescaped non-ASCII byte") from error

    state = "code"
    line_start = True
    index = 0
    while index < len(source):
        char = source[index]
        following = source[index + 1] if index + 1 < len(source) else ""
        if state == "code":
            if line_start and char in " \t":
                index += 1
                continue
            if line_start and char == "#":
                end = source.find("\n", index)
                end = len(source) if end < 0 else end
                directive = source[index:end]
                if not (directive.startswith("#include ") or directive.startswith("#line ")):
                    raise CASTFuzzFailure(f"{path} emitted an unsafe directive: {directive!r}")
            line_start = False
            if char == "/" and following == "*":
                state = "comment"
                index += 2
                continue
            if char == '"':
                state = "string"
            elif char == "'":
                state = "character"
        elif state == "comment":
            if char == "*" and following == "/":
                state = "code"
                index += 2
                continue
        elif state in ("string", "character"):
            if char == "\\":
                index += 2
                continue
            if char == "\n":
                raise CASTFuzzFailure(f"{path} contains an unterminated {state}")
            if (state == "string" and char == '"') or (
                state == "character" and char == "'"
            ):
                state = "code"
        if char == "\n":
            line_start = True
        index += 1
    if state != "code":
        raise CASTFuzzFailure(f"{path} ends inside a {state}")


def check_scanner_guards() -> None:
    invalid_sources = {
        "unterminated-comment.c": b"/* open",
        "unterminated-string.c": b'const char *value = "open\n',
        "injected-directive.c": b"#error source-data-escaped\n",
    }
    for path, source in invalid_sources.items():
        try:
            validate_terminated_source(path, source)
        except CASTFuzzFailure:
            continue
        raise CASTFuzzFailure(f"lexical guard accepted {path}")


def run_native(fixture_root: Path, build_root: Path, toolchain: str) -> dict[str, object]:
    sources = sorted(path.name for path in fixture_root.glob("seed-*.c"))
    projects = tuple(
        CFixtureProject(
            source.removeprefix("seed-").removesuffix(".c"),
            (source,),
            (),
            (),
            EXPECTED_STDOUT,
            ("comments", "directives", "declarators", "strings"),
        )
        for source in sources
    )
    report = run_c_fixture_corpus(
        suite="c-ast-fuzz",
        projects=projects,
        fixture_root=fixture_root,
        build_root=build_root,
        repository_root=ROOT,
        requested_toolchain=toolchain,
    )
    validate_report(
        report,
        required_coverage={"comments", "directives", "declarators", "strings"},
    )
    return report


def minimize_failing_prefix(total: int, fails: Callable[[int], bool]) -> int:
    """Returns the first failing prefix for a monotonic, prefix-stable failure."""
    if total < 1 or not fails(total):
        raise CASTFuzzFailure("minimizer requires a failing positive prefix")
    low = 1
    high = total
    while low < high:
        middle = (low + high) // 2
        if fails(middle):
            high = middle
        else:
            low = middle + 1
    if low > 1 and fails(low - 1):
        for candidate in range(1, low):
            if fails(candidate):
                return candidate
        raise CASTFuzzFailure("failure predicate changed during minimization")
    return low


def check_minimizer_regression() -> None:
    value = json.loads(MINIMIZER_REGRESSION.read_text(encoding="utf-8"))
    threshold = value["failureStartsAt"]
    actual = minimize_failing_prefix(
        value["totalCases"], lambda cases: cases >= threshold
    )
    if actual != value["expectedMinimalCases"]:
        raise CASTFuzzFailure(
            f"minimizer regression expected {value['expectedMinimalCases']}, got {actual}"
        )


def write_seed(path: Path, seed: Seed) -> None:
    path.write_text(f"{seed.name}\t{seed.value}\n", encoding="utf-8")


def run_once(seeds_path: Path, cases: int, toolchain: str) -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-ast-fuzz-") as temporary:
        root = Path(temporary)
        fixture_root = render(root / "rendered", seeds_path, cases)
        return run_native(fixture_root, root / "native", toolchain)


def minimize_failure(seeds: tuple[Seed, ...], cases: int, toolchain: str) -> dict[str, object] | None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-ast-minimize-") as temporary:
        seed_path = Path(temporary) / "seed.tsv"
        for seed in seeds:
            write_seed(seed_path, seed)

            def fails(prefix: int) -> bool:
                try:
                    run_once(seed_path, prefix, toolchain)
                except (
                    CASTFuzzFailure,
                    CFixtureFailure,
                ):
                    return True
                return False

            if fails(cases):
                minimal = minimize_failing_prefix(cases, fails)
                return {
                    "seedName": seed.name,
                    "seed": seed.value,
                    "minimalCases": minimal,
                    "toolchain": toolchain,
                }
    return None


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cases", type=int, default=DEFAULT_CASES)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        seeds = read_seeds()
        validate_dictionary()
        check_minimizer_regression()
        check_scanner_guards()
        if not 1 <= args.cases <= MAX_CASES:
            raise CASTFuzzFailure(f"--cases must be between 1 and {MAX_CASES}")
        if len(seeds) * args.cases > MAX_TOTAL_CASES:
            raise CASTFuzzFailure(
                f"requested {len(seeds) * args.cases} cases; limit is {MAX_TOTAL_CASES}"
            )
        if shutil.which(development_tool("haxe")) is None:
            raise CASTFuzzFailure("pinned Haxe executable is unavailable")
    except (CASTFuzzFailure, OSError) as error:
        print(f"c-ast-fuzz: ERROR: {error}", file=sys.stderr)
        return 1

    try:
        report = run_once(SEEDS, args.cases, args.toolchain)
    except (
        CASTFuzzFailure,
        CFixtureFailure,
        OSError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"c-ast-fuzz: ERROR: {error}", file=sys.stderr)
        if isinstance(error, subprocess.TimeoutExpired):
            print(
                "c-ast-fuzz: resource timeouts are not printer failures and are not minimized",
                file=sys.stderr,
            )
            return 1
        try:
            minimized = minimize_failure(read_seeds(), args.cases, args.toolchain)
        except (
            CASTFuzzFailure,
            CFixtureFailure,
            OSError,
            subprocess.TimeoutExpired,
        ) as minimize_error:
            print(f"c-ast-fuzz: minimizer could not finish: {minimize_error}", file=sys.stderr)
        else:
            if minimized is not None:
                print(
                    "c-ast-fuzz: minimized regression: "
                    + json.dumps(minimized, sort_keys=True),
                    file=sys.stderr,
                )
        return 1

    print("c-ast-fuzz-report: " + report_json(report, compact=True))
    print(
        f"c-ast-fuzz: OK: {len(seeds)} seeds x {args.cases} cases were "
        "deterministic, lexically closed, strict-C11 clean, and runnable"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
