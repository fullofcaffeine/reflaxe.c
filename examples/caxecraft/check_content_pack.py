#!/usr/bin/env python3
"""Check Caxecraft's validated content data and generated Haxe adapter.

This is intentionally a focused lane. It catches malformed pack data and a
stale generated adapter quickly, then runs one Eval semantic oracle and one
small runtime-free Haxe-to-C build. The larger playable test remains the proof
that content, renderer, native libraries, and packaging work together.
"""

from __future__ import annotations

import copy
import json
import os
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Callable
from pathlib import Path

CASE = Path(__file__).resolve().parent
ROOT = CASE.parents[1]
sys.path.insert(0, str(CASE))

from content_pack import (  # noqa: E402
    ASSET_SOURCE,
    OUTPUT,
    SOURCE,
    ContentPackFailure,
    check_scenario_reference,
    decode_document,
    load_json_document,
    load_pack,
    render_haxe,
    validate_document,
)
from play import stage_content_catalogs  # noqa: E402

EXPECTED_EVAL = (
    "caxecraft-content-pack: typed JSON adapter and first-playable resolution passed\n"
)
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic-errors",
)


class ContentPackCheckFailure(RuntimeError):
    """The focused content-pack contract did not hold."""


def haxe_tool() -> str:
    """Return the checkout-owned Haxe shim so the repository pin stays active."""

    local = ROOT / "node_modules/.bin/haxe"
    if local.is_file():
        return str(local)
    resolved = shutil.which("haxe")
    if resolved is None:
        raise ContentPackCheckFailure("pinned Haxe executable is unavailable")
    return resolved


def run(arguments: list[str], *, cwd: Path, timeout: int, label: str) -> subprocess.CompletedProcess[str]:
    """Run one bounded child process and retain useful failure output."""

    try:
        result = subprocess.run(
            arguments,
            cwd=cwd,
            env={**os.environ, "HAXE_NO_SERVER": "1", "LC_ALL": "C"},
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            timeout=timeout,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise ContentPackCheckFailure(f"{label} could not run: {error}") from error
    if result.returncode != 0:
        raise ContentPackCheckFailure(
            f"{label} failed with exit {result.returncode}\n{result.stdout}{result.stderr}"
        )
    return result


def check_source_and_adapter() -> None:
    """Validate source, generated Haxe, and packaged source-byte ownership."""

    pack = load_pack()
    check_scenario_reference(pack)
    rendered = render_haxe(pack)
    if rendered != render_haxe(pack):
        raise ContentPackCheckFailure("content adapter generation is nondeterministic")
    if not OUTPUT.is_file() or OUTPUT.read_text(encoding="utf-8") != rendered:
        raise ContentPackCheckFailure(
            "generated adapter is stale; run python3 examples/caxecraft/content_pack.py"
        )
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-content-stage-") as temporary:
        destination = Path(temporary)
        stage_content_catalogs(destination)
        packaged = destination / "content" / SOURCE.relative_to(CASE)
        if packaged.read_bytes() != SOURCE.read_bytes():
            raise ContentPackCheckFailure("packaged content manifest differs from its reviewed source bytes")


def expect_invalid(
    label: str,
    source: dict[str, object],
    assets: dict[str, object],
    mutate: Callable[[dict[str, object]], None],
    expected: str,
) -> None:
    """Apply one isolated malformed-data mutation and check its diagnostic."""

    changed = copy.deepcopy(source)
    mutate(changed)
    try:
        validate_document(changed, assets)
    except ContentPackFailure as error:
        if expected not in str(error):
            raise ContentPackCheckFailure(
                f"{label} produced the wrong diagnostic: {error}"
            ) from error
        return
    raise ContentPackCheckFailure(f"{label} malformed content was accepted")


def object_array(document: dict[str, object], key: str) -> list[dict[str, object]]:
    """Return one known object array from the already-valid test document."""

    value = document[key]
    if not isinstance(value, list) or any(not isinstance(item, dict) for item in value):
        raise ContentPackCheckFailure(f"valid fixture lost object array {key!r}")
    return [item for item in value if isinstance(item, dict)]


def object_field(document: dict[str, object], key: str) -> dict[str, object]:
    """Return one known object field from the already-valid test document."""

    value = document[key]
    if not isinstance(value, dict):
        raise ContentPackCheckFailure(f"valid fixture lost object field {key!r}")
    return value


def check_malformed_inputs() -> None:
    """Exercise representative shape, identity, reference, and visual failures."""

    source = load_json_document(SOURCE, "packs/caxecraft/base/content.json")
    assets = load_json_document(ASSET_SOURCE, "assets/manifest.json")
    try:
        decode_document('{"schemaVersion":1,"schemaVersion":1}')
    except ContentPackFailure as error:
        if "duplicate JSON key" not in str(error):
            raise ContentPackCheckFailure(f"duplicate-key diagnostic drifted: {error}") from error
    else:
        raise ContentPackCheckFailure("duplicate JSON keys were accepted")

    expect_invalid(
        "unknown root field",
        source,
        assets,
        lambda value: value.__setitem__("surprise", True),
        "keys differ",
    )
    expect_invalid(
        "traversing logical path",
        source,
        assets,
        lambda value: value.__setitem__("logicalPath", "packs/../escape"),
        "canonical relative logical path",
    )
    expect_invalid(
        "unregistered engine feature",
        source,
        assets,
        lambda value: value.__setitem__("features", ["caxecraft:missing"]),
        "registers exactly the caxecraft:core feature",
    )
    expect_invalid(
        "unknown collision policy",
        source,
        assets,
        lambda value: object_array(value, "blocks")[0].__setitem__("collision", "mist"),
        "unknown block collision/edit policy",
    )
    expect_invalid(
        "duplicate storage code",
        source,
        assets,
        lambda value: object_array(value, "blocks")[1].__setitem__("storageCode", 0),
        "storageCode values must be unique",
    )
    expect_invalid(
        "wrong-kind placement block",
        source,
        assets,
        lambda value: object_array(value, "items")[3].__setitem__(
            "placementBlock", "caxecraft:berries"
        ),
        "unknown placement block",
    )
    expect_invalid(
        "unknown visual cell",
        source,
        assets,
        lambda value: object_field(object_array(value, "items")[0], "icon").__setitem__("cell", "missing"),
        "unknown cell",
    )
    expect_invalid(
        "unknown fluid profile",
        source,
        assets,
        lambda value: object_array(value, "fluids")[0].__setitem__("simulationProfile", "instant-ocean"),
        "unregistered mechanic/profile",
    )
    expect_invalid(
        "unknown equipped aquatic profile",
        source,
        assets,
        lambda value: object_array(value, "items")[-1].__setitem__("aquaticProfile", "caxecraft:missing-aquatics"),
        "references unknown aquatic profile",
    )
    expect_invalid(
        "non-boolean aquatic capability",
        source,
        assets,
        lambda value: object_array(value, "aquaticProfiles")[0].__setitem__("underwaterMining", 1),
        "must be true or false",
    )


def check_eval_oracle() -> None:
    """Run the target-neutral semantic and first-playable resolution proof."""

    result = run(
        [haxe_tool(), "--cwd", str(CASE), "content-pack.hxml"],
        cwd=ROOT,
        timeout=30,
        label="content-pack Eval oracle",
    )
    if result.stdout != EXPECTED_EVAL or result.stderr:
        raise ContentPackCheckFailure(
            "content-pack Eval trace changed\n"
            f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
        )


def check_generated_c() -> None:
    """Compile the generated adapter through haxe.c and strict native C."""

    compiler = shutil.which("clang") or shutil.which("gcc") or shutil.which("cc")
    if compiler is None:
        raise ContentPackCheckFailure("no native C compiler is available")
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-content-") as temporary:
        root = Path(temporary)
        generated = root / "generated"
        executable = root / "content-pack"
        run(
            [
                haxe_tool(),
                "--cwd",
                str(CASE),
                "content-pack-c.hxml",
                "--custom-target",
                f"c={generated}",
            ],
            cwd=ROOT,
            timeout=60,
            label="content-pack Haxe-to-C build",
        )
        sources = sorted(generated.rglob("*.c"), key=lambda path: path.as_posix().encode("utf-8"))
        if not sources:
            raise ContentPackCheckFailure("content-pack C build emitted no C sources")
        module = generated / "src/modules/caxecraft/content/BaseContentPack.c"
        module_text = module.read_text(encoding="utf-8")
        required = ("itemMaximumStack", "switch (", "default:", "abort();")
        missing = [fragment for fragment in required if fragment not in module_text]
        if missing or "goto " in module_text:
            raise ContentPackCheckFailure(
                "content-pack generated C lost its structured exhaustive switch proof: "
                + ", ".join(missing or ["unexpected goto"])
            )
        runtime_plan = json.loads((generated / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
        if runtime_plan.get("features") != []:
            raise ContentPackCheckFailure("scalar content adapter unexpectedly selected hxrt")
        native = run(
            [
                compiler,
                *STRICT_FLAGS,
                "-I",
                str(generated / "include"),
                *(str(path) for path in sources),
                "-o",
                str(executable),
            ],
            cwd=ROOT,
            timeout=30,
            label="content-pack strict native build",
        )
        if native.stdout or native.stderr:
            raise ContentPackCheckFailure("warning-clean native content build produced output")
        executed = run([str(executable)], cwd=ROOT, timeout=10, label="content-pack native run")
        if executed.stdout or executed.stderr:
            raise ContentPackCheckFailure("runtime-free native content probe produced output")


def main() -> int:
    try:
        check_source_and_adapter()
        check_malformed_inputs()
        check_eval_oracle()
        check_generated_c()
    except (ContentPackCheckFailure, ContentPackFailure, OSError, UnicodeError, json.JSONDecodeError) as error:
        print(f"caxecraft-content-pack: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "caxecraft-content-pack: OK: strict data validation, current typed Haxe, "
        "first-playable resolution, and runtime-free strict C"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
