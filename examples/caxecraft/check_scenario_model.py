#!/usr/bin/env python3
"""Check the target-neutral CAXEMAP 1 model and its nominal identity boundary."""

from __future__ import annotations

import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
MODEL = CASE / "src/caxecraft/scenario"
FIXTURE = CASE / "test/fixtures/caxemap/minimal.caxemap"
EXPECTED_TRACE = "scenario-model: 264908270\n"
EXPECTED_CODEC_TRACE = (
    "scenario-codec: 1192 + 4027 + 4488 bytes, staged round-trip and exact "
    "malformed-input audit\n"
)
FORBIDDEN_MODEL_TEXT = (
    re.compile(r"#if\b"),
    re.compile(r"\bDynamic\b"),
    re.compile(r"\bAny\b"),
    re.compile(r"\bReflect\b"),
    re.compile(r"\buntyped\b"),
    re.compile(r"\b__c__\b"),
    re.compile(r"^import\s+c\.", re.MULTILINE),
    re.compile(r"^import\s+raylib\.", re.MULTILINE),
)


class ScenarioModelFailure(RuntimeError):
    pass


def haxe_tool() -> str:
    local = ROOT / "node_modules/.bin/haxe"
    if local.is_file():
        return str(local)
    resolved = shutil.which("haxe")
    if resolved is None:
        raise ScenarioModelFailure("pinned Haxe executable is unavailable")
    return resolved


def run_haxe(hxml: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [haxe_tool(), "--cwd", str(CASE), hxml],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        encoding="utf-8",
    )


def check_positive_model() -> None:
    result = run_haxe("scenario-model.hxml")
    if result.returncode != 0:
        raise ScenarioModelFailure(
            "CAXEMAP model probe failed:\n" + result.stdout + result.stderr
        )
    if result.stdout != EXPECTED_TRACE or result.stderr:
        raise ScenarioModelFailure(
            "CAXEMAP model trace changed:\n"
            f"expected stdout: {EXPECTED_TRACE!r}\n"
            f"actual stdout:   {result.stdout!r}\n"
            f"actual stderr:   {result.stderr!r}"
        )


def check_codec() -> None:
    result = run_haxe("scenario-codec.hxml")
    if result.returncode != 0:
        raise ScenarioModelFailure(
            "CAXEMAP codec probe failed:\n" + result.stdout + result.stderr
        )
    if result.stdout != EXPECTED_CODEC_TRACE or result.stderr:
        raise ScenarioModelFailure(
            "CAXEMAP codec trace changed:\n"
            f"expected stdout: {EXPECTED_CODEC_TRACE!r}\n"
            f"actual stdout:   {result.stdout!r}\n"
            f"actual stderr:   {result.stderr!r}"
        )


def check_nominal_identity_failure() -> None:
    result = run_haxe("scenario-identity-negative.hxml")
    combined = result.stdout + result.stderr
    if result.returncode == 0:
        raise ScenarioModelFailure(
            "ContentId was incorrectly accepted where ScenarioId is required"
        )
    required = (
        "ScenarioIdentityMixup.hx:11",
        "caxecraft.scenario.ContentId should be caxecraft.scenario.ScenarioId",
    )
    missing = [fragment for fragment in required if fragment not in combined]
    if missing:
        raise ScenarioModelFailure(
            "nominal identity failure lost its useful type evidence: "
            + ", ".join(missing)
            + "\n"
            + combined
        )


def check_model_boundary() -> None:
    sources = sorted(MODEL.glob("*.hx"), key=lambda path: path.name.encode("utf-8"))
    if not sources:
        raise ScenarioModelFailure("CAXEMAP model source inventory is empty")
    for source in sources:
        text = source.read_text(encoding="utf-8")
        for pattern in FORBIDDEN_MODEL_TEXT:
            if pattern.search(text):
                raise ScenarioModelFailure(
                    f"{source.relative_to(ROOT)} crosses the target-neutral model boundary: "
                    f"{pattern.pattern}"
                )


def check_canonical_fixture_shape() -> None:
    raw = FIXTURE.read_bytes()
    if raw.startswith(b"\xef\xbb\xbf"):
        raise ScenarioModelFailure("minimal.caxemap must not contain a UTF-8 BOM")
    if b"\r" in raw or not raw.endswith(b"\n"):
        raise ScenarioModelFailure("minimal.caxemap must use canonical LF with one final LF")
    text = raw.decode("utf-8")
    lines = text.splitlines()
    if not lines or lines[0] != "CAXEMAP 1" or lines[-1] != "end-map":
        raise ScenarioModelFailure("minimal.caxemap lost its version or end marker")
    object_ids = [line.split()[1] for line in lines if line.startswith("object ")]
    if object_ids != sorted(object_ids, key=lambda value: value.encode("utf-8")):
        raise ScenarioModelFailure("minimal.caxemap object records are not ID-sorted")
    run_total = sum(int(line.split()[2]) for line in lines if line.startswith("  run "))
    if run_total != 64:
        raise ScenarioModelFailure(
            f"minimal.caxemap RLE covers {run_total} cells instead of 64"
        )


def main() -> int:
    try:
        check_model_boundary()
        check_canonical_fixture_shape()
        check_positive_model()
        check_codec()
        check_nominal_identity_failure()
    except (OSError, UnicodeError, ScenarioModelFailure) as error:
        print(f"caxemap-model: ERROR: {error}", file=sys.stderr)
        return 1
    print("caxemap-model: OK: closed target-neutral model, canonical fixture, and nominal IDs")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
